<?php
/**
 * @file
 * wks_user.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wks_user_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}
