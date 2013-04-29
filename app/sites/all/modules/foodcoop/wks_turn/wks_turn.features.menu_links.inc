<?php
/**
 * @file
 * wks_turn.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function wks_turn_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-tury:archiwum-tur
  $menu_links['menu-tury:archiwum-tur'] = array(
    'menu_name' => 'menu-tury',
    'link_path' => 'archiwum-tur',
    'router_path' => 'archiwum-tur',
    'link_title' => 'Archiwum tur',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '1',
  );
  // Exported menu link: menu-tury:tura
  $menu_links['menu-tury:tura'] = array(
    'menu_name' => 'menu-tury',
    'link_path' => 'tura',
    'router_path' => 'tura',
    'link_title' => 'Aktywna tura',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Aktywna tura');
  t('Archiwum tur');


  return $menu_links;
}
