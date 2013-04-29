<?php
/**
 * @file
 * wks_commerce.quicktabs.inc
 */

/**
 * Implements hook_quicktabs_default_quicktabs().
 */
function wks_commerce_quicktabs_default_quicktabs() {
  $export = array();

  $quicktabs = new stdClass();
  $quicktabs->disabled = FALSE; /* Edit this to true to make a default quicktabs disabled initially */
  $quicktabs->api_version = 1;
  $quicktabs->machine_name = 'produkty';
  $quicktabs->ajax = 0;
  $quicktabs->hide_empty_tabs = FALSE;
  $quicktabs->default_tab = 1;
  $quicktabs->title = 'Produkty';
  $quicktabs->tabs = array(
    0 => array(
      'vid' => 'produkty',
      'display' => 'page_7',
      'args' => '',
      'title' => 'Magazyn',
      'weight' => '-100',
      'type' => 'view',
    ),
    1 => array(
      'vid' => 'produkty',
      'display' => 'page',
      'args' => '',
      'title' => 'Wszystkie',
      'weight' => '-99',
      'type' => 'view',
    ),
    2 => array(
      'vid' => 'produkty',
      'display' => 'page_1',
      'args' => '',
      'title' => 'Warzywa',
      'weight' => '-98',
      'type' => 'view',
    ),
    3 => array(
      'vid' => 'produkty',
      'display' => 'page_2',
      'args' => '',
      'title' => 'Owoce',
      'weight' => '-98',
      'type' => 'view',
    ),
    4 => array(
      'vid' => 'produkty',
      'display' => 'page_4',
      'args' => '',
      'title' => 'Bakalie',
      'weight' => '-97',
      'type' => 'view',
    ),
    5 => array(
      'vid' => 'produkty',
      'display' => 'page_6',
      'args' => '',
      'title' => 'Strączki',
      'weight' => '-96',
      'type' => 'view',
    ),
    6 => array(
      'vid' => 'produkty',
      'display' => 'page_5',
      'args' => '',
      'title' => 'Inne',
      'weight' => '-95',
      'type' => 'view',
    ),
    7 => array(
      'vid' => 'produkty',
      'display' => 'page_8',
      'args' => '',
      'title' => 'Ulubione',
      'weight' => '-93',
      'type' => 'view',
    ),
  );
  $quicktabs->renderer = 'quicktabs';
  $quicktabs->style = 'Wkstabstyle';
  $quicktabs->options = array();

  // Translatables
  // Included for use with string extractors like potx.
  t('Bakalie');
  t('Inne');
  t('Magazyn');
  t('Owoce');
  t('Produkty');
  t('Strączki');
  t('Ulubione');
  t('Warzywa');
  t('Wszystkie');

  $export['produkty'] = $quicktabs;

  return $export;
}
