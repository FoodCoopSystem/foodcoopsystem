<?php

/**
 * @file
 * Callback functions for the shipping module's checkout panes.
 */


/**
 * Checkout pane callback: builds a shipping quote selection form.
 */
function commerce_shipping_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();

  // TODO, before we active shipping methods etc, we should check that there
  // actually are shippable products. Right not that is not possible because
  // physical products aren't defined yet.


  // Invoke the shipping methods event that will populate the order with
  // an array of method IDs for available shipping methods.
  $order->commerce_shipping_methods = array();
  rules_invoke_all('commerce_shipping_methods', $order);

  // Generate an array of shipping method options for the checkout form.
  $options = array();
  foreach ($order->commerce_shipping_methods as $instance_id => $method_info) {
    $plugin = commerce_shipping_method_instance_load($instance_id);
    $class = new $plugin['handler']['class']($plugin['settings'], $order);
    if ($class->form_label()) {
      $options[$instance_id] = $class->form_label();
    }
    else {
      $plugin_title = isset($plugin['display_title']) ? $plugin['display_title'] : $plugin['title'];
      $options[$instance_id] = !empty($plugin['shipping_label']) ?
        $plugin['shipping_label'] : $plugin_title;
    }
  }

  // If no shipping methods were found, return the empty form.
  if (empty($options)) {
    return $pane_form;
  }

  // Store the shipping methods in the form for validation purposes.
  $pane_form['shipping_methods'] = array(
    '#type' => 'value',
    '#value' => $order->commerce_shipping_methods,
  );

  // If at least one shipping option is available...
  if (!empty($options)) {
    // Add a radio select widget to specify the shipping method.
    $pane_form['shipping_method'] = array(
      '#type' => 'radios',
      '#options' => $options,
      '#ajax' => array(
        'callback' => 'commerce_shipping_pane_checkout_form_details_refresh',
        'wrapper' => 'shipping-details',
      ),
    );

    // Find the default shipping method using either the preselected value stored
    // in the order / checkout pane or the first available method.
    $pane_values = !empty($form_state['values'][$checkout_pane['pane_id']]) ? $form_state['values'][$checkout_pane['pane_id']] : array();

    if (isset($pane_values['shipping_method']) && isset($options[$pane_values['shipping_method']])) {
      $default_value = $pane_values['shipping_method'];
    }
    elseif (isset($order->data['shipping_method']) && isset($options[$order->data['shipping_method']])) {
      $default_value = $order->data['shipping_method'];
    }
    else {
      reset($options);
      $default_value = key($options);
    }

    // Set the default value for the shipping method radios.
    $pane_form['shipping_method']['#default_value'] = $default_value;
    $pane_form['shipping_details'] = array();

    // Store the invoked plugin class of the selected shipping method for
    // later use.
    $plugin = commerce_shipping_method_instance_load($pane_form['shipping_method']['#default_value']);
    $class = new $plugin['handler']['class']($plugin['settings'], $order);
    $pane_form['shipping_details'] = $class->submit_form($pane_values, $checkout_pane);
    $pane_form['commerce_shipping_plugin'] = array(
      '#type' => 'value',
      '#value' => $class
    );

    $pane_form['shipping_details']['#prefix'] = '<div id="shipping-details">';
    $pane_form['shipping_details']['#suffix'] = '</div>';
  }
  return $pane_form;
}

/**
 * Returns the shipping details element for display via AJAX.
 */
function commerce_shipping_pane_checkout_form_details_refresh($form, $form_state) {
  return $form['commerce_shipping']['shipping_details'];
}

/**
 * shipping pane: validation callback.
 */
function commerce_shipping_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  $pane_form = $form[$checkout_pane['pane_id']];
  $pane_values = &$form_state['values'][$checkout_pane['pane_id']];
  $class = $pane_form['commerce_shipping_plugin']['#value'];
  if (!isset($pane_values['shipping_details'])) {
    $pane_values['shipping_details'] = array();
  }

  // Only attempt validation if there were shipping methods available.
  if ($pane_values['shipping_methods']) {
    // If the selected shipping method was changed, we always need to rebuild
    // to update the plugin class.
    if ($pane_values['shipping_method'] != $pane_form['shipping_method']['#default_value']) {
      return FALSE;
    }

    // Run the validation that the plugin class inplements.
    $result = $class->submit_form_validate($pane_form['shipping_details'], $pane_values['shipping_details'], array($checkout_pane['pane_id'], 'shipping_details'), $order);
    return $result === FALSE ? FALSE : TRUE;
  }
  // Nothing to validate.
  return TRUE;
}

/**
 * shipping pane: submit callback.
 */
function commerce_shipping_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // In case this order already have shipping line items, we need to remove them.
  commerce_shipping_clear_order($order);

  $pane_form = $form[$checkout_pane['pane_id']];
  $pane_values = $form_state['values'][$checkout_pane['pane_id']];
  $class = $pane_form['commerce_shipping_plugin']['#value'];
  $rule_ids = explode('|', $pane_values['shipping_method']);
  $method_id = $rule_ids[0];

  // Only process if there were shipping methods available.
  if ($pane_values['shipping_methods']) {
    $order->data['shipping_method'] = $pane_values['shipping_method'];

    $default_currency_code = commerce_default_currency();
    if ($balance = commerce_payment_order_balance($order)) {
      $default_currency_code = $balance['currency_code'];
    }
    $form_values = isset($form_state['values']['commerce_shipping']['shipping_details']) ? $form_state['values']['commerce_shipping']['shipping_details'] : array();

    // Let the shipping method calculate the shipping price.
    $plugin = commerce_shipping_plugin_get_plugin('quotes', $method_id);
    $shipping_line_items = $class->calculate_quote($default_currency_code, $form_values, $order, $form, $form_state);

    // Loop through the result and create line items if possible.
    if (is_array($shipping_line_items)) {
      foreach ($shipping_line_items as $shipping_line_item) {
        $line_item = commerce_shipping_line_item_new($plugin);
        $line_item->order_id = $order->order_id;
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
        if (is_numeric($shipping_line_item)) {
          $price = array(
            'amount' => $shipping_line_item,
          );
        }
        elseif (isset($shipping_line_item['amount'])) {
          $price = array(
            'amount' => $shipping_line_item['amount'],
            'currency_code' => !empty($shipping_line_item['currency_code']) ? $shipping_line_item['currency_code'] : $default_currency_code,
          );
        }
        elseif (isset($shipping_line_item['price'])) {
            $price = $shipping_line_item['price'];
        }
        if (isset($shipping_line_item['label'])) {
          $line_item_wrapper->line_item_label = $shipping_line_item['label'];
        }
        if (isset($shipping_line_item['quantity'])) {
          $line_item_wrapper->quantity = $shipping_line_item['quantity'];
        }

        // Require that the price is set.
        if (isset($price)) {
          // Add component if needed
          if (empty($price['data']['components'])) {
            $price_component = 'quote';
            if (!empty($plugin['price_component'])) {
              $price_component = 'quote_' . $plugin['name'];
            }
            $price['data'] = commerce_price_component_add(
              $price,
              $price_component,
              $price,
              TRUE,
              FALSE
            );
          }
          // Make sure the currency code is set.
          if (empty($price['currency_code'])) {
            $price['currency_code'] = $default_currency_code;
          }
          $line_item_wrapper->commerce_unit_price = $price;
          rules_invoke_all('commerce_shipping_calculate', $line_item);

          $line_item_wrapper->save();
          $order_wrapper->commerce_line_items[] = $line_item_wrapper->value();
        }
      }
    }

    // Lastly we save the order.
    commerce_order_save($order);

    // This is not actually needed, but for flexibility allow shipping methods
    // to react on the form submission.
    $class->shipping_items_created($pane_form['shipping_details'], $pane_values['shipping_details'], $order);
  }
}
