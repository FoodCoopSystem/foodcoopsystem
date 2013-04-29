<?php
/**
 * @file
 * wks_turn.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function wks_turn_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-tury.
  $menus['menu-tury'] = array(
    'menu_name' => 'menu-tury',
    'title' => 'Tury',
    'description' => 'Menu dla tur',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Menu dla tur');
  t('Tury');


  return $menus;
}
