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
 * File for CiviCRM APIv3 utilitity functions
 *
 * @package CiviCRM_APIv3
 * @subpackage API_utils
 *
 * @copyright CiviCRM LLC (c) 2004-2011
 * @version $Id: utils.php 30879 2010-11-22 15:45:55Z shot $
 *
 */

/**
 * Initialize CiviCRM - should be run at the start of each API function
 *
 * $useException boolean raise exception if set
 */
function _civicrm_api3_initialize($useException = TRUE) {
  require_once 'CRM/Core/Config.php';
  $config = CRM_Core_Config::singleton();
  if ($useException) {
    CRM_Core_Error::setRaiseException();
  }
}

/*
 * Wrapper Function for civicrm_verify_mandatory to make it simple to pass either / or fields for checking
 *
 * @param array $params array of fields to check
 * @param array $daoName string DAO to check for required fields (create functions only)
 * @param array $keys list of required fields options. One of the options is required
 * @return null or throws error if there the required fields not present

 * @
 *
 */
function civicrm_api3_verify_one_mandatory($params, $daoName = NULL, $keyoptions = array(
  )) {
  $keys = array(array());
  foreach ($keyoptions as $key) {
    $keys[0][] = $key;
  }
  civicrm_api3_verify_mandatory($params, $daoName, $keys);
}

/*
 * Function to check mandatory fields are included
 *
 * @param array $params array of fields to check
 * @param array $daoName string DAO to check for required fields (create functions only)
 * @param array $keys list of required fields. A value can be an array denoting that either this or that is required.
 * @param bool $verifyDAO
 *
 * @return null or throws error if there the required fields not present
 */
function civicrm_api3_verify_mandatory($params, $daoName = NULL, $keys = array(
  ), $verifyDAO = TRUE) {
  // moving this to civicrm_api - remove the check for array pending testing
  if (!is_array($params)) {
    throw new Exception('Input variable `params` is not an array');
  }

  if ($daoName != NULL && $verifyDAO && !CRM_Utils_Array::value('id', $params)) {
    if (!is_array($unmatched = _civicrm_api3_check_required_fields($params, $daoName, TRUE))) {
      $unmatched = array();
    }
  }
  else {
    // always define to prevent E_NOTICE warning
    $unmatched = array();
  }
  require_once 'CRM/Utils/Array.php';
  if (CRM_Utils_Array::value('id', $params)) {
    $keys = array('version');
  }
  else {
    if (!in_array('version', $keys)) {
      // required from v3 onwards
      $keys[] = 'version';
    }
  }
  foreach ($keys as $key) {
    if (is_array($key)) {
      $match = 0;
      $optionset = array();
      foreach ($key as $subkey) {
        if (!array_key_exists($subkey, $params) || empty($params[$subkey])) {
          $optionset[] = $subkey;
        }
        else {
          // as long as there is one match then we don't need to rtn anything
          $match = 1;
        }
      }
      if (empty($match) && !empty($optionset)) {
        $unmatched[] = "one of (" . implode(", ", $optionset) . ")";
      }
    }
    else {
      if (!array_key_exists($key, $params) || empty($params[$key])) {
        $unmatched[] = $key;
      }
    }
  }
  if (!empty($unmatched)) {
    throw new Exception("Mandatory key(s) missing from params array: " . implode(", ", $unmatched));
  }
}

/**
 *
 * @param <type> $msg
 * @param <type> $data
 * @param object $dao DAO / BAO object to be freed here
 *
 * @return <type>
 */
function &civicrm_api3_create_error($msg, $data = NULL, &$dao = NULL) {
  if (is_object($dao)) {
    $dao->free();
  }
  if (is_array($dao) && $msg == 'DB Error: constraint violation') {
    try {
      _civicrm_api3_validate_fields($dao['entity'], $dao['action'], $dao['params'], True);
    }
    catch(Exception$e) {
      $msg = $e->getMessage();
    }
  }
  return CRM_Core_Error::createAPIError($msg, $data);
}

/**
 * Format array in result output styple
 *
 * @param array $params
 * @dao object DAO object to be freed here
 *
 * @return array $result
 */
function civicrm_api3_create_success($values = 1, $params = array(
  ), $entity = NULL, $action = NULL, &$dao = NULL) {
  $result = array();
  $result['is_error'] = 0;
  //lets set the ['id'] field if it's not set & we know what the entity is
  if (is_array($values) && !empty($entity)) {
    foreach ($values as $key => $item) {
      if (empty($item['id']) && !empty($item[$entity . "_id"])) {
        $values[$key]['id'] = $item[$entity . "_id"];
      }
    }
  }
  //if ( array_key_exists ('debug',$params) && is_object ($dao)) {
  if (is_array($params) && array_key_exists('debug', $params)) {
    if (!is_object($dao)) {
      $d = _civicrm_api3_get_DAO(CRM_Utils_Array::value('entity', $params));
      if (!empty($d)) {
        $file = str_replace('_', '/', $d) . ".php";
        require_once ($file);
        $dao = new $d();
      }
    }
    if (is_string($action)) {
      $apiFields = civicrm_api($entity, 'getfields', array('version' => 3, 'action' => $action)+ $params);
    }
    else {
      $apiFields = civicrm_api($entity, 'getfields', array('version' => 3)+ $params);
    }

    $allFields = array();
    if (is_array(CRM_Utils_Array::value('values', $apiFields))) {
      $allFields = array_keys($apiFields['values']);
    }
    $paramFields = array_keys($params);
    $undefined = array_diff($paramFields, $allFields, array_keys($_COOKIE), array('action', 'entity', 'debug', 'version', 'check_permissions', 'IDS_request_uri', 'IDS_user_agent', 'return', 'sequential', 'rowCount', 'option_offset', 'option_limit', 'custom', 'option_sort'));
    if ($undefined) {
      $result['undefined_fields'] = array_merge($undefined);
    }
  }
  if (is_object($dao)) {
    $dao->free();
  }

  $result['version'] = 3;
  if (is_array($values)) {
    $result['count'] = count($values);

    // Convert value-separated strings to array
    _civicrm_api3_separate_values($values);

    if ($result['count'] == 1) {
      list($result['id']) = array_keys($values);
    }
    elseif (!empty($values['id']) && is_int($values['id'])) {
      $result['id'] = $values['id'];
    }
  }
  else {
    $result['count'] = !empty($values) ? 1 : 0;
  }

  if (is_array($values) && isset($params['sequential']) &&
    $params['sequential'] == 1
  ) {
    $result['values'] = array_values($values);
  }
  else {
    $result['values'] = $values;
  }

  return $result;
}
/*
 * Load the DAO of the entity
 */
function _civicrm_api3_load_DAO($entity) {
  $dao = _civicrm_api3_get_DAO($entity);
  if (empty($dao)) {
    return FALSE;
  }
  $file = str_replace('_', '/', $dao) . ".php";
  require_once ($file);
  $d = new $dao();
  return $d;
}
/*
 * Function to return the DAO of the function or Entity
 * @param  $name is either a function of the api (civicrm_{entity}_create or the entity name
 * return the DAO name to manipulate this function
 * eg. "civicrm_api3_contact_create" or "Contact" will return "CRM_Contact_BAO_Contact"
 */
function _civicrm_api3_get_DAO($name) {
  static $dao = NULL;
  if (!$dao) {
    require ('CRM/Core/DAO/.listAll.php');
  }

  if (strpos($name, 'civicrm_api3') !== FALSE) {
    $last = strrpos($name, '_');
    // len ('civicrm_api3_') == 13
    $name = substr($name, 13, $last - 13);
  }

  if (strtolower($name) == 'individual' || strtolower($name) == 'household' || strtolower($name) == 'organization') {
    $name = 'Contact';
  }
  return CRM_Utils_Array::value(_civicrm_api_get_camel_name($name, 3), $dao);
}

/*
 * Function to return the DAO of the function or Entity
 * @param  $name is either a function of the api (civicrm_{entity}_create or the entity name
 * return the DAO name to manipulate this function
 * eg. "civicrm_contact_create" or "Contact" will return "CRM_Contact_BAO_Contact"
 */
function _civicrm_api3_get_BAO($name) {
  $dao = _civicrm_api3_get_DAO($name);
  $dao = str_replace("DAO", "BAO", $dao);
  return $dao;
}

/**
 *  Recursive function to explode value-separated strings into arrays
 *
 */
function _civicrm_api3_separate_values(&$values) {
  $sp = CRM_Core_DAO::VALUE_SEPARATOR;
  foreach ($values as & $value) {
    if (is_array($value)) {
      _civicrm_api3_separate_values($value);
    }
    elseif (is_string($value)) {
      if (strpos($value, $sp) !== FALSE) {
        $value = explode($sp, trim($value, $sp));
      }
    }
  }
}

/**
 *
 * @param array $fields
 * @param array $params
 * @param array $values
 *
 * @return Bool $valueFound
 */
function _civicrm_api3_store_values(&$fields, &$params, &$values) {
  $valueFound = FALSE;

  $keys = array_intersect_key($params, $fields);
  foreach ($keys as $name => $value) {
    if ($name !== 'id') {
      $values[$name] = $value;
      $valueFound = TRUE;
    }
  }
  return $valueFound;
}

/*
 * Function transfers the filters being passed into the DAO onto the params object
 */
function _civicrm_api3_dao_set_filter(&$dao, $params, $unique = TRUE) {
  $entity = substr($dao->__table, 8);

  $fields = _civicrm_api3_build_fields_array($dao, $unique);
  $fields = array_intersect(array_keys($fields), array_keys($params));
  if (isset($params[$entity . "_id"])) {
    //if entity_id is set then treat it as ID (will be overridden by id if set)
    $dao->id = $params[$entity . "_id"];
  }
  //apply options like sort
  _civicrm_api3_apply_options_to_dao($params, $dao);

  //accept filters like filter.activity_date_time_high
  // std is now 'filters' => ..
  if (strstr(implode(',', array_keys($params)), 'filter')) {
    if (is_array($params['filters'])) {
      foreach ($params['filters'] as $paramkey => $paramvalue) {
        _civicrm_api3_apply_filters_to_dao($paramkey, $paramvalue, $dao);
      }
    }
    else {
      foreach ($params as $paramkey => $paramvalue) {
        if (strstr($paramkey, 'filter')) {
          _civicrm_api3_apply_filters_to_dao(substr($paramkey, 7), $paramvalue, $dao);
        }
      }
    }
  }
  // http://issues.civicrm.org/jira/browse/CRM-9150 - stick with 'simple' operators for now
  // support for other syntaxes is discussed in ticket but being put off for now
  $acceptedSQLOperators = array('=', '<=', '>=', '>', '<', 'LIKE', "<>", "!=", "NOT LIKE", 'IN', 'NOT IN');
  if (!$fields) {
    return;
  }
  foreach ($fields as $field) {
    if (is_array($params[$field])) {
      //array is the syntax for SQL clause
      foreach ($params[$field] as $operator => $criteria) {
        if (in_array($operator, $acceptedSQLOperators)) {
          switch ($operator) {
            // unary operators

            case 'IS NULL':
            case 'IS NOT NULL':
              $dao->whereAdd(sprintf('%s %s', $field, $operator));
              break;

            // ternary operators

            case 'BETWEEN':
            case 'NOT BETWEEN':
              if (empty($criteria[0]) || empty($criteria[1])) {
                error();
              }
              $dao->whereAdd(sprintf('%s BETWEEN "%s" AND "%s"', $field, DAO::escapeString($criteria[0]), DAO::escapeString($criteria[1])));
              break;

            // n-ary operators

            case 'IN':
            case 'NOT IN':
              if (empty($criteria)) {
                error();
              }
              $escapedCriteria = array_map(array('CRM_Core_DAO', 'escapeString'), $criteria);
              $dao->whereAdd(sprintf('%s %s ("%s")', $field, $operator, implode('", "', $escapedCriteria)));
              break;

            // binary operators

            default:

              $dao->whereAdd(sprintf('%s %s "%s"', $field, $operator, CRM_Core_DAO::escapeString($criteria)));
          }
        }
      }
    }
    else {
      $dao->$field = $params[$field];
    }
  }
  if (!empty($params['return']) && is_array($params['return'])) {
    $dao->selectAdd();
    foreach ($params['return'] as $returnValue) {
      if (in_array($returnValue, $fields)) {
        $dao->selectAdd($returnValue);
      }
    }
    $dao->selectAdd('id');
  }
}

/*
 * Apply filters (e.g. high, low) to DAO object (prior to find)
 * @param string $filterField field name of filter
 * @param string $filterValue field value of filter
 * @param object $dao DAO object
 */
function _civicrm_api3_apply_filters_to_dao($filterField, $filterValue, &$dao) {
  if (strstr($filterField, 'high')) {
    $fieldName = substr($filterField, 0, -5);
    $dao->whereAdd("($fieldName <= $filterValue )");
  }
  if (strstr($filterField, 'low')) {
    $fieldName = substr($filterField, 0, -4);
    $dao->whereAdd("($fieldName >= $filterValue )");
  }
  if($filterField == 'is_current' && $filterValue == 1){
    $dao->whereAdd('(start_date <= NOW() OR start_date IS NULL) AND (end_date >= NOW() OR end_date IS NULL)');
    if(property_exists($dao, 'is_active')){
      $dao->whereAdd('is_active = 1');
    }
  }
}
/*
 * @param array $params params array as passed into civicrm_api
 * @return array $options options extracted from params
 */
function _civicrm_api3_get_options_from_params(&$params) {

  $options          = array();
  $inputParams      = array();
  $returnProperties = array();
  $otherVars        = array('sort', 'offset', 'rowCount');

  $sort     = NULL;
  $offset   = 0;
  $rowCount = 25;
  foreach ($params as $n => $v) {
    if (substr($n, 0, 7) == 'return.') {
      $returnProperties[substr($n, 7)] = $v;
    }
    elseif (in_array($n, $otherVars)) {
      $$n = $v;
    }
    else {
      $inputParams[$n] = $v;
    }
  }
  $options['sort'] = $sort;
  $options['limit'] = $rowCount;
  $options['offset'] = $offset;
  $options['return'] = $returnProperties;
  $options['input_params'] = $inputParams;
  return $options;
}
/*
 * Apply options (e.g. sort, limit, order by) to DAO object (prior to find)
 * @param array $params params array as passed into civicrm_api
 * @param object $dao DAO object
 */
function _civicrm_api3_apply_options_to_dao(&$params, &$dao, $defaults = array(
  )) {
  $sort = CRM_Utils_Array::value('option.sort', $params, 0);
  $sort = CRM_Utils_Array::value('option_sort', $params, $sort);

  $offset = CRM_Utils_Array::value('option.offset', $params, 0);
  // dear PHP thought it would be a good idea to transform a.b into a_b in the get/post
  $offset = CRM_Utils_Array::value('option_offset', $params, $offset);

  //XAV->eileen do you want it?     $offset = CRM_Utils_Array::value('offset', $params,  $offset);
  $limit = CRM_Utils_Array::value('option.limit', $params, 25);
  $limit = CRM_Utils_Array::value('option_limit', $params, $limit);


  if (is_array(CRM_Utils_Array::value('options', $params))) {
    $offset = CRM_Utils_Array::value('offset', $params['options'], $offset);
    $limit  = CRM_Utils_Array::value('limit', $params['options'], $limit);
    $sort   = CRM_Utils_Array::value('sort', $params['options'], $sort);
  }

  $dao->limit((int)$offset, (int)$limit);


  if (!empty($sort)) {
    $dao->orderBy($sort);
  }
}

/*
 * build fields array. This is the array of fields as it relates to the given DAO
 * returns unique fields as keys by default but if set but can return by DB fields
 */
function _civicrm_api3_build_fields_array(&$dao, $unique = TRUE) {
  $fields = $dao->fields();
  if ($unique) {
    return $fields;
  }

  foreach ($fields as $field) {
    $dbFields[$field['name']] = $field;
  }
  return $dbFields;
}

/**
 * Converts an DAO object to an array
 *
 * @param  object   $dao           (reference )object to convert
 * @params array of arrays (key = id) of array of fields
 * @static void
 * @access public
 */
function _civicrm_api3_dao_to_array($dao, $params = NULL, $uniqueFields = TRUE, $entity = "") {
  $result = array();
  if (empty($dao) || !$dao->find()) {
    return array();
  }

  //if custom fields are required we will endeavour to set them . NB passing $entity in might be a bit clunky / unrequired
  if (!empty($entity) && CRM_Utils_Array::value('return', $params) && is_array($params['return'])) {
    foreach ($params['return'] as $return) {
      if (substr($return, 0, 6) == 'custom') {
        $custom = TRUE;
      }
    }
  }


  $fields = array_keys(_civicrm_api3_build_fields_array($dao, $uniqueFields));

  while ($dao->fetch()) {
    $tmp = array();
    foreach ($fields as $key) {
      if (array_key_exists($key, $dao)) {
        // not sure on that one
        if ($dao->$key !== NULL) {
          $tmp[$key] = $dao->$key;
        }
      }
    }
    $result[$dao->id] = $tmp;
    if (!empty($custom)) {
      _civicrm_api3_custom_data_get($result[$dao->id], $entity, $dao->id);
    }
  }


  return $result;
}

/**
 * Converts an object to an array
 *
 * @param  object   $dao           (reference )object to convert
 * @param  array    $dao           (reference )array
 * @param array  $uniqueFields
 *
 * @return array
 * @static void
 * @access public
 */
function _civicrm_api3_object_to_array(&$dao, &$values, $uniqueFields = FALSE) {

  $fields = _civicrm_api3_build_fields_array($dao, $uniqueFields);
  foreach ($fields as $key => $value) {
    if (array_key_exists($key, $dao)) {
      $values[$key] = $dao->$key;
    }
  }
}

/*
 * Wrapper for _civicrm_object_to_array when api supports unique fields
 */
function _civicrm_api3_object_to_array_unique_fields(&$dao, &$values) {
  return _civicrm_api3_object_to_array($dao, $values, TRUE);
}

/**
 *
 * @param array $params
 * @param array $values
 * @param string $extends entity that this custom field extends (e.g. contribution, event, contact)
 * @param string $entityId ID of entity per $extends
 */
function _civicrm_api3_custom_format_params($params, &$values, $extends, $entityId = NULL) {
  $values['custom'] = array();

  require_once 'CRM/Core/BAO/CustomField.php';
  foreach ($params as $key => $value) {
    list($customFieldID, $customValueID) = CRM_Core_BAO_CustomField::getKeyID($key, TRUE);
    if ($customFieldID) {
      CRM_Core_BAO_CustomField::formatCustomField($customFieldID, $values['custom'],
        $value, $extends, $customValueID, $entityId, FALSE, FALSE
      );
    }
  }
}

/**
 * This function ensures that we have the right input parameters
 *
 * We also need to make sure we run all the form rules on the params list
 * to ensure that the params are valid
 *
 * @param array  $params       Associative array of property name/value
 *                             pairs to insert in new history.
 * @daoName string DAO to check params agains
 *
 * @return bool should the missing fields be returned as an array (core error created as default)
 *
 * @todo the check for required fields unsets the ID as that isn't required for create but potentially also unsets other ID fields, note also the DAO might be a bit 'harsh' in it's required fields as the BAO handles some
 *
 * @return bool true if all fields present, depending on $result a core error is created of an array of missing fields is returned
 * @access public
 */
function _civicrm_api3_check_required_fields($params, $daoName, $return = FALSE) {
  if (isset($params['extends'])) {
    if (($params['extends'] == 'Activity' ||
        $params['extends'] == 'Phonecall' ||
        $params['extends'] == 'Meeting' ||
        $params['extends'] == 'Group' ||
        $params['extends'] == 'Contribution'
      ) &&
      ($params['style'] == 'Tab')
    ) {
      return civicrm_api3_create_error(ts("Can not create Custom Group in Tab for " . $params['extends']));
    }
  }

  require_once (str_replace('_', DIRECTORY_SEPARATOR, $daoName) . ".php");

  $dao = new $daoName();
  $fields = $dao->fields();

  $missing = array();
  foreach ($fields as $k => $v) {
    if ($v['name'] == 'id') {
      continue;
    }

    if (CRM_Utils_Array::value('required', $v)) {
      // 0 is a valid input for numbers, CRM-8122
      if (empty($params[$k]) && !($params[$k] === 0)) {
        $missing[] = $k;
      }
    }
  }

  if (!empty($missing)) {
    if (!empty($return)) {
      return $missing;
    }
    else {
      return civicrm_api3_create_error(ts("Required fields " . implode(',', $missing) . " for $daoName are not present"));
    }
  }

  return TRUE;
}

/**
 * Check permissions for a given API call.
 *
 * @param $entity string API entity being accessed
 * @param $action string API action being performed
 * @param $params array  params of the API call
 * @param $throw bool    whether to throw exception instead of returning false
 *
 * @return bool whether the current API user has the permission to make the call
 */
function _civicrm_api3_api_check_permission($entity, $action, &$params, $throw = TRUE) {
  // return early unless we’re told explicitly to do the permission check
  if (empty($params['check_permissions']) or $params['check_permissions'] == FALSE) {
    return TRUE;
  }

  require_once 'CRM/Core/Permission.php';

  require_once 'CRM/Core/DAO/.permissions.php';
  $permissions = _civicrm_api3_permissions($entity, $action, $params);

  // $params might’ve been reset by the alterAPIPermissions() hook
  if (isset($params['check_permissions']) and $params['check_permissions'] == FALSE) {
    return TRUE;
  }

  foreach ($permissions as $perm) {
    if (!CRM_Core_Permission::check($perm)) {
      if ($throw) {
        throw new Exception("API permission check failed for $entity/$action call; missing permission: $perm.");
      }
      else {
        return FALSE;
      }
    }
  }
  return TRUE;
}

/*
 * Function to do a 'standard' api get - when the api is only doing a $bao->find then use this
 *
 * @param string $bao_name name of BAO
 * @param array $params params from api
 * @param bool $returnAsSuccess return in api success format
 */
function _civicrm_api3_basic_get($bao_name, &$params, $returnAsSuccess = TRUE, $entity = "") {
  $bao = new $bao_name();
  _civicrm_api3_dao_set_filter($bao, $params, FALSE);
  if ($returnAsSuccess) {
    return civicrm_api3_create_success(_civicrm_api3_dao_to_array($bao, $params, FALSE, $entity), $params, $bao);
  }
  else {
    return _civicrm_api3_dao_to_array($bao, $params, FALSE, $entity);
  }
}

/*
 * Function to do a 'standard' api create - when the api is only doing a $bao::create then use this
 */
function _civicrm_api3_basic_create($bao_name, &$params) {

  $args = array(&$params);
  if (method_exists($bao_name, 'create')) {
    $fct = 'create';
  }
  elseif (method_exists($bao_name, 'add')) {
    $fct = 'add';
  }
  if (!isset($fct)) {
    return civicrm_api3_create_error('Entity not created, missing create or add method for ' . $bao_name);
  }
  $bao = call_user_func_array(array($bao_name, $fct), $args);
  if (is_null($bao)) {
    return civicrm_api3_create_error('Entity not created ' . $bao_name . '::' . $fct);
  }
  else {
    $values = array();
    _civicrm_api3_object_to_array($bao, $values[$bao->id]);
    return civicrm_api3_create_success($values, $params, $bao, 'create');
  }
}

/*
 * Function to do a 'standard' api del - when the api is only doing a $bao::del then use this
 * if api::del doesn't exist it will try DAO delete method
 */
function _civicrm_api3_basic_delete($bao_name, &$params) {

  civicrm_api3_verify_mandatory($params, NULL, array('id'));
  $args = array(&$params['id']);
  if (method_exists($bao_name, 'del')) {
    $bao = call_user_func_array(array($bao_name, 'del'), $args);
    return civicrm_api3_create_success(TRUE);
  }
  elseif (method_exists($bao_name, 'delete')) {
    $dao = new $bao_name();
    $dao->id = $params['id'];
    if ($dao->find()) {
      while ($dao->fetch()) {
        $dao->delete();
        return civicrm_api3_create_success();
      }
    }
    else {
      return civicrm_api3_create_error('Could not delete entity id ' . $params['id']);
    }
  }

  return civicrm_api3_create_error('no delete method found');
}

/*
 * Get custom data for the given entity & Add it to the returnArray as 'custom_123' = 'custom string' AND 'custom_123_1' = 'custom string'
 * Where 123 is field value & 1 is the id within the custom group data table (value ID)
 *
 * @param array $returnArray - array to append custom data too - generally $result[4] where 4 is the entity id.
 * @param string $entity  e.g membership, event
 * @param int $groupID - per CRM_Core_BAO_CustomGroup::getTree
 * @param int $subType e.g. membership_type_id where custom data doesn't apply to all membership types
 * @param string $subName - Subtype of entity
 *
 */
function _civicrm_api3_custom_data_get(&$returnArray, $entity, $entity_id, $groupID = NULL, $subType = NULL, $subName = NULL) {
  require_once 'CRM/Core/BAO/CustomGroup.php';
  require_once 'CRM/Core/BAO/CustomField.php';
  $groupTree = &CRM_Core_BAO_CustomGroup::getTree($entity,
    CRM_Core_DAO::$_nullObject,
    $entity_id,
    $groupID,
    $subType,
    $subName
  );
  $groupTree = CRM_Core_BAO_CustomGroup::formatGroupTree($groupTree, 1, CRM_Core_DAO::$_nullObject);
  $customValues = array();
  CRM_Core_BAO_CustomGroup::setDefaults($groupTree, $customValues);
  if (!empty($customValues)) {
    foreach ($customValues as $key => $val) {
      if (strstr($key, '_id')) {
        $idkey = substr($key, 0, -3);
        $returnArray['custom_' . (CRM_Core_BAO_CustomField::getKeyID($idkey) . "_id")] = $val;
        $returnArray[$key] = $val;
      }
      else {
        // per standard - return custom_fieldID
        $returnArray['custom_' . (CRM_Core_BAO_CustomField::getKeyID($key))] = $val;

        //not standard - but some api did this so guess we should keep - cheap as chips
        $returnArray[$key] = $val;
      }
    }
  }
}

/*
 * Validate fields being passed into API. This function relies on the getFields function working accurately
 * for the given API.
 *
 * As of writing only date was implemented.
 * @param string $entity
 * @param string $action
 * @param array $params -
 * all variables are the same as per civicrm_api
 */
function _civicrm_api3_validate_fields($entity, $action, &$params, $errorMode = NULL) {
  //skip any entities without working getfields functions
  $skippedEntities = array('entity', 'mailinggroup', 'customvalue', 'custom_value', 'mailing_group');
  if (in_array(strtolower($entity), $skippedEntities) || strtolower($action) == 'getfields') {
    return;
  }
  $fields = civicrm_api($entity, 'getfields', array('version' => 3));
  $fields = $fields['values'];
  foreach ($fields as $fieldname => $fieldInfo) {
    switch (CRM_Utils_Array::value('type', $fieldInfo)) {
      case 4:
      case 12:
        //field is of type date or datetime
        _civicrm_api3_validate_date($params, $fieldname, $fieldInfo);
        break;
    }
    if (!empty($errorMode) && strtolower($action) == 'create'
      && CRM_Utils_Array::value('FKClassName', $fieldInfo)
      && CRM_Utils_Array::value($fieldname, $params)
    ) {
      _civicrm_api3_validate_constraint($params, $fieldname, $fieldInfo);
    }
  }
}

/*
 * Validate date fields being passed into API.
 * It currently converts both unique fields and DB field names to a mysql date.
 * It also checks against the RULE:date function. This is a centralisation of code that was scattered and
 * may not be the best thing to do. There is no code level documentation on the existing functions to work off
 *
 * @param array $params params from civicrm_api
 * @param string $fieldname uniquename of field being checked
 * @param array $fieldinfo array of fields from getfields function
 */
function _civicrm_api3_validate_date(&$params, &$fieldname, &$fieldInfo) {
  //should we check first to prevent it from being copied if they have passed in sql friendly format?
  if (CRM_Utils_Array::value($fieldInfo['name'], $params)) {
    //accept 'whatever strtotime accepts
    if (strtotime($params[$fieldInfo['name']]) == 0) {
      throw new exception($fieldInfo['name'] . " is not a valid date: " . $params[$fieldInfo['name']]);
    }
    $params[$fieldInfo['name']] = CRM_Utils_Date::processDate($params[$fieldInfo['name']]);
  }
  if ((CRM_Utils_Array::value('name', $fieldInfo) != $fieldname) && CRM_Utils_Array::value($fieldname, $params)) {
    //If the unique field name differs from the db name & is set handle it here
    if (strtotime($params[$fieldname]) == 0) {
      throw new exception($fieldname . " is not a valid date: " . $params[$fieldname]);
    }
    $params[$fieldname] = CRM_Utils_Date::processDate($params[$fieldname]);
  }
}
/*
 * Validate foreign constraint fields being passed into API.
 *
 * @param array $params params from civicrm_api
 * @param string $fieldname uniquename of field being checked
 * @param array $fieldinfo array of fields from getfields function
 */
function _civicrm_api3_validate_constraint(&$params, &$fieldname, &$fieldInfo) {
  $file = str_replace('_', '/', $fieldInfo['FKClassName']) . ".php";
  require_once ($file);
  $dao = new $fieldInfo['FKClassName'];
  $dao->id = $params[$fieldname];
  $dao->selectAdd();
  $dao->selectAdd('id');
  if (!$dao->find()) {
    throw new exception($fieldname . " is not valid : " . $params[$fieldname]);
  }
}

/**
 * Generic implementation of the "replace" action.
 *
 * Replace the old set of entities (matching some given keys) with a new set of
 * entities (matching the same keys).
 *
 * Note: This will verify that 'values' is present, but it does not directly verify
 * any other parameters.
 *
 * @param string $entity entity name
 * @param array $params params from civicrm_api, including:
 *   - 'values': an array of records to save
 *   - all other items: keys which identify new/pre-existing records
 */
function _civicrm_api3_generic_replace($entity, $params) {

  require_once 'CRM/Core/Transaction.php';
  $tx = new CRM_Core_Transaction();
  try {
    if (!is_array($params['values'])) {
      throw new Exception("Mandatory key(s) missing from params array: values");
    }

    // Extract the keys -- somewhat scary, don't think too hard about it
    $baseParams = $params;
    unset($baseParams['values']);
    unset($baseParams['sequential']);

    // Lookup pre-existing records
    $preexisting = civicrm_api($entity, 'get', $baseParams, $params);
    if (civicrm_error($preexisting)) {
      $tx->rollback();
      return $preexisting;
    }

    // Save the new/updated records
    $creates = array();
    foreach ($params['values'] as $replacement) {
      // Sugar: Don't force clients to duplicate the 'key' data
      $replacement = array_merge($baseParams, $replacement);
      $action      = (isset($replacement['id']) || isset($replacement[$entity . '_id'])) ? 'update' : 'create';
      $create      = civicrm_api($entity, $action, $replacement);
      if (civicrm_error($create)) {
        $tx->rollback();
        return $create;
      }
      foreach ($create['values'] as $entity_id => $entity_value) {
        $creates[$entity_id] = $entity_value;
      }
    }

    // Remove stale records
    $staleIDs = array_diff(
      array_keys($preexisting['values']),
      array_keys($creates)
    );
    foreach ($staleIDs as $staleID) {
      $delete = civicrm_api($entity, 'delete', array(
          'version' => $params['version'],
          'id' => $staleID,
        ));
      if (civicrm_error($delete)) {
        $tx->rollback();
        return $delete;
      }
    }

    return civicrm_api3_create_success($creates, $params);
  }
  catch(PEAR_Exception$e) {
    $tx->rollback();
    return civicrm_api3_create_error($e->getMessage());
  }
  catch(Exception$e) {
    $tx->rollback();
    return civicrm_api3_create_error($e->getMessage());
  }
}

/*
 * returns fields allowable by api
 * @param $entity string Entity to query
 * @param bool $unique index by unique fields?
 */
function _civicrm_api_get_fields($entity, $unique = FALSE, &$params = array(
  )) {
  $unsetIfEmpty = array('dataPattern', 'headerPattern', 'default', 'export', 'import');
  $dao = _civicrm_api3_get_DAO($entity);
  if (empty($dao)) {
    return array();
  }
  $file = str_replace('_', '/', $dao) . ".php";
  require_once ($file);
  $d = new $dao();
  $fields = $d->fields();
  // replace uniqueNames by the normal names as the key
  if (empty($unique)) {
    foreach ($fields as $name => & $field) {
      //getting rid of unused attributes
      foreach ($unsetIfEmpty as $attr) {
        if (empty($field[$attr])) {
          unset($field[$attr]);
        }
      }
      if ($name == $field['name']) {
        continue;
      }
      if (array_key_exists($field['name'], $fields)) {
        $field['error'] = 'name conflict';
        // it should never happen, but better safe than sorry
        continue;
      }
      $fields[$field['name']] = $field;
      $fields[$field['name']]['uniqueName'] = $name;
      unset($fields[$name]);
    }
  }
  $fields += _civicrm_api_get_custom_fields($entity, $params);
  return $fields;
}

/*
 * Return an array of fields for a given entity - this is the same as the BAO function but
 * fields are prefixed with 'custom_' to represent api params
 */
function _civicrm_api_get_custom_fields($entity, &$params) {
  require_once 'CRM/Core/BAO/CustomField.php';
  $customfields = array();
  if (strtolower($entity) == 'contact') {
    $entity = CRM_Utils_Array::value('contact_type', $params);
  }
  $customfields = CRM_Core_BAO_CustomField::getFields($entity,
    FALSE,
    FALSE,
    CRM_Utils_Array::value('contact_sub_type', $params, FALSE),
    NULL,
    empty($params['contact_sub_type']),
    FALSE,
    FALSE
  );

  foreach ($customfields as $key => $value) {
    $customfields['custom_' . $key] = $value;
    unset($customfields[$key]);
  }
  return $customfields;
}
/*
 * Return array of defaults for the given API (function is a wrapper on getfields)
 */
function _civicrm_api3_getdefaults($apiRequest) {
  $defaults = array();

  $result = civicrm_api($apiRequest['entity'],
    'getfields',
    array(
      'version' => 3,
      'action' => $apiRequest['action'],
    )
  );

  foreach ($result['values'] as $field => $values) {
    if (CRM_Utils_Array::value('api.default', $values)) {
      $defaults[$field] = $values['api.default'];
    }
  }
  return $defaults;
}

/*
 * Return array of defaults for the given API (function is a wrapper on getfields)
 */
function _civicrm_api3_getrequired($apiRequest) {
  $required = array('version');

  $result = civicrm_api($apiRequest['entity'],
    'getfields',
    array(
      'version' => 3,
      'action' => $apiRequest['action'],
    )
  );
  foreach ($result['values'] as $field => $values) {
    if (CRM_Utils_Array::value('api.required', $values)) {
      $required[] = $field;
    }
  }
  return $required;
}

/*
 * Fill params array with alternate (alias) values where a field has an alias and that is filled & the main field isn't
 * If multiple aliases the last takes precedence
 */
function _civicrm_api3_swap_out_aliases(&$apiRequest) {
  if (strtolower($apiRequest['action'] == 'getfields')) {
    if (!CRM_Utils_Array::value('action', $apiRequest['params']) && CRM_Utils_Array::value('api_action', $apiRequest['params'])) {
      $apiRequest['params']['action'] = $apiRequest['params']['api_action'];
    }
    return;
  }
  $result = civicrm_api($apiRequest['entity'],
    'getfields',
    array(
      'version' => 3,
      'action' => $apiRequest['action'],
    )
  );

  foreach ($result['values'] as $field => $values) {
    if (CRM_Utils_Array::value('api.aliases', $values)) {
      // if aliased field is not set we try to use field alias
      if (!isset($apiRequest['params'][$field])) {
        foreach ($values['api.aliases'] as $alias) {
          if (isset($apiRequest['params'][$alias])) {
            $apiRequest['params'][$field] = $apiRequest['params'][$alias];
          }
          //unset original field  nb - need to be careful with this as it may bring inconsistencies
          // out of the woodwork but will be implementing only as _spec function extended
          unset($apiRequest['params'][$alias]);
        }
      }
    }
    elseif (!isset($apiRequest['params'][$field])
      && CRM_Utils_Array::value('name', $values)
      && $field != $values['name']
      && isset($apiRequest['params'][$values['name']])
    ) {
      $apiRequest['params'][$field] = $apiRequest['params'][$values['name']];
      // note that it would make sense to unset the original field here but tests need to be in place first
    }
    elseif (!isset($apiRequest['params'][$field])
      && CRM_Utils_Array::value('uniqueName', $values)
      && $field != $values['uniqueName']
      && array_key_exists($values['uniqueName'], $apiRequest['params'])
    ) {
      $apiRequest['params'][$field] = CRM_Utils_Array::value($values['uniqueName'], $apiRequest['params']);
      // note that it would make sense to unset the original field here but tests need to be in place first
    }
  }
}

