<?php
/**
 * @file
 * wks.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function wks_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:<front>
  $menu_links['main-menu:<front>'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Historia zamówień',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '1',
    'external' => '1',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-46',
  );
  // Exported menu link: main-menu:cart
  $menu_links['main-menu:cart'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'cart',
    'router_path' => 'cart',
    'link_title' => 'Koszyk',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '1',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-48',
  );
  // Exported menu link: main-menu:produkty
  $menu_links['main-menu:produkty'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'produkty',
    'router_path' => 'produkty',
    'link_title' => 'Zakupy',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '1',
    'weight' => '-50',
  );
  // Exported menu link: main-menu:tura
  $menu_links['main-menu:tura'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'tura',
    'router_path' => 'tura',
    'link_title' => 'Tura',
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
    'weight' => '-49',
  );
  // Exported menu link: main-menu:user
  $menu_links['main-menu:user'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'user',
    'router_path' => 'user',
    'link_title' => 'Konto',
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
    'weight' => '-47',
  );
  // Exported menu link: main-menu:user/logout
  $menu_links['main-menu:user/logout'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'user/logout',
    'router_path' => 'user/logout',
    'link_title' => 'Wyloguj',
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
    'weight' => '-45',
  );
  // Exported menu link: menu-administrator-menu:admin/config/people/taxonomy_access
  $menu_links['menu-administrator-menu:admin/config/people/taxonomy_access'] = array(
    'menu_name' => 'menu-administrator-menu',
    'link_path' => 'admin/config/people/taxonomy_access',
    'router_path' => 'admin/config/people/taxonomy_access',
    'link_title' => 'Dodatkowe uprawnienia dostawców',
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
    'weight' => '-48',
  );
  // Exported menu link: menu-administrator-menu:admin/people/permissions
  $menu_links['menu-administrator-menu:admin/people/permissions'] = array(
    'menu_name' => 'menu-administrator-menu',
    'link_path' => 'admin/people/permissions',
    'router_path' => 'admin/people/permissions',
    'link_title' => 'Uprawnienia',
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
    'weight' => '-50',
  );
  // Exported menu link: menu-administrator-menu:admin/people/permissions/roles
  $menu_links['menu-administrator-menu:admin/people/permissions/roles'] = array(
    'menu_name' => 'menu-administrator-menu',
    'link_path' => 'admin/people/permissions/roles',
    'router_path' => 'admin/people/permissions/roles',
    'link_title' => 'Role',
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
    'weight' => '-49',
  );
  // Exported menu link: menu-administrator-menu:admin/structure/taxonomy/dostawca
  $menu_links['menu-administrator-menu:admin/structure/taxonomy/dostawca'] = array(
    'menu_name' => 'menu-administrator-menu',
    'link_path' => 'admin/structure/taxonomy/dostawca',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Kategorie dostawców',
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
    'weight' => '-47',
  );
  // Exported menu link: menu-administrator-menu:admin/structure/taxonomy/tags
  $menu_links['menu-administrator-menu:admin/structure/taxonomy/tags'] = array(
    'menu_name' => 'menu-administrator-menu',
    'link_path' => 'admin/structure/taxonomy/tags',
    'router_path' => 'admin/structure/taxonomy/%',
    'link_title' => 'Kategorie Tagi',
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
    'weight' => '-46',
  );
  // Exported menu link: menu-administrator-menu:node
  $menu_links['menu-administrator-menu:node'] = array(
    'menu_name' => 'menu-administrator-menu',
    'link_path' => 'node',
    'router_path' => 'node',
    'link_title' => 'Instrukcja jak dodać nowego dostawcę',
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
    'weight' => '-44',
  );
  // Exported menu link: menu-menu-funkcyjne:admin/commerce/orders
  $menu_links['menu-menu-funkcyjne:admin/commerce/orders'] = array(
    'menu_name' => 'menu-menu-funkcyjne',
    'link_path' => 'admin/commerce/orders',
    'router_path' => 'admin/commerce/orders',
    'link_title' => 'Zarządzaj zamówieniami',
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
    'weight' => '-45',
  );
  // Exported menu link: menu-menu-funkcyjne:admin/commerce/orders/carts
  $menu_links['menu-menu-funkcyjne:admin/commerce/orders/carts'] = array(
    'menu_name' => 'menu-menu-funkcyjne',
    'link_path' => 'admin/commerce/orders/carts',
    'router_path' => 'admin/commerce/orders/carts',
    'link_title' => 'Zobacz zamówienia aktualnej tury',
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
    'weight' => '-46',
  );
  // Exported menu link: menu-menu-funkcyjne:admin/commerce/products
  $menu_links['menu-menu-funkcyjne:admin/commerce/products'] = array(
    'menu_name' => 'menu-menu-funkcyjne',
    'link_path' => 'admin/commerce/products',
    'router_path' => 'admin/commerce/products',
    'link_title' => 'Zarzadzaj produktami',
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
    'weight' => '-47',
  );
  // Exported menu link: menu-menu-funkcyjne:admin/commerce/products/product-display
  $menu_links['menu-menu-funkcyjne:admin/commerce/products/product-display'] = array(
    'menu_name' => 'menu-menu-funkcyjne',
    'link_path' => 'admin/commerce/products/product-display',
    'router_path' => 'admin/commerce/products/product-display',
    'link_title' => 'Dodaj produkt',
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
    'weight' => '-49',
  );
  // Exported menu link: menu-menu-funkcyjne:admin/commerce/reports
  $menu_links['menu-menu-funkcyjne:admin/commerce/reports'] = array(
    'menu_name' => 'menu-menu-funkcyjne',
    'link_path' => 'admin/commerce/reports',
    'router_path' => 'admin/commerce/reports',
    'link_title' => 'Statystyki i fundusz',
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
    'weight' => '-44',
  );
  // Exported menu link: menu-menu-funkcyjne:admin/people
  $menu_links['menu-menu-funkcyjne:admin/people'] = array(
    'menu_name' => 'menu-menu-funkcyjne',
    'link_path' => 'admin/people',
    'router_path' => 'admin/people',
    'link_title' => 'Użytkownicy',
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
  // Exported menu link: menu-menu-funkcyjne:node/add/tura
  $menu_links['menu-menu-funkcyjne:node/add/tura'] = array(
    'menu_name' => 'menu-menu-funkcyjne',
    'link_path' => 'node/add/tura',
    'router_path' => 'node/add/tura',
    'link_title' => 'Dodaj turę',
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
    'weight' => '-50',
  );
  // Exported menu link: menu-menu-prints:admin/commerce/prints/print-name
  $menu_links['menu-menu-prints:admin/commerce/prints/print-name'] = array(
    'menu_name' => 'menu-menu-prints',
    'link_path' => 'admin/commerce/prints/print-name',
    'router_path' => 'admin/commerce/prints/print-name',
    'link_title' => 'Lista zamówień dla kasjera',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '1',
    'weight' => '-49',
  );
  // Exported menu link: menu-menu-prints:admin/commerce/prints/products
  $menu_links['menu-menu-prints:admin/commerce/prints/products'] = array(
    'menu_name' => 'menu-menu-prints',
    'link_path' => 'admin/commerce/prints/products',
    'router_path' => 'admin/commerce/prints/products',
    'link_title' => 'Produkty do kupienia',
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
    'weight' => '-50',
  );
  // Exported menu link: menu-menu-prints:admin/commerce/prints/quantieties
  $menu_links['menu-menu-prints:admin/commerce/prints/quantieties'] = array(
    'menu_name' => 'menu-menu-prints',
    'link_path' => 'admin/commerce/prints/quantieties',
    'router_path' => 'admin/commerce/prints/quantieties',
    'link_title' => 'Wydruk ważacego grupowany po produktach',
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
    'weight' => '-48',
  );
  // Exported menu link: menu-menu-prints:admin/commerce/prints/quantieties-names
  $menu_links['menu-menu-prints:admin/commerce/prints/quantieties-names'] = array(
    'menu_name' => 'menu-menu-prints',
    'link_path' => 'admin/commerce/prints/quantieties-names',
    'router_path' => 'admin/commerce/prints/quantieties-names',
    'link_title' => 'Wydruk ważacego grupowany po zamówieniach',
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
    'weight' => '-47',
  );
  // Exported menu link: navigation:node/add
  $menu_links['navigation:node/add'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'node/add',
    'router_path' => 'node/add',
    'link_title' => 'Add content',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '1',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Add content');
  t('Dodaj produkt');
  t('Dodaj turę');
  t('Dodatkowe uprawnienia dostawców');
  t('Historia zamówień');
  t('Instrukcja jak dodać nowego dostawcę');
  t('Kategorie Tagi');
  t('Kategorie dostawców');
  t('Konto');
  t('Koszyk');
  t('Lista zamówień dla kasjera');
  t('Produkty do kupienia');
  t('Role');
  t('Statystyki i fundusz');
  t('Tura');
  t('Uprawnienia');
  t('Użytkownicy');
  t('Wydruk ważacego grupowany po produktach');
  t('Wydruk ważacego grupowany po zamówieniach');
  t('Wyloguj');
  t('Zakupy');
  t('Zarzadzaj produktami');
  t('Zarządzaj zamówieniami');
  t('Zobacz zamówienia aktualnej tury');


  return $menu_links;
}
