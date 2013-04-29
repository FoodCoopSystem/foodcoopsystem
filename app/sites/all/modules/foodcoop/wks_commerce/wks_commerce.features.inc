<?php
/**
 * @file
 * wks_commerce.features.inc
 */

/**
 * Implements hook_commerce_product_default_types().
 */
function wks_commerce_commerce_product_default_types() {
  $items = array(
    'magazyn' => array(
      'type' => 'magazyn',
      'name' => 'kg magazyn',
      'description' => 'Produkty w magazynie na kilogramy',
      'help' => ' SKU dodajemy według schematu "nazwa-towaru_magazyn".',
      'revision' => '1',
    ),
    'na_sztuki' => array(
      'type' => 'na_sztuki',
      'name' => 'sztuki',
      'description' => 'Produkty sprzedawane na opakowania lub sztuki',
      'help' => 'Produkt na sztuki.  SKU dodajemy według schematu "nazwa-towaru_dostawca".',
      'revision' => '1',
    ),
    'product' => array(
      'type' => 'product',
      'name' => 'kg',
      'description' => 'Podstawowy typ produktu.',
      'help' => 'Produkt podawany w kg. SKU dodajemy według schematu "nazwa-towaru_dostawca".',
      'revision' => '1',
    ),
    'sztuki_magazyn' => array(
      'type' => 'sztuki_magazyn',
      'name' => 'sztuki magazyn',
      'description' => 'Produkty na sztuki w magazynie',
      'help' => ' SKU dodajemy według schematu "nazwa-towaru_magazyn".',
      'revision' => '1',
    ),
  );
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function wks_commerce_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "commerce_decimal_quantities" && $api == "default_commerce_decimal_quantities_presets") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "quicktabs" && $api == "quicktabs") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function wks_commerce_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function wks_commerce_node_info() {
  $items = array(
    'typ_produktu' => array(
      'name' => t('Typ produktu'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Nazwa typu produktu'),
      'help' => '',
    ),
  );
  return $items;
}
