<?php
/**
 * @file
 * wks_commerce.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function wks_commerce_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-menu-dostawcy:admin/commerce/products
  $menu_links['menu-menu-dostawcy:admin/commerce/products'] = array(
    'menu_name' => 'menu-menu-dostawcy',
    'link_path' => 'admin/commerce/products',
    'router_path' => 'admin/commerce/products',
    'link_title' => 'Zarządzaj swoimi produktami',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Zarządzaj swoimi produktami');


  return $menu_links;
}
