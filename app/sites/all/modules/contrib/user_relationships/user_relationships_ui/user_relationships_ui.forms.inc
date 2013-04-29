<?php

/**
 * @file
 * User Relationships forms
 */
module_load_include('inc', 'user_relationships_ui', 'user_relationships_ui.actions');

/**
 * Request new user to user relationship
 * @param $requester user object for the requesting user
 * @param $requestee user object for the target user
 * @param $form_values array $form['values'] when called from form_alter()
 */
function user_relationships_ui_request_form($requester, $requestee, $form_values = NULL) {
  $relationships = user_relationships_get_requestable_rtypes($requester, $requestee);
  //try to find out desired relationship type
  $default_relationship = NULL;
  if (isset($form_values['rtid'])) {//given through form
    $default_relationship = $form_values['rtid'];
  }
  elseif (is_numeric(arg(3)) && user_relationships_type_load(arg(3))) {//given through URL arguments, e.g. relationship/{uid}/request/{rtid}
    $default_relationship = arg(3);
  }
  elseif (count($relationships) == 1) {//preselect the only one.
    $rtids = array_keys($relationships);
    $default_relationship = $rtids[0];
  }

  //verify default relationship may be requested
  $default_relationship = isset($relationships[$default_relationship]) ? $default_relationship : NULL;

  if (count($relationships)) {
    //hide control if the relationship is chosen
    if ($default_relationship && variable_get('user_relationships_show_direct_links', 1)) {
      $form['rtid'] = array(
        '#type' => 'hidden',
        '#default_value' => $default_relationship,
      );
    }
    else {
      $form['rtid'] = array(
        '#title'          => t('What type of relationship are you requesting with !name?', array('!name' => theme('username', array('account' => $requestee)))),
        '#type'           => 'radios',
        '#options'        => $relationships,
        '#default_value'  => $default_relationship,
      );
    }
  }
  $form['requester'] = array(
    '#type'   => 'value',
    '#value'  => $requester->uid,
  );
  $form['requestee'] = array(
    '#type'   => 'value',
    '#value'  => $requestee->uid,
  );

  return $form;
}

/**
 * Ajax handler function that is utilized to display the form
 * in a popup rather than having the full page reload for the users
 */
function user_relationships_ui_request_ajax($requestee) {
  $form = drupal_get_form('user_relationships_ui_request', $requestee);
  if (isset($_GET['ajax'])) {
    print drupal_render($form);
  }
  else {
    return $form;
  }
}

/**
 * Request a new relationship with another user
 */
function user_relationships_ui_request($form, &$form_state, $requestee) {
  global $user;
  $requester = $user;

  drupal_set_title(t('Request relationship'));

  $script = '<script type="text/javascript">setTimeout(\'Drupal.user_relationships_ui.hidePopup()\', 10000);</script>';
  if (empty($requestee->name)) {
    $form['error'] = array(
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#markup' => user_relationships_get_message('non_existent_user') . $script
    );
    return $form;
  }
  elseif ($user->uid == $requestee->uid) {
    $form['error'] = array(
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#markup' => user_relationships_get_message('self_request') . $script
    );
    return $form;
  }
  elseif (!variable_get('user_relationships_allow_multiple', TRUE) &&
           user_relationships_load(array('between' => array($requester->uid, $requestee->uid)), array('count' => TRUE))) {
    $form['error'] = array(
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#markup' => user_relationships_get_message('too_many_relationships') . $script
    );
    return $form;
  }
  // This is the default which means we will display the form
  $form = user_relationships_ui_request_form($requester, $requestee, $form_state);
  if (!isset($form['rtid'])) {
    $data['error'] = array(
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#markup' => user_relationships_get_message('too_many_relationships') . $script
    );
    return $data;
  }
  else {
    if ($form['rtid']['#default_value']) {
      $relationship = user_relationships_type_load($form['rtid']['#default_value']);
    }
    // Respect destination param if given.
    if (isset($_GET['destination'])) {
      $dest = $_GET['destination'];
    }
    else {
      $dest = "user/$requestee->uid";
    }
    $form = confirm_form(
      $form,
      t('Request relationship'),
      $dest,
      isset($relationship) ?
      ($relationship->requires_approval
       ? t('Are you sure you want to send a new %rel_name request to !name?', array('%rel_name' => ur_tt("user_relationships:rtid:$relationship->rtid:name", $relationship->name), '%rel_plural_name' => ur_tt("user_relationships:rtid:$relationship->rtid:plural_name", $relationship->plural_name), '!name' => theme('username', array('account' => $requestee))))
       : t("Are you sure you want to become !name's %rel_name?", array('!name' => theme('username', array('account' => $requestee)), '%rel_name' => ur_tt("user_relationships:rtid:$relationship->rtid:name", $relationship->name)))) :
      '',
      t('Send'), t('Cancel'),
      'user_relationships_request_confirm'
    );
    return $form;
  }
}

/**
 * Ajax handler function that is utilized to display the form
 * in a popup rather than having the full page reload for the users
 */
function user_relationships_ui_pending_requested_ajax($action, $account, $relationship) {
  $form = drupal_get_form('user_relationships_ui_pending_requested', $action, $account, $relationship);
  if (isset($_GET['ajax'])) {
    print drupal_render($form);
  }
  else {
    return $form;
  }
}

/**
 * Approve, Decline, or Cancel confirmation form
 */
function user_relationships_ui_pending_requested($form, &$form_state, $action, $account, $relationship) {
  global $user;
  $viewed_id = $account->uid;

  $form['rid'] = array(
    '#type'   => 'value',
    '#value'  => $relationship->rid
  );
  $form['action'] = array(
    '#type'   => 'value',
    '#value'  => $action
  );
  $form['viewed_id'] = array(
    '#type'   => 'value',
    '#value'  => $viewed_id
  );

  //pick the correct question message: approve and disapprove are requests to current user, cancel is a request from current user
  switch ($action) {
    case 'approve':
      $confirmation_message = t('Are you sure you want to approve the %relationship_name relationship request from !name?', array(
        '@action'             => $action,
        '%relationship_name'  => ur_tt("user_relationships:rtid:$relationship->rtid:name", $relationship->name),
        '!name'               => theme('username', array('account' => user_load(($viewed_id == $relationship->requester_id ? $relationship->requestee_id : $relationship->requester_id))))
      ));
      $action_message = t('Approve relationship');
      $request_url = 'relationships/received';
      break;
    case 'disapprove':
      $confirmation_message = t('Are you sure you want to decline the %relationship_name relationship request from !name?', array(
        '@action'             => $action,
        '%relationship_name'  => ur_tt("user_relationships:rtid:$relationship->rtid:name", $relationship->name),
        '!name'               => theme('username', array('account' => user_load(($viewed_id == $relationship->requester_id ? $relationship->requestee_id : $relationship->requester_id))))
      ));
      $action_message = t('Decline relationship');
      $request_url = 'relationships/received';
      break;
    default:
      $confirmation_message = t('Are you sure you want to cancel your %relationship_name relationship request to !name?', array(
        '@action'             => $action,
        '%relationship_name'  => ur_tt("user_relationships:rtid:$relationship->rtid:name", $relationship->name),
        '!name'               => theme('username', array('account' => user_load(($viewed_id == $relationship->requester_id ? $relationship->requestee_id : $relationship->requester_id))))
      ));
      $action_message = t('Cancel relationship');
      $request_url = 'relationships/sent';
  }

  // http://drupal.org/node/489954 respect destination param if given
  if (isset($_GET['destination'])) {
    $dest = $_GET['destination'];
  }
  else {

    $dest = ($viewed_id == $user->id ? $request_url : "user/{$viewed_id}/$request_url");
  }

  $output = confirm_form(
    $form,
    $action_message,
    $dest,
    $confirmation_message,
    t('Yes'), t('No'),
    'user_relationships_approve_confirm'
  );

  return $output;
}

/**
 * Ajax handler function that is utilized to display the form
 * in a popup rather than having the full page reload for the users
 */
function user_relationships_ui_remove_ajax($account, $relationship) {
  //#382668 makes sure a valid relationship id was supplied
  if (!$relationship->rid) {
    if (isset($_GET['ajax'])) {
      print '';
    }
    drupal_goto();
  }
  $form = drupal_get_form('user_relationships_ui_remove', $account, $relationship);
  if (isset($_GET['ajax'])) {
    print drupal_render($form);
  }
  else {
    return $form;
  }
}

/**
 * Confirm relationship removal.
 */
function user_relationships_ui_remove($form, &$form_state, $account, $relationship) {
  global $user;

  $viewed_id = $account->uid;

  $form['rid'] = array(
    '#type'   => 'value',
    '#value'  => $relationship->rid
  );
  $form['viewed_id'] = array(
    '#type'   => 'value',
    '#value'  => $viewed_id
  );

  // Respect destination param if given.
  if (isset($_GET['destination'])) {
    $dest = $_GET['destination'];
  }
  else {
    $dest = ($viewed_id == $user->uid ? 'relationships' : "user/{$viewed_id}/relationships");
  }

  $output = confirm_form(
    $form,
    t('Remove relationship'),
    $dest,
    t("Are you sure you want to delete the %relationship_name relationship with !name?", array(
      '%relationship_name'  => ur_tt("user_relationships:rtid:$relationship->rtid:name", $relationship->name),
      '!name'               => theme('username', array('account' => user_load(($viewed_id == $relationship->requester_id ? $relationship->requestee_id : $relationship->requester_id))))
    )),
    t('Yes'), t('No'),
    'user_relationships_remove_confirm'
  );

  return $output;
}
