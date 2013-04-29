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

/**
 * This class contains all the function that are called using AJAX (dojo)
 */
class CRM_Member_Page_AJAX {

  /**
   * Function to setDefaults according to membership type
   */
  function getMemberTypeDefaults($config) {
    require_once 'CRM/Utils/Type.php';
    if (!$_POST['mtype']) {
      $details['name'] = '';
      $details['auto_renew'] = '';
      $details['total_amount'] = '';

      echo json_encode($details);
      CRM_Utils_System::civiExit();
    }
    $memType = CRM_Utils_Type::escape($_POST['mtype'], 'Integer');



    $query = "SELECT name, minimum_fee AS total_amount, contribution_type_id, auto_renew 
FROM    civicrm_membership_type
WHERE   id = %1";

    $dao = CRM_Core_DAO::executeQuery($query, array(1 => array($memType, 'Positive')));
    $properties = array('contribution_type_id', 'total_amount', 'name', 'auto_renew');
    while ($dao->fetch()) {
      foreach ($properties as $property) {
        $details[$property] = $dao->$property;
      }
    }

    // fix the display of the monetary value, CRM-4038
    require_once 'CRM/Utils/Money.php';
    $details['total_amount'] = CRM_Utils_Money::format($details['total_amount'], NULL, '%a');

    $options = array(ts('No auto-renew option'), ts('Give option, but not required'), ts('Auto-renew required '));
    $details['auto_renew'] = $options[$details['auto_renew']];
    echo json_encode($details);
    CRM_Utils_System::civiExit();
  }
}

