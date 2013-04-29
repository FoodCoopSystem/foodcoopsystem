<?php
/**
 * @file
 * wks.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function wks_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: main-menu.
  $menus['main-menu'] = array(
    'menu_name' => 'main-menu',
    'title' => 'Main menu',
    'description' => 'The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.',
  );
  // Exported menu: menu-administrator-menu.
  $menus['menu-administrator-menu'] = array(
    'menu_name' => 'menu-administrator-menu',
    'title' => 'Blok administratora',
    'description' => '',
  );
  // Exported menu: menu-menu-funkcyjne.
  $menus['menu-menu-funkcyjne'] = array(
    'menu_name' => 'menu-menu-funkcyjne',
    'title' => 'Blok funkcyjny',
    'description' => '',
  );
  // Exported menu: menu-menu-prints.
  $menus['menu-menu-prints'] = array(
    'menu_name' => 'menu-menu-prints',
    'title' => 'Blok wydruki',
    'description' => '',
  );
  // Exported menu: user-menu.
  $menus['user-menu'] = array(
    'menu_name' => 'user-menu',
    'title' => 'User menu',
    'description' => 'The <em>User</em> menu contains links related to the user\'s account, as well as the \'Log out\' link.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Blok administratora');
  t('Blok funkcyjny');
  t('Blok wydruki');
  t('Main menu');
  t('The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.');
  t('The <em>User</em> menu contains links related to the user\'s account, as well as the \'Log out\' link.');
  t('User menu');


  return $menus;
}
