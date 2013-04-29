<?php

/**
 * @file
 * Contains the CommerceShippingQuote class and CommerceShippingQuoteInterface interface.
 *
 * All quote plugins should have CommerceShippingQuote as a parent class
 */

/**
 * Declares an interface for shipping quote classes
 */
interface CommerceShippingQuoteInterface {
  /**
   * Used to customize the Rules settings form for rules activating the shipping quote.
   *
   * @param $form
   *    RulesDataUI InputForm
   * @param $rules_settings
   *    Array of values passed by rules.
   */
  public function settings_form(&$form, $rules_settings);

  /**
   * Function to dynamically set the label
   *
   * @param $order
   *    Optionally pass the order, if the class wasn't instantiated with the order.
   */
  public function form_label($order = NULL);

  /**
   * Allow quote to add an form displayed when the shipping quote is selected.
   *
   * @param $pane_values
   *    $form_state['values'] from the pane where the form is located
   * @param $checkout_pane
   *    $checkout_pane passed by commerce.
   * @param $order
   *    Optionally pass the order, if the class wasn't instantiated with the order.
   */
  public function submit_form($pane_values, $checkout_pane, $order = NULL);

  /**
   * Perform validation on submit_form
   *
   * @param $pane_values
   *    $form_state['values'] from the pane where the form is located
   * @param $checkout_pane
   *    $checkout_pane passed by commerce.
   * @param $form_parents
   *    The submit_form's parents, which can be used when setting form_errors
   * @param $order
   *    Optionally pass the order, if the class wasn't instantiated with the order.
   */
  public function submit_form_validate($pane_form, $pane_values, $form_parents = array(), $order = NULL);

  /**
   * Called After shipping items is created.
   *
   * @param $pane_values
   *    $form_state['values'] from the pane where the form is located
   * @param $checkout_pane
   *    $checkout_pane passed by commerce.
   * @param $order
   *    Optionally pass the order, if the class wasn't instantiated with the order.
   */
  public function shipping_items_created($pane_values, $checkout_pane, $order = NULL);

  /**
   * Do the actual shipping cost calculation.
   *
   * @param $currency_code
   *    The currency code that the shipping should use when returning the cost.
   * @param $form_values
   *    Array of values inputted in the submit form by the user.
   * @param $order
   *    The order for which operations should be done.
   * @param $pane_form
   *    The complete form array of the pane that shipping method is located on.
   * @param $pane_values
   *    The complete $form_state of the pane that shipping method is located on.
   *
   * @return array of values, one for each line item created containing one of.
   *  - (array) containing any of these keys: 'amount', 'currency_code', 'quantity', 'label'
   *  - (int) the price of the shipping, the order currency will be used, formatted as integer.
   */
  public function calculate_quote($currency_code, $form_values = array(), $order = NULL, $pane_form = NULL, $pane_values = NULL);
}


abstract class CommerceShippingQuote implements CommerceShippingQuoteInterface {
  /**
   * Constructor.
   *
   * Initialize class variables.
   *
   * @param $settings
   *    Array of settings, could hold rule settings or values selected by the user.
   * @param $order
   *    The order for which operations should be done.
   */
  public function __construct($settings = array(), $order = NULL) {
    $this->settings = $settings;
    $this->order = $order;
  }

  public function settings_form(&$form, $rules_settings) {}

  public function form_label($order = NULL) {
    return '';
  }

  public function submit_form($pane_values, $checkout_pane, $order = NULL) {
    return array();
  }

  public function submit_form_validate($pane_form, $pane_values, $form_parents = array(), $order = NULL) {}

  public function calculate_quote($currency_code, $form_values = array(), $order = NULL, $pane_form = NULL, $pane_values = NULL) {}

  public function shipping_items_created($pane_values, $checkout_pane, $order = NULL) {}
}