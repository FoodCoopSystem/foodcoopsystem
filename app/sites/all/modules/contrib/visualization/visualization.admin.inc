<?php
/**
 * @file
 * Settings form.
 */

/**
 * Settings form for visualization.
 */
function visualization_settings_form($form, &$form_state) {
  ctools_include('plugins');

  $options = array();
  foreach (ctools_get_plugins('visualization', 'library') as $id => $plugin) {
    if ($plugin['handler']->available()) {
      $options[$id] = $plugin['label'];
    }
  }

  $form['library'] = array(
    '#type' => 'select',
    '#title' => t('Preferred charting library'),
    '#options' => $options,
    '#description' => t('Your charting library of preference will be used when generating charts (as long as it offers support for the requested type). The available options are the charting libraries detected on your system, please refer to the help section for more information about enabling more libraries.'),
    '#default_value' => variable_get('visualization', 'gva'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit handler for visualization settings form.
 */
function visualization_settings_form_submit($form, &$form_state) {
  variable_set('visualization', $form_state['values']['library']);

  drupal_set_message(t('Visualization settings saved.'));
}
