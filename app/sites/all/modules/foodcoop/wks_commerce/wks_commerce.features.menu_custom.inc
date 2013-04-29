<?php
/**
 * @file
 * wks_commerce.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function wks_commerce_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-menu-dostawcy.
  $menus['menu-menu-dostawcy'] = array(
    'menu_name' => 'menu-menu-dostawcy',
    'title' => 'Menu dostawcy',
    'description' => '',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Menu dostawcy');


  return $menus;
}
