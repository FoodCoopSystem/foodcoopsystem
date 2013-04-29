<?php
/**
 * @file
 * Dashboard for reporting module.
 */

define('COMMERCE_REPORTS_DASHBOARD_BLOCK', 1);
define('COMMERCE_REPORTS_DASHBOARD_BROAD_BLOCK', 2);
define('COMMERCE_REPORTS_DASHBOARD_ROW', 3);

function commerce_reports_dashboard_block_width($items = array()) {
  $width = 0;

  foreach ($items as $item) {
    $width += isset($item['#width']) ? $item['#width'] : COMMERCE_REPORTS_DASHBOARD_BLOCK;
  }

  return $width;
}

/**
 * Renders the dashboard.
 */
function commerce_reports_dashboard() {
  drupal_add_js(drupal_get_path('module', 'commerce_reports') . '/js/dashboard.js', 'file');
  drupal_add_css(drupal_get_path('module', 'commerce_reports') . '/commerce_reports_dashboard.css');

  $info = module_invoke_all('commerce_reports_dashboard');
  drupal_alter('commerce_reports_dashboard', $info);

  $weights = array();
  foreach ($info as $block) {
    $weights[] = isset($block['weight']) ? $block['weight'] : 1000;
  }

  array_multisort($weights, $info);

  $render = array();

  $items = array();
  foreach ($info as $name => $block) {
    $type = isset($block['type']) ? $block['type'] : COMMERCE_REPORTS_DASHBOARD_BLOCK;

    if (commerce_reports_dashboard_block_width($items) + $type > 3) {
      $render[] = commerce_reports_dashboard_row($items);
      $items = array();
    }

    $items[] = commerce_reports_dashboard_block($name, $block);
  }

  if (!empty($items)) {
    $render[] = commerce_reports_dashboard_row($items);
    $items = array();
  }

  return $render;
}

function commerce_reports_dashboard_row($items = array()) {
  return array(
    '#theme_wrappers' => array('commerce_reports_dashboard_row'),
    'items' => $items,
  );
}

/**
 * Renders a single block on the dashboard.
 */
function commerce_reports_dashboard_block($name, $options = array()) {
  $defaults = array(
    'title' => '',
    'sections' => array(),
  );

  $options += $defaults;

  $render = array(
    '#theme_wrappers' => array('commerce_reports_dashboard_block'),
    '#title' => $options['title'],
    '#name' => $name,
    '#report' => !empty($options['report']) ? l($options['report']['title'], $options['report']['path']) : NULL,
    '#width' => !empty($options['type']) ? $options['type'] : COMMERCE_REPORTS_DASHBOARD_BLOCK,
  );

  $section_links = array();

  $keys = array_keys($options['sections']);
  $first = reset($keys);

  foreach ($options['sections'] as $name => $information) {
    $block = block_load($information['module'], $information['block']);
    $block_render = module_invoke($block->module, 'block_view', $block->delta);

    $render['sections'][$name] = $block_render;
    $render['sections'][$name]['#width'] = empty($options['switchSections']) ? 100 : NULL;

    if (!empty($options['switchSections'])) {
      $link = array(
        'title' => $information['title'],
        'href' => 'admin/commerce/reports/dashboard',
        'attributes' => array(
          'class' => array(
            'switchSection'
          ),
          'data-section' => $name,
        ),
      );

      if ($name === $first) {
        $link['attributes']['class'][] = 'activated';
      }

      $section_links[] = $link;
    }
  }

  $render['#operations'] = theme('links', array('links' => $section_links, 'attributes' => array()));

  if (!empty($options['switchSections'])) {
    $render['#visible'] = array($first);
  }
  else {
    $render['#visible'] = array_keys($options['sections']);
  }

  return $render;
}

