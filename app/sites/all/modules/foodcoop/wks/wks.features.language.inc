<?php
/**
 * @file
 * wks.features.language.inc
 */

/**
 * Implements hook_locale_default_languages().
 */
function wks_locale_default_languages() {
  $languages = array();

  // Exported language: en.
  $languages['en'] = array(
    'language' => 'en',
    'name' => 'English',
    'native' => 'English',
    'direction' => '0',
    'enabled' => '1',
    'plurals' => '0',
    'formula' => '',
    'domain' => '',
    'prefix' => 'en',
    'weight' => '0',
  );
  // Exported language: pl.
  $languages['pl'] = array(
    'language' => 'pl',
    'name' => 'Polish',
    'native' => 'Polski',
    'direction' => '0',
    'enabled' => '1',
    'plurals' => '3',
    'formula' => '(($n==1)?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))',
    'domain' => '',
    'prefix' => 'pl',
    'weight' => '0',
  );
  return $languages;
}
