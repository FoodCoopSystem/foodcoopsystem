<?php
/**
 * @file
 * wks_commerce.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function wks_commerce_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'suppliers';
  $context->description = 'Suppliers of products context';
  $context->tag = 'Commerce';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '~admin/commerce*' => '~admin/commerce*',
      ),
    ),
    'user' => array(
      'values' => array(
        'Dostawca Bronisze' => 'Dostawca Bronisze',
        'Dostawca Rutkowscy' => 'Dostawca Rutkowscy',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-menu-dostawcy' => array(
          'module' => 'menu',
          'delta' => 'menu-menu-dostawcy',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Commerce');
  t('Suppliers of products context');
  $export['suppliers'] = $context;

  return $export;
}
