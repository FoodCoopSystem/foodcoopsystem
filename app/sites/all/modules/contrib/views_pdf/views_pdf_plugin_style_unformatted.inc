<?php


/**
 * @file
 * Unformatted PDF style
 */


/**
 * This class holds all the funtionality used for the unformatted style plugin.
 */
class views_pdf_plugin_style_unformatted extends views_plugin_style {
  /**
   * Rendering function
   */
  function render() {
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }

    $output = '';

    $this->view->numberOfRecords = count($this->view->result);
    if ($this->uses_row_plugin()) {
      $this->row_plugin->render($this->view->result);
    }

    return $output;
  }

  /**
   * Attach this view to another display as a feed.
   *
   * Provide basic functionality for all export style views like attaching a
   * feed image link.
   */
  function attach_to($display_id, $path, $title) {
    $display = $this->view->display[$display_id]->handler;
    $url_options = array();
    $input = $this->view->get_exposed_input();
    if ($input) {
      $url_options['query'] = $input;
    }

    if (empty($this->view->feed_icon)) {
      $this->view->feed_icon = '';
    }
    $this->view->feed_icon .= theme(
      'views_pdf_icon',
      array(
        'path' => $this->view->get_url(NULL, $path),
        'title' => $title,
        'options' => $url_options
      )
    );
  }

}
