<?php

/**
 * @file
 * Functions for rules integration.
 */

/**
 * Implements of hook_rules_event_info().
 * @ingroup rules
 */
function webform_rules_rules_event_info() {
  return array(
    'webform_rules_submit_as_draft' => array(
      'label' => t('After a webform has been saved as draft'),
      'group' => t('Webform'),
      'variables' => _webform_rules_event_variables(),
      'access callback' => 'rules_node_integration_access',
    ),
    'webform_rules_submit' => array(
      'label' => t('After a webform has been submitted'),
      'group' => t('Webform'),
      'variables' => _webform_rules_event_variables(),
      'access callback' => 'rules_node_integration_access',
    ),
    'webform_rules_insert' => array(
      'label' => t('After a webform submission has been added'),
      'group' => t('Webform'),
      'variables' => _webform_rules_event_variables(),
      'access callback' => 'rules_node_integration_access',
    ),
    'webform_rules_update' => array(
      'label' => t('After a webform submission has been updated'),
      'group' => t('Webform'),
      'variables' => _webform_rules_event_variables(),
      'access callback' => 'rules_node_integration_access',
    ),
    'webform_rules_delete' => array(
      'label' => t('After a webform submission has been deleted'),
      'group' => t('Webform'),
      'variables' => _webform_rules_event_variables(),
      'access callback' => 'rules_node_integration_access',
    ),
  );
}

/**
 * Implementation of hook_condition_info().
 */
function webform_rules_rules_condition_info() {
  return array(
    'webform_has_id' => array(
      'label' => t('Webform has name'),
      'parameter' => array(
        'form_id' => array(
          'type' => 'text',
          'label' => t('The form id of the submitted form'),
        ),
        'selected_webform' => array(
          'type' => 'list<text>',
          'label' => t('Webforms'),
          'options list' => 'webform_rules_get_webforms',
          'description' => t('The name of the webform to check for.'),
          'restriction' => 'input',
        ),
      ),
      'help' => t('This condition just compares two texts. It returns TRUE, if both texts are equal.'),
      'group' => t('Webform'),
      'base' => 'webform_rules_condition_webform_has_id',
    ),
  );
}

/**
 * Implements hook_rules_data_info().
 */
function webform_rules_data_info() {
  return array(
    'webform_data' => array(
      'label' => t('webform data'),
      'group' => t('Webform'),
      'token type' => 'webform',
      'property info' => array(),
    ),
  );
}

/**
 * Function to compare the form id of the submitted webform with the selected
 * form.
 */
function webform_rules_condition_webform_has_id($form_id, $selected_webform) {
  if (is_array($selected_webform)) {
    return in_array($form_id, $selected_webform);
  }
  elseif (is_string($selected_webform)) {
    return $form_id == $selected_webform;
  }
  return FALSE;
}

/**
 * Get an option list of all webforms.
 */
function webform_rules_get_webforms() {
  // Get a list of all webform-enabled content types.
  $webform_types = webform_variable_get('webform_node_types');
  // Get a list of all nodes that are configured to use a webform.
  $query = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('n.type', $webform_types, 'IN');
  // Join to limi result list to node that really have a webform.
  $query->join('webform', 'w', 'n.nid = w.nid');
  // Get the result list.
  $results = $query->execute();

  $options = array();
  foreach ($results as $record) {
    $options["webform-client-form-{$record->nid}"] = $record->title;
  }
  return $options;
}

/**
 * Helper function for event variables.
 *
 * @return
 *   All available variables for the rules events provided by webform_rules.
 */
function _webform_rules_event_variables() {
  return array(
    'user' => array(
      'type' => 'user',
      'label' => t('User, who submitted the webform'),
    ),
    'node' => array(
      'type' => 'node',
      'label' => t('The webform node'),
    ),
    'data' => array(
      'type' => 'webform',
      'label' => t('The submitted webform data'),
    ),
    'form_id' => array(
      'type' => 'text',
      'label' => t('The form id of the submitted form'),
      'hidden' => TRUE,
    ),
    'selected_webform' => array(
      'type' => 'list<text>',
      'label' => t('Webforms'),
      'options list' => 'webform_rules_get_webforms',
      'description' => t('The name of the webform to check for.'),
      'restriction' => 'input',
      'hidden' => TRUE,
      'optional' => TRUE,
    ),
  );
}
