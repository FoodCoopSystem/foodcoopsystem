<?php
/**
 * @file
 * wks.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function wks_user_default_permissions() {
  $permissions = array();

  // Exported permission: access administration menu.
  $permissions['access administration menu'] = array(
    'name' => 'access administration menu',
    'roles' => array(),
    'module' => 'admin_menu',
  );

  // Exported permission: access administration pages.
  $permissions['access administration pages'] = array(
    'name' => 'access administration pages',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'system',
  );

  // Exported permission: access all views.
  $permissions['access all views'] = array(
    'name' => 'access all views',
    'roles' => array(),
    'module' => 'views',
  );

  // Exported permission: access all webform results.
  $permissions['access all webform results'] = array(
    'name' => 'access all webform results',
    'roles' => array(),
    'module' => 'webform',
  );

  // Exported permission: access checkout.
  $permissions['access checkout'] = array(
    'name' => 'access checkout',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_checkout',
  );

  // Exported permission: access commerce reports.
  $permissions['access commerce reports'] = array(
    'name' => 'access commerce reports',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'commerce_reports',
  );

  // Exported permission: access commerce tax reports.
  $permissions['access commerce tax reports'] = array(
    'name' => 'access commerce tax reports',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_reports_tax',
  );

  // Exported permission: access content.
  $permissions['access content'] = array(
    'name' => 'access content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: access content overview.
  $permissions['access content overview'] = array(
    'name' => 'access content overview',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: access contextual links.
  $permissions['access contextual links'] = array(
    'name' => 'access contextual links',
    'roles' => array(),
    'module' => 'contextual',
  );

  // Exported permission: access dashboard.
  $permissions['access dashboard'] = array(
    'name' => 'access dashboard',
    'roles' => array(),
    'module' => 'dashboard',
  );

  // Exported permission: access own webform results.
  $permissions['access own webform results'] = array(
    'name' => 'access own webform results',
    'roles' => array(),
    'module' => 'webform',
  );

  // Exported permission: access own webform submissions.
  $permissions['access own webform submissions'] = array(
    'name' => 'access own webform submissions',
    'roles' => array(),
    'module' => 'webform',
  );

  // Exported permission: access print.
  $permissions['access print'] = array(
    'name' => 'access print',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kooperatysta',
      4 => 'Koordynator',
      5 => 'anonymous user',
      6 => 'authenticated user',
    ),
    'module' => 'print',
  );

  // Exported permission: access rules debug.
  $permissions['access rules debug'] = array(
    'name' => 'access rules debug',
    'roles' => array(),
    'module' => 'rules',
  );

  // Exported permission: access site in maintenance mode.
  $permissions['access site in maintenance mode'] = array(
    'name' => 'access site in maintenance mode',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'system',
  );

  // Exported permission: access site reports.
  $permissions['access site reports'] = array(
    'name' => 'access site reports',
    'roles' => array(),
    'module' => 'system',
  );

  // Exported permission: access user profiles.
  $permissions['access user profiles'] = array(
    'name' => 'access user profiles',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'user',
  );

  // Exported permission: administer Terms and Conditions.
  $permissions['administer Terms and Conditions'] = array(
    'name' => 'administer Terms and Conditions',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'legal',
  );

  // Exported permission: administer actions.
  $permissions['administer actions'] = array(
    'name' => 'administer actions',
    'roles' => array(),
    'module' => 'system',
  );

  // Exported permission: administer blocks.
  $permissions['administer blocks'] = array(
    'name' => 'administer blocks',
    'roles' => array(),
    'module' => 'block',
  );

  // Exported permission: administer checkout.
  $permissions['administer checkout'] = array(
    'name' => 'administer checkout',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_checkout',
  );

  // Exported permission: administer commerce_customer_profile entities.
  $permissions['administer commerce_customer_profile entities'] = array(
    'name' => 'administer commerce_customer_profile entities',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: administer commerce_extra.
  $permissions['administer commerce_extra'] = array(
    'name' => 'administer commerce_extra',
    'roles' => array(),
    'module' => 'commerce_extra',
  );

  // Exported permission: administer commerce_order entities.
  $permissions['administer commerce_order entities'] = array(
    'name' => 'administer commerce_order entities',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: administer commerce_product entities.
  $permissions['administer commerce_product entities'] = array(
    'name' => 'administer commerce_product entities',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: administer content types.
  $permissions['administer content types'] = array(
    'name' => 'administer content types',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: administer customer profile types.
  $permissions['administer customer profile types'] = array(
    'name' => 'administer customer profile types',
    'roles' => array(),
    'module' => 'commerce_customer',
  );

  // Exported permission: administer date tools.
  $permissions['administer date tools'] = array(
    'name' => 'administer date tools',
    'roles' => array(),
    'module' => 'date_tools',
  );

  // Exported permission: administer features.
  $permissions['administer features'] = array(
    'name' => 'administer features',
    'roles' => array(),
    'module' => 'features',
  );

  // Exported permission: administer fieldgroups.
  $permissions['administer fieldgroups'] = array(
    'name' => 'administer fieldgroups',
    'roles' => array(),
    'module' => 'field_group',
  );

  // Exported permission: administer filters.
  $permissions['administer filters'] = array(
    'name' => 'administer filters',
    'roles' => array(),
    'module' => 'filter',
  );

  // Exported permission: administer flags.
  $permissions['administer flags'] = array(
    'name' => 'administer flags',
    'roles' => array(),
    'module' => 'flag',
  );

  // Exported permission: administer image styles.
  $permissions['administer image styles'] = array(
    'name' => 'administer image styles',
    'roles' => array(),
    'module' => 'image',
  );

  // Exported permission: administer invitations.
  $permissions['administer invitations'] = array(
    'name' => 'administer invitations',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'invite',
  );

  // Exported permission: administer languages.
  $permissions['administer languages'] = array(
    'name' => 'administer languages',
    'roles' => array(),
    'module' => 'locale',
  );

  // Exported permission: administer line item types.
  $permissions['administer line item types'] = array(
    'name' => 'administer line item types',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_line_item',
  );

  // Exported permission: administer line items.
  $permissions['administer line items'] = array(
    'name' => 'administer line items',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_line_item',
  );

  // Exported permission: administer menu.
  $permissions['administer menu'] = array(
    'name' => 'administer menu',
    'roles' => array(),
    'module' => 'menu',
  );

  // Exported permission: administer modules.
  $permissions['administer modules'] = array(
    'name' => 'administer modules',
    'roles' => array(),
    'module' => 'system',
  );

  // Exported permission: administer nodes.
  $permissions['administer nodes'] = array(
    'name' => 'administer nodes',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: administer pathauto.
  $permissions['administer pathauto'] = array(
    'name' => 'administer pathauto',
    'roles' => array(),
    'module' => 'pathauto',
  );

  // Exported permission: administer payment methods.
  $permissions['administer payment methods'] = array(
    'name' => 'administer payment methods',
    'roles' => array(),
    'module' => 'commerce_payment',
  );

  // Exported permission: administer payments.
  $permissions['administer payments'] = array(
    'name' => 'administer payments',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_payment',
  );

  // Exported permission: administer permissions.
  $permissions['administer permissions'] = array(
    'name' => 'administer permissions',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'user',
  );

  // Exported permission: administer print.
  $permissions['administer print'] = array(
    'name' => 'administer print',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'print',
  );

  // Exported permission: administer product pricing.
  $permissions['administer product pricing'] = array(
    'name' => 'administer product pricing',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Koordynator',
    ),
    'module' => 'commerce_product_pricing_ui',
  );

  // Exported permission: administer product types.
  $permissions['administer product types'] = array(
    'name' => 'administer product types',
    'roles' => array(),
    'module' => 'commerce_product',
  );

  // Exported permission: administer quicktabs.
  $permissions['administer quicktabs'] = array(
    'name' => 'administer quicktabs',
    'roles' => array(),
    'module' => 'quicktabs',
  );

  // Exported permission: administer role expire.
  $permissions['administer role expire'] = array(
    'name' => 'administer role expire',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'role_expire',
  );

  // Exported permission: administer rules.
  $permissions['administer rules'] = array(
    'name' => 'administer rules',
    'roles' => array(),
    'module' => 'rules',
  );

  // Exported permission: administer search.
  $permissions['administer search'] = array(
    'name' => 'administer search',
    'roles' => array(),
    'module' => 'search',
  );

  // Exported permission: administer shortcuts.
  $permissions['administer shortcuts'] = array(
    'name' => 'administer shortcuts',
    'roles' => array(),
    'module' => 'shortcut',
  );

  // Exported permission: administer site configuration.
  $permissions['administer site configuration'] = array(
    'name' => 'administer site configuration',
    'roles' => array(),
    'module' => 'system',
  );

  // Exported permission: administer software updates.
  $permissions['administer software updates'] = array(
    'name' => 'administer software updates',
    'roles' => array(),
    'module' => 'system',
  );

  // Exported permission: administer taxes.
  $permissions['administer taxes'] = array(
    'name' => 'administer taxes',
    'roles' => array(),
    'module' => 'commerce_tax_ui',
  );

  // Exported permission: administer taxonomy.
  $permissions['administer taxonomy'] = array(
    'name' => 'administer taxonomy',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: administer themes.
  $permissions['administer themes'] = array(
    'name' => 'administer themes',
    'roles' => array(),
    'module' => 'system',
  );

  // Exported permission: administer url aliases.
  $permissions['administer url aliases'] = array(
    'name' => 'administer url aliases',
    'roles' => array(),
    'module' => 'path',
  );

  // Exported permission: administer users.
  $permissions['administer users'] = array(
    'name' => 'administer users',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'user',
  );

  // Exported permission: administer views.
  $permissions['administer views'] = array(
    'name' => 'administer views',
    'roles' => array(),
    'module' => 'views',
  );

  // Exported permission: administer views calc.
  $permissions['administer views calc'] = array(
    'name' => 'administer views calc',
    'roles' => array(),
    'module' => 'views_calc',
  );

  // Exported permission: block IP addresses.
  $permissions['block IP addresses'] = array(
    'name' => 'block IP addresses',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'system',
  );

  // Exported permission: bypass node access.
  $permissions['bypass node access'] = array(
    'name' => 'bypass node access',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: bypass rules access.
  $permissions['bypass rules access'] = array(
    'name' => 'bypass rules access',
    'roles' => array(),
    'module' => 'rules',
  );

  // Exported permission: cancel account.
  $permissions['cancel account'] = array(
    'name' => 'cancel account',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Kooperatysta',
      5 => 'Koordynator',
      6 => 'authenticated user',
    ),
    'module' => 'user',
  );

  // Exported permission: change own username.
  $permissions['change own username'] = array(
    'name' => 'change own username',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'user',
  );

  // Exported permission: configure commerce tax reports.
  $permissions['configure commerce tax reports'] = array(
    'name' => 'configure commerce tax reports',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'commerce_reports_tax',
  );

  // Exported permission: configure order settings.
  $permissions['configure order settings'] = array(
    'name' => 'configure order settings',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: configure store.
  $permissions['configure store'] = array(
    'name' => 'configure store',
    'roles' => array(),
    'module' => 'commerce',
  );

  // Exported permission: create commerce_customer_profile entities.
  $permissions['create commerce_customer_profile entities'] = array(
    'name' => 'create commerce_customer_profile entities',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: create commerce_customer_profile entities of bundle billing.
  $permissions['create commerce_customer_profile entities of bundle billing'] = array(
    'name' => 'create commerce_customer_profile entities of bundle billing',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: create commerce_customer_profile entities of bundle shipping.
  $permissions['create commerce_customer_profile entities of bundle shipping'] = array(
    'name' => 'create commerce_customer_profile entities of bundle shipping',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: create commerce_order entities.
  $permissions['create commerce_order entities'] = array(
    'name' => 'create commerce_order entities',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: create commerce_order entities of bundle commerce_order.
  $permissions['create commerce_order entities of bundle commerce_order'] = array(
    'name' => 'create commerce_order entities of bundle commerce_order',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: create commerce_product entities.
  $permissions['create commerce_product entities'] = array(
    'name' => 'create commerce_product entities',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: create commerce_product entities of bundle magazyn.
  $permissions['create commerce_product entities of bundle magazyn'] = array(
    'name' => 'create commerce_product entities of bundle magazyn',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: create commerce_product entities of bundle na_sztuki.
  $permissions['create commerce_product entities of bundle na_sztuki'] = array(
    'name' => 'create commerce_product entities of bundle na_sztuki',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: create commerce_product entities of bundle product.
  $permissions['create commerce_product entities of bundle product'] = array(
    'name' => 'create commerce_product entities of bundle product',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: create commerce_product entities of bundle sztuki_magazyn.
  $permissions['create commerce_product entities of bundle sztuki_magazyn'] = array(
    'name' => 'create commerce_product entities of bundle sztuki_magazyn',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: create kasjer content.
  $permissions['create kasjer content'] = array(
    'name' => 'create kasjer content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create kasjer_2 content.
  $permissions['create kasjer_2 content'] = array(
    'name' => 'create kasjer_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create kasjer_3 content.
  $permissions['create kasjer_3 content'] = array(
    'name' => 'create kasjer_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create koordynacja content.
  $permissions['create koordynacja content'] = array(
    'name' => 'create koordynacja content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create koordynacja_2 content.
  $permissions['create koordynacja_2 content'] = array(
    'name' => 'create koordynacja_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create nadz_r_ksi_gowo_ci content.
  $permissions['create nadz_r_ksi_gowo_ci content'] = array(
    'name' => 'create nadz_r_ksi_gowo_ci content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create nadz_r_ksi_gowo_ci_2 content.
  $permissions['create nadz_r_ksi_gowo_ci_2 content'] = array(
    'name' => 'create nadz_r_ksi_gowo_ci_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create page content.
  $permissions['create page content'] = array(
    'name' => 'create page content',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: create payments.
  $permissions['create payments'] = array(
    'name' => 'create payments',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_payment',
  );

  // Exported permission: create sprz_tanie content.
  $permissions['create sprz_tanie content'] = array(
    'name' => 'create sprz_tanie content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create sprz_tanie_2 content.
  $permissions['create sprz_tanie_2 content'] = array(
    'name' => 'create sprz_tanie_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create sprz_tanie_3 content.
  $permissions['create sprz_tanie_3 content'] = array(
    'name' => 'create sprz_tanie_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create transport content.
  $permissions['create transport content'] = array(
    'name' => 'create transport content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create transport_2 content.
  $permissions['create transport_2 content'] = array(
    'name' => 'create transport_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create tura content.
  $permissions['create tura content'] = array(
    'name' => 'create tura content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create typ_produktu content.
  $permissions['create typ_produktu content'] = array(
    'name' => 'create typ_produktu content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create url aliases.
  $permissions['create url aliases'] = array(
    'name' => 'create url aliases',
    'roles' => array(),
    'module' => 'path',
  );

  // Exported permission: create views calc.
  $permissions['create views calc'] = array(
    'name' => 'create views calc',
    'roles' => array(),
    'module' => 'views_calc',
  );

  // Exported permission: create wazacy content.
  $permissions['create wazacy content'] = array(
    'name' => 'create wazacy content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create wazacy_2 content.
  $permissions['create wazacy_2 content'] = array(
    'name' => 'create wazacy_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create wazacy_3 content.
  $permissions['create wazacy_3 content'] = array(
    'name' => 'create wazacy_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: create webform content.
  $permissions['create webform content'] = array(
    'name' => 'create webform content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: customize shortcut links.
  $permissions['customize shortcut links'] = array(
    'name' => 'customize shortcut links',
    'roles' => array(),
    'module' => 'shortcut',
  );

  // Exported permission: delete all webform submissions.
  $permissions['delete all webform submissions'] = array(
    'name' => 'delete all webform submissions',
    'roles' => array(),
    'module' => 'webform',
  );

  // Exported permission: delete any kasjer content.
  $permissions['delete any kasjer content'] = array(
    'name' => 'delete any kasjer content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any kasjer_2 content.
  $permissions['delete any kasjer_2 content'] = array(
    'name' => 'delete any kasjer_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any kasjer_3 content.
  $permissions['delete any kasjer_3 content'] = array(
    'name' => 'delete any kasjer_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any koordynacja content.
  $permissions['delete any koordynacja content'] = array(
    'name' => 'delete any koordynacja content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any koordynacja_2 content.
  $permissions['delete any koordynacja_2 content'] = array(
    'name' => 'delete any koordynacja_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any nadz_r_ksi_gowo_ci content.
  $permissions['delete any nadz_r_ksi_gowo_ci content'] = array(
    'name' => 'delete any nadz_r_ksi_gowo_ci content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any nadz_r_ksi_gowo_ci_2 content.
  $permissions['delete any nadz_r_ksi_gowo_ci_2 content'] = array(
    'name' => 'delete any nadz_r_ksi_gowo_ci_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any page content.
  $permissions['delete any page content'] = array(
    'name' => 'delete any page content',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any sprz_tanie content.
  $permissions['delete any sprz_tanie content'] = array(
    'name' => 'delete any sprz_tanie content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any sprz_tanie_2 content.
  $permissions['delete any sprz_tanie_2 content'] = array(
    'name' => 'delete any sprz_tanie_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any sprz_tanie_3 content.
  $permissions['delete any sprz_tanie_3 content'] = array(
    'name' => 'delete any sprz_tanie_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any transport content.
  $permissions['delete any transport content'] = array(
    'name' => 'delete any transport content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any transport_2 content.
  $permissions['delete any transport_2 content'] = array(
    'name' => 'delete any transport_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any tura content.
  $permissions['delete any tura content'] = array(
    'name' => 'delete any tura content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any typ_produktu content.
  $permissions['delete any typ_produktu content'] = array(
    'name' => 'delete any typ_produktu content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any wazacy content.
  $permissions['delete any wazacy content'] = array(
    'name' => 'delete any wazacy content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any wazacy_2 content.
  $permissions['delete any wazacy_2 content'] = array(
    'name' => 'delete any wazacy_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any wazacy_3 content.
  $permissions['delete any wazacy_3 content'] = array(
    'name' => 'delete any wazacy_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any webform content.
  $permissions['delete any webform content'] = array(
    'name' => 'delete any webform content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own kasjer content.
  $permissions['delete own kasjer content'] = array(
    'name' => 'delete own kasjer content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own kasjer_2 content.
  $permissions['delete own kasjer_2 content'] = array(
    'name' => 'delete own kasjer_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own kasjer_3 content.
  $permissions['delete own kasjer_3 content'] = array(
    'name' => 'delete own kasjer_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own koordynacja content.
  $permissions['delete own koordynacja content'] = array(
    'name' => 'delete own koordynacja content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own koordynacja_2 content.
  $permissions['delete own koordynacja_2 content'] = array(
    'name' => 'delete own koordynacja_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own nadz_r_ksi_gowo_ci content.
  $permissions['delete own nadz_r_ksi_gowo_ci content'] = array(
    'name' => 'delete own nadz_r_ksi_gowo_ci content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own nadz_r_ksi_gowo_ci_2 content.
  $permissions['delete own nadz_r_ksi_gowo_ci_2 content'] = array(
    'name' => 'delete own nadz_r_ksi_gowo_ci_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own page content.
  $permissions['delete own page content'] = array(
    'name' => 'delete own page content',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own sprz_tanie content.
  $permissions['delete own sprz_tanie content'] = array(
    'name' => 'delete own sprz_tanie content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own sprz_tanie_2 content.
  $permissions['delete own sprz_tanie_2 content'] = array(
    'name' => 'delete own sprz_tanie_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own sprz_tanie_3 content.
  $permissions['delete own sprz_tanie_3 content'] = array(
    'name' => 'delete own sprz_tanie_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own transport content.
  $permissions['delete own transport content'] = array(
    'name' => 'delete own transport content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own transport_2 content.
  $permissions['delete own transport_2 content'] = array(
    'name' => 'delete own transport_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own tura content.
  $permissions['delete own tura content'] = array(
    'name' => 'delete own tura content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own typ_produktu content.
  $permissions['delete own typ_produktu content'] = array(
    'name' => 'delete own typ_produktu content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own wazacy content.
  $permissions['delete own wazacy content'] = array(
    'name' => 'delete own wazacy content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own wazacy_2 content.
  $permissions['delete own wazacy_2 content'] = array(
    'name' => 'delete own wazacy_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own wazacy_3 content.
  $permissions['delete own wazacy_3 content'] = array(
    'name' => 'delete own wazacy_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own webform content.
  $permissions['delete own webform content'] = array(
    'name' => 'delete own webform content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own webform submissions.
  $permissions['delete own webform submissions'] = array(
    'name' => 'delete own webform submissions',
    'roles' => array(),
    'module' => 'webform',
  );

  // Exported permission: delete payments.
  $permissions['delete payments'] = array(
    'name' => 'delete payments',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_payment',
  );

  // Exported permission: delete revisions.
  $permissions['delete revisions'] = array(
    'name' => 'delete revisions',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete terms in 1.
  $permissions['delete terms in 1'] = array(
    'name' => 'delete terms in 1',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: delete terms in 2.
  $permissions['delete terms in 2'] = array(
    'name' => 'delete terms in 2',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: delete terms in 3.
  $permissions['delete terms in 3'] = array(
    'name' => 'delete terms in 3',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: display drupal links.
  $permissions['display drupal links'] = array(
    'name' => 'display drupal links',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'admin_menu',
  );

  // Exported permission: edit all webform submissions.
  $permissions['edit all webform submissions'] = array(
    'name' => 'edit all webform submissions',
    'roles' => array(),
    'module' => 'webform',
  );

  // Exported permission: edit any commerce_customer_profile entity.
  $permissions['edit any commerce_customer_profile entity'] = array(
    'name' => 'edit any commerce_customer_profile entity',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: edit any commerce_customer_profile entity of bundle billing.
  $permissions['edit any commerce_customer_profile entity of bundle billing'] = array(
    'name' => 'edit any commerce_customer_profile entity of bundle billing',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: edit any commerce_customer_profile entity of bundle shipping.
  $permissions['edit any commerce_customer_profile entity of bundle shipping'] = array(
    'name' => 'edit any commerce_customer_profile entity of bundle shipping',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: edit any commerce_order entity.
  $permissions['edit any commerce_order entity'] = array(
    'name' => 'edit any commerce_order entity',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: edit any commerce_order entity of bundle commerce_order.
  $permissions['edit any commerce_order entity of bundle commerce_order'] = array(
    'name' => 'edit any commerce_order entity of bundle commerce_order',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: edit any commerce_product entity.
  $permissions['edit any commerce_product entity'] = array(
    'name' => 'edit any commerce_product entity',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: edit any commerce_product entity of bundle magazyn.
  $permissions['edit any commerce_product entity of bundle magazyn'] = array(
    'name' => 'edit any commerce_product entity of bundle magazyn',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: edit any commerce_product entity of bundle na_sztuki.
  $permissions['edit any commerce_product entity of bundle na_sztuki'] = array(
    'name' => 'edit any commerce_product entity of bundle na_sztuki',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: edit any commerce_product entity of bundle product.
  $permissions['edit any commerce_product entity of bundle product'] = array(
    'name' => 'edit any commerce_product entity of bundle product',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: edit any commerce_product entity of bundle sztuki_magazyn.
  $permissions['edit any commerce_product entity of bundle sztuki_magazyn'] = array(
    'name' => 'edit any commerce_product entity of bundle sztuki_magazyn',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: edit any kasjer content.
  $permissions['edit any kasjer content'] = array(
    'name' => 'edit any kasjer content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any kasjer_2 content.
  $permissions['edit any kasjer_2 content'] = array(
    'name' => 'edit any kasjer_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any kasjer_3 content.
  $permissions['edit any kasjer_3 content'] = array(
    'name' => 'edit any kasjer_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any koordynacja content.
  $permissions['edit any koordynacja content'] = array(
    'name' => 'edit any koordynacja content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any koordynacja_2 content.
  $permissions['edit any koordynacja_2 content'] = array(
    'name' => 'edit any koordynacja_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any nadz_r_ksi_gowo_ci content.
  $permissions['edit any nadz_r_ksi_gowo_ci content'] = array(
    'name' => 'edit any nadz_r_ksi_gowo_ci content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any nadz_r_ksi_gowo_ci_2 content.
  $permissions['edit any nadz_r_ksi_gowo_ci_2 content'] = array(
    'name' => 'edit any nadz_r_ksi_gowo_ci_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any page content.
  $permissions['edit any page content'] = array(
    'name' => 'edit any page content',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any sprz_tanie content.
  $permissions['edit any sprz_tanie content'] = array(
    'name' => 'edit any sprz_tanie content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any sprz_tanie_2 content.
  $permissions['edit any sprz_tanie_2 content'] = array(
    'name' => 'edit any sprz_tanie_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any sprz_tanie_3 content.
  $permissions['edit any sprz_tanie_3 content'] = array(
    'name' => 'edit any sprz_tanie_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any transport content.
  $permissions['edit any transport content'] = array(
    'name' => 'edit any transport content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any transport_2 content.
  $permissions['edit any transport_2 content'] = array(
    'name' => 'edit any transport_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any tura content.
  $permissions['edit any tura content'] = array(
    'name' => 'edit any tura content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any typ_produktu content.
  $permissions['edit any typ_produktu content'] = array(
    'name' => 'edit any typ_produktu content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any wazacy content.
  $permissions['edit any wazacy content'] = array(
    'name' => 'edit any wazacy content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any wazacy_2 content.
  $permissions['edit any wazacy_2 content'] = array(
    'name' => 'edit any wazacy_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any wazacy_3 content.
  $permissions['edit any wazacy_3 content'] = array(
    'name' => 'edit any wazacy_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any webform content.
  $permissions['edit any webform content'] = array(
    'name' => 'edit any webform content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own commerce_customer_profile entities.
  $permissions['edit own commerce_customer_profile entities'] = array(
    'name' => 'edit own commerce_customer_profile entities',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: edit own commerce_customer_profile entities of bundle billing.
  $permissions['edit own commerce_customer_profile entities of bundle billing'] = array(
    'name' => 'edit own commerce_customer_profile entities of bundle billing',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: edit own commerce_customer_profile entities of bundle shipping.
  $permissions['edit own commerce_customer_profile entities of bundle shipping'] = array(
    'name' => 'edit own commerce_customer_profile entities of bundle shipping',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: edit own commerce_order entities.
  $permissions['edit own commerce_order entities'] = array(
    'name' => 'edit own commerce_order entities',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: edit own commerce_order entities of bundle commerce_order.
  $permissions['edit own commerce_order entities of bundle commerce_order'] = array(
    'name' => 'edit own commerce_order entities of bundle commerce_order',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: edit own commerce_product entities.
  $permissions['edit own commerce_product entities'] = array(
    'name' => 'edit own commerce_product entities',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: edit own commerce_product entities of bundle magazyn.
  $permissions['edit own commerce_product entities of bundle magazyn'] = array(
    'name' => 'edit own commerce_product entities of bundle magazyn',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: edit own commerce_product entities of bundle na_sztuki.
  $permissions['edit own commerce_product entities of bundle na_sztuki'] = array(
    'name' => 'edit own commerce_product entities of bundle na_sztuki',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: edit own commerce_product entities of bundle product.
  $permissions['edit own commerce_product entities of bundle product'] = array(
    'name' => 'edit own commerce_product entities of bundle product',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: edit own commerce_product entities of bundle sztuki_magazyn.
  $permissions['edit own commerce_product entities of bundle sztuki_magazyn'] = array(
    'name' => 'edit own commerce_product entities of bundle sztuki_magazyn',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: edit own kasjer content.
  $permissions['edit own kasjer content'] = array(
    'name' => 'edit own kasjer content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own kasjer_2 content.
  $permissions['edit own kasjer_2 content'] = array(
    'name' => 'edit own kasjer_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own kasjer_3 content.
  $permissions['edit own kasjer_3 content'] = array(
    'name' => 'edit own kasjer_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own koordynacja content.
  $permissions['edit own koordynacja content'] = array(
    'name' => 'edit own koordynacja content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own koordynacja_2 content.
  $permissions['edit own koordynacja_2 content'] = array(
    'name' => 'edit own koordynacja_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own nadz_r_ksi_gowo_ci content.
  $permissions['edit own nadz_r_ksi_gowo_ci content'] = array(
    'name' => 'edit own nadz_r_ksi_gowo_ci content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own nadz_r_ksi_gowo_ci_2 content.
  $permissions['edit own nadz_r_ksi_gowo_ci_2 content'] = array(
    'name' => 'edit own nadz_r_ksi_gowo_ci_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own page content.
  $permissions['edit own page content'] = array(
    'name' => 'edit own page content',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own sprz_tanie content.
  $permissions['edit own sprz_tanie content'] = array(
    'name' => 'edit own sprz_tanie content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own sprz_tanie_2 content.
  $permissions['edit own sprz_tanie_2 content'] = array(
    'name' => 'edit own sprz_tanie_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own sprz_tanie_3 content.
  $permissions['edit own sprz_tanie_3 content'] = array(
    'name' => 'edit own sprz_tanie_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own transport content.
  $permissions['edit own transport content'] = array(
    'name' => 'edit own transport content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own transport_2 content.
  $permissions['edit own transport_2 content'] = array(
    'name' => 'edit own transport_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own tura content.
  $permissions['edit own tura content'] = array(
    'name' => 'edit own tura content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own typ_produktu content.
  $permissions['edit own typ_produktu content'] = array(
    'name' => 'edit own typ_produktu content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own wazacy content.
  $permissions['edit own wazacy content'] = array(
    'name' => 'edit own wazacy content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own wazacy_2 content.
  $permissions['edit own wazacy_2 content'] = array(
    'name' => 'edit own wazacy_2 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own wazacy_3 content.
  $permissions['edit own wazacy_3 content'] = array(
    'name' => 'edit own wazacy_3 content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own webform content.
  $permissions['edit own webform content'] = array(
    'name' => 'edit own webform content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own webform submissions.
  $permissions['edit own webform submissions'] = array(
    'name' => 'edit own webform submissions',
    'roles' => array(),
    'module' => 'webform',
  );

  // Exported permission: edit terms in 1.
  $permissions['edit terms in 1'] = array(
    'name' => 'edit terms in 1',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: edit terms in 2.
  $permissions['edit terms in 2'] = array(
    'name' => 'edit terms in 2',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: edit terms in 3.
  $permissions['edit terms in 3'] = array(
    'name' => 'edit terms in 3',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: execute flag_comment_action.
  $permissions['execute flag_comment_action'] = array(
    'name' => 'execute flag_comment_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute flag_node_action.
  $permissions['execute flag_node_action'] = array(
    'name' => 'execute flag_node_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute flag_user_action.
  $permissions['execute flag_user_action'] = array(
    'name' => 'execute flag_user_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute node_assign_owner_action.
  $permissions['execute node_assign_owner_action'] = array(
    'name' => 'execute node_assign_owner_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute node_make_sticky_action.
  $permissions['execute node_make_sticky_action'] = array(
    'name' => 'execute node_make_sticky_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute node_make_unsticky_action.
  $permissions['execute node_make_unsticky_action'] = array(
    'name' => 'execute node_make_unsticky_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute node_promote_action.
  $permissions['execute node_promote_action'] = array(
    'name' => 'execute node_promote_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute node_publish_action.
  $permissions['execute node_publish_action'] = array(
    'name' => 'execute node_publish_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute node_save_action.
  $permissions['execute node_save_action'] = array(
    'name' => 'execute node_save_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute node_unpromote_action.
  $permissions['execute node_unpromote_action'] = array(
    'name' => 'execute node_unpromote_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute node_unpublish_action.
  $permissions['execute node_unpublish_action'] = array(
    'name' => 'execute node_unpublish_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute node_unpublish_by_keyword_action.
  $permissions['execute node_unpublish_by_keyword_action'] = array(
    'name' => 'execute node_unpublish_by_keyword_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute pathauto_node_update_action.
  $permissions['execute pathauto_node_update_action'] = array(
    'name' => 'execute pathauto_node_update_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute pathauto_taxonomy_term_update_action.
  $permissions['execute pathauto_taxonomy_term_update_action'] = array(
    'name' => 'execute pathauto_taxonomy_term_update_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute pathauto_user_update_action.
  $permissions['execute pathauto_user_update_action'] = array(
    'name' => 'execute pathauto_user_update_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute system_block_ip_action.
  $permissions['execute system_block_ip_action'] = array(
    'name' => 'execute system_block_ip_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute system_goto_action.
  $permissions['execute system_goto_action'] = array(
    'name' => 'execute system_goto_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute system_message_action.
  $permissions['execute system_message_action'] = array(
    'name' => 'execute system_message_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute system_send_email_action.
  $permissions['execute system_send_email_action'] = array(
    'name' => 'execute system_send_email_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute user_block_user_action.
  $permissions['execute user_block_user_action'] = array(
    'name' => 'execute user_block_user_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute views_bulk_operations_archive_action.
  $permissions['execute views_bulk_operations_archive_action'] = array(
    'name' => 'execute views_bulk_operations_archive_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute views_bulk_operations_argument_selector_action.
  $permissions['execute views_bulk_operations_argument_selector_action'] = array(
    'name' => 'execute views_bulk_operations_argument_selector_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute views_bulk_operations_delete_item.
  $permissions['execute views_bulk_operations_delete_item'] = array(
    'name' => 'execute views_bulk_operations_delete_item',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute views_bulk_operations_modify_action.
  $permissions['execute views_bulk_operations_modify_action'] = array(
    'name' => 'execute views_bulk_operations_modify_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute views_bulk_operations_script_action.
  $permissions['execute views_bulk_operations_script_action'] = array(
    'name' => 'execute views_bulk_operations_script_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: execute views_bulk_operations_user_roles_action.
  $permissions['execute views_bulk_operations_user_roles_action'] = array(
    'name' => 'execute views_bulk_operations_user_roles_action',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'actions_permissions',
  );

  // Exported permission: flush caches.
  $permissions['flush caches'] = array(
    'name' => 'flush caches',
    'roles' => array(),
    'module' => 'admin_menu',
  );

  // Exported permission: grant content access.
  $permissions['grant content access'] = array(
    'name' => 'grant content access',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'content_access',
  );

  // Exported permission: grant own content access.
  $permissions['grant own content access'] = array(
    'name' => 'grant own content access',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'content_access',
  );

  // Exported permission: manage features.
  $permissions['manage features'] = array(
    'name' => 'manage features',
    'roles' => array(),
    'module' => 'features',
  );

  // Exported permission: node-specific print configuration.
  $permissions['node-specific print configuration'] = array(
    'name' => 'node-specific print configuration',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'print',
  );

  // Exported permission: notify of path changes.
  $permissions['notify of path changes'] = array(
    'name' => 'notify of path changes',
    'roles' => array(),
    'module' => 'pathauto',
  );

  // Exported permission: revert revisions.
  $permissions['revert revisions'] = array(
    'name' => 'revert revisions',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: search content.
  $permissions['search content'] = array(
    'name' => 'search content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'search',
  );

  // Exported permission: select account cancellation method.
  $permissions['select account cancellation method'] = array(
    'name' => 'select account cancellation method',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'user',
  );

  // Exported permission: send invitations.
  $permissions['send invitations'] = array(
    'name' => 'send invitations',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'invite',
  );

  // Exported permission: send mass invitations.
  $permissions['send mass invitations'] = array(
    'name' => 'send mass invitations',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'invite',
  );

  // Exported permission: switch shortcut sets.
  $permissions['switch shortcut sets'] = array(
    'name' => 'switch shortcut sets',
    'roles' => array(),
    'module' => 'shortcut',
  );

  // Exported permission: track invitations.
  $permissions['track invitations'] = array(
    'name' => 'track invitations',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'invite',
  );

  // Exported permission: translate interface.
  $permissions['translate interface'] = array(
    'name' => 'translate interface',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'locale',
  );

  // Exported permission: update payments.
  $permissions['update payments'] = array(
    'name' => 'update payments',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_payment',
  );

  // Exported permission: use advanced search.
  $permissions['use advanced search'] = array(
    'name' => 'use advanced search',
    'roles' => array(),
    'module' => 'search',
  );

  // Exported permission: use flag import.
  $permissions['use flag import'] = array(
    'name' => 'use flag import',
    'roles' => array(),
    'module' => 'flag',
  );

  // Exported permission: use text format filtered_html.
  $permissions['use text format filtered_html'] = array(
    'name' => 'use text format filtered_html',
    'roles' => array(
      0 => 'Administrator',
      1 => 'authenticated user',
    ),
    'module' => 'filter',
  );

  // Exported permission: use text format full_html.
  $permissions['use text format full_html'] = array(
    'name' => 'use text format full_html',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'filter',
  );

  // Exported permission: view Terms and Conditions.
  $permissions['view Terms and Conditions'] = array(
    'name' => 'view Terms and Conditions',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Kooperatysta',
      5 => 'Koordynator',
      6 => 'authenticated user',
    ),
    'module' => 'legal',
  );

  // Exported permission: view any commerce_customer_profile entity.
  $permissions['view any commerce_customer_profile entity'] = array(
    'name' => 'view any commerce_customer_profile entity',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: view any commerce_customer_profile entity of bundle billing.
  $permissions['view any commerce_customer_profile entity of bundle billing'] = array(
    'name' => 'view any commerce_customer_profile entity of bundle billing',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: view any commerce_customer_profile entity of bundle shipping.
  $permissions['view any commerce_customer_profile entity of bundle shipping'] = array(
    'name' => 'view any commerce_customer_profile entity of bundle shipping',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: view any commerce_order entity.
  $permissions['view any commerce_order entity'] = array(
    'name' => 'view any commerce_order entity',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: view any commerce_order entity of bundle commerce_order.
  $permissions['view any commerce_order entity of bundle commerce_order'] = array(
    'name' => 'view any commerce_order entity of bundle commerce_order',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: view any commerce_product entity.
  $permissions['view any commerce_product entity'] = array(
    'name' => 'view any commerce_product entity',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: view any commerce_product entity of bundle magazyn.
  $permissions['view any commerce_product entity of bundle magazyn'] = array(
    'name' => 'view any commerce_product entity of bundle magazyn',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: view any commerce_product entity of bundle na_sztuki.
  $permissions['view any commerce_product entity of bundle na_sztuki'] = array(
    'name' => 'view any commerce_product entity of bundle na_sztuki',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: view any commerce_product entity of bundle product.
  $permissions['view any commerce_product entity of bundle product'] = array(
    'name' => 'view any commerce_product entity of bundle product',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: view any commerce_product entity of bundle sztuki_magazyn.
  $permissions['view any commerce_product entity of bundle sztuki_magazyn'] = array(
    'name' => 'view any commerce_product entity of bundle sztuki_magazyn',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: view invite statistics.
  $permissions['view invite statistics'] = array(
    'name' => 'view invite statistics',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Koordynator',
    ),
    'module' => 'invite_stats',
  );

  // Exported permission: view own commerce_customer_profile entities.
  $permissions['view own commerce_customer_profile entities'] = array(
    'name' => 'view own commerce_customer_profile entities',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: view own commerce_customer_profile entities of bundle billing.
  $permissions['view own commerce_customer_profile entities of bundle billing'] = array(
    'name' => 'view own commerce_customer_profile entities of bundle billing',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: view own commerce_customer_profile entities of bundle shipping.
  $permissions['view own commerce_customer_profile entities of bundle shipping'] = array(
    'name' => 'view own commerce_customer_profile entities of bundle shipping',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: view own commerce_order entities.
  $permissions['view own commerce_order entities'] = array(
    'name' => 'view own commerce_order entities',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: view own commerce_order entities of bundle commerce_order.
  $permissions['view own commerce_order entities of bundle commerce_order'] = array(
    'name' => 'view own commerce_order entities of bundle commerce_order',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: view own commerce_product entities.
  $permissions['view own commerce_product entities'] = array(
    'name' => 'view own commerce_product entities',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Kooperatysta',
      5 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: view own commerce_product entities of bundle magazyn.
  $permissions['view own commerce_product entities of bundle magazyn'] = array(
    'name' => 'view own commerce_product entities of bundle magazyn',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: view own commerce_product entities of bundle na_sztuki.
  $permissions['view own commerce_product entities of bundle na_sztuki'] = array(
    'name' => 'view own commerce_product entities of bundle na_sztuki',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Kooperatysta',
      5 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: view own commerce_product entities of bundle product.
  $permissions['view own commerce_product entities of bundle product'] = array(
    'name' => 'view own commerce_product entities of bundle product',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Kooperatysta',
      5 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: view own commerce_product entities of bundle sztuki_magazyn.
  $permissions['view own commerce_product entities of bundle sztuki_magazyn'] = array(
    'name' => 'view own commerce_product entities of bundle sztuki_magazyn',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Dostawca Bronisze',
      2 => 'Dostawca Rutkowscy',
      3 => 'Kasjer/Kierowca/Ważący',
      4 => 'Kooperatysta',
      5 => 'Koordynator',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: view own invite statistics.
  $permissions['view own invite statistics'] = array(
    'name' => 'view own invite statistics',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'invite_stats',
  );

  // Exported permission: view own unpublished content.
  $permissions['view own unpublished content'] = array(
    'name' => 'view own unpublished content',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'node',
  );

  // Exported permission: view payments.
  $permissions['view payments'] = array(
    'name' => 'view payments',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Kooperatysta',
      3 => 'Koordynator',
    ),
    'module' => 'commerce_payment',
  );

  // Exported permission: view revisions.
  $permissions['view revisions'] = array(
    'name' => 'view revisions',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: view the administration theme.
  $permissions['view the administration theme'] = array(
    'name' => 'view the administration theme',
    'roles' => array(
      0 => 'Administrator',
      1 => 'Kasjer/Kierowca/Ważący',
      2 => 'Koordynator',
    ),
    'module' => 'system',
  );

  // Exported permission: withdraw own accepted invitations.
  $permissions['withdraw own accepted invitations'] = array(
    'name' => 'withdraw own accepted invitations',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'invite',
  );

  // Exported permission: withdraw own invitations.
  $permissions['withdraw own invitations'] = array(
    'name' => 'withdraw own invitations',
    'roles' => array(
      0 => 'Administrator',
    ),
    'module' => 'invite',
  );

  return $permissions;
}
