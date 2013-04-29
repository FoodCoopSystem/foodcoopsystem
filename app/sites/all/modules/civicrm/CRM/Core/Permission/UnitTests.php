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
 *
 */
class CRM_Core_Permission_UnitTests {

  public static function getPermission() {
    return CRM_Core_Permission::EDIT;
  }

  public static function whereClause($type, &$tables, &$whereTables) {
    return '( 1 )';
  }

  public static function &group($groupType = NULL, $excludeHidden = TRUE) {
    return CRM_Core_PseudoConstant::allGroup($groupType, $excludeHidden);
  }

  // permission mapping to stub check() calls
  public static $permissions = NULL;

  static function check($str) {
    // return the stubbed permission (defaulting to true if the array is missing)
    return is_array(self::$permissions) ? in_array($str, self::$permissions) : TRUE;
  }

  /**
   * Given a roles array, check for access requirements
   *
   * @param array $array the roles to check
   *
   * @return boolean true if yes, else false
   * @static
   * @access public
   */
  static
  function checkGroupRole($array) {
    return FALSE;
  }

  /**
   * Get all the contact emails for users that have a specific permission
   *
   * @param string $permissionName name of the permission we are interested in
   *
   * @return string a comma separated list of email addresses
   */
  public static function permissionEmails($permissionName) {
    return '';
  }

  /**
   * Get all the contact emails for users that have a specific role
   *
   * @param string $roleName name of the role we are interested in
   *
   * @return string a comma separated list of email addresses
   */
  public static function roleEmails($roleName) {
    return '';
  }
}

