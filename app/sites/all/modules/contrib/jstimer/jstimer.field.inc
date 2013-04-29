<?php
/**
 * @file
 * Field hooks to implement a jstimer field.
 */

/**
 * Implements hook_field_formatter_info().
 */
function jstimer_field_formatter_info() {
  $widgets = jstimer_get_widgets();
  $formatters = array();
  foreach ($widgets as $widget) {
    $key = $widget->name;
    $formatters[$key] = array(
      'label' => $widget->label,
      'field types' => array('date', 'datestamp', 'datetime'),
    );
    if (isset($widget->settings)) {
      $formatters[$key]['settings'] = $widget->settings;
    }
  }
  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function jstimer_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $formatter = $display['type'];
  $widgets = jstimer_get_widgets();
  $widget_found = '';
  foreach ($widgets as $widget) {
    if ($widget->name == $formatter) {
      $widget_found = $widget;
    }
  }
  if (!empty($widget_found)) {
    if (isset($widget->settings)) {
      $form_callback = $widget->name.'_formatter_settings_form';
      return $form_callback ($field, $instance, $view_mode, $form, $form_state);
    }
  }
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function jstimer_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $formatter = $display['type'];
  $widgets = jstimer_get_widgets();
  $widget_found = '';
  foreach ($widgets as $widget) {
    if ($widget->name == $formatter) {
      $widget_found = $widget;
    }
  }
  if (!empty($widget_found)) {
    if (isset($widget->settings)) {
      $form_callback = $widget->name.'_formatter_settings_summary';
      return $form_callback ($field, $instance, $view_mode);
    }
  }
}


/**
 * Implements hook_field_formatter_view().
 *
 */
function jstimer_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];
  $widgets = jstimer_get_widgets();
  $widget_names = array();
  foreach ($widgets as $widget) {
    $widget_names[] = $widget->name;
  }
  if (in_array($display['type'], $widget_names)) {
    foreach ($items as $delta => $item) {
      if (!empty($entity->date_id) && !in_array($delta, $selected_deltas)) {
        continue;
      }
      else {
        $dates = date_formatter_process('date_default', $entity_type, $entity, $field, $instance, $langcode, $item, $display);
        if (empty($item['value2']) || $item['value'] == $item['value2']) {
          $widget_args = _jstimer_date_to_widget($dates['value']['formatted_iso'], $display['type'], $settings);
          $element[$delta] = array('#markup' => theme('jstimer', $widget_args));
        }
        else {
          $widget_val = _jstimer_date_to_widget($dates['value']['formatted_iso'], $display['type'], $settings);
          $widget_val2 = _jstimer_date_to_widget($dates['value2']['formatted_iso'], $display['type'], $settings);
          $element[$delta] = array('#markup' => t('!start-date to !end-date', array('!start-date' => theme('jstimer', $widget_val), '!end-date' => theme('jstimer', $widget_val2))));
        }
      }
    }
  }
  return $element;
}

function _jstimer_date_to_widget($date, $widget_name, $settings = array()) {
  $datetime = date_iso8601(strtotime($date));
  $widget_args = array(
    'widget_name' => $widget_name,
    'widget_args' => array(
      'datetime' => $datetime,
    )
  );
  $widget_args['widget_args'] += $settings;
  return $widget_args;
}
