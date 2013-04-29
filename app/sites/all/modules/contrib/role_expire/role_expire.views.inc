<?php

/**
 * @file
 * Role Expire Views hooks
 *
 * Views module integration with the role expire module. Exposes the following
 * to Views:
 *  Fields:
 *    "Role expiration date/time" - the date/time that a role expires
 *    "Role expiration role" - the role that expires at the given Role expiration date/time
 *  Filters:
 *    "Role expiration date/time"
 *    "Role expiration role"
 *  Arguments (aka "Contextual Filters"):
 *    "Role expiration role" - on the querystring as a role ID, not as a role name.
 *
 * NOTE: The Views API hook hook_views_api must be defined in the main module.
 * @see role_expire_views_api() in role_expire.module.
 */


/**
 * Implementation of hook_views_data().
 *
 * The purpose of this hook is to tell Views what data we make available.
 */
function role_expire_views_data() {
  $data['role_expire']['table']['group']  = t('User');

  $data['role_expire']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // Expose the role expiration date
  $data['role_expire']['expiry_timestamp'] = array(
    'title' => t('Role expiration date/time'),
    'help' => t('Date and time the role will expire. (See also Role expiration role.)'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Expose the role id from role_expire
  $data['role_expire']['rid'] = array(
    'title' => t('Role expiration role'),
    'help' => t('The Role that corresponds with the Role expiration date/time'),
    // Information for displaying the rid
    'field' => array(
      'handler' => 'role_expire_handler_field_rid',
      'click sortable' => TRUE,
    ),
    // Information for accepting a rid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_users_roles_rid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'rid',
    ),
    // Information for accepting a uid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_user_roles',
    ),
    // Information for sorting on a uid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}

class role_expire_handler_field_rid extends views_handler_field {

  // Derived from views_handler_field_user_roles
  // Purpose: get the *names* that correspond to the role_expire_rids.
  function pre_render($values) {
    $roles = array();
    $this->items = array();

    // Get all the unique role ids into the keys of $roles. Initializing into
    // array_keys helps prevent us from having a list where the same rid appears
    // over and over and over.
    foreach ($values as $result) {
      $roles[$this->get_value($result, NULL, TRUE)] = FALSE;
    }

    if ($roles) {
      $result = db_query("SELECT r.rid, r.name FROM {role} r WHERE r.rid IN (:rids) ORDER BY r.name",
        array(':rids' => array_keys($roles)));
      foreach ($result as $role) {
        $this->items[$role->rid]['role'] = check_plain($role->name);
        $this->items[$role->rid]['rid'] = $role->rid;
      }
    }
  }

  // Render the rid as the role name.
  function render($values) {

    // Return the role name corresponding to the role ID.
    // TODO: Should I be using this->get_value() here?
    $rid = $values->role_expire_rid;
    if ($rid) {
      $role = $this->items[$rid]['role'];
      if (!empty($role)) {
        return $role;
      }
    }
  }
}
