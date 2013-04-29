<?php

/**
 * Implements hook_views_data_alter().
 */
function commerce_add_to_cart_extras_views_data_alter(&$data) {
  $data['commerce_product']['add_to_cart_quantity'] = array(
    'title' => t('Quantity input field'),
    'help' => t('Adds a quantity input field and creates an add to cart field around the view.'),
    'real field' => 'product_id',
    'field' => array(
      'handler' => 'commerce_add_to_cart_extras_handler_field_quantity',
    ),
  );
}