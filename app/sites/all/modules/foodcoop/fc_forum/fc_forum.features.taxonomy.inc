<?php
/**
 * @file
 * fc_forum.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function fc_forum_taxonomy_default_vocabularies() {
  return array(
    'forums' => array(
      'name' => 'Fora',
      'machine_name' => 'forums',
      'description' => 'Słownik nawigacyjny forum',
      'hierarchy' => '1',
      'module' => 'forum',
      'weight' => '-10',
    ),
  );
}
