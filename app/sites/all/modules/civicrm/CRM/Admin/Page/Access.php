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

require_once 'CRM/Core/Page/Basic.php';

/**
 * Dashboard page for managing Access Control
 * For initial version, this page only contains static links - so this class is empty for now.
 */
class CRM_Admin_Page_Access extends CRM_Core_Page {
  function run() {
    $config = CRM_Core_Config::singleton();

    if ($config->userFramework == 'Drupal') {
      $this->assign('ufAccessURL', CRM_Utils_System::url('admin/people/permissions'));
    }
    elseif ($config->userFramework == 'Drupal6') {
      $this->assign('ufAccessURL', CRM_Utils_System::url('admin/user/permissions'));
    }
    elseif ($config->userFramework == 'Joomla') {
      JHTML::_('behavior.modal');
      $url = $config->userFrameworkBaseURL . "index.php?option=com_config&view=component&component=com_civicrm&tmpl=component";
      $jparams = 'rel="{handler: \'iframe\', size: {x: 875, y: 550}, onClose: function() {}}" class="modal"';
      $this->assign('ufAccessURL', $url);
      $this->assign('jAccessParams', $jparams);
    }
    return parent::run();
  }
}

