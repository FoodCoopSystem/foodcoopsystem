<?php
/*
 +--------------------------------------------------------------------+
 | CiviCRM version 4.1                                                |
 +--------------------------------------------------------------------+
 | Copyright CiviCRM LLC (c) 2004-2011                                |
 +--------------------------------------------------------------------+
 | This file is a part of CiviCRM.                                    |
 |                                                                    |
 | CiviCRM is free software; you can copy, modify, and distribute it  |
 | under the terms of the GNU Affero General Public License           |
 | Version 3, 19 November 2007 and the CiviCRM Licensing Exception.   |
 |                                                                    |
 | CiviCRM is distributed in the hope that it will be useful, but     |
 | WITHOUT ANY WARRANTY; without even the implied warranty of         |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               |
 | See the GNU Affero General Public License for more details.        |
 |                                                                    |
 | You should have received a copy of the GNU Affero General Public   |
 | License and the CiviCRM Licensing Exception along                  |
 | with this program; if not, contact CiviCRM LLC                     |
 | at info[AT]civicrm[DOT]org. If you have questions about the        |
 | GNU Affero General Public License or the licensing of CiviCRM,     |
 | see the CiviCRM license FAQ at http://civicrm.org/licensing        |
 +--------------------------------------------------------------------+
*/

/**
 *
 * @package CRM
 * @copyright CiviCRM LLC (c) 2004-2011
 * $Id$
 *
 */

require_once 'CRM/Utils/System/Base.php';

/**
 * Joomla specific stuff goes here
 */
class CRM_Utils_System_Joomla extends CRM_Utils_System_Base {
  function __construct() {
    $this->is_drupal = FALSE;
  }

  /**
   * Function to create a user of Joomla.
   *
   * @param array  $params associated array
   * @param string $mail email id for cms user
   *
   * @return uid if user exists, false otherwise
   *
   * @access public
   */
  function createUser(&$params, $mail) {
    require_once JPATH_SITE . '/components/com_users/models/registration.php';

    $userParams = JComponentHelper::getParams('com_users');
    $model      = new UsersModelRegistration();
    $ufID       = NULL;

    // get the default usertype
    $userType = $userParams->get('new_usertype');
    if (!$userType) {
      $userType = 2;
    }

    if (isset($params['name'])) {
      $fullname = trim($params['name']);
    }
    elseif (isset($params['contactID'])) {
      require_once 'CRM/Contact/BAO/Contact.php';
      $fullname = trim(CRM_Contact_BAO_Contact::displayName($params['contactID']));
    }
    else {
      $fullname = trim($params['cms_name']);
    }

    // Prepare the values for a new Joomla user.
    $values              = array();
    $values['name']      = $fullname;
    $values['username']  = trim($params['cms_name']);
    $values['password1'] = $values['password2'] = $params['cms_pass'];
    $values['email1']    = $values['email2'] = trim($params[$mail]);

    $lang = JFactory::getLanguage();
    $lang->load('com_users');

    $register = $model->register($values);

    $ufID = JUserHelper::getUserId($values['username']);
    return $ufID;
  }

  /*
     *  Change user name in host CMS
     *  
     *  @param integer $ufID User ID in CMS
     *  @param string $ufName User name
     */
  function updateCMSName($ufID, $ufName) {
    $ufID = CRM_Utils_Type::escape($ufID, 'Integer');
    $ufName = CRM_Utils_Type::escape($ufName, 'String');

    $values = array();
    $user = &JUser::getInstance($ufID);

    $values['email'] = $ufName;
    $user->bind($values);

    $user->save();
  }

  /**
   * Check if username and email exists in the Joomla! db
   *
   * @params $params    array   array of name and mail values
   * @params $errors    array   array of errors
   * @params $emailName string  field label for the 'email'
   *
   * @return void
   */
  function checkUserNameEmailExists(&$params, &$errors, $emailName = 'email') {
    $config = CRM_Core_Config::singleton();

    $dao   = new CRM_Core_DAO();
    $name  = $dao->escape(CRM_Utils_Array::value('name', $params));
    $email = $dao->escape(CRM_Utils_Array::value('mail', $params));
    //don't allow the special characters and min. username length is two
    //regex \\ to match a single backslash would become '/\\\\/'
    $isNotValid = (bool) preg_match('/[\<|\>|\"|\'|\%|\;|\(|\)|\&|\\\\|\/]/im', $name);
    if ($isNotValid || strlen($name) < 2) {
      $errors['cms_name'] = ts('Your username contains invalid characters or is too short');
    }


    $JUserTable = &JTable::getInstance('User', 'JTable');

    $db = $JUserTable->getDbo();
    $query = $db->getQuery(TRUE);
    $query->select('username, email');
    $query->from($JUserTable->getTableName());
    $query->where('(LOWER(username) = LOWER(\'' . $name . '\')) OR (LOWER(email) = LOWER(\'' . $email . '\'))');
    $db->setQuery($query, 0, 10);
    $users = $db->loadAssocList();

    $row = array();;
    if (count($users)) {
      $row = $users[0];
    }

    if (!empty($row)) {
      $dbName = CRM_Utils_Array::value('username', $row);
      $dbEmail = CRM_Utils_Array::value('email', $row);
      if (strtolower($dbName) == strtolower($name)) {
        $errors['cms_name'] = ts('The username %1 is already taken. Please select another username.',
          array(1 => $name)
        );
      }
      if (strtolower($dbEmail) == strtolower($email)) {
        $errors[$emailName] = ts('This email %1 is already registered. Please select another email.',
          array(1 => $email)
        );
      }
    }
  }

  /**
   * sets the title of the page
   *
   * @param string $title title to set
   * @param string $pageTitle
   *
   * @return void
   * @access public
   */
  function setTitle($title, $pageTitle = NULL) {
    if (!$pageTitle) {
      $pageTitle = $title;
    }

    $template = CRM_Core_Smarty::singleton();
    $template->assign('pageTitle', $pageTitle);

    $document = JFactory::getDocument();
    $document->setTitle($title);

    return;
  }

  /**
   * Append an additional breadcrumb tag to the existing breadcrumb
   *
   * @param string $title
   * @param string $url
   *
   * @return void
   * @access public
   */
  function appendBreadCrumb($breadCrumbs) {
    $template = CRM_Core_Smarty::singleton();
    $bc = $template->get_template_vars('breadcrumb');

    if (is_array($breadCrumbs)) {
      foreach ($breadCrumbs as $crumbs) {
        if (stripos($crumbs['url'], 'id%%')) {
          $args = array('cid', 'mid');
          foreach ($args as $a) {
            $val = CRM_Utils_Request::retrieve($a, 'Positive', CRM_Core_DAO::$_nullObject,
              FALSE, NULL, $_GET
            );
            if ($val) {
              $crumbs['url'] = str_ireplace("%%{$a}%%", $val, $crumbs['url']);
            }
          }
        }
        $bc[] = $crumbs;
      }
    }
    $template->assign_by_ref('breadcrumb', $bc);
    return;
  }

  /**
   * Reset an additional breadcrumb tag to the existing breadcrumb
   *
   * @param string $bc the new breadcrumb to be appended
   *
   * @return void
   * @access public
   */
  function resetBreadCrumb() {
    return;
  }

  /**
   * Append a string to the head of the html file
   *
   * @param string $head the new string to be appended
   *
   * @return void
   * @access public
   */
  function addHTMLHead($string = NULL, $includeAll = FALSE) {
    $document = JFactory::getDocument();

    if ($string) {
      $document->addCustomTag($string);
    }

    if ($includeAll) {
      require_once 'CRM/Core/Config.php';
      $config = CRM_Core_Config::singleton();

      $document->addStyleSheet("{$config->resourceBase}css/deprecate.css");
      $document->addStyleSheet("{$config->resourceBase}css/civicrm.css");

      if (!$config->userFrameworkFrontend) {
        $document->addStyleSheet("{$config->resourceBase}css/joomla.css");
      }
      else {
        $document->addStyleSheet("{$config->resourceBase}css/joomla_frontend.css");
      }
      if (isset($config->customCSSURL) && !empty($config->customCSSURL)) {
        $document->addStyleSheet($config->customCSSURL);
      }

      $document->addStyleSheet("{$config->resourceBase}css/extras.css");

      $document->addScript("{$config->resourceBase}js/Common.js");

      $template = CRM_Core_Smarty::singleton();

      // CRM-6819 + CRM-7086
      $lang     = substr($config->lcMessages, 0, 2);
      $l10nFile = "{$config->smartyDir}../jquery/jquery-ui-1.8.11/development-bundle/ui/i18n/jquery.ui.datepicker-{$lang}.js";
      $l10nURL  = "{$config->resourceBase}packages/jquery/jquery-ui-1.8.11/development-bundle/ui/i18n/jquery.ui.datepicker-{$lang}.js";
      if (file_exists($l10nFile)) {
        $template->assign('l10nURL', $l10nURL);
      }

      $document->addCustomTag($template->fetch('CRM/common/jquery.tpl'));
      $document->addCustomTag($template->fetch('CRM/common/action.tpl'));
    }
  }

  /**
   * Generate an internal CiviCRM URL
   *
   * @param $path     string   The path being linked to, such as "civicrm/add"
   * @param $query    string   A query string to append to the link.
   * @param $absolute boolean  Whether to force the output to be an absolute link (beginning with http:).
   *                           Useful for links that will be displayed outside the site, such as in an
   *                           RSS feed.
   * @param $fragment string   A fragment identifier (named anchor) to append to the link.
   * @param $htmlize  boolean  whether to convert to html eqivalant
   * @param $frontend boolean  a gross joomla hack
   *
   * @return string            an HTML string containing a link to the given path.
   * @access public
   *
   */
  function url($path = NULL, $query = NULL, $absolute = TRUE,
    $fragment = NULL, $htmlize = TRUE,
    $frontend = FALSE
  ) {
    $config    = CRM_Core_Config::singleton();
    $separator = $htmlize ? '&amp;' : '&';
    $Itemid    = '';
    $script    = '';
    require_once 'CRM/Utils/String.php';
    $path = CRM_Utils_String::stripPathChars($path);

    if ($config->userFrameworkFrontend) {
      $script = 'index.php';
      if (JRequest::getVar("Itemid")) {
        $Itemid = "{$separator}Itemid=" . JRequest::getVar("Itemid");
      }
    }

    if (isset($fragment)) {
      $fragment = '#' . $fragment;
    }

    if (!isset($config->useFrameworkRelativeBase)) {
      $base = parse_url($config->userFrameworkBaseURL);
      $config->useFrameworkRelativeBase = $base['path'];
    }
    $base = $absolute ? $config->userFrameworkBaseURL : $config->useFrameworkRelativeBase;

    if (!empty($query)) {
      $url = "{$base}{$script}?option=com_civicrm{$separator}task={$path}{$Itemid}{$separator}{$query}{$fragment}";
    }
    else {
      $url = "{$base}{$script}?option=com_civicrm{$separator}task={$path}{$Itemid}{$fragment}";
    }

    // gross hack for joomla, we are in the backend and want to send a frontend url
    if ($frontend &&
      $config->userFramework == 'Joomla'
    ) {
      // handle both joomla v1.5 and v1.6, CRM-7939
      $url = str_replace('/administrator/index2.php', '/index.php', $url);
      $url = str_replace('/administrator/index.php', '/index.php', $url);

      // CRM-8215
      $url = str_replace('/administrator/', '/index.php', $url);
    }

    return $url;
  }

  /**
   * rewrite various system urls to https
   *
   * @return void
   * access public
   */
  function mapConfigToSSL() {
    // dont need to do anything, let CMS handle their own switch to SSL
    return;
  }

  /**
   * figure out the post url for the form
   *
   * @param $action the default action if one is pre-specified
   *
   * @return string the url to post the form
   * @access public
   */
  function postURL($action) {
    if (!empty($action)) {
      return $action;
    }

    return $this->url(CRM_Utils_Array::value('task', $_GET),
      NULL, TRUE, NULL, FALSE
    );
  }

  /**
   * Function to set the email address of the user
   *
   * @param object $user handle to the user object
   *
   * @return void
   * @access public
   */
  function setEmail(&$user) {
    global $database;
    $query = "SELECT email FROM #__users WHERE id='$user->id'";
    $database->setQuery($query);
    $user->email = $database->loadResult();
  }

  /**
   * Authenticate the user against the joomla db
   *
   * @param string $name     the user name
   * @param string $password the password for the above user name
   * @param $loadCMSBootstrap boolean load cms bootstrap?
   *
   * @return mixed false if no auth
   *               array(
      contactID, ufID, unique string ) if success
   * @access public
   */
  function authenticate($name, $password, $loadCMSBootstrap = FALSE) {
    require_once 'DB.php';

    $config = CRM_Core_Config::singleton();

    if ($loadCMSBootstrap) {
      $bootStrapParams = array();
      if ($name && $password) {
        $bootStrapParams = array(
          'name' => $name,
          'pass' => $password,
        );
      }
      CRM_Utils_System::loadBootStrap($bootStrapParams);
    }

    jimport('joomla.application.component.helper');
    jimport('joomla.database.table');

    $JUserTable = &JTable::getInstance('User', 'JTable');

    $db = $JUserTable->getDbo();
    $query = $db->getQuery(TRUE);
    $query->select('id, username, email, password');
    $query->from($JUserTable->getTableName());
    $query->where('(LOWER(username) = LOWER(\'' . $name . '\')) AND (block = 0)');
    $db->setQuery($query, 0, 0);
    $users = $db->loadAssocList();

    $row = array();;
    if (count($users)) {
      $row = $users[0];
    }

    $user = NULL;
    require_once 'CRM/Core/BAO/UFMatch.php';
    if (!empty($row)) {
      $dbPassword = CRM_Utils_Array::value('password', $row);
      $dbId       = CRM_Utils_Array::value('id', $row);
      $dbEmail    = CRM_Utils_Array::value('email', $row);

      // now check password
      if (strpos($dbPassword, ':') === FALSE) {
        if ($dbPassword != md5($password)) {
          return FALSE;
        }
      }
      else {
        list($hash, $salt) = explode(':', $dbPassword);
        $cryptpass = md5($password . $salt);
        if ($hash != $cryptpass) {
          return FALSE;
        }
      }

      CRM_Core_BAO_UFMatch::synchronizeUFMatch($user, $dbId, $dbEmail, 'Joomla');
      $contactID = CRM_Core_BAO_UFMatch::getContactId($dbId);
      if (!$contactID) {
        return FALSE;
      }
      return array($contactID, $dbId, mt_rand());
    }
    return FALSE;
  }

  /**
   * Set a message in the UF to display to a user
   *
   * @param string $message  the message to set
   *
   * @access public
   */
  function setMessage($message) {
    return;
  }

  function loadUser($user) {
    return TRUE;
  }

  function permissionDenied() {
    CRM_Core_Error::fatal(ts('You do not have permission to access this page'));
  }

  function logout() {
    session_destroy();
    header("Location:index.php");
  }

  /**
   * Get the locale set in the hosting CMS
   *
   * @return string  the used locale or null for none
   */
  function getUFLocale() {
    if (defined('_JEXEC')) {
      $conf = JFactory::getConfig();
      $locale = $conf->getValue('config.language');
      return str_replace('-', '_', $locale);
    }
    return NULL;
  }

  function getVersion() {
    if (class_exists('JVersion')) {
      $version = new JVersion;
      return $version->getShortVersion();
    }
    else {
      return 'Unknown';
    }
  }

  /* 
     * load joomla bootstrap
     *
     * @param $params array with uid or name and password 
     * @param $loadUser boolean load cms user?
     * @param $throwError throw error on failure?
     */
  function loadBootStrap($params = array(
    ), $loadUser = TRUE, $throwError = TRUE) {
    // Setup the base path related constant.
    $joomlaBase = dirname(dirname(dirname(dirname(dirname(dirname(dirname(dirname(__FILE__))))))));

    // load BootStrap here if needed
    // We are a valid Joomla entry point.
    if (!defined('_JEXEC')) {
      define('_JEXEC', 1);
      define('DS', DIRECTORY_SEPARATOR);
      define('JPATH_BASE', $joomlaBase . '/administrator/');
      require $joomlaBase . '/administrator/includes/defines.php';
    }

    // Get the framework.
    require $joomlaBase . '/libraries/import.php';
    require $joomlaBase . '/configuration.php';
    jimport('joomla.application.cli');

    return TRUE;
  }

  /**
   * check is user logged in.
   *
   * @return boolean true/false.
   */
  public function isUserLoggedIn() {
    $user = JFactory::getUser();
    return ($user->guest) ? FALSE : TRUE;
  }

  /**
   * Get currently logged in user uf id.
   *
   * @return int logged in user uf id.
   */
  public function getLoggedInUfID() {
    $user = JFactory::getUser();
    return ($user->guest) ? NULL : $user->id;
  }
}

