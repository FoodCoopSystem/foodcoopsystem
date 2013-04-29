<?php
/**
 * @file
 * Views style plugin for Visualization.
 */

/**
 * Style plugin uses views ui to configure views data for rendering charts.
 *
 * @ingroup views_style_plugins
 */
class visualization_plugin_style extends views_plugin_style {

  /**
   * Set default options.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'line');

    $options['fields'] = array('default' => array());

    $options['yAxis']['title'] = array('default' => '');

    $options['xAxis']['labelField'] = array('default' => FALSE);
    $options['xAxis']['invert'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Returns the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $fields = $this->display->handler->get_field_labels();

    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Chart type'),
      '#options' => array(
        'line' => 'Line chart',
        'pie' => 'Pie chart',
        'bar' => 'Bar chart',
        'column' => 'Column chart',
      ),
      '#default_value' => $this->options['type'],
      '#empty_value' => FALSE,
    );

    $form['fields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Field settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    foreach ($fields as $field => $column) {
      $handler = $this->display->handler->handlers['field'][$field];

      $form['fields'][$field] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($field),
        '#collapsible' => TRUE,
        '#collapsed' => empty($this->options['fields'][$field]),
      );

      $form['fields'][$field]['enabled'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable this field in the chart'),
        '#default_value' => $this->options['fields'][$field]['enabled'],
        '#dependency' => array('style_options', 'fields', $field, 'type'),
      );

      if ($handler->click_sortable()) {
        $form['fields'][$field]['sort'] = array(
          '#type' => 'select',
          '#title' => t('Sort'),
          '#options' => array(
            'DESC' => t('Descending'),
            'ASC' => t('Ascending'),
          ),
          '#default_value' => $this->options['fields'][$field]['sort'],
          '#empty_value' => FALSE,
        );
      }
    }

    $form['xAxis'] = array(
      '#type' => 'fieldset',
      '#title' => t('X-axis settings'),
      '#collapsible' => TRUE,
      '#collapsed' => !empty($this->options['xAxis']['labelField']) || !empty($this->options['xAxis']['invert']),
    );

    $form['xAxis']['labelField'] = array(
      '#type' => 'select',
      '#title' => t('X-axis labels'),
      '#options' => $fields,
      '#default_value' => $this->options['xAxis']['labelField'],
      '#empty_value' => FALSE,
    );

    $form['xAxis']['invert'] = array(
      '#type' => 'checkbox',
      '#title' => t('Should the x-axis get inverted?'),
      '#default_value' => $this->options['xAxis']['invert'],
    );

    $form['yAxis'] = array(
      '#type' => 'fieldset',
      '#title' => t('Y-axis settings'),
      '#collapsible' => TRUE,
      '#collapsed' => empty($this->options['yAxis']['title']),
    );

    $form['yAxis']['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Y-axis title'),
    );
  }

  /**
   * Adds sorting to the individual fields.
   */
  public function build_sort() {
    foreach ($this->options['fields'] as $field => $option) {
      $handler = $this->display->handler->handlers['field'][$field];

      if ($option['sort'] && $handler->click_sortable()) {
        $handler->click_sort($option['sort']);
      }
    }

    return FALSE;
  }

}
