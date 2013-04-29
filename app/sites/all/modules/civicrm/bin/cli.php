#!/usr/bin/env php
<?php
/*
 +--------------------------------------------------------------------+
 | CiviCRM version 4.1                                                |
 +--------------------------------------------------------------------+
 | Copyright Tech To The People http:tttp.eu (c) 2008                 |
 +--------------------------------------------------------------------+
 |                                                                    |
 | CiviCRM is free software; you can copy, modify, and distribute it  |
 | under the terms of the GNU Affero General Public License           |
 | Version 3, 19 November 2007.                                       |
 |                                                                    |
 | CiviCRM is distributed in the hope that it will be useful, but     |
 | WITHOUT ANY WARRANTY; without even the implied warranty of         |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               |
 | See the GNU Affero General Public License for more details.        |
 |                                                                    |
 | You should have received a copy of the GNU Affero General Public   |
 | License along with this program; if not, contact CiviCRM LLC       |
 | at info[AT]civicrm[DOT]org. If you have questions about the        |
 | GNU Affero General Public License or the licensing of CiviCRM,     |
 | see the CiviCRM license FAQ at http://civicrm.org/licensing        |
 +--------------------------------------------------------------------+
*/
class civicrm_Cli {
  // required values that must be passed
  // via the command line
  var $_entity = NULL;
  var $_action = NULL;
  var $_output = FALSE;
  var $_joblog = FALSE;
  var $_config;

  // optional arguments
  var $_site = 'localhost';
  var $_user = NULL;

  // all other arguments populate the parameters
  // array that is passed to civicrm_api
  var $_params = array('version' => 3);

  var $_errors = array();

  public function initialize() {
    if (!$this->_parseOptions()) {
      return FALSE;
    }
    if (!$this->_bootstrap()) {
      return FALSE;
    }
    if (!$this->_validateOptions()) {
      return FALSE;
    }
    return TRUE;
  }

  public function callApi() {
    require_once 'api/api.php';

    //  CRM-9822 -'execute' action always goes thru Job api and always writes to log
    if ($this->_action != 'execute' && $this->_joblog) {
      require_once 'CRM/Core/JobManager.php';
      $facility = new CRM_Core_JobManager();
      $facility->setSingleRunParams($this->_entity, $this->_action, $this->_params, 'From Cli.php');
      $facility->executeJobByAction($this->_entity, $this->_action);
    }
    else {
      // CRM-9822 cli.php calls don't require site-key, so bypass site-key authentication
      $this->_params['auth'] = FALSE;
      $result = civicrm_api($this->_entity, $this->_action, $this->_params);
    }

    if ($result['is_error'] != 0) {
      $this->_log($result['error_message']);
      return FALSE;
    }
    elseif ($this->_output) {
      print_r($result['values']);
    }

    return true;
  }

  private function _parseOptions() {
    $args = $_SERVER['argv'];
    // remove the first argument, which is the name
    // of this script
    array_shift($args);

    while (list($k, $arg) = each($args)) {
      // sanitize all user input
      $arg = $this->_sanitize($arg);

      // if we're not parsing an option signifier
      // continue to the next one
      if (!preg_match('/^-/', $arg)) {
        continue;
      }

      // find the value of this arg
      if (preg_match('/=/', $arg)) {
        $parts = explode('=', $arg);
        $arg   = $parts[0];
        $value = $parts[1];
      }
      else {
        if (isset($args[$k + 1])) {
          $next_arg = $this->_sanitize($args[$k + 1]);
          // if the next argument is not another option
          // it's the value for this argument
          if (!preg_match('/^-/', $next_arg)) {
            $value = $next_arg;
          }
        }
      }

      // parse the special args first
      if ($arg == '-e' || $arg == '--entity') {
        $this->_entity = $value;
      }
      elseif ($arg == '-a' || $arg == '--action') {
        $this->_action = $value;
      }
      elseif ($arg == '-s' || $arg == '--site') {
        $this->_site = $value;
      }
      elseif ($arg == '-u' || $arg == '--user') {
        $this->_user = $value;
      }
      elseif ($arg == '-p' || $arg == '--password') {
        $this->_password = $value;
      }
      elseif ($arg == '-o' || $arg == '--output') {
        $this->_output = TRUE;
      }
      elseif ($arg == '-j' || $arg == '--joblog') {
        $this->_joblog = TRUE;
      }
      else {
        // all other arguments are parameters
        $key = ltrim($arg, '--');
        $this->_params[$key] = $value;
      }
    }
    return TRUE;
  }

  private function _bootstrap() {
    // so the configuration works with php-cli
    $_SERVER['PHP_SELF'] = "/index.php";
    $_SERVER['HTTP_HOST'] = $this->_site;
    $_SERVER['REMOTE_ADDR'] = "127.0.0.1";
    // SCRIPT_FILENAME needed by CRM_Utils_System::cmsRootPath
    $_SERVER['SCRIPT_FILENAME'] = __FILE__;
    // CRM-8917 - check if script name starts with /, if not - prepend it.
    if (ord($_SERVER['SCRIPT_NAME']) != 47) {
      $_SERVER['SCRIPT_NAME'] = '/' . $_SERVER['SCRIPT_NAME'];
    }

    $civicrm_root = dirname(dirname(__FILE__));
    chdir($civicrm_root);
    require_once ('civicrm.config.php');

    require_once ('CRM/Core/Config.php');
    $this->_config = CRM_Core_Config::singleton();

    require_once ('CRM/Utils/System.php');
    $class = 'CRM_Utils_System_' . $this->_config->userFramework;

    $cms = new $class();
    if (!CRM_Utils_System::loadBootstrap(array(
      ), FALSE, FALSE, $civicrm_root)) {
      $this->_log(ts("Failed to bootstrap CMS"));
      return FALSE;
    }

    if (strtolower($this->_entity) == 'job') {
      if (!$cms->authenticate($this->_user, $this->_password, FALSE, $civicrm_root)) {
        $this->_log(ts("Jobs called from cli.php require valid user and password as parameter", array('1' => $this->_user)));
        return FALSE;
      }
    }

    if (!empty($this->_user)) {
      if (!$cms->loadUser($this->_user)) {
        $this->_log(ts("Failed to login as %1", array('1' => $this->_user)));
        return FALSE;
      }
    }

    return TRUE;
  }

  private function _validateOptions() {
    $required = array('action', 'entity');
    while (list(, $var) = each($required)) {
      $index = '_' . $var;
      if (empty($this->$index)) {
        $missing_arg = '--' . $var;
        $this->_log(ts("The %1 argument is required", array(1 => $missing_arg)));
        $this->_log($this->_getUsage());
        return FALSE;
      }
    }
    return TRUE;
  }

  private function _sanitize($value) {
    // restrict user input - we should not be needing anything
    // other than normal alpha numeric plus - and _.
    return trim(preg_replace('/^[^a-zA-Z0-9\-_=]$/', '', $value));
  }

  private function _getUsage() {
    $out = "Usage: cli.php -e entity -a action [-u user] [-s site] [--output] [PARAMS]\n";
    $out .= "  entity is the name of the entity, e.g. Contact, Event, etc.\n";
    $out .= "  action is the name of the action e.g. Get, Create, etc.\n";
    $out .= "  user is an optional username to run the script as\n";
    $out .= "  site is the domain name of the web site (for Drupal multi site installs)\n";
    $out .= "  --output will print the result from the api call\n";
    $out .= "  PARAMS is one or more --param=value combinations to pass to the api\n";
    return ts($out);
  }

  private function _log($error) {
    // fixme, this should call some CRM_Core_Error:: function
    // that properly logs
    print "$error\n";
  }
}

function main() {
  $cli = new civicrm_Cli();
  $cli->initialize() || die( 'Died during initialization' );
  $cli->callApi() || die( 'Died during callApi' );
}

main();

