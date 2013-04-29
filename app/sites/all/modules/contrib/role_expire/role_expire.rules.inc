<?php

/**
 * @file
 * Rules integration for the role expire module.
 */

/**
 * Implementation of hook_rules_action_info().
 */
function role_expire_rules_action_info() {
  $items['role_expire_rules_action_set_role_expire'] = array(
    'label' => t('Add expire time to role'),
    'help' => t('Add an expiration time to a role a user has. This action does not add the role, only the expiration time.'),
    'arguments' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('The user which the expiration time will be added'),
      ),
    ),
    'eval input' => array('timestamp'),
    'module' => 'Role expire',
  );

  $items['role_expire_rules_action_remove_role_expire'] = array(
    'label' => t('Remove expire time from role'),
    'help' => t('Remove an expiration time to a role a user has. This action does not remove the role, only the expiration time.'),
    'arguments' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('The user which the expiration time will be removed'),
      ),
    ),
    'module' => 'Role expire',
  );

  return $items;
}

/**
 * Action: Add expire time to role.
 */
function role_expire_rules_action_set_role_expire($account, $settings) {
  role_expire_write_record($account->uid, $settings['rid'], strtotime($settings['timestamp']));
}

/**
 * Action: Add expire time to role form configuration.
 */
function role_expire_rules_action_set_role_expire_form($settings = array(), &$form) {
  $settings += array('rid' => '', 'timestamp' => '');


  // The role ID.
  $form['settings']['rid'] = _role_expire_get_rids($settings);

  //The timestamp.
  $form['settings']['timestamp'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiration time'),
    '#required' => TRUE,
    '#description' => t('Enter the time the role will expire. Enter date and time in format: <em>dd-mm-yyyy hh:mm:ss</em> or use relative time i.e. now, 1 day, 2 months, 1 year, 3 years.'),
    '#default_value' => $settings['timestamp'],
  );
}

/**
 * Action: Remove expire time from role
 */
function role_expire_rules_action_remove_role_expire($account, $settings) {
  role_expire_delete_record($account->uid, $settings['rid']);
}

/**
 * Action: Add expire time to role form configuration.
 */
function role_expire_rules_action_remove_role_expire_form($settings = array(), &$form) {
  $settings += array('rid' => '');

  // The role ID.
  $form['settings']['rid'] = _role_expire_get_rids($settings);
}


/**
 * Helper function; Return a form element with the valid roles.
 * @return unknown_type
 */
function _role_expire_get_rids($settings) {
  // Get valid roles.
  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  // The role ID.
  return array(
    '#type' => 'select',
    '#options' => $roles,
    '#title' => t('Role'),
    '#required' => TRUE,
    '#description' => t('Select a role.'),
    '#default_value' => $settings['rid'],
  );
}