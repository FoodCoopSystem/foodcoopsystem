<?php
/**
 * @file
 * wks_turn.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function wks_turn_user_default_roles() {
  $roles = array();

  // Exported role: Kasjer.
  $roles['Kasjer'] = array(
    'name' => 'Kasjer',
    'weight' => '5',
  );

  // Exported role: Kierowca.
  $roles['Kierowca'] = array(
    'name' => 'Kierowca',
    'weight' => '8',
  );

  // Exported role: Koordynator.
  $roles['Koordynator'] = array(
    'name' => 'Koordynator',
    'weight' => '15',
  );

  // Exported role: Ważący.
  $roles['Ważący'] = array(
    'name' => 'Ważący',
    'weight' => '9',
  );

  return $roles;
}
