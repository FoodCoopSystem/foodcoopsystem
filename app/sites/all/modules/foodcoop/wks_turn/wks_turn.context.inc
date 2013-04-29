<?php
/**
 * @file
 * wks_turn.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function wks_turn_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'turn_all';
  $context->description = 'Achieve and active turn';
  $context->tag = 'turn';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'tura:page' => 'tura:page',
        'tura:page_1' => 'tura:page_1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'system-main' => array(
          'module' => 'system',
          'delta' => 'main',
          'region' => 'content',
          'weight' => '-8',
        ),
        'menu-menu-tury' => array(
          'module' => 'menu',
          'delta' => 'menu-tury',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
    'menu' => 'tura',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Achieve and active turn');
  t('turn');
  $export['turn_all'] = $context;

  return $export;
}
