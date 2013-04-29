<?php

/**
 * $apiRequest is an array with keys:
 *  - entity: string
 *  - action: string
 *  - version: string
 *  - function: callback (mixed)
 *  - params: array, varies
 */
function civicrm_api3_generic_getfields($apiRequest) {
  if (empty($apiRequest['params']['action'])) {
    return civicrm_api3_create_success(_civicrm_api_get_fields($apiRequest['entity']));
  }
  $unique = TRUE;
  // should this be passed in already lower?
  $entity = strtolower($apiRequest['entity']);
  // defaults based on data model and API policy
  switch (strtolower($apiRequest['params']['action'])) {
    case 'getfields':
      return civicrm_api3_create_success(_civicrm_api_get_fields($apiRequest['entity'], $apiRequest['params']));

    case 'create':
    case 'update':
    case 'replace':
      $unique = FALSE;
    case 'get':
      $metadata = _civicrm_api_get_fields($apiRequest['entity'], $unique, $apiRequest['params']);
      if (empty($metadata['id']) && !empty($metadata[$apiRequest['entity'] . '_id'])) {
        $metadata['id'] = $metadata[$entity . '_id'];
        $metadata['id']['api.aliases'] = array($entity . '_id');
        unset($metadata[$entity . '_id']);
      }
      break;

    case 'delete':
      $metadata = array(
        'id' => array('title' => 'Unique Identifier',
          'api.required' => 1,
          'api.aliases' => array($entity . '_id'),
        ));
      break;

    default:
      // oddballs are on their own
      $metadata = array();
  }
  // find any supplemental information
  $hypApiRequest = array('entity' => $apiRequest['entity'], 'action' => $apiRequest['params']['action'], 'version' => $apiRequest['version']);
  $hypApiRequest += _civicrm_api_resolve($hypApiRequest);
  $helper = '_' . $hypApiRequest['function'] . '_spec';
  if (function_exists($helper)) {
    // alter
    $helper($metadata);
  }
  return civicrm_api3_create_success($metadata);
}

function civicrm_api3_generic_getcount($apiRequest) {
  $result = civicrm_api($apiRequest['entity'], 'get', $apiRequest['params']);
  return $result['count'];
}

function civicrm_api3_generic_getsingle($apiRequest) {
  // so the first entity is always result['values'][0]
  $apiRequest['params']['sequential'] = 1;
  $result = civicrm_api($apiRequest['entity'], 'get', $apiRequest['params']);
  if ($result['is_error'] !== 0) {
    return $result;
  }
  if ($result['count'] === 1) {
    return $result['values'][0];
  }
  if ($result['count'] !== 1) {
    return civicrm_api3_create_error("Expected one " . $apiRequest['entity'] . " but found " . $result['count'], array('count' => $result['count']));
  }
  return civicrm_api3_create_error("Undefined behavior");
}

function civicrm_api3_generic_getvalue($apiRequest) {
  $apiRequest['params']['sequential'] = 1;
  $result = civicrm_api($apiRequest['entity'], 'get', $apiRequest['params']);
  if ($result['is_error'] !== 0) {
    return $result;
  }
  if ($result['count'] !== 1) {
    $result = civicrm_api3_create_error("Expected one " . $apiRequest['entity'] . " but found " . $result['count'], array('count' => $result['count']));
    return $result;
  }

  // we only take "return=" as valid options
  if (CRM_Utils_Array::value('return', $apiRequest['params'])) {
    if (!isset($result['values'][0][$apiRequest['params']['return']])) {
      return civicrm_api3_create_error("field " . $apiRequest['params']['return'] . " unset or not existing", array('invalid_field' => $apiRequest['params']['return']));
    }

    return $result['values'][0][$apiRequest['params']['return']];
  }

  return civicrm_api3_create_error("missing param return=field you want to read the value of", array('error_type' => 'mandatory_missing', 'missing_param' => 'return'));
}

function civicrm_api3_generic_replace($apiRequest) {
  return _civicrm_api3_generic_replace($apiRequest['entity'], $apiRequest['params']);
}

