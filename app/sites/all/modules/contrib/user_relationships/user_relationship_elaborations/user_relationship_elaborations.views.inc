<?php

/**
 * @file
 * User Relationships Elaboration Views support.
 **/

/**
 * Implements hook_views_data().
 */
function user_relationship_elaborations_views_data() {
  $tables['user_relationship_elaborations'] = array(
    'table' => array(
      'group' => t('User relationships'),
      'join' => array(
        'user_relationships' => array(
          'table' => 'user_relationship_elaborations',
          'left_field' => 'rid',
          'field' => 'rid',
         ),
       ),
      'base' => array(
        'field' => 'rid',
        'title' => t('User relationship'),
        'help' => t('Relationships that link users to one another. Use this to show user data, profile fields, or content_profile nodes of related users.'),
        'weight' => 0,
      ),
    ),
    'elaboration' => array(
      'title' => t('Elaboration'),
      'help' => t('Provides the elaboration of the relationship request.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
  );
  return $tables;
}
