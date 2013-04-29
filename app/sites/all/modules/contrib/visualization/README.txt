Visualization is a module for Drupal 7.x that provides a solid and easy
accessible way to visualize data. 

It provides a theme hook that takes a data array and some options and will then
render a chart in-place. It also provides a Views Display plugin so that users
can easily visualize data retrieved through Views.

The different charting libraries are pluggable through the CTools Plugin system.
The module provides an implementation for the Google Visualization API and
Highcharts. Other modules will be able to add their own charting libraries
by implementing an interface and making it known to CTools.

Modules can add their own charting libraries by implementing 
hook_ctools_plugin_directory (see CTools documentation) and by placing
.inc files with implementations of the VisualizationHandlerInterface interface. 

/**
 * Implements hook_ctools_plugin_directory().
 */
function visualization_ctools_plugin_directory($module, $plugin) {
  if (($module == 'visualization') && ($plugin == 'library')) {
    return 'includes/plugins';
  }
}

For information about implementing VisualizationHandlerInterface, check out
includes/interfaces.inc or use the current implementations of the
Google Visualization API or Highcharts as examples.