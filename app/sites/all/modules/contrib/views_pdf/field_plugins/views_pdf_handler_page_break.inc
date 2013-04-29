<?php

/**
 * @file
 * The page break plugin for PDF page display.
 *
 * This plugin is used to add a page break to a PDF display.
 *
 */

/**
 * Plugin class that holds the functionality for the
 * page break in a PDF display.
 *
 */
class views_pdf_handler_page_break extends views_handler_field {

  protected $countRecords = 0;

  /**
   * This method  is used to query data. In our case
   * we want that no data is queried.
   *
   */
  function query() {
    // Override parent::query() and don't alter query.
    $this->field_alias = 'pdf_page_break_' . $this->position;
  }

  /**
   * This method contains the defintion of the options for the page
   * break.
   *
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['last_row'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Option form
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['last_row'] = array(
      '#type' => 'checkbox',
      '#title' => t('Exclude from last row'),
      '#default_value' => $this->options['last_row'],
      '#description' => t('Check this box to not add new page on last row.'),
    );
  }

  /**
   * This method renders the page break. It uses the PDF class to
   * add a page break.
   */
  function render($values) {
    if (isset($this->view->pdf) && is_object($this->view->pdf)) {
      if ($this->options['last_row'] == TRUE && ($this->countRecords + 1 >= $this->view->numberOfRecords)) {
        return '';
      }

      $this->countRecords++;
      $this->view->pdf->addPage();
      return '';
    }

  }

  /**
   * We dont want to use advanced rendering.
   */
  function allow_advanced_render() {
    return FALSE;
  }
}
