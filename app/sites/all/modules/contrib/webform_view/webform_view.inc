<?php
/**
 * @file
 *   Additional component for webform that allows views to be used as embeddable elements.
 */


/**
 * Implements _webform_defaults_component().
 *
 * @see _webform_select_view()
 *
 * Define the extra params we wish to save as settings for webform embedded view.
 * Specifically - the view ID.
 */
function _webform_defaults_view() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'view' => '',
      'description' => '',
      'private' => FALSE,
    ),
  );
}

/**
 * Presents the view options when editing a webform view component
 *
 * Allows an editor to select which view to pull in and embed.
 *
 * Implements _webform_edit_component().
 */
function _webform_edit_view($component) {
  $form = array();

  $options = _webform_view_options();

  $form['view'] = array(
    '#type' => 'select',
    '#title' => t('View'),
    '#options' => $options,
    '#parents' => array('extra', 'view'),
    '#default_value' => $component['extra']['view'],
    '#description' => t("Choose an existing view that will be embedded in the webform. Each row of this view will be displays and selections made there will become form fields that will be submitted as part of this form."),
  );
  return $form;
}

/**
 * Load Webform view options - a list of available views
 */
function _webform_view_options() {
  static $options;
  if (!isset($options)) {
    $views = views_get_enabled_views();
    $options = array();
    foreach ($views as $view_id => $view) {
      $options[$view->human_name] = array();
      foreach ($view->display as $display_id => $display) {
        // Create a key here, identical to that used by views_block for 'delta'
        $key = $view_id . '-' . $display_id;
        $options[$view->human_name][$key] = (object)array('option' => array($key => $display->display_title));
      }
    }
    drupal_alter('webform_view_options', $options);
  }
  return $options;
}


/**
 * Show the embedded view on the webform.
 *
 * Implements _webform_render_component().
 *
 * @return a render array representing the element to be themed.
 * May include form elements & nesting.
 */
function _webform_render_view($component, $value_serialized = NULL, $filter = TRUE) {
  if (!empty($value_serialized) && is_array($value_serialized)) {
    $value = unserialize(reset($value_serialized));
  }

  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;
  $view_key = $component['extra']['view'];
  $placeholder_fields = array();

  // Nicely stolen from views_block_view()
  // If the delta doesn't contain valid data return nothing.
  $explode = explode('-', $view_key);
  if (count($explode) != 2) {
    return;
  }

  list($name, $display_id) = $explode;
  // Load the view
  if (!($view = views_get_view($name))) {
    watchdog('webform_view', 'Invalid view - cannot embed %name %display_id', array('%name' => $name, '%display_id' => $display_id));
    return;
  }

  if (! $view->access($display_id)) {
    watchdog('webform_view', 'View access denied - cannot embed %name %display_id', array('%name' => $name, '%display_id' => $display_id));
  }

  // execute_display produces cooked text.
  // It can include a placeholder string for our form elements,
  // if a field chooses to use the 'webform_placeholder' as a renderer.
  $output = $view->execute_display($display_id);
  if (empty($output)) {
    return;
  }

  // Go through the rows to prepare the webform-like fields.
  // Do this by CLONING the children elements already nested in this one.
  foreach ($view->result as $index => $row) {

    // Key on the real row ID if possible
    $key = isset($row->nid) ? $row->nid : $index;

    // If reviewing or editing fields, the $value array may be available.
    // It's our job to put it back into the element.
    $component_value = isset($value[$key]) ? $value[$key] : array();
    // This is now an array representing the data last stored in this row.

    // One of these fields should probably be a textfield that can hold the item identifier
    // Should use tokens to prefill it. Would be good.
    // For now, just steal values direct from the row result.
    // A token like [node_title] will pull the id from a view - as long as the view row has that data
    $tokens = array();
    foreach ($row as $token_key => $token_val) {
      if (is_string($token_val)) {
        $tokens["[$token_key]"] = $token_val;
      }
      // Handle the way field data comes back from field-based views too. Incomplete.
      else if (is_array($token_val) && $sub_val = reset($token_val)) {
        if (isset($sub_val) && isset($sub_val['raw']) && isset($sub_val['raw']['value'])) {
          $tokens["[$token_key]"] = $sub_val['raw']['value'];
        }
      }
    }

    // Each component that is defined as a child of this view component
    // gets replicated.
    foreach ($component['children'] as $webform_component) {

      // Some webform welements are not native. @see webform_element_info
      $type = $webform_component['type'];
      $webform_elements = array('number' => 'webform_number');
      if (isset($webform_elements[$type])) {
        $type = $webform_elements[$type];
      }

      // Use replacements to put the item identifier into a field value.
      $default_value = NULL;
      if (!empty($webform_component['value'])) {
        $default_value = str_replace(array_keys($tokens), array_values($tokens), $webform_component['value']);
      }

      // Copy any previously set value into our element default value.
      if (isset($component_value[$webform_component['form_key']])) {
        $default_value = $component_value[$webform_component['form_key']];
      }


      // Webform components are very unlike normal form elements.
      // that's a bore. Copy some parameters like size across.

      // Can I delegate this to each elements own webform_render_THING
      // callback - I feel like doing this by hand could be errorful.?
      $render_func = '_webform_render_' . $webform_component['type'];
      if (function_exists($render_func)) {
        // The value arg is apparently expected to be an array? Whatever.
        $placeholder_fields[$key][$webform_component['form_key']] = $render_func($webform_component, array($default_value));
        $placeholder_fields[$key][$webform_component['form_key']]['#webform_component'] = $webform_component;
      }
      else {
        //////SNIP THIS
        // This was me doing it by hand - probably redundant if ^ is working.
        $placeholder_fields[$key][$webform_component['form_key']] = array(
          '#type' => $type,
          '#title' => $webform_component['name'],
          '#size' => isset($webform_component['extra']['width']) ? $webform_component['extra']['width'] : NULL,
          '#default_value' => isset($default_value) ? $default_value : NULL,
          '#webform_component' => $webform_component,
          // Need to call in the theme wrapper to get field markup and title to show.
          '#theme_wrappers' => ($type == 'hidden') ? array() : array('webform_element'),
        );
        /////
      }
    }
  }

  #  dpm(get_defined_vars());

  $display = $view->display[$view->current_display];
  $element = array(
    #'#title' => $display->display_title,
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#theme_wrappers' => array('webform_element'),
    '#theme' => 'webform_view_embedded',
    '#pre_render' => array(), // Needed to disable double-wrapping of radios and checkboxes.
    '#translatable' => array('title', 'description', 'options'),
    // When webform renders elements, the content is expected to be inside
    // element #children. #markup is not respected.
   #'#suffix' => $output,
    '#type' => 'markup',
  );
  // Add the made-up placeholder fields as children
  $element += $placeholder_fields;

  // Add our embedded view as a renderable.
  // Using #markup so that even if rendering starts falling apart later,
  // it will still show up. Normally however, this value gets caught and
  // preprocessed in theme_webform_view_embedded()
  $element['view']['#markup'] = $output;

  #dpm(get_defined_vars());

  // Don't do this until we are finished with the data.
  // $view->destroy();

  return $element;
}



/**
 * Deal with what happens when one of our custom components is submitted.
 *
 * Implements _webform_submit_component().
 */
function _webform_submit_view($component, $value) {
  // $value is an array, should contain indexed clusters of information
  // one for each row.
  // eg array(
  //   51 => array('item' => 'the node title', 'quantity' => 2)
  //  )


  // Can clear out blank values here
  $quantity_key = 'quantity';
  $return = array();
  foreach ($value as $key => $row) {
    if (is_array($row)) {
      if (!empty($row[$quantity_key])) {
        $return[$key] = $row;
      }
    }
  }

  // Webform internals just can't deal with nested arrays, so at this point we
  // have to hide our data in serialized form. Dull, but the only way to be
  // safe it seems.

  #dpm(get_defined_vars());
  return serialize($return);

}

/**
 * Declare rendering routines for our element.
 *
 * Called by webform to register theme functions that the individual
 * cpmponent uses.
 *
 * Implements _webform_theme_COMPONENT().
 */
function _webform_theme_view() {
  // Declares different theme func for on-screen and text-output of the results.
  // Note, not the form  element.
  return array(
    // Text version
    'webform_display_view' => array(
      'render element' => 'element',
    ),
    // HTML version
    # ARG! name collision
    #'webform_view' => array(
    #  'render element' => 'element',
    #),
    // This renders the element when in the actual webform
    // needs to be named _embedded to avoid a name collision
    'webform_view_embedded' => array(
      'render element' => 'element',
    ),

  );
}

/**
 * Render the embedded view element in the form.
 * Most content has now been prepared and the child items rendered.
 * My job is to MOVE the rendered form element markup and embed it into the
 * view displays where the placeholders are.
 */
function theme_webform_view_embedded($variables) {
  $element = $variables['element'];
  // The rendered, already-built view is text in $element['view']['#markup'].
  // The form elements are my direct children.

  // Build an array of replacements for str_replace efficiency
  $replacements = array();
  foreach (element_children($element) as $key) {
    $replace_pattern = "[webform_view_" . $key . "_placeholder]";
    // Child item has already been rendered/built by now. (#printed is true)
    // Copy its processed text in here
    if (!empty($element[$key]['#printed'])) {
      $rendered_child = $element[$key]['#children'];
    }
    else {
      // Dunno why, but maybe I should render i myself. Used to be required, maybe this should never happen.
      $rendered_child = drupal_render($element[$key]);
    }
    $replacements[$replace_pattern] = $rendered_child;
  }

  // I earlier placed the rendered view into $element['view']['#markup']
  $element['view']['#markup'] = str_replace(array_keys($replacements), array_values($replacements), $element['view']['#markup']);

  return $element['view']['#markup'];
}


/**
 * Format the output of data for this component.
 *
 * This renders the data that's been submitted.
 * As seen on the results pages, also used for the email.
 */
function theme_webform_display_view($variables) {
  $element = $variables['element'];
  // Theme the order like a table.
  if ($element['#format'] == 'html') {
    $header = array_keys(reset($element['#value']));
    return theme('table', array('header' => $header, 'rows' => $element['#value']));
  }
  // Plaintext equivalent.
  else {
    return array_to_plaintext_table($element['#value']);
  }
}

/**
 * Rendering function to emulate table layout.
 * @param array $rows
 */
function array_to_plaintext_table($rows) {
  $cols = array();
  // Count the col widths first.
  foreach ($rows as $row) {
    foreach ($row as $col => $cell) {
      $cols[$col] = max(@$cols[$col], strlen($cell), strlen($col));
    }
  }
  // Now format.

  $lines = array();
  $printf_format = '';
  // Build the string template.
  foreach ($cols as $colwidth) {
    $printf_format .= "%-{$colwidth}s : ";
  }
  // First the header row.
  $lines[] = call_user_func_array('sprintf', array('format' => $printf_format) + array_keys($cols));
  foreach ($rows as $row) {
    $lines[] = call_user_func_array('sprintf', array('format' => $printf_format) + $row);
  }
  return join("\n", $lines);
}

/**
 * Implements _webform_display_component().
 * @param $value this has been flattened and serialized - unpack it before use.
 */
function _webform_display_view($component, $value_serialized, $format = 'html') {
  $value = array();
  if (! empty($value_serialized)) {
    $value = unserialize(reset($value_serialized));
  }
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_view',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    #'#options' => _webform_select_options($component, !$component['extra']['aslist']),
    '#value' => (array) $value,
    '#translatable' => array('title', 'options'),
  );
}
