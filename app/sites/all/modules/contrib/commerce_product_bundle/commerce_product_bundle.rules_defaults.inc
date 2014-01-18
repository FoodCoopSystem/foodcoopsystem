<?php

/**
 * @file
 * Defines default product bundle rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_product_bundle_default_rules_configuration() {
  $rules = array();

  // Create default bundle price calculation method.
  $rule = rules_reaction_rule();

  $rule->label = t('Calculate Bundle Product Price');
  $rule->active = TRUE;

  $rule
  ->event('commerce_product_calculate_sell_price')
  ->action('commerce_product_bundle_calculate_price', array(
    'commerce_line_item:select' => 'commerce-line-item',
    'multiply_by_quantity' => '1',
  ));

  $rules['commerce_product_bundle_calculation_of_product_price'] = $rule;

  return $rules;
}
