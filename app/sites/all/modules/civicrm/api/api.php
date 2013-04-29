<?php

/**
 * File for the CiviCRM APIv3 API wrapper
 *
 * @package CiviCRM_APIv3
 * @subpackage API
 *
 * @copyright CiviCRM LLC (c) 2004-2011
 * @version $Id: api.php 30486 2010-11-02 16:12:09Z shot $
 */

/*
 * @param string $entity
 *   type of entities to deal with
 * @param string $action
 *   create, get, delete or some special action name.
 * @param array $params
 *   array to be passed to function
 */
function civicrm_api($entity, $action, $params, $extra = NULL) {
  try {
    require_once ('api/v3/utils.php');
    if (!is_array($params)) {
      throw new Exception('Input variable `params` is not an array');
    }
    _civicrm_api3_initialize(TRUE);
    require_once 'CRM/Utils/String.php';
    require_once 'CRM/Utils/Array.php';
    $apiRequest = array();
    $apiRequest['entity'] = CRM_Utils_String::munge($entity);
    $apiRequest['action'] = CRM_Utils_String::munge($action);
    $apiRequest['version'] = civicrm_get_api_version($params);
    $apiRequest['params'] = $params;
    $apiRequest['extra'] = $extra;
    // look up function, file, is_generic
    $apiRequest += _civicrm_api_resolve($apiRequest);

    $errorFnName = ($apiRequest['version'] == 2) ? 'civicrm_create_error' : 'civicrm_api3_create_error';
    if ($apiRequest['version'] > 2) {
      _civicrm_api3_api_check_permission($apiRequest['entity'], $apiRequest['action'], $apiRequest['params']);
    }
    // we do this before we
    _civicrm_api3_swap_out_aliases($apiRequest);
    if (strtolower($action) != 'getfields') {
      if (!CRM_Utils_Array::value('id', $params)) {
        $apiRequest['params'] = array_merge(_civicrm_api3_getdefaults($apiRequest), $apiRequest['params']);
      }
      //if 'id' is set then only 'version' will be checked but should still be checked for consistency
      civicrm_api3_verify_mandatory($apiRequest['params'], NULL, _civicrm_api3_getrequired($apiRequest));
    }
    $function = $apiRequest['function'];
    if ($apiRequest['function'] && $apiRequest['is_generic']) {
      // Unlike normal API implementations, generic implementations require explicit
      // knowledge of the entity and action (as well as $params). Bundle up these bits
      // into a convenient data structure.
      $result = $function($apiRequest);
    }
    elseif ($apiRequest['function'] && !$apiRequest['is_generic']) {
      _civicrm_api3_validate_fields($apiRequest['entity'], $apiRequest['action'], $apiRequest['params']);
      $result = isset($extra) ? $function($apiRequest['params'], $extra) : $function($apiRequest['params']);
    }
    else {
      return $errorFnName("API (" . $apiRequest['entity'] . "," . $apiRequest['action'] . ") does not exist (join the API team and implement it!)");
    }

    if (CRM_Utils_Array::value('format.is_success', $apiRequest['params']) == 1) {
      if ($result['is_error'] === 0) {
        return 1;
      }
      else {
        return 0;
      }
    }
    if (CRM_Utils_Array::value('format.only_id', $apiRequest['params']) && isset($result['id'])) {
      return $result['id'];
    }
    if (CRM_Utils_Array::value('is_error', $result, 0) == 0) {
      _civicrm_api_call_nested_api($apiRequest['params'], $result, $apiRequest['action'], $apiRequest['entity'], $apiRequest['version']);
    }
    if (CRM_Utils_Array::value('format.smarty', $apiRequest['params']) || CRM_Utils_Array::value('format_smarty', $apiRequest['params'])) {
      // return _civicrm_api_parse_result_through_smarty($result,$apiRequest['params']);
    }
    if (function_exists('xdebug_time_index') && CRM_Utils_Array::value('debug', $apiRequest['params'])) {
      $result['xdebug']['peakMemory'] = xdebug_peak_memory_usage();
      $result['xdebug']['memory'] = xdebug_memory_usage();
      $result['xdebug']['timeIndex'] = xdebug_time_index();
    }

    return $result;
  }
  catch(PEAR_Exception$e) {
    if (CRM_Utils_Array::value('format.is_success', $apiRequest['params']) == 1) {
      return 0;
    }
    $err = civicrm_api3_create_error($e->getMessage(), NULL, $apiRequest);
    if (CRM_Utils_Array::value('debug', $apiRequest['params'])) {
      $err['trace'] = $e->getTraceSafe();
    }
    else {
      $err['tip'] = "add debug=1 to your API call to have more info about the error";
    }
    return $err;
  }
  catch(Exception$e) {
    if (CRM_Utils_Array::value('format.is_success', $apiRequest['params']) == 1) {
      return 0;
    }
    return civicrm_api3_create_error($e->getMessage(), NULL, $apiRequest);
  }
}

/**
 * Look up the implementation for a given API request
 *
 * @param $apiRequest array with keys:
 *  - entity: string, required
 *  - action: string, required
 *  - params: array
 *  - version: scalar, required
 *
 * @return array with keys
 *  - function: callback (mixed)
 *  - is_generic: boolean
 */
function _civicrm_api_resolve($apiRequest) {
  static $cache;
  $cachekey = strtolower($apiRequest['entity']) . ':' . strtolower($apiRequest['action']) . ':' . $apiRequest['version'];
  if (isset($cache[$cachekey])) {
    return $cache[$cachekey];
  }

  $camelName = _civicrm_api_get_camel_name($apiRequest['entity'], $apiRequest['version']);
  $actionCamelName = _civicrm_api_get_camel_name($apiRequest['action']);

  // Determine if there is an entity-specific implementation of the action
  $stdFunction = civicrm_api_get_function_name($apiRequest['entity'], $apiRequest['action'], $apiRequest['version']);
  if (function_exists($stdFunction)) {
    // someone already loaded the appropriate file
    // FIXME: This has the affect of masking bugs in load order; this is included to provide bug-compatibility
    $cache[$cachekey] = array('function' => $stdFunction, 'is_generic' => FALSE);
    return $cache[$cachekey];
  }

  $stdFiles = array(
    // By convention, the $camelName.php is more likely to contain the function, so test it first
    'api/v' . $apiRequest['version'] . '/' . $camelName . '.php',
    'api/v' . $apiRequest['version'] . '/' . $camelName . '/' . $actionCamelName . '.php',
  );
  foreach ($stdFiles as $stdFile) {
    require_once 'CRM/Utils/File.php';
    if (CRM_Utils_File::isIncludable($stdFile)) {
      require_once $stdFile;
      if (function_exists($stdFunction)) {
        $cache[$cachekey] = array('function' => $stdFunction, 'is_generic' => FALSE);
        return $cache[$cachekey];
      }
    }
  }

  // Determine if there is a generic implementation of the action
  require_once 'api/v3/Generic.php';
  # $genericFunction = 'civicrm_api3_generic_' . $apiRequest['action'];
  $genericFunction = civicrm_api_get_function_name('generic', $apiRequest['action'], $apiRequest['version']);
  $genericFiles = array(
    // By convention, the Generic.php is more likely to contain the function, so test it first
    'api/v' . $apiRequest['version'] . '/Generic.php',
    'api/v' . $apiRequest['version'] . '/Generic/' . $actionCamelName . '.php',
  );
  foreach ($genericFiles as $genericFile) {
    require_once 'CRM/Utils/File.php';
    if (CRM_Utils_File::isIncludable($genericFile)) {
      require_once $genericFile;
      if (function_exists($genericFunction)) {
        $cache[$cachekey] = array('function' => $genericFunction, 'is_generic' => TRUE);
        return $cache[$cachekey];
      }
    }
  }

  $cache[$cachekey] = array('function' => FALSE, 'is_generic' => FALSE);
  return $cache[$cachekey];
}

/**
 *
 * @deprecated
 */
function civicrm_api_get_function_name($entity, $action, $version = NULL) {
  static $_map = NULL;

  if (empty($version)) {
    $version = civicrm_get_api_version();
  }

  if (!isset($_map[$version])) {

    if ($version === 2) {
      $_map[$version]['event']['get'] = 'civicrm_event_search';
      $_map[$version]['group_roles']['create'] = 'civicrm_group_roles_add_role';
      $_map[$version]['group_contact']['create'] = 'civicrm_group_contact_add';
      $_map[$version]['group_contact']['delete'] = 'civicrm_group_contact_remove';
      $_map[$version]['entity_tag']['create'] = 'civicrm_entity_tag_add';
      $_map[$version]['entity_tag']['delete'] = 'civicrm_entity_tag_remove';
      $_map[$version]['group']['create'] = 'civicrm_group_add';
      $_map[$version]['contact']['create'] = 'civicrm_contact_add';
      $_map[$version]['relationship_type']['get'] = 'civicrm_relationship_types_get';
      $_map[$version]['uf_join']['create'] = 'civicrm_uf_join_add';

      if (isset($_map[$version][$entity][$action])) {
        return $_map[$version][$entity][$action];
      }
    }
  }
  $entity = _civicrm_api_get_entity_name_from_camel($entity);
  // $action = _civicrm_api_get_entity_name_from_camel($action);
  if ($version === 2) {
    return 'civicrm' . '_' . $entity . '_' . $action;
  }
  else {
    return 'civicrm_api3' . '_' . $entity . '_' . $action;
  }
}

/**
 * We must be sure that every request uses only one version of the API.
 *
 * @param $desired_version : array or integer
 *   One chance to set the version number.
 *   After that, this version number will be used for the remaining request.
 *   This can either be a number, or an array(
   .., 'version' => $version, ..).
 *   This allows to directly pass the $params array.
 */
function civicrm_get_api_version($desired_version = NULL) {

  if (is_array($desired_version)) {
    // someone gave the full $params array.
    $params = $desired_version;
    $desired_version = empty($params['version']) ? NULL : (int) $params['version'];
  }
  if (isset($desired_version) && is_integer($desired_version)) {
    $_version = $desired_version;
    // echo "\n".'version: '. $_version ." (parameter)\n";
  }
  else {
    // we will set the default to version 3 as soon as we find that it works.
    $_version = 3;
    // echo "\n".'version: '. $_version ." (default)\n";
  }
  return $_version;
}

/**
 * This function exists ONLY to support API v2 via the API wrapper (which is not actually supported :-)_
 * It was put in basically as part of a big cock up & needs to be deleted but there are still a couple of functions
 * that call it
 *
 * @param $entity deprecated
 * @param $rest_interface deprecated
 * @deprecated
 */
function civicrm_api_include($entity, $rest_interface = FALSE, $version = NULL) {

  $version    = civicrm_get_api_version($version);
  $camel_name = _civicrm_api_get_camel_name($entity, $version);
  $file       = 'api/v' . $version . '/' . $camel_name . '.php';
  if ($rest_interface) {
    $apiPath = substr($_SERVER['SCRIPT_FILENAME'], 0, -15);
    // check to ensure file exists, else die
    if (!file_exists($apiPath . $apiFile)) {
      return self::error('Unknown function invocation.');
    }
    $file = $apiPath . $file;
  }

  if (file_exists(dirname(__FILE__) . DIRECTORY_SEPARATOR . ".." . DIRECTORY_SEPARATOR . $file)) {
    require_once $file;
  }
}

/**
 * Check if the result is an error. Note that this function has been retained from
 * api v2 for convenience but the result is more standardised in v3 and param
 * 'format.is_success' => 1
 * will result in a boolean success /fail being returned if that is what you need.
 *
 * @param  array   $params           (reference ) input parameters
 *
 * @return boolean true if error, false otherwise
 * @static void
 * @access public
 */
function civicrm_error($result) {
  if (is_array($result)) {
    return (array_key_exists('is_error', $result) &&
      $result['is_error']
    ) ? TRUE : FALSE;
  }
  return FALSE;
}

function _civicrm_api_get_camel_name($entity, $version = NULL) {
  static $_map = NULL;

  if (empty($version)) {
    $version = civicrm_get_api_version();
  }

  if (!isset($_map[$version])) {
    $_map[$version]['utils'] = 'utils';
    if ($version === 2) {
      // TODO: Check if $_map needs to contain anything.
      $_map[$version]['contribution'] = 'Contribute';
      $_map[$version]['custom_field'] = 'CustomGroup';
    }
    else {
      // assume $version == 3.
    }
  }
  if (isset($_map[$version][strtolower($entity)])) {
    return $_map[$version][strtolower($entity)];
  }

  $fragments = explode('_', $entity);
  foreach ($fragments as & $fragment) {
    $fragment = ucfirst($fragment);
  }
  // Special case: UFGroup, UFJoin, UFMatch, UFField
  if ($fragments[0] === 'Uf') {
    $fragments[0] = 'UF';
  }
  return implode('', $fragments);
}

/*
 * Call any nested api calls
 */
function _civicrm_api_call_nested_api(&$params, &$result, $action, $entity, $version) {
  $entity = _civicrm_api_get_entity_name_from_camel($entity);
  foreach ($params as $field => $newparams) {
    if ((is_array($newparams) || $newparams === 1) && substr($field, 0, 3) == 'api') {

      // 'api.participant.delete' => 1 is a valid options - handle 1 instead of an array
      if ($newparams === 1) {
        $newparams = array('version' => $version);
      }
      // can be api_ or api.
      $separator = $field[3];
      if (!($separator == '.' || $separator == '_')) {
        continue;
      }
      $subAPI = explode($separator, $field);

      $subaction = empty($subAPI[2]) ? $action : $subAPI[2];
      $subParams = array();
      $subEntity = $subAPI[1];

      foreach ($result['values'] as $idIndex => $parentAPIValues) {

        if (strtolower($subEntity) != 'contact') {
          //contact spits the dummy at activity_id so what else won't it like?
          //set entity_id & entity table based on the parent's id & entity. e.g for something like
          //note if the parent call is contact 'entity_table' will be set to 'contact' & 'id' to the contact id from
          //the parent call.
          //in this case 'contact_id' will also be set to the parent's id
          $subParams["entity_id"] = $parentAPIValues['id'];
          $subParams['entity_table'] = 'civicrm_' . _civicrm_api_get_entity_name_from_camel($entity);
          $subParams[strtolower($entity) . "_id"] = $parentAPIValues['id'];
        }
        if (strtolower($entity) != 'contact' && CRM_Utils_Array::value(strtolower($subEntity . "_id"), $parentAPIValues)) {
          //e.g. if event_id is in the values returned & subentity is event then pass in event_id as 'id'
          //don't do this for contact as it does some wierd things like returning primary email &
          //thus limiting the ability to chain email
          //TODO - this might need the camel treatment
          $subParams['id'] = $parentAPIValues[$subEntity . "_id"];
        }

        if (CRM_Utils_Array::value('entity_table', $result['values'][$idIndex]) == $subEntity) {
          $subParams['id'] = $result['values'][$idIndex]['entity_id'];
        }
        // if we are dealing with the same entity pass 'id' through (useful for get + delete for example)
        if (strtolower($entity) == strtolower($subEntity)) {
          $subParams['id'] = $result['values'][$idIndex]['id'];
        }


        $subParams['version'] = $version;
        $subParams['sequential'] = 1;
        if (array_key_exists(0, $newparams)) {
          // it is a numerically indexed array - ie. multiple creates
          foreach ($newparams as $entity => $entityparams) {
            $subParams = array_merge($subParams, $entityparams);
            _civicrm_api_replace_variables($subAPI[1], $subaction, $subParams, $result['values'][$idIndex], $separator);
            $result['values'][$result['id']][$field][] = civicrm_api($subEntity, $subaction, $subParams);
          }
        }
        else {

          $subParams = array_merge($subParams, $newparams);
          _civicrm_api_replace_variables($subAPI[1], $subaction, $subParams, $result['values'][$idIndex], $separator);
          $result['values'][$idIndex][$field] = civicrm_api($subEntity, $subaction, $subParams);
        }
      }
    }
  }
}

/*
 * Swap out any $values vars - ie. the value after $value is swapped for the parent $result
 * 'activity_type_id' => '$value.testfield',
   'tag_id'  => '$value.api.tag.create.id',  
    'tag1_id' => '$value.api.entity.create.0.id'
 */
function _civicrm_api_replace_variables($entity, $action, &$params, &$parentResult, $separator = '.') {


  foreach ($params as $field => $value) {

    if (is_string($value) && substr($value, 0, 6) == '$value') {
      $valuesubstitute = substr($value, 7);

      if (!empty($parentResult[$valuesubstitute])) {
        $params[$field] = $parentResult[$valuesubstitute];
      }
      else {

        $stringParts = explode($separator, $value);
        unset($stringParts[0]);

        $fieldname = array_shift($stringParts);

        //when our string is an array we will treat it as an array from that . onwards
        $count = count($stringParts);
        while ($count > 0) {
          $fieldname .= "." . array_shift($stringParts);
          if (is_array($parentResult[$fieldname])) {
            $arrayLocation = $parentResult[$fieldname];
            foreach ($stringParts as $key => $value) {
              $arrayLocation = $arrayLocation[$value];
            }
            $params[$field] = $arrayLocation;
          }
          $count = count($stringParts);
        }
      }
    }
  }
}

/*
 * Convert possibly camel name to underscore separated entity name
 *
 * @param string $entity entity name in various formats e.g. Contribution, contribution, OptionValue, option_value, UFJoin, uf_join
 * @return string $entity entity name in underscore separated format
 */
function _civicrm_api_get_entity_name_from_camel($entity) {
  if ($entity == strtolower($entity)) {
    $entity = $entity;
  }
  else {
    $entity = ltrim(strtolower(str_replace('U_F',
          'uf',
          // That's CamelCase, beside an odd UFCamel that is expected as uf_camel
          preg_replace('/(?=[A-Z])/', '_$0', $entity)
        )), '_');
  }
  return $entity;
}

/*
 * Parses result through smarty
 * @param array $result result of API call
 */
function _civicrm_api_parse_result_through_smarty(&$result, &$params) {
  require_once 'CRM/Core/Smarty.php';
  $smarty = CRM_Core_Smarty::singleton();
  $smarty->assign('result', $result);
  $template = CRM_Utils_Array::value('format.smarty', $params, $params['format_smarty']);
  return $smarty->fetch("../templates/" . $template);
}

