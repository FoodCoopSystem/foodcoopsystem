<?php
/**
 * @file
 * wks_user.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function wks_user_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'user_context';
  $context->description = 'Account context';
  $context->tag = 'User';
  $context->conditions = array(
    'user_page' => array(
      'values' => array(
        'form' => 'form',
        'view' => 'view',
      ),
      'options' => array(
        'mode' => 'current',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'user',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Account context');
  t('User');
  $export['user_context'] = $context;

  return $export;
}
