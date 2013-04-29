<?php
/**
 * @file
 * wks_commerce.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function wks_commerce_taxonomy_default_vocabularies() {
  return array(
    'dostawca' => array(
      'name' => 'Dostawca',
      'machine_name' => 'dostawca',
      'description' => 'Dostawca warzyw',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
    'tagi_produkt_w' => array(
      'name' => 'Tagi produktów',
      'machine_name' => 'tagi_produkt_w',
      'description' => 'Tagi produktów tworzone przez użytkowników',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
    'tags' => array(
      'name' => 'Kategorie produktów',
      'machine_name' => 'tags',
      'description' => 'kategorie produktów ustalane na sztywno',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
  );
}
