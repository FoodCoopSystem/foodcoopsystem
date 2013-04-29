<?php

/**
 * @file
 * Page callbacks and form builder functions for administering customer profiles type ui.
 */


/**
 * Form callback wrapper: create or edit a customer profile type.
 */
function customer_profile_type_ui_add_page() {
  // Add the breadcrumb for the form's location.
  //commerce_customer_ui_set_breadcrumb();

  return drupal_get_form('customer_profile_type_ui_add_form');
}
 
 
/**
 * Form callback: create a customer profile type.
 */
function customer_profile_type_ui_add_form($form, &$form_state) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'customer_profile_type_ui') . '/includes/customer_profile_type_ui.ui.inc';

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Enter a human readable name for this new customer profile type.'),
    '#maxlength' => 60,
    '#weight' => -1,
  );

  // Have to limit this to 13 chars since the customer profile type reference field
  // that is auto added to the order type appends 'commerce_customer_' on the front
  // and field machine names must also be less than 32 chars.
  $form['type'] = array(
    '#type' => 'textfield',
    '#title' => t('Type'),
    '#description' => t('Enter a machine name for this new customer profile type. Has to be alpha-numeric/underscores and less then 13 characters.'),
    '#maxlength' => 13,
    '#weight' => 5,
  );

  $form['addressfield'] = array(
    '#type' => 'radios',
    '#title' => t('Include the default Address field'),
    '#description' => t('If you plan on having an address field in this profile type then it is recommended that you select \'yes\' to use the default that comes with customer profile types. This will ensure that address fields are kept in sync accross customer profile types.'),
    '#options' => array(0 => 'no', 1 => 'yes'),
    '#default' => 0,
    '#weight' => 10,
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('Describe the new Customer profile type. This will show up on admin pages.'),
    '#maxlength' => 255,
    '#weight' => 15,
  );

  $form['help'] = array(
    '#type' => 'textfield',
    '#title' => t('Help text'),
    '#description' => t('Enter some help text to aid users in filling out the customer profile form on checkout.'),
    '#maxlength' => 255,
    '#weight' => 20,
  );


  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 100,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save profile type'),
    '#submit' => $submit + array('customer_profile_type_ui_add_form_submit'),
  );

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'customer_profile_type_ui_add_form_validate';

  return $form;
}


/**
 * Validation callback for customer_profile_type_ui_customer_profile_type_form().
 */
function customer_profile_type_ui_add_form_validate($form, &$form_state) {

  if (((int)$form_state['values']['addressfield'] < 0) || ((int)$form_state['values']['addressfield'] > 1)) form_set_error('addressfield', t('You must choose a valid addressfield option.'));
  
  if (is_numeric($form_state['values']['type'])) {
    form_set_error('type', t('Machine-readable names must not consist of numbers only.'));
  }

  $result = db_query('SELECT cpt.type FROM {customer_profile_type_ui} cpt WHERE cpt.type = :type', array(':type' => $form_state['values']['type']));
  // Result is returned as a iterable object that returns a stdClass object on each iteration
  if ($result->rowCount() > 0) {
    form_set_error('type', t('The Customer profile type !type already exists. Please choose a different machine name for your new type.', array('!type' => $form_state['values']['type'])));
  }
}


/**
 * Submit callback for customer_profile_type_ui_customer_profile_type_form().
 */
function customer_profile_type_ui_add_form_submit($form, &$form_state){
  $result = db_insert('customer_profile_type_ui')
->fields(array(
  'type' => check_plain($form_state['values']['type']),
  'name' => check_plain($form_state['values']['name']),
  'addressfield' => ((int)$form_state['values']['addressfield']) ? 1 : 0,
  'description' => check_plain($form_state['values']['description']),
  'help' => check_plain($form_state['values']['help']),
))
->execute();

  // Configure the new customer profile type.
  commerce_customer_configure_customer_fields(array('customer_profile_type_ui'));
  commerce_order_configure_order_fields(array('customer_profile_type_ui'));
  menu_rebuild();
  // Redirect based on the button clicked.
  drupal_set_message(t('Customer profile type \'!type\' has been created.', array('!type' => $form_state['values']['type'])));
  drupal_goto('admin/commerce/customer-profiles/types');
}