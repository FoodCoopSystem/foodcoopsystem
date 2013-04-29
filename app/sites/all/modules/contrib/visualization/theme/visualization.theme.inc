<?php
/**
 * @file
 * Theme file for the visualization part of the module.
 */

/**
 * Preprocess function for visualization function.
 *
 * Used by both regular calls to theme() and the visualization Views handler.
 */
function template_preprocess_visualization(&$vars) {
  $options = $vars['options'];

  if (!empty($vars['view'])) {
    $view = $vars['view'];

    // Fields.
    foreach ($view->field as $name => $field) {
      $options['fields'][$name]['label'] = $field->options['label'];
    }

    // Title.
    if (empty($options['title'])) {
      $options['title'] = $view->human_name;
    }

    // Data.
    $data = $view->style_plugin->rendered_fields;
  }
  else {
    $data = $options['data'];
  }

  if (!empty($options['xAxis']['invert'])) {
    $data = array_reverse($data);
  }

  $chart_id = 'visualization_' . uniqid();

  $plugin = visualization_plugin($options['type']);
  $vars['chart'] = $plugin['handler']->render($chart_id, $data, $options);

  $vars['chart_attributes'] = drupal_attributes(
    array(
      'id' => $chart_id,
      'class' => array(
        'visualization-chart',
        'visualization-chart-' . $plugin['name'],
      ),
    )
  );

  $plugin['handler']->postRender();
}
