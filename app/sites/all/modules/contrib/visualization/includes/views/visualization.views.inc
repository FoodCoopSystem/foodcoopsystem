<?php
/**
 * @file
 * Integrates Views with Visualization.
 */

/**
 * Implements hook_views_plugins().
 */
function visualization_views_plugins() {
  return array(
    'style' => array(
      'visualization' => array(
        'title' => t('Visualization'),
        'help' => t('Display the resulting data set as a chart.'),
        'handler' => 'visualization_plugin_style',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE,
        'even empty' => TRUE,
        'type' => 'normal',
        'theme' => 'visualization',
        'theme path' => drupal_get_path('module', 'visualization') . '/theme',
        'theme file' => 'visualization.theme.inc',
      ),
    ),
  );
}
