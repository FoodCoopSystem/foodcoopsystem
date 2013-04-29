<?php

$plugin = array(
  // Basic CTools plugin properties
  'title' => t('Example plugin'),
  'description' => t('This plugin demonstrates the creation of a shipping method and documents various commerce_shipping features.'),
  'handler' => array(
    'class' => 'CommerceShippingExample',
    'parent' => 'quote_base'
  ),

  // Commerce Shipping properties

  // The display title is used for front end displays. The default value is the
  // plugin's title property.
  'display_title' => t('Example shipping method'),

  // The label of the method on a shipping quote selection form. The default
  // value is the plugin's display_title property.
  'shipping_label' => t('Example shipping method radio button'),

  // Boolean value indicating whether or not a default Rule and action should be
  // created for this shipping method.
  'create_rule' => TRUE,

  // Default settings for the method's Rules action form.
  'settings' => array(
    'shipping_price' => 42,
  ),

  // Defines the price component that will be used for the unit price of
  // shipping line items created for this shipping method.
  'price_component' => array(
    'title' => t('Shipping costs from example module'),
    'display_title' => t('Example shipping costs'),
    'weight' => -40,
  ),
);
