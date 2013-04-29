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
 * This class stores logic for managing CiviCRM extensions.
 *
 * @package CRM
 * @copyright CiviCRM LLC (c) 2004-2011
 * $Id$
 *
 */

require_once 'CRM/Core/Config.php';
class CRM_Core_Extensions_Module {

  /**
   *
   */
  CONST CUSTOM_SEARCH_GROUP_NAME = 'custom_search';

  public function __construct($ext) {
    $this->ext = $ext;

    $this->config = CRM_Core_Config::singleton();
  }


  public function install() {
    if (array_key_exists($this->ext->key, $this->config->civiModules)) {
      CRM_Core_Error::fatal('This civiModule is already registered.');
    }

    $config = CRM_Core_Config::singleton();
    $params['civiModules'] = $config->civiModules;
    $params['civiModules'][$this->ext->file] = $this->ext->key . DIRECTORY_SEPARATOR . $this->ext->file . ".php";

    require_once 'CRM/Admin/Form/Setting.php';
    CRM_Admin_Form_Setting::commonProcess($params);
  }

  public function uninstall() {
    if (!array_key_exists($this->ext->key, $this->customSearches)) {
      CRM_Core_Error::fatal('This civiModule is not registered.');
    }

    $params['civiModules'] = $config->civiModules;
    $params['civiModules'] = array_diff($config->civiModules,
      array($this->ext->key)
    );

    require_once 'CRM/Admin/Form/Setting.php';
    CRM_Admin_Form_Setting::commonProcess($params);
  }

  public function disable() {
    $this->uninstall();
  }

  public function enable() {
    $this->install();
  }
}

