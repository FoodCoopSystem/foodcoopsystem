<?php
/**
 * @file
 * fc_forum.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function fc_forum_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'forum_context';
  $context->description = 'Forum context';
  $context->tag = 'Forum';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'forum*' => 'forum*',
        'forum' => 'forum',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'forum/',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Forum');
  t('Forum context');
  $export['forum_context'] = $context;

  return $export;
}
