<?php
/**
 * @file
 * wks.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wks_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "commerce_decimal_quantities" && $api == "default_commerce_decimal_quantities_presets") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_flag_default_flags().
 */
function wks_flag_default_flags() {
  $flags = array();
  // Exported flag: "Ulubione".
  $flags['bookmarks'] = array(
    'content_type' => 'node',
    'title' => 'Bookmarks',
    'global' => '0',
    'types' => array(
      0 => 'typ_produktu',
    ),
    'flag_short' => 'Lubię to',
    'flag_long' => 'Dodaj do ulubionych',
    'flag_message' => 'Dodano do ulubionych.',
    'unflag_short' => 'Nie lubię',
    'unflag_long' => 'Usun z ulubionych',
    'unflag_message' => 'Usunieto z ulubionych.',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'roles' => array(
      'flag' => array(
        0 => '7',
        1 => '5',
        2 => '6',
        3 => '3',
      ),
      'unflag' => array(
        0 => '7',
        1 => '5',
        2 => '6',
        3 => '3',
      ),
    ),
    'weight' => 0,
    'show_on_form' => 1,
    'access_author' => '',
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_contextual_link' => 0,
    'i18n' => 0,
    'module' => 'wks',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;

}

/**
 * Implements hook_node_info().
 */
function wks_node_info() {
  $items = array(
    'page' => array(
      'name' => t('Basic page'),
      'base' => 'node_content',
      'description' => t('Use <em>basic pages</em> for your static content, such as an \'About us\' page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'webform' => array(
      'name' => t('Webform'),
      'base' => 'node_content',
      'description' => t('Create a new form or questionnaire accessible to users. Submission results and statistics are recorded and accessible to privileged users.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
