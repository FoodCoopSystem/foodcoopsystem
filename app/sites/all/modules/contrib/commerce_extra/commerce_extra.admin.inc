<?php
/**
 * @file
 * Commerce Extra settings for admins.
 */

/**
 * Form callback for module settings.
 */
function commerce_extra_settings($form, &$form_state) {

  // We need this for finding out which submodules are enabled/disabled
  $module_list = module_list();

  // List features
  foreach (_commerce_extra_features() as $feature => $info) {
    $form[$feature] = array(
      '#type' => 'fieldset',
      '#title' => $info['title'],
      '#description' => $info['description'],
    );
    $form[$feature][$feature . '_enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable'),
      '#default_value' => isset($module_list[$feature]),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submission callback for module settings.
 */
function commerce_extra_settings_submit($form, &$form_state) {

  // We need this for finding out which submodules are enabled/disabled
  $module_list = module_list();
  $enable = array();
  $disable = array();

  // Loop thru
  foreach (_commerce_extra_features() as $feature => $info) {
    // If new settings differs from current state
    if ($form_state['values'][$feature . '_enable'] != isset($module_list[$feature])) {
      if ($form_state['values'][$feature . '_enable']) {
        // We want enable the submodule
        $enable[] = $feature;
      }
      else {
        // We want disable the submodule
        $disable[] = $feature;
      }
    }
  }

  // Enable/disable possible modules
  if (count($enable) > 0) module_enable($enable);
  if (count($disable) > 0) module_disable($disable);

}

/**
 * Helper function for getting list of features.
 * @return
 *   Returns associative array containing features and their details.
 */
function _commerce_extra_features() {
  return array(
    'commerce_extra_address_populate' => array(
      'title' => t('Address Populate'),
      'description' => t('Uses user account address field to pre-populate address information in checkout. After enabling, <a href="@url">edit your settings</a> of "Billing information" pane or other customer profile pane.', array('@url' => url('admin/commerce/config/checkout/form/pane/customer_profile_billing'))),
    ),
    'commerce_extra_login_page' => array(
      'title' => t('Login Page'),
      'description' => t('Creates extra step for checkout process where user can alternatively login.'),
    ),
    'commerce_extra_quantity' => array(
      'title' => t('Quantity'),
      'description' => t('Wraps quantity fields with - and + links to decrease or increase quantity.'),
    ),
  );
}
