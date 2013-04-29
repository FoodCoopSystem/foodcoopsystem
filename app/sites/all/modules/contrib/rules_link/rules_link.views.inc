<?php

/**
 * @file
 * Provide views data.
 */

/**
 * Implements hook_views_data().
 */
function rules_link_views_data() {
  $data = array();

  foreach (rules_link_get_links() as $name => $rules_link) {
    $entity = entity_get_info($rules_link->entity_type);
    $data[$entity['base table']][$name]['field']['moved to'] = array('views_entity_' . $rules_link->entity_type, $name, 'field');
    $data['views_entity_' . $rules_link->entity_type][$name]['field'] = array(
      'title' => $rules_link->label,
      'help' => t('Execute the selected rules link.'),
      'handler' => 'views_handler_field_rules_link',
      'rules link name' => $name,
    );
  }
  return $data;
}

/**
 * Renders the links for views.
 */
class views_handler_field_rules_link extends views_handler_field_entity {
  function render($value) {
    $replacements = filter_xss_admin($this->options['rules_link']['rules_link_parameters']);
    $token = $this->get_render_tokens('');
    $parameters = explode(',', strtr($replacements, $token));

    // Trim the spaces from the
    array_walk($parameters, 'rules_link_trim_parameters');

    $rules_link = rules_link_load($this->definition['rules link name']);
    $entity = $this->get_value($value);
    list($id, $rev, $bundle) = entity_extract_ids($rules_link->entity_type, $entity);

    // If there are multiple bundle types, we only have to display the links to
    // the entities which have the correct bundle.
    if (empty($rules_link->settings['bundles']) || in_array($bundle, $rules_link->settings['bundles'])) {
      return rules_link_render_link($rules_link, $id, drupal_get_destination(), $parameters);
    }
  }

  /**
   * Override option_definition.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['rules_link']['rules_link_parameters'] = array('default' => '');
    return $options;
  }

  /**
   * Override options_form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['rules_link'] = array(
      '#type' => 'fieldset',
      '#title' => t('Rules Link settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['rules_link']['rules_link_parameters'] = array(
      '#type' => 'textfield',
      '#title' => t('Additional parameters for the rules link'),
      '#description' => t('Enter additional parameter using replacement patterns, use comma to enter multiple items. Whitespaces at the beginning and end of token will be trimmed.'),
      '#default_value' => $this->options['rules_link']['rules_link_parameters'],
    );
    $form['rules_link']['rules_link_patterns'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#value' => $form['alter']['help']['#value'],
    );
  }
}
