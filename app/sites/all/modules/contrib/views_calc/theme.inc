<?php

/**
 * @file theme.inc
 *
 * An array of preprocessors to fill variables for templates and helper
 * functions to make theming easier.
 */

/**
 * Theme the form for the table style plugin
 */
function theme_views_calc_ui_table($variables) {
  $form = $variables['form'];

  $output = drupal_render($form['description_markup']);

  $header = array(
    t('Field'),
    t('Justification'),
    t('Column calculations'),
    t('Column'),
    t('Separator'),
    array(
      'data' => t('Sortable'),
      'align' => 'center',
    ),
    array(
      'data' => t('Default sort'),
      'align' => 'center',
    ),
    array(
      'data' => t('Default sort order'),
      'align' => 'center',
    ),
  );

  $rows = array();
  foreach (element_children($form['columns']) as $id) {
    $row = array();
    $row[] = drupal_render($form['info'][$id]['name']);
    $row[] = drupal_render($form['info'][$id]['align']);
    $row[] = drupal_render($form['info'][$id]['has_calc']) . drupal_render($form['info'][$id]['calc']);
    $row[] = drupal_render($form['columns'][$id]);
    $row[] = drupal_render($form['info'][$id]['separator']);
    if (!empty($form['info'][$id]['sortable'])) {
      $row[] = array(
        'data' => drupal_render($form['info'][$id]['sortable']),
        'align' => 'center',
      );
      $row[] = array(
        'data' => drupal_render($form['default'][$id]),
        'align' => 'center',
      );
      $row[] = array(
        'data' => drupal_render($form['info'][$id]['default_sort_order']),
        'align' => 'center',
      );
    }
    else {
      $row[] = '';
      $row[] = '';
      $row[] = '';
    }
    // Remove the option to hide empty columns.
    // This may create problems since we need to know what
    // the columns are for the summary queries.
    unset($form['info'][$id]['empty_column']);
    $rows[] = $row;
  }

  // Add the special 'None' row.
  $rows[] = array(t('None'), '', '', '', '', '', array(
      'align' => 'center',
      'data' => drupal_render($form['default'][-1]),
    ), '');

  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  $output .= drupal_render_children($form);
  return $output;
}


/**
 * Display a view as a table style.
 */
function template_preprocess_views_calc_table(&$vars) {
  $view = $vars['view'];
  if (!empty($view->views_calc_calculation)) {
    $vars['rows'] = array();
    return;
  }

  drupal_add_css(drupal_get_path('module', 'views_calc') . '/views_calc.css');

  // We need the raw data for this grouping, which is passed in as $vars['rows'].
  // However, the template also needs to use for the rendered fields.  We
  // therefore swap the raw data out to a new variable and reset $vars['rows']
  // so that it can get rebuilt.
  $result   = $vars['rows'];
  $vars['rows'] = array();

  $options  = $view->style_plugin->options;
  $handler  = $view->style_plugin;
  $vars['options'] = $options;
  $hide_details = $options['detailed_values'];

  $fields   = &$view->field;
  $columns  = $handler->sanitize_columns($options['columns'], $fields);

  $active   = !empty($handler->active) ? $handler->active : '';
  $order    = !empty($handler->order) ? $handler->order : 'asc';

  $query    = tablesort_get_query_parameters();

  if (isset($view->exposed_raw_input)) {
    $query += $view->exposed_raw_input;
  }

  // Render the header labels.
  foreach ($columns as $field => $column) {
    // Create a second variable so we can easily find what fields we have and what the
    // CSS classes should be.
    $vars['fields'][$field] = drupal_clean_css_identifier($field);
    if ($active == $field) {
      $vars['fields'][$field] .= ' active';
    }

    // render the header labels
    if ($field == $column && empty($fields[$field]->options['exclude'])) {
      $label = check_plain(!empty($fields[$field]) ? $fields[$field]->label() : '');
      if (empty($options['info'][$field]['sortable']) || !$fields[$field]->click_sortable()) {
        $vars['header'][$field] = $label;
      }
      else {
        $initial = !empty($options['info'][$field]['default_sort_order']) ? $options['info'][$field]['default_sort_order'] : 'asc';

        if ($active == $field) {
          $initial = ($order == 'asc') ? 'desc' : 'asc';
        }

        $image = theme('tablesort_indicator', array('style' => $initial));
        $title = t('sort by @s', array('@s' => $label));
        if ($active == $field) {
          $label .= theme('tablesort_indicator', array('style' => $initial));
        }

        $query['order'] = $field;
        $query['sort'] = $initial;
        $link_options = array(
          'html' => true,
          'attributes' => array('title' => $title),
          'query' => $query,
        );
        $vars['header'][$field] = l($label, $_GET['q'], $link_options);
      }
      $vars['header_classes'][$field] = '';
      // Set up the header label class.
      if ($fields[$field]->options['element_default_classes']) {
        $vars['header_classes'][$field] .= "views-field views-field-" . $vars['fields'][$field];
      }
      $class = $fields[$field]->element_label_classes(0);
      if ($class) {
        if ($vars['header_classes'][$field]) {
          $vars['header_classes'][$field] .= ' ';
        }
        $vars['header_classes'][$field] .= $class;
      }

      // Add a header label wrapper if one was selected.
      if ($vars['header'][$field]) {
        $element_label_type = $fields[$field]->element_label_type(TRUE, TRUE);
        if ($element_label_type) {
          $vars['header'][$field] = '<' . $element_label_type . '>' . $vars['header'][$field] . '</' . $element_label_type . '>';
        }
      }

    }

    // Add a CSS align class to each field if one was set
    if (!empty($options['info'][$field]['align'])) {
      $vars['fields'][$field] .= ' ' . drupal_clean_css_identifier($options['info'][$field]['align']);
    }
  }

  // Render each field into its appropriate column. Preserve rows.
  foreach ($result as $num => $row) {
    foreach ($columns as $field => $column) {
      $field_output = $fields[$field]->theme($row);
      if (!empty($fields[$field]) && empty($fields[$field]->options['exclude'])) {

        // Don't bother with separators and stuff if the field does not show up.
        if (!isset($field_output) && isset($vars['rows'][$num][$column])) {
          continue;
        }

        if ($hide_details) {
          continue;
        }

        // Place the field into the column, along with an optional separator.
        if (isset($vars['rows'][$num][$column])) {
          if (!empty($options['info'][$column]['separator'])) {
            $vars['rows'][$num][$column] .= filter_xss_admin($options['info'][$column]['separator']);
          }
        }
        else {
          $vars['rows'][$num][$column] = '';
        }

        $vars['rows'][$num][$column] .= $field_output;
      }
    }
  }

  // Add totals.
  $vars['totals'] = array();
  $vars['sub_totals'] = array();
  if ($view->query->pager->get_total_items() > $view->get_items_per_page()
      && $view->sub_totals) {
    views_calc_table_total($vars, 'sub_totals', $view->sub_totals);
  }
  if ($view->totals) {
    views_calc_table_total($vars, 'totals', $view->totals);
  }

  // Add classes.
  $vars['class'] = 'views-table';
  if (!empty($options['sticky'])) {
    drupal_add_js('misc/tableheader.js');
    $vars['class'] .= " sticky-enabled";
  }
}

/**
 * Build total var line.
 */
function views_calc_table_total(&$vars, $key, $totals) {

  $view = $vars['view'];

  $options  = $view->style_plugin->options;
  $handler  = $view->style_plugin;
  $fields   = &$view->field;
  $columns  = $handler->sanitize_columns($options['columns'], $fields);

  $vars[$key] = array();
  $added_label = array();
  // Build a data baserow with the default fields of the view
  $baserow = new StdClass();
  foreach ($view->field as $field) {
    if (isset($field->aliases['entity_type'])) {
      $query_alias = $field->aliases['entity_type'];
    }
    else {
      $query_alias = $field->field_alias;
    }
    $baserow->{$query_alias} = $totals->{$query_alias};
  }
  // Build aggregation rows, one per function.
  foreach ($view->views_calc_fields as $calc => $calc_fields) {
    // Rebuild row as if it where single queries.
    $row = clone $baserow;

    foreach ($view->field as $field) {
      if (isset($field->aliases['entity_type'])) {
        $query_alias = $field->aliases['entity_type'];
      }
      else {
        $query_alias = $field->field_alias;
      }
      $ext_alias = strtolower($calc) . '__' . $query_alias;
      if (in_array($field->field, $calc_fields)) {
        $row->{$query_alias} = $totals->{$ext_alias};
      }
    }
    // Build row output data.
    foreach ($columns as $field => $column) {
      if ((array_key_exists('entity_type', $fields[$field]->aliases))
      && isset($fields[$field]->aliases['entity_type'])) {
        $field_alias = $fields[$field]->aliases['entity_type'];
      }
      else {
        $field_alias = $fields[$field]->field_alias;
      }
      if ($field == $column && empty($fields[$field]->options['exclude'])) {
        // Process only calculated, non-excluded fields.
        if (in_array($field, $calc_fields) && isset($row->$field_alias)) {

          if ($calc == 'COUNT') {

            // COUNT is always a numeric value, no matter what kind of field it is.

            $vars[$key][$calc][$column] =  number_format($row->$field_alias, 0, 0, ',');
          }
          else {

            // Calculations other than COUNT should run the value through the field's theme.
            // This will allow dates and numeric values to apply the right formatting to the result.
            // Unfortunately, there seems to be no easy way to push an arbitrary value through
            // the field theme. The theme may be retrieving its value from the cached entity.
            // We would like to do $fields[$field]->theme($row), but that won't work.

            // If this is a numeric field, get its options. We can at least use that in the aggregation.
            // The settings we need are in ->options['settings'] for fields created by the field module,
            // and in ->options for other fields that are based on the numeric field handler.
            if (isset($fields[$field]->field_info)) {
              $separator = !empty($fields[$field]->options['settings']['thousands_separator']) ? $fields[$field]->options['settings']['thousands_separator'] : $options['separator'];
              $$decimal = !empty($fields[$field]->options['settings']['decimal_separator']) ? $fields[$field]->options['settings']['decimal_separator'] : $options['decimal'];
              $precision = !empty($fields[$field]->options['settings']['scale']) ? $fields[$field]->options['settings']['scale'] : $options['precision'];
            }
            else {
              $separator = !empty($fields[$field]->options['separator']) ? $fields[$field]->options['separator'] : $options['separator'];
              $decimal = !empty($fields[$field]->options['decimal']) ? $fields[$field]->options['decimal'] : $options['decimal'];
              $precision = !empty($fields[$field]->options['precision']) ? $fields[$field]->options['precision'] : $options['precision'];
            }
            $vars[$key][$calc][$column] = number_format($row->$field_alias, $precision, $decimal, $separator);
          }
        }
        else {
          // Add the calc type label into the first empty column.
          // Identify which is the sub total and which the grand total
          // when both are provided.
          if (empty($added_label[$calc])) {
            if ($key == 'sub_totals') {
              $label = t("Page !Calculation", array("!Calculation" => $calc));
            }
            else {
              $label = t("Total !Calculation", array("!Calculation" => $calc));
            }
            $vars[$key][$calc][$column] = $label;
            $added_label[$calc] = TRUE;
          }
          else {
            $vars[$key][$calc][$column] = '';
          }
        }
      }
    }
  }
}
