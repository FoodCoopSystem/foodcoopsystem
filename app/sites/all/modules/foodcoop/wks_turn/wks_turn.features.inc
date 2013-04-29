<?php
/**
 * @file
 * wks_turn.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wks_turn_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function wks_turn_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function wks_turn_flag_default_flags() {
  $flags = array();
  // Exported flag: "Zgłaszam sie".
  $flags['signup'] = array(
    'content_type' => 'node',
    'title' => 'Zgłaszam sie',
    'global' => '0',
    'types' => array(
      0 => 'kasjer',
      1 => 'kasjer_2',
      2 => 'kasjer_3',
      3 => 'koordynacja',
      4 => 'koordynacja_2',
      5 => 'nadz_r_ksi_gowo_ci',
      6 => 'nadz_r_ksi_gowo_ci_2',
      7 => 'sprz_tanie',
      8 => 'sprz_tanie_2',
      9 => 'sprz_tanie_3',
      10 => 'transport',
      11 => 'transport_2',
      12 => 'wazacy',
      13 => 'wazacy_2',
      14 => 'wazacy_3',
    ),
    'flag_short' => 'Zgłaszam się',
    'flag_long' => 'Zgłoś się do zadania',
    'flag_message' => 'Zgłosiłeś się do zadania [node:type]',
    'unflag_short' => 'Rezygnuję',
    'unflag_long' => 'Rezygnuj z uczestnistwa w zadaniu',
    'unflag_message' => 'Zrezygnowałeś z zadania [node:type]',
    'unflag_denied_text' => 'Nie masz uprawnień do zgłoszenia',
    'link_type' => 'toggle',
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
    'show_contextual_link' => 1,
    'i18n' => 0,
    'module' => 'wks_turn',
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
function wks_turn_node_info() {
  $items = array(
    'kasjer' => array(
      'name' => t('Kasjer/Kasjerka 1'),
      'base' => 'node_content',
      'description' => t('Może być troje kasjerów/kasjerek'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'kasjer_2' => array(
      'name' => t('Kasjer/Kasjerka 2'),
      'base' => 'node_content',
      'description' => t('Może być troje kasjerów/kasjerek'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'kasjer_3' => array(
      'name' => t('Kasjer/Kasjerka 3'),
      'base' => 'node_content',
      'description' => t('Może być troje kasjerów/kasjerek'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'koordynacja' => array(
      'name' => t('Koordynacja'),
      'base' => 'node_content',
      'description' => t('Koordynator lub koordynatorka tury musi mieć doświadczenie.'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'koordynacja_2' => array(
      'name' => t('Koordynacja(Pomoc)'),
      'base' => 'node_content',
      'description' => t('Pomocnik koordynatora lub koordynatorki.'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'nadz_r_ksi_gowo_ci' => array(
      'name' => t('Nadzór księgowości 1'),
      'base' => 'node_content',
      'description' => t('Nadzór księgowości'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'nadz_r_ksi_gowo_ci_2' => array(
      'name' => t('Nadzór księgowości 2'),
      'base' => 'node_content',
      'description' => t('Nadzór księgowości'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'sprz_tanie' => array(
      'name' => t('Sprzątanie 1'),
      'base' => 'node_content',
      'description' => t('Sprzątanie po turze'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'sprz_tanie_2' => array(
      'name' => t('Sprzątanie 2'),
      'base' => 'node_content',
      'description' => t('Sprzątanie po turze'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'sprz_tanie_3' => array(
      'name' => t('Sprzątanie 3'),
      'base' => 'node_content',
      'description' => t('Sprzątanie po turze'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'transport' => array(
      'name' => t('Transport/Kierowca'),
      'base' => 'node_content',
      'description' => t('Kierowca musi dysponować samochodem.'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'transport_2' => array(
      'name' => t('Transport/Pomoc do zakupów'),
      'base' => 'node_content',
      'description' => t('Pomocnik przy zakupach.'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'tura' => array(
      'name' => t('Tura'),
      'base' => 'node_content',
      'description' => t('Zawartość typu tura'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => t('* - pola zaznaczone gwiazdką są obowiązkowe.'),
    ),
    'wazacy' => array(
      'name' => t('Ważacy/Ważaca 1'),
      'base' => 'node_content',
      'description' => t('Zadanie ważący lub ważąca'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'wazacy_2' => array(
      'name' => t('Ważacy/Ważaca 2'),
      'base' => 'node_content',
      'description' => t('Zadanie ważący lub ważąca'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
    'wazacy_3' => array(
      'name' => t('Ważacy/Ważaca 3'),
      'base' => 'node_content',
      'description' => t('Zadanie ważący lub ważąca'),
      'has_title' => '1',
      'title_label' => t('Opis'),
      'help' => '',
    ),
  );
  return $items;
}
