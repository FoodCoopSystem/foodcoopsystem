<?php

/**
 * @file
 * Checkout pane callback functions for the customer_profiles_type_ui module.
 */


/**
 * Checkout pane callback: returns the customer profile pane's settings form.
 */
function customer_profile_type_ui_pane_settings_form($checkout_pane) {
  $original_panes = _customer_profile_type_ui_store_checkout_pane_contexts();
  if ($callback = commerce_checkout_pane_callback($original_panes[$checkout_pane['pane_id']], 'settings_form')) {
    // Generate the original settings form.
    return $callback($checkout_pane);
  }
}


/**
 * Checkout pane callback: returns a customer profile edit form.
 */
function customer_profile_type_ui_pane_checkout_form($form, &$form_state, $checkout_pane, &$order) {

  // Check to see if this pane should be disabled or not.
  if (_customer_profile_type_ui_disable_this_pane($checkout_pane['pane_id'], $order)) return NULL; // Pane is disabled so don't display it.
  
  // Not disabled so get original callback and call it.
  $original_panes = _customer_profile_type_ui_store_checkout_pane_contexts();
  if ($callback = commerce_checkout_pane_callback($original_panes[$checkout_pane['pane_id']], 'checkout_form')) {
    // Generate the original pane form.
    return $callback($form, $form_state, $checkout_pane, $order);
  }
}


/**
 * Checkout pane callback: validates a customer profile edit form.
 */
function customer_profile_type_ui_pane_checkout_form_validate($form, &$form_state, $checkout_pane, &$order) {
  // If this customer profile pane is disabled, then we want to return true so that verification 
  // of this pane doesn't get in the way of proceeding through checkout.
  if (_customer_profile_type_ui_disable_this_pane($checkout_pane['pane_id'], $order)) return TRUE;
  
  // Not disabled so get original callback and call it.
  $original_panes = _customer_profile_type_ui_store_checkout_pane_contexts();
  if ($callback = commerce_checkout_pane_callback($original_panes[$checkout_pane['pane_id']], 'checkout_form_validate')) {
    // Validate the form using the original pane callback.
    return $callback($form, $form_state, $checkout_pane, $order);
  }
}

/**
 * Checkout pane callback: submits a customer profile edit form.
 */
function customer_profile_type_ui_pane_checkout_form_submit($form, &$form_state, $checkout_pane, &$order) {
  // Check to see if this pane should be disabled or not.
  if (_customer_profile_type_ui_disable_this_pane($checkout_pane['pane_id'], $order)) return; // Disabled so nothing to do.
  
  // Not disabled so get original callback and call it.
  $original_panes = _customer_profile_type_ui_store_checkout_pane_contexts();
  if ($callback = commerce_checkout_pane_callback($original_panes[$checkout_pane['pane_id']], 'checkout_form_submit')) {
    // Submit the form using the original pane callback.
    return $callback($form, $form_state, $checkout_pane, $order);
  }
}


/**
 * Checkout pane callback: returns the cart contents review data for the
 *   Review checkout pane.
 */
function customer_profile_type_ui_pane_review($form, $form_state, $checkout_pane, &$order) {
  // Check to see if this pane should be disabled or not.
  if (_customer_profile_type_ui_disable_this_pane($checkout_pane['pane_id'], $order)) return NULL; // Pane is disabled so don't display it.
  
  // Not disabled so get original callback and call it.
  $original_panes = _customer_profile_type_ui_store_checkout_pane_contexts();
  if ($callback = commerce_checkout_pane_callback($original_panes[$checkout_pane['pane_id']], 'review')) {
    // return the cart contents review data using the original pane callback.
    return $callback($form, $form_state, $checkout_pane, $order);
  }
}

function _customer_profile_type_ui_disable_this_pane($checkout_pane_id, &$order) {
  if (!isset($order->disabled_customer_profiles)) {
    // Invoke the event that will populate the order with an array
    // of customer profile types to disable in the checkout panes.
    // We'll only do this once to save time.
    $order->disabled_customer_profiles = array();
    rules_invoke_all('before_customer_profile_type_panes', $order);
  }

  // Check to see if this pane should be disabled or not.
  return (isset($order->disabled_customer_profiles[$checkout_pane_id]));
}