<?php

class CommerceShippingExample extends CommerceShippingQuote {
  /**
   * Settings form callback: adds our custom settings to the Rules action form.
   */
  public function settings_form(&$form, $rules_settings) {
    $form['shipping_price'] = array(
      '#type' => 'textfield',
      '#title' => t('Shipping price'),
      '#description' => t('Configure what the shipping price per order should be.'),
      '#default_value' => is_array($rules_settings) && isset($rules_settings['shipping_price']) ? $rules_settings['shipping_price'] : 42,
      '#element_validate' => array('rules_ui_element_decimal_validate'),
    );
  }

  /**
   * Submit form callback: adds additional elements to the checkout pane when
   * this shipping method is selected.
   */
  public function submit_form($pane_values, $checkout_pane, $order = NULL) {
    $form = parent::submit_form($pane_values, $checkout_pane, $order);

    // Default to the order in the object's scope if none is explicitly passed in.
    if (empty($order)) {
      $order = $this->order;
    }

    // Merge values from the order into the checkout pane values.
    if (!empty($order->data['commerce_shipping_example'])) {
      $pane_values += $order->data['commerce_shipping_example'];
    }

    // Then merge in default values.
    $pane_values += array(
      'express' => 0,
      'name' => '',
    );

    $form['express'] = array(
      '#type' => 'checkbox',
      '#title' => t('Express delivery'),
      '#description' => t('Express delivery costs twice the normal amount.'),
      '#default_value' => $pane_values['express'],
    );

    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#description' => t('This is a demonstration field coded to fail validation for single character values.'),
      '#default_value' => $pane_values['name'],
      '#required' => TRUE,
    );

    return $form;
  }

  /**
   * Submit form validation callback: validates data entered via our custom
   * submit form elements. Failed validation requires a FALSE return value.
   * Otherwise nothing needs to be returned.
   */
  public function submit_form_validate($pane_form, $pane_values, $form_parents = array(), $order = NULL) {
    // Throw an error if a long enough name was not provided.
    if (strlen($pane_values['name']) < 2) {
      form_set_error(implode('][', array_merge($form_parents, array('name'))), t('You must enter a name two or more characters long.'));

      // Even though the form error is enough to stop the submission of the form,
      // it's not enough to stop it from a Commerce standpoint because of the
      // combined validation / submission going on per-pane in the checkout form.
      return FALSE;
    }
  }

  /**
   * Calculate quote callback: the bulk of the shipping method is usually found
   * here. This is where we do the actual calculations to figure out what the
   * shipping costs should be. We can return a single price or for more control
   * an array of arrays containing:
   *    - label
   *    - quantity
   *    - amount
   *    - currency code
   *
   * Only the amount is needed as the rest have default values.
   */
  public function calculate_quote($currency_code, $form_values = array(), $order = NULL, $pane_form = NULL, $pane_values = NULL) {
    if (empty($order)) {
      $order = $this->order;
    }

    $settings = $this->settings;

    $shipping_line_items = array();

    $shipping_line_items[] = array(
      'amount' => commerce_currency_decimal_to_amount($settings['shipping_price'], $currency_code),
      'currency_code' => $currency_code,
      'label' => t('Normal shipping'),
    );

    if (!empty($form_values['express'])) {
      $shipping_line_items[] = array(
        'amount' => commerce_currency_decimal_to_amount($settings['shipping_price'], $currency_code),
        'currency_code' => $currency_code,
        'label' => t('Express fee'),
      );
    }

    return $shipping_line_items;
  }
}
