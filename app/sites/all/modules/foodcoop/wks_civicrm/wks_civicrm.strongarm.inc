<?php
/**
 * @file
 * wks_civicrm.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function wks_civicrm_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'civicrmtheme_theme_admin';
  $strongarm->value = 'wks_theme';
  $export['civicrmtheme_theme_admin'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'civicrmtheme_theme_public';
  $strongarm->value = '0';
  $export['civicrmtheme_theme_public'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'civicrm_member_roles_sync_method';
  $strongarm->value = '1';
  $export['civicrm_member_roles_sync_method'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_civicrm_contribution_type';
  $strongarm->value = '3';
  $export['commerce_civicrm_contribution_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_civicrm_groups';
  $strongarm->value = array(
    6 => '6',
    1 => 0,
    4 => 0,
    5 => 0,
    2 => 0,
    3 => 0,
  );
  $export['commerce_civicrm_groups'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_civicrm_shipping_field_id';
  $strongarm->value = '66';
  $export['commerce_civicrm_shipping_field_id'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_civicrm_tax_field_id';
  $strongarm->value = '65';
  $export['commerce_civicrm_tax_field_id'] = $strongarm;

  return $export;
}
