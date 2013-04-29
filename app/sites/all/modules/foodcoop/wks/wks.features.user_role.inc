<?php
/**
 * @file
 * wks.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function wks_user_default_roles() {
  $roles = array();

  // Exported role: Administrator.
  $roles['Administrator'] = array(
    'name' => 'Administrator',
    'weight' => '16',
  );

  // Exported role: Dostawca Bronisze.
  $roles['Dostawca Bronisze'] = array(
    'name' => 'Dostawca Bronisze',
    'weight' => '9',
  );

  // Exported role: Dostawca Rutkowscy.
  $roles['Dostawca Rutkowscy'] = array(
    'name' => 'Dostawca Rutkowscy',
    'weight' => '10',
  );

  // Exported role: Kasjer/Kierowca/Ważący.
  $roles['Kasjer/Kierowca/Ważący'] = array(
    'name' => 'Kasjer/Kierowca/Ważący',
    'weight' => '14',
  );

  // Exported role: Kooperatysta.
  $roles['Kooperatysta'] = array(
    'name' => 'Kooperatysta',
    'weight' => '12',
  );

  // Exported role: Koordynator.
  $roles['Koordynator'] = array(
    'name' => 'Koordynator',
    'weight' => '15',
  );

  return $roles;
}
