<?php

/**
 * @file
 * User Relationships Actions
 */


/**
 * Validate relationship request.
 */
function user_relationships_ui_request_validate($form, &$form_state) {
  $requester = user_load($form_state['values']['requester']);
  $requestee = user_load($form_state['values']['requestee']);
  $rtid = $form_state['values']['rtid'];
  $rtype = user_relationships_type_load($rtid);

  //check that a type has been chosen
  $current_relationships = user_relationships_load(array('between' => array($requester->uid, $requestee->uid)), array('sort' => 'rtid'));
  //check for multiple relationships
  if (!variable_get('user_relationships_allow_multiple', TRUE) && count($current_relationships)) {
  form_set_error('rtid', user_relationships_get_message('too_many_relationships'));
    if (isset($_GET['ajax'])) {
      drupal_goto();
    }
    else {
      return;
    }
  }
  //check user is allowed to request this type
  if (!user_relationships_can_request($requester, $rtype)) {
    $rtype->requester = $requester;
    $rtype->requestee = $requestee;
    form_set_error('rtid', user_relationships_get_message('relationship_type_not_allowed', $rtype));
    if (isset($_GET['ajax'])) {
      drupal_goto();
    }
    else {
      return;
    }
  }

  //check user is allowed to receive this type
  if (!user_relationships_can_receive($requestee, $rtype)) {
    $rtype->requester = $requester;
    $rtype->requestee = $requestee;
    form_set_error('rtid', user_relationships_get_message('relationship_type_not_allowed', $rtype));
    if (isset($_GET['ajax'])) {
      drupal_goto();
    }
    else {
      return;
    }
  }

  //check if this request contradicts existing relationships or requests of this type
  //gather rtids of relationships that have already been requested from requester to requestee
  $test_relationships = array();
  foreach ($current_relationships as $key => $value) {
    foreach ($value as $val_rel) {
      //existing two way relationships count no matter the direction
      if (!$val_rel->is_oneway) {
        $test_relationships[$key]=$key;
        continue;
      }
      //if it's one way, but not reciprocal, then it counts no matter the direction
      if (!$val_rel->is_reciprocal) {
        $test_relationships[$key]=$key;
        continue;
      }
      //remaining case is a reciprocal relationship - it counts unless the request is in the opposite direction
      if ($requester->uid == $val_rel->requester_id) {
        $test_relationships[$key]=$key;
      }
    }
  }
  //check if there aren't any available (not yet used) types that can be established
  $select = db_select('user_relationship_types');
  $select->addExpression('COUNT(*)', 'count');
  if ($test_relationships) {
    $select->condition('rtid', $test_relationships, 'NOT IN');
  }
  if (!$select->execute()->fetchField()) {
    drupal_set_message(user_relationships_get_message('too_many_relationships'), 'error');
    drupal_goto();
  }
  //check if desired relationship type is among the unused ones
  elseif (isset($test_relationships[$rtid])) {
    $message_name = $test_relationships[$rtid]->approved ? 'existing_relationship' : 'existing_request';
    form_set_error('rtid', user_relationships_get_message($message_name, NULL, array(
      '!requester'                => theme('username', array('account' => $requester)),
      '!requestee'                => theme('username', array('account' => $requestee)),
      '%relationship_name'        => $current_relationships[$rtid][0]->name,
      '%relationship_plural_name' => $current_relationships[$rtid][0]->plural_name
    )));
  }
}

/**
 * Process relationship request.
 */
function user_relationships_ui_request_submit($form, &$form_state) {
  //#578372 leave all form values in the object for other functions to use
  $relationship = (object) $form_state['values'];
  $relationship->requester = user_load($form_state['values']['requester']);
  $relationship->requestee = user_load($form_state['values']['requestee']);
  $relationship->type = user_relationships_type_load($form_state['values']['rtid']);

  $relationship = user_relationships_request_relationship($relationship);

  if ($relationship === FALSE) {
    drupal_set_message(user_relationships_get_message('unknown_error', $relationship));
  }

  drupal_set_message(user_relationships_get_message(($relationship->approved ? 'pre_approved' : 'submitted'), $relationship));
}


/**
 * Approve, Decline, or Cancel a relationship request
 */
function user_relationships_ui_pending_requested_submit($form, &$form_state) {
  global $user;

  $relationships = user_relationships_load(array('rid' => $form_state['values']['rid']), array('include_user_info' => TRUE));
  $relationship = $relationships[$form_state['values']['rid']];
  $requester    = $relationship->requester;
  $requestee    = $relationship->requestee;

  switch ($form_state['values']['action']) {
  case 'approve':
    if (!user_relationships_ui_check_access('approve', NULL, $relationship)) {
      drupal_access_denied();
      exit();
    }
    $relationship->approved = TRUE;
    user_relationships_save_relationship($relationship, $form_state['values']['action']);
    drupal_set_message(user_relationships_get_message('accepted', $relationship));
    break;

  case 'cancel':
    if (!user_relationships_ui_check_access('request', NULL, $relationship)) {
      drupal_access_denied();
      exit();
    }
    user_relationships_delete_relationship($relationship, $user, $form_state['values']['action']);
    drupal_set_message(user_relationships_get_message($form_state['values']['action'], $relationship));
    break;

  case 'disapprove':
    if (!user_relationships_ui_check_access('approve', NULL, $relationship)) {
      drupal_access_denied();
      exit();
    }
    user_relationships_delete_relationship($relationship, $user, $form_state['values']['action']);
    drupal_set_message(user_relationships_get_message($form_state['values']['action'], $relationship));
    break;

  default:
    drupal_set_message(user_relationships_get_message('default'));
  }

  $form_state['redirect'] = str_replace('destination=', '', drupal_get_destination());
}


/**
 * Remove a relationship
 */
function user_relationships_ui_remove_submit($form, &$form_state) {
  global $user;

  $relationship = user_relationships_load($form_state['values']['rid']);

  if (!user_relationships_ui_check_access('delete', NULL, $relationship)) {
    drupal_access_denied();
    exit();
  }

  user_relationships_delete_relationship($relationship, $user);
  drupal_set_message(user_relationships_get_message('removed', $relationship));

  return "user/{$form_state['values']['viewed_id']}/relationships";
}
