<?php

/**
 * @file
 * Rules integration for shipping.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_shipping_rules_event_info() {
  // The line item rule file is needed for the access callback.
  module_load_include('inc', 'commerce_line_item', 'commerce_line_item.rules');
  $items = array(
    'commerce_shipping_methods' => array(
      'label' => t('Select available shipping methods for an order'),
      'group' => t('Commerce - shipping'),
      'variables' => entity_rules_events_variables('commerce_order', t('Order', array(), array('context' => 'a drupal commerce order'))),
      'access callback' => 'commerce_order_rules_access',
    ),
    'commerce_shipping_calculate' => array(
      'label' => t('Calculate the shipping quote for a line item'),
      'group' => t('Commerce - shipping'),
      'variables' => entity_rules_events_variables('commerce_line_item', t('Line Item', array(), array('context' => 'a drupal commerce line item'))),
      'access callback' => 'commerce_line_item_rules_access',
    ),
  );

  return $items;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_shipping_rules_action_info() {
  $actions = array();

  // Add an action for each plugin that want's it.
  foreach (commerce_shipping_plugin_get_plugins('quotes') as $key => $plugin) {
    if (!isset($plugin['create_rule']) || $plugin['create_rule'] !== FALSE) {
      $actions['commerce_shipping_enable_' . $key] = array(
        'label' => t('Enable shipping method: @method', array('@method' => $plugin['title'])),
        'parameter' => array(
          'commerce_order' => array('type' => 'commerce_order', 'label' => t('Order')),
          'shipping_method' => array(
            'type' => 'commerce_shipping_settings',
            'restriction' => 'input',
            'label' => t('Shipping settings'),
            'shipping_method' => $key,
          ),
        ),
        'group' => t('Commerce Shipping'),
        'base' => $plugin['plugin module'] . '_' . $key,
        'callbacks' => array(
          'execute' => 'commerce_shipping_enable_method',
        ),
      );
    }
  }

  $actions['commerce_shipping_remove_all'] = array(
    'label' => t('Remove all shipping items from an order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to remove shipping items from'),
      ),
    ),
    'group' => t('Commerce Shipping'),
    'callbacks' => array(
      'execute' => 'commerce_shipping_rules_remove_all',
    ),
  );

  return $actions;
}

/**
 * Rules action: Remove all shipping line items on an order.
 */
function commerce_shipping_rules_remove_all($order) {
  commerce_shipping_clear_order($order);
}

/**
 * Generic execution callback for the shipping method.
 */
function commerce_shipping_enable_method($order, $shipping_method, $action_settings, $rule_state, $action, $callback_type) {
  // Find the Rule that contains this action.
  $rule = $action->parentElement();

  while ($rule->getPluginName() != 'reaction rule') {
    $rule = $rule->parentElement();
  }

  // Initialize variables for the shipping method ID and settings.
  if (is_array($shipping_method)) {
    $method_id = $shipping_method['method_id'];
  }
  else {
    $method_id = $shipping_method;
    $shipping_method = commerce_shipping_plugin_get_plugin('quotes', $method_id);
  }
  // Create a unique key for the instance of the shipping method represented by
  // this action.
  $instance_id = commerce_shipping_method_instance_id($method_id, $rule);

  // Set the shipping method to the order along with its settings and context.

  $order->commerce_shipping_methods[$instance_id] = array(
    'method_id' => $method_id,
    'settings' => $shipping_method['settings'],
    'plugin' => $shipping_method,
    'rule_name' => $rule->name,
    'label' => $rule->label,
  );
}

/**
 * Implements hook_rules_data_info().
 */
function commerce_shipping_rules_data_info() {
  $data['commerce_shipping_settings'] = array(
    'label' => t('Shipping settings'),
    'ui class' => 'RulesDataUIShippingSettings',
  );
  return $data;
}

/**
 * Adds a shipping method settings form to the enabling action.
 */
class RulesDataUIShippingSettings extends RulesDataUI implements RulesDataDirectInputFormInterface {
  public static function getDefaultMode() {
    return 'input';
  }

  public static function inputForm($name, $info, $settings, RulesPlugin $element) {
    $plugin = str_replace('plugin-', '', $info['shipping_method']);
    $class = commerce_shipping_plugin_get_plugin_class_init('quotes', $plugin);
    if ($class === FALSE) {
      $form[$name]['invalid']['#markup'] = t('Error getting shipping method.');
    }
    else {
      $form[$name]['method_id'] = array('#type' => 'value', '#value' => $info['shipping_method']);
      $form[$name]['shipping_label'] = array(
        '#type' => 'textfield',
        '#title' => t('Shipping label'),
        '#description' => t('Customise the shipping label as it would appear for customers'),
        '#default_value' => is_array($settings[$name]) && isset($settings[$name]['shipping_label']) ? $settings[$name]['shipping_label'] : '',
      );
      $form[$name]['settings'] = array();
      // Prepare an array of shipping method settings defaults.
      $rules_settings = !empty($settings[$name]) && isset($settings[$name]['settings']) ? $settings[$name]['settings'] : array();
      $class->settings_form($form[$name]['settings'], $rules_settings);
      }
    /*else {
      $form[$name]['invalid']['#markup'] = t('Invalid shipping method.');
    }*/
    return $form;
  }

  public static function render($value) {
    return array();
  }
}

/**
 * @}
 */
