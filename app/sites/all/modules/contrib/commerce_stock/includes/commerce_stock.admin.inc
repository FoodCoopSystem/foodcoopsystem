<?php

/**
 * @file
 * Administrative callbacks and form builder functions for Commerce Stock.
 */

/**
 * Commerce Stock admin form.
 */
function commerce_stock_admin_form($form, &$form_state) {
  // Find out what our status is. Use both the state of existing fields
  // and the state of variables to determine what's right.

  $field_name = 'commerce_stock';
  $field = field_info_field($field_name);

  $form['#tree'] = TRUE;

  $form['product_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enable stock management for these product types'),
    '#description' => t('Note that disabling stock management removes the Stock field from the product type, deleting any existing stock data for that product type.')
  );

  $form['product_types_override'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enable stock management override for these product types'),
    '#description' => t('Note that disabling stock management override removes the Stock override field from the product type, deleting any existing stock override data for that product type.')
  );

  // Create a checkbox for each product type, set with the current stock-
  // enabled state.
  foreach (commerce_product_types() as $type => $product_type) {
    $instance[$type] = field_info_instance('commerce_product', 'commerce_stock', $type);
    $enabled[$type] = (!empty($instance[$type]));

    $form['product_types'][$type] = array(
      '#type' => 'checkbox',
      '#default_value' => $enabled[$type],
      '#title' => t('@name (@machine_name)', array('@name' => $product_type['name'], '@machine_name' => $type)),
    );

    if ($enabled[$type]) {
      $instance[$type] = field_info_instance('commerce_product', 'commerce_stock_override', $type);
      $enabled[$type] = (!empty($instance[$type]));

      $form['product_types_override'][$type] = array(
        '#type' => 'checkbox',
        '#default_value' => $enabled[$type],
        '#title' => t('Allow stock override for @name (@machine_name)', array('@name' => $product_type['name'], '@machine_name' => $type)),
      );
    }

  }

  // Add a checkbox that requires them to say "I do", but don't show it
  // (#access == FALSE) unless they're deleting.
  if (!empty($form_state['commerce_stock']['delete_instances'])) {
    $type_plural = format_plural(count($form_state['commerce_stock']['delete_instances']), 'type', 'types');
    $affirmation = t('I understand that all stock data will be permanently removed from the product @type_plural %product_types.',
      array(
        '@type_plural' => $type_plural,
        '%product_types' => implode(', ', $form_state['commerce_stock']['delete_instances']),
      )
    );
  }
  $form['confirmation'] = array(
    '#type' => 'checkbox',
    '#title' => !empty($affirmation) ? $affirmation : '',
    '#default_value' => FALSE,
    '#access' => FALSE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  // If they're deleting, show the confirmation checkbox.
  if (!empty($form_state['commerce_stock']['delete_instances'])) {
    $form['confirmation']['#access'] = TRUE;
    drupal_set_message(t('You must click the confirmation checkbox to confirm that you want to delete stock data'), 'warning');
  }

  return $form;
}

/**
 * Form validator. If they are deleting and have not checked the confirmation
 * checkbox, make them do so.
 */
function commerce_stock_admin_form_validate($form, &$form_state) {
  if (!empty($form_state['commerce_stock']['delete_instances']) && empty($form_state['values']['confirmation'])) {
    form_set_error('confirmation', t('Please check the "I understand" checkbox to indicate you understand that all stock data in these fields will be deleted: %fields.', array('%fields' => implode(', ', $form_state['commerce_stock']['delete_instances']))));
  }
}

/**
 * This functions parses all the product of the specified bundle, and initialize its stock field (commerce_stock) to a value
 * @param  String $bundle
 *  Commerce product bundle type to initialize
 * @param  int $init_stock_value
 *  Stock value to initialize to
 * @return void
 */
function commerce_stock_admin_init($bundle, $init_stock_value) {
  //Load every product of $bundle type
  $products = commerce_product_load_multiple(array(), array('type' => $bundle));
  //Parses each product to initialize its stock value
  foreach($products as $product) {
    $wrapper = entity_metadata_wrapper('commerce_product', $product);
    //dpm($wrapper->getPropertyInfo());
    $wrapper->commerce_stock = $init_stock_value;
    $wrapper->save();
  }
}

/**
 * Add or remove the Stock field from product types.
 */
function commerce_stock_admin_form_submit($form, &$form_state) {
  $form_state['commerce_stock']['delete_instances'] = array();
  foreach ($form_state['values']['product_types'] as $type => $enable) {
    $instance = field_info_instance('commerce_product', 'commerce_stock', $type);

    $currently_enabled = commerce_stock_product_type_enabled($type);
    // If they want us to enable it and it doesn't currently exist, do the work.
    if ($enable && !$currently_enabled) {
      commerce_stock_create_instance('commerce_stock', 'number_integer', TRUE, 'commerce_product', $type, t('Stock'));
      //Initialize fields of every existing product of $type bundle to 0
      commerce_stock_admin_init($type, 0);
      drupal_set_message(t('Stock management has been enabled on the %type product type', array('%type' => $type)));
    }
    // Conversely, if they *don't* want it and it's currently enabled,
    // warn them about the consequences or do it.
    else if (!$enable && $currently_enabled) {
      // If they haven't clicked the "confirm" checkbox, rebuild and get them
      // to do it.
      if (empty($form_state['values']['confirmation'])) {
        $form_state['commerce_stock']['delete_instances'][] = $type;
        $form_state['rebuild'] = TRUE;
      }
      // Otherwise they already have clicked it and we can delete.
      else {
        // Remove the instance.
        field_delete_instance($instance);

        // Remove override if enabled
        if (commerce_stock_product_type_override_enabled($type)) {
          $override = field_info_instance('commerce_product', 'commerce_stock_override', $type);
          field_delete_instance($override);
        }

        drupal_set_message(t('Stock management has been disabled on the %type product type', array('%type' => $type)));
      }
    }
  }

  if (!empty($form_state['values']['product_types_override'])) {
    foreach ($form_state['values']['product_types_override'] as $type => $enable) {
      $instance = field_info_instance('commerce_product', 'commerce_stock_override', $type);

      $currently_enabled = commerce_stock_product_type_override_enabled($type);
      $stock_enabled = commerce_stock_product_type_enabled($type);
      // If they want us to enable it and it doesn't currently exist, do the work.
      if ($enable && $stock_enabled && !$currently_enabled) {
        commerce_stock_create_instance('commerce_stock_override', 'list_boolean', FALSE, 'commerce_product', $type, t('Disable stock for this product'));
        drupal_set_message(t('Stock management override has been enabled on the %type product type', array('%type' => $type)));
      }
      // Conversely, if they *don't* want it and it's currently enabled,
      // warn them about the consequences or do it.
      else if (!$enable && $currently_enabled) {
        // If they haven't clicked the "confirm" checkbox, rebuild and get them
        // to do it.
        if (empty($form_state['values']['confirmation'])) {
          $form_state['commerce_stock']['delete_instances'][] = $type;
          $form_state['rebuild'] = TRUE;
        }
        // Otherwise they already have clicked it and we can delete.
        else {
          // Remove the instance.
          field_delete_instance($instance);
          drupal_set_message(t('Stock management override has been disabled on the %type product type', array('%type' => $type)));
        }
      }
    }
  }
}
