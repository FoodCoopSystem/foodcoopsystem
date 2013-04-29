<?php
/**
 * Implements hook_views_data().
 *
 * Add calc fields to views field list.
 */
function views_calc_views_data() {
  $data = array();
  $results = _views_calc_fields();
  foreach ($results as $field) {
    $data[$field->base]['cid' . $field->cid] = array(
      'group' => t('Views Calc'),
      'title' => t($field->label),
      'help' => $field->calc,
      'field' => array(
        'field' => 'cid' . $field->cid,
        'table' => $field->base,
        'handler' => 'views_calc_field_handler',
        'click sortable' => TRUE,
        'allow empty' => TRUE,
        'cid' => $field->cid,
        'format' => $field->format,
        'custom' => $field->custom,
        'calc' => $field->calc,
      ),
      'sort' => array(
        'handler' => 'views_calc_handler_sort',
      ),
    );
  }
  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function views_calc_views_plugins() {
  $path = drupal_get_path('module', 'views_calc');
  $views_path = drupal_get_path('module', 'views');
  require_once DRUPAL_ROOT . '/' . "./$path/theme.inc";

  $data = array(
    'module' => 'views_calc', // This just tells our themes are elsewhere.
    'style' => array(
      // Style plugin for the navigation.
      'views_calc' => array(
        'title' => t('Views Calc Table'),
        'help' => t('Creates a table with column calculations.'),
        'handler' => 'views_calc_table',
        'path' => $path,
        'parent' => 'table',
        'theme' => 'views_calc_table',
        'theme file' => 'theme.inc',
        'theme path' => "$path",
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => FALSE,
      ),
    ),
  );
  return $data;
}
