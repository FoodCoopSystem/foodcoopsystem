<?php
/**
 * @file
 * wks.default_commerce_decimal_quantities_presets.inc
 */

/**
 * Implements hook_default_commerce_decimal_quantities().
 */
function wks_default_commerce_decimal_quantities() {
  $export = array();

  $product_decimal = new stdClass();
  $product_decimal->disabled = FALSE; /* Edit this to true to make a default product_decimal disabled initially */
  $product_decimal->api_version = 1;
  $product_decimal->product_type = 'na_sztuki';
  $product_decimal->allow_decimal = 0;
  $export['na_sztuki'] = $product_decimal;

  $product_decimal = new stdClass();
  $product_decimal->disabled = FALSE; /* Edit this to true to make a default product_decimal disabled initially */
  $product_decimal->api_version = 1;
  $product_decimal->product_type = 'product';
  $product_decimal->allow_decimal = 1;
  $export['product'] = $product_decimal;

  return $export;
}
