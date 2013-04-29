<?php

/**
 * @file
 * Default rules configuration for Payment.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_shipping_default_rules_configuration() {
  $rules = array();

  // Add a default rule fore each available plugin.
  foreach (commerce_shipping_plugin_get_plugins('quotes') as $key => $plugin) {
    // Don't create rule if plugin don't want it.
    if (!isset($plugin['create_rule']) || $plugin['create_rule'] !== FALSE) {
      $rule = rules_reaction_rule();

      $rule->label = $plugin['title'];
      $rule->active = FALSE;

      $rule
        ->event('commerce_shipping_methods')
        ->action('commerce_shipping_enable_' . $key, array(
          'commerce_order:select' => 'commerce-order',
          'shipping_method' => $key,
        ));

      $rules['commerce_shipping_' . $key] = $rule;
    }
  }

  return $rules;
}
