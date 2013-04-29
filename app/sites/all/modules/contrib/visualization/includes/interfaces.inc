<?php
/**
 * @file
 * Provides interfaces used by the module and its plugins.
 */

interface VisualizationHandlerInterface {

  /**
   * Renders a chart with a given HTML identifier, data set and options.
   *
   * @param $chart_id
   *	 A unique identifier associated with the chart. The HTML of the chart is
   *   also contained within a div with this identifier.
   *
   * @param $data
   *	 An array of rows. Each row is an associative array with as key the
   *   internal field name.
   *
   * @param $options
   *	 An associative array that contains options to be used when rendering, 
   *   users can pass additional options for specific charting library 
   *   implementations.
   *
   *	 Some default information is:
   *	  - title: the label of the chart
   *		- type: the internal name of the type of the chart; default types are
   *      	line, pie and column. Specific charting libraries are welcome to
   *				implement custom chart types as they please.
   *	 	- fields: an associative array of internal field names and their labels.
   *		- xAxis: options concerning the x-axis (if appropriate)
   *		- yAxis: options concerning the y-axis (if appropriate)
   *
   * @return
   *	 A string that will be the body of the chart div.
   */
  public function render($chart_id, $data, $options);

  /**
   * Performs anything that should be considered after rendering.
   */
  public function postRender();

  /**
   * Whether or not the handler is available for rendering.
   */
  public function available();

  /**
   * Returns an array containing the chart types supported by this handler.
   */
  public function supportedTypes();

}
