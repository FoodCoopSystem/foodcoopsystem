<?php

/**
 * @file
 * Rules integration for customer profile types ui.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function customer_profile_type_ui_rules_event_info() {
  // So that we can use the entity_rules_events_variables() helper function.
  module_load_include('inc', 'entity', 'entity.rules');

  $events = array();

  $events['before_customer_profile_type_panes'] = array(
    'label' => t('Before customer profile panes are displayed'),
    'group' => t('Commerce Customer profile'),
    'variables' => entity_rules_events_variables('commerce_order', t('Order', array(), array('context' => 'a drupal commerce order'))),
    'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}


/**
 * Implements hook_rules_action_info().
 */
function customer_profile_type_ui_rules_action_info() {
  $actions = array(
    // Add an action to disable a customer profile type pane.
    'disable_a_customer_profile' => array(
      'label' => t('Disable a customer profile pane'),
      'parameter' => array(
        'commerce_order' => array('type' => 'commerce_order', 'label' => t('Order', array(), array('context' => 'a drupal commerce order'))),
        'customer_profile_types' => array(
          'type' => 'text',
          'options list' => 'customer_profile_type_ui_rules_options_list',
          'restriction' => 'input',
          'label' => t('Customer Profile Pane to disable'),
        ),
      ),
      'group' => t('Commerce Customer profile'),
      'callbacks' => array(
        'execute' => 'customer_profile_type_ui_disable_pane_method',
      ),
    ),
    // Add an action to enable a customer profile type pane.
    'enable_a_customer_profile' => array(
      'label' => t('Enable a customer profile pane'),
      'parameter' => array(
        'commerce_order' => array('type' => 'commerce_order', 'label' => t('Order', array(), array('context' => 'a drupal commerce order'))),
        'customer_profile_types' => array(
          'type' => 'text',
          'options list' => 'customer_profile_type_ui_rules_options_list',
          'restriction' => 'input',
          'label' => t('Customer Profile Pane to enable if it is not already'),
        ),
      ),
      'group' => t('Commerce Customer profile'),
      'callbacks' => array(
        'execute' => 'customer_profile_type_ui_enable_pane_method',
      ),
    ),
    // Add an action to disable all customer profile type pane.
    'disable_all_customer_profiles' => array(
      'label' => t('Disable all customer profile panes'),
      'parameter' => array(
        'commerce_order' => array('type' => 'commerce_order', 'label' => t('Order', array(), array('context' => 'a drupal commerce order'))),
      ),
      'group' => t('Commerce Customer profile'),
      'callbacks' => array(
        'execute' => 'customer_profile_type_ui_disable_all_panes_method',
      ),
    ),
  );

  return $actions;
}

/**
 * Generic execution callback for disabling customer profile panes.
 */
function customer_profile_type_ui_disable_pane_method($order, $customer_profile_type, $action_settings, $rule_state, $action, $callback_type) {
  // Add the selected customer profile type to the list of checkout panes to disable.
  $order->disabled_customer_profiles[$customer_profile_type] = TRUE;
}


/**
 * Generic execution callback for enabling customer profile panes.
 */
function customer_profile_type_ui_enable_pane_method($order, $customer_profile_type, $action_settings, $rule_state, $action, $callback_type) {
  // Remove the selected customer profile type from the list of checkout panes to disable if its present.
  if(isset($order->disabled_customer_profiles[$customer_profile_type]))
    unset($order->disabled_customer_profiles[$customer_profile_type]);
}


/**
 * Callback for disabling all customer profile panes.
 */
function customer_profile_type_ui_disable_all_panes_method($order, $action_settings, $rule_state, $action, $callback_type) {
  // Add the selected customer profile type to the list of checkout panes to disable.
  $order->disabled_customer_profiles = customer_profile_type_ui_rules_options_list();
}
