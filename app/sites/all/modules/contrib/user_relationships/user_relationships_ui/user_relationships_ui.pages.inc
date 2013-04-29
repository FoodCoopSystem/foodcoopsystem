<?php
/**
 * @file
 * Various user relationships admin and user pages
 */

/**
 * Main list of relationships for a specified user
 */
function user_relationships_page($account = NULL, $rtid = NULL) {
  global $user;
  if (!$account || !is_object($account) || !$account->uid) {
    $account = $user;
  }

  // Check if this is a valid rtid.
  if (!empty($rtid) && !user_relationships_type_load($rtid)) {
    return MENU_NOT_FOUND;
  }

  $args = array('user' => $account->uid, 'approved' => TRUE);
  $relationship_type = NULL;
  if (!empty($rtid)) {
    $relationship_type = user_relationships_type_load($rtid);
    $args['rtid'] = $rtid;
  }
  else {
    // Get hidden relationships.
    $hidden = db_query('SELECT rtid FROM {user_relationships_ui_settings} WHERE hide = 1')->fetchCol();
    if (!empty($hidden)) {
      $args['exclude_rtids'] = $hidden;
    }
  }

  $options = array(
    'include_user_info' => TRUE,
    'paging' => variable_get('user_relationships_relationships_per_page', 16),
  );

  $relationships = user_relationships_load($args, $options);

  if ($account->uid == $user->uid) {
    $msg = isset($relationship_type)
           ? t("My %relationships", array('%relationships' => $relationship_type->plural_name ? ur_tt("user_relationships:rtid:" . $relationship_type->rtid . ":plural_name", $relationship_type->plural_name) : ur_tt("user_relationships:rtid:" . $relationship_type->rtid . ":name", $relationship_type->name)))
           : t("My relationships");
  }
  else {
    $msg = isset($relationship_type)
           ? t("%username's %relationships", array('%username' => format_username($account), '%relationships' => $relationship_type->plural_name ? ur_tt("user_relationships:rtid:" . $relationship_type->rtid . ":plural_name", $relationship_type->plural_name) : ur_tt("user_relationships:rtid:" . $relationship_type->rtid . ":name", $relationship_type->name)))
           : t("%username's relationships", array('%username' => format_username($account)));
  }
  drupal_set_title($msg, PASS_THROUGH);

  $header = user_relationships_ui_get_table_header(user_relationships_ui_check_access('delete', $account, $relationship_type));

  $rows = array();
  foreach ($relationships as $relationship) {
    $rows[$relationship->rid] = user_relationships_ui_get_table_row($relationship, $account);
    if (count($rows[$relationship->rid]) < count($header)) {
      // Add an empty space to the operations table row.
      $rows[$relationship->rid][] = '&nbsp;';
    }
   }

  if (!empty($rtid)) {
    $empty = t('You do not have any %plural_name.', array('%plural_name' => ur_tt("user_relationships:rtid:$relationship_type->rtid:plural_name", $relationship_type->plural_name)));
  }
  else {
    $empty = t('You do not have any relationships with other users.');
  }

  $output = array(
    'list' => array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#header' => $header,
      '#empty' => $empty,
      '#attributes' => array('class' => array('user-relationships-listing-table')),
    ),
    // Theme pager so that it uses the correct pager query.
    'pager' => array(
      '#markup' => theme('pager'),
    )
  );

  return $output;
}

/**
 * List of pending requests from other users
 */
function user_relationships_pending_requests_page($column, $account = NULL) {
  global $user;
  if (!$account) {
    $account = $user;
  }

  if (!is_object($account) || !$account->uid) {
    return MENU_NOT_FOUND;
  }

  if ($column == 'requester_id') {
    $permission = 'request';
    if ($account->uid == $user->uid) {
      $msg = t('My relationships (sent requests)');
      $empty = t('You have not sent any relationship requests that are currently pending.');
    }
    else {
      $msg = t("%username's relationships (sent requests)", array('%username' => format_username($account)));
      $empty = t('%username has not sent any relationship requests that are currently pending.', array('%username' => format_username($account)));
    }
  }
  else {
    $permission = 'approve';
    if ($account->uid == $user->uid) {
      $msg = t('My relationships (received requests)');
      $empty = t('You have not received any relationship requests that are currently pending.');
    }
    else {
      $msg = t("%username's relationships (received requests)", array('%username' => format_username($account)));
      $empty = t('%username has not received any relationship requests that are currently pending.', array('%username' => format_username($account)));
    }
  }
  drupal_set_title($msg, PASS_THROUGH);

  $options = array(
    'include_user_info' => TRUE,
    'paging' => variable_get('user_relationships_relationships_per_page', 16),
  );

  $header = user_relationships_ui_get_table_header(user_relationships_ui_check_access($permission, $account));
  $relationships = user_relationships_load(array($column => $account->uid, 'approved' => FALSE), $options);
  $rows = array();
  foreach ($relationships as $relationship) {
    $rows[$relationship->rid] = user_relationships_ui_get_table_row($relationship, $account);
    if (count($rows[$relationship->rid]) < count($header)) {
      // Add an empty space to the operations table row.
      $rows[$relationship->rid][] = '&nbsp;';
    }
  }

  $output['list'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
    '#empty' => $empty,
    'attributes' => array('class' => array('user-relationships-pending-listing-table')),
  );
  $output['pager'] = array(
    '#markup' => theme('pager'),
  );

  return $output;
}

/**
 * Builds a table row array from a relationship.
 *
 * @param $relationship
 *   Relationship object.
 * @param $account
 *   User account object for which the relationship is being displayed.
 *
 * @return
 *   Array with the table row content.
 *
 * @see hok_user_relationships_ui_table_row_alter()
 */
function user_relationships_ui_get_table_row($relationship, $account) {
  global $user;

  $this_user_str  = $account->uid == $relationship->requestee_id ? 'requester' : 'requestee';
  $this_user      = $relationship->{$this_user_str};

  $row = array(
    theme('username', array('account' => $this_user)),
    ur_tt("user_relationships:rtid:$relationship->rtid:name", $relationship->name) . ($relationship->is_oneway ? ($this_user_str == 'requestee' ? t(' (You to Them)') : t(' (Them to You)')) : NULL),
  );

  $permission = $account->uid == $relationship->requestee_id ? 'request' : 'approve';
  $links = array();
  if ($relationship->approved) {
    if (user_relationships_ui_check_access('delete', $account, $relationship)) {
      $links[] = theme('user_relationships_remove_link', array('uid' => $account->uid, 'rid' => $relationship->rid));
    }
  }
  else if (user_relationships_ui_check_access($permission, $account, $relationship)) {
    if ($this_user_str == 'requestee') {
      // Sent requests, display cancel link.
      $links[] = theme('user_relationships_pending_request_cancel_link', array('uid' => $account->uid, 'rid' => $relationship->rid));
    }
    else {
      // Received requests, display approve and decline links.
      $links[] = theme('user_relationships_pending_request_approve_link', array('uid' => $account->uid, 'rid' => $relationship->rid));
      $links[] = theme('user_relationships_pending_request_disapprove_link', array('uid' => $account->uid, 'rid' => $relationship->rid));
    }
  }
  if (!empty($links)) {
    $row[] = implode(' ', $links);
  }

  if (variable_get('user_relationships_show_user_pictures', 0)) {
    array_unshift($row, theme('user_picture', array('account' => $this_user)));
  }

  drupal_alter('user_relationships_ui_table_row', $row, $relationship, $account);

  return $row;
}

/**
 * Return the table header for a relationship listing.
 *
 * @param $edit_access
 *   If the user has edit access.
 *
 * @return
 *   Array with the table header definition.
 *
 * @see hook_user_relationships_ui_table_header_alter()
 */
function user_relationships_ui_get_table_header($edit_access) {
  $header = array(t('User'), t('Relationship'), t('Operations'));

  if (!$edit_access) {
    // Remove operations column.
    array_pop($header);
  }

  if (variable_get('user_relationships_show_user_pictures', 0)) {
    array_unshift($header, t('Picture'));
  }

  drupal_alter('user_relationships_ui_table_header', $header, $edit_access);

  return $header;
}
