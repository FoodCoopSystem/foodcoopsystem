<?php
/**
 * @file
 * wks_layout.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function wks_layout_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'admin_context';
  $context->description = 'Admin role context';
  $context->tag = 'Admin context';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '~cart' => '~cart',
        '~admin*' => '~admin*',
        '~node/add*' => '~node/add*',
        '~tura/*' => '~tura/*',
        '~tura' => '~tura',
        '~archiwum*' => '~archiwum*',
      ),
    ),
    'user' => array(
      'values' => array(
        'Administrator' => 'Administrator',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-administrator-menu' => array(
          'module' => 'menu',
          'delta' => 'menu-administrator-menu',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Admin context');
  t('Admin role context');
  $export['admin_context'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'cart';
  $context->description = 'Kontekst związany z zamawianiem';
  $context->tag = 'Commerce';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'user/*' => 'user/*',
        'taxonomy/term/*' => 'taxonomy/term/*',
        '<front>' => '<front>',
        'produkty/*' => 'produkty/*',
      ),
    ),
    'user' => array(
      'values' => array(
        'Administrator' => 'Administrator',
        'Kasjer/Kierowca/Ważący' => 'Kasjer/Kierowca/Ważący',
        'Kooperatysta' => 'Kooperatysta',
        'Koordynator' => 'Koordynator',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'search-form' => array(
          'module' => 'search',
          'delta' => 'form',
          'region' => 'sidebar_first',
          'weight' => '-20',
        ),
        'commerce_cart-cart' => array(
          'module' => 'commerce_cart',
          'delta' => 'cart',
          'region' => 'sidebar_first',
          'weight' => '-20',
        ),
        'tagclouds-3' => array(
          'module' => 'tagclouds',
          'delta' => 3,
          'region' => 'sidebar_first',
          'weight' => '-16',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Commerce');
  t('Kontekst związany z zamawianiem');
  $export['cart'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'koordynator_block';
  $context->description = 'Kontekst związany z zarządzaniem';
  $context->tag = 'Admin context';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'user/*' => 'user/*',
        'user' => 'user',
        'taxonomy/term/*' => 'taxonomy/term/*',
        '<front>' => '<front>',
        'produkty/*' => 'produkty/*',
        'produkty' => 'produkty',
      ),
    ),
    'user' => array(
      'values' => array(
        'Administrator' => 'Administrator',
        'Kasjer/Kierowca/Ważący' => 'Kasjer/Kierowca/Ważący',
        'Koordynator' => 'Koordynator',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-menu-funkcyjne' => array(
          'module' => 'menu',
          'delta' => 'menu-menu-funkcyjne',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Admin context');
  t('Kontekst związany z zarządzaniem');
  $export['koordynator_block'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'order_profile';
  $context->description = 'Order profile context';
  $context->tag = 'Commerce';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'user/*/orders/*' => 'user/*/orders/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'wks_user-back_to_orders' => array(
          'module' => 'wks_user',
          'delta' => 'back_to_orders',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Commerce');
  t('Order profile context');
  $export['order_profile'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'prints_context';
  $context->description = 'Function role context';
  $context->tag = 'Admin context';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '~cart' => '~cart',
        '~admin*' => '~admin*',
        '~node/add*' => '~node/add*',
        '~tura/*' => '~tura/*',
        '~tura' => '~tura',
        '~archiwum*' => '~archiwum*',
      ),
    ),
    'user' => array(
      'values' => array(
        'Kasjer/Kierowca/Ważący' => 'Kasjer/Kierowca/Ważący',
        'Koordynator' => 'Koordynator',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-menu-prints' => array(
          'module' => 'menu',
          'delta' => 'menu-menu-prints',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Admin context');
  t('Function role context');
  $export['prints_context'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'products';
  $context->description = 'Kontekst związany z listą produktów';
  $context->tag = 'Commerce';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
        'produkty' => 'produkty',
      ),
    ),
    'user' => array(
      'values' => array(
        'Administrator' => 'Administrator',
        'Kasjer/Kierowca/Ważący' => 'Kasjer/Kierowca/Ważący',
        'Kooperatysta' => 'Kooperatysta',
        'Koordynator' => 'Koordynator',
        'authenticated user' => 'authenticated user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-tura-block_1' => array(
          'module' => 'views',
          'delta' => 'tura-block_1',
          'region' => 'content',
          'weight' => '-10',
        ),
        'quicktabs-produkty' => array(
          'module' => 'quicktabs',
          'delta' => 'produkty',
          'region' => 'content',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Commerce');
  t('Kontekst związany z listą produktów');
  $export['products'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'tura';
  $context->description = 'Kontekst Tury';
  $context->tag = 'CRM';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'tura' => 'tura',
        'tura:page' => 'tura:page',
        'tura:page_1' => 'tura:page_1',
      ),
    ),
  );
  $context->reactions = array(
    'theme' => array(
      'title' => '<none>',
      'subtitle' => '',
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('CRM');
  t('Kontekst Tury');
  $export['tura'] = $context;

  return $export;
}
