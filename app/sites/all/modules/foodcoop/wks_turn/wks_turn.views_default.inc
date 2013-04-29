<?php
/**
 * @file
 * wks_turn.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function wks_turn_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'tura';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Tura';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'więcej';
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    5 => '5',
    7 => '7',
    6 => '6',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Zastosuj';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Przywróć';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortuj po';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Rosnąco';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Opis';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'title_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
    1 => array(
      'field' => 'field_poczatek_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'type' => 'type',
    'field_poczatek' => 'field_poczatek',
    'title' => 'title',
    'field_zgloszeni' => 'field_zgloszeni',
    'field_poczatek_1' => 'field_poczatek_1',
    'title_1' => 'title_1',
  );
  $handler->display->display_options['style_options']['default'] = 'type';
  $handler->display->display_options['style_options']['info'] = array(
    'type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_poczatek' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_zgloszeni' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_poczatek_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Wygląd przy braku wyników: Globalne: Pole tekstowe */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'Brak atywnych tur';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Zależność: Zawartość: Tura (field_tura) */
  $handler->display->display_options['relationships']['field_tura_nid']['id'] = 'field_tura_nid';
  $handler->display->display_options['relationships']['field_tura_nid']['table'] = 'field_data_field_tura';
  $handler->display->display_options['relationships']['field_tura_nid']['field'] = 'field_tura_nid';
  $handler->display->display_options['relationships']['field_tura_nid']['delta'] = '-1';
  /* Zależność: Flags: signup */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'signup';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Pole: Zawartość: typ */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Zadanie';
  /* Pole: Zawartość: Czas zapisów */
  $handler->display->display_options['fields']['field_poczatek']['id'] = 'field_poczatek';
  $handler->display->display_options['fields']['field_poczatek']['table'] = 'field_data_field_poczatek';
  $handler->display->display_options['fields']['field_poczatek']['field'] = 'field_poczatek';
  $handler->display->display_options['fields']['field_poczatek']['label'] = 'Realizacja zadania';
  $handler->display->display_options['fields']['field_poczatek']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Pole: Zawartość: Tytuł */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Opis';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Pole: Zawartość: Zgloszeni */
  $handler->display->display_options['fields']['field_zgloszeni']['id'] = 'field_zgloszeni';
  $handler->display->display_options['fields']['field_zgloszeni']['table'] = 'field_data_field_zgloszeni';
  $handler->display->display_options['fields']['field_zgloszeni']['field'] = 'field_zgloszeni';
  $handler->display->display_options['fields']['field_zgloszeni']['label'] = 'Złosili się';
  $handler->display->display_options['fields']['field_zgloszeni']['type'] = 'user_reference_plain';
  $handler->display->display_options['fields']['field_zgloszeni']['delta_offset'] = '0';
  /* Pole: Zawartość: Czas zapisów */
  $handler->display->display_options['fields']['field_poczatek_1']['id'] = 'field_poczatek_1';
  $handler->display->display_options['fields']['field_poczatek_1']['table'] = 'field_data_field_poczatek';
  $handler->display->display_options['fields']['field_poczatek_1']['field'] = 'field_poczatek';
  $handler->display->display_options['fields']['field_poczatek_1']['relationship'] = 'field_tura_nid';
  $handler->display->display_options['fields']['field_poczatek_1']['label'] = 'Czas zakupów';
  $handler->display->display_options['fields']['field_poczatek_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_poczatek_1']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Pole: Zawartość: Tytuł */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_tura_nid';
  $handler->display->display_options['fields']['title_1']['label'] = 'Tura';
  $handler->display->display_options['fields']['title_1']['exclude'] = TRUE;
  /* Kryterium sortowania: Zawartość: Czas zapisów (field_poczatek) */
  $handler->display->display_options['sorts']['field_poczatek_value']['id'] = 'field_poczatek_value';
  $handler->display->display_options['sorts']['field_poczatek_value']['table'] = 'field_data_field_poczatek';
  $handler->display->display_options['sorts']['field_poczatek_value']['field'] = 'field_poczatek_value';
  $handler->display->display_options['sorts']['field_poczatek_value']['relationship'] = 'field_tura_nid';
  /* Kryterium filtrowania: Zawartość: Do publikacji */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Kryterium filtrowania: Zawartość: typ */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'kasjer' => 'kasjer',
    'koordynacja' => 'koordynacja',
    'nadz_r_ksi_gowo_ci' => 'nadz_r_ksi_gowo_ci',
    'sprz_tanie' => 'sprz_tanie',
    'transport' => 'transport',
    'wazacy' => 'wazacy',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Kryterium filtrowania: Zawartość: Czas zapisów (field_poczatek) */
  $handler->display->display_options['filters']['field_poczatek_value']['id'] = 'field_poczatek_value';
  $handler->display->display_options['filters']['field_poczatek_value']['table'] = 'field_data_field_poczatek';
  $handler->display->display_options['filters']['field_poczatek_value']['field'] = 'field_poczatek_value';
  $handler->display->display_options['filters']['field_poczatek_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_poczatek_value']['group'] = 1;
  $handler->display->display_options['filters']['field_poczatek_value']['default_date'] = 'now';
  /* Kryterium filtrowania: Zawartość: Tura (field_tura) */
  $handler->display->display_options['filters']['field_tura_nid']['id'] = 'field_tura_nid';
  $handler->display->display_options['filters']['field_tura_nid']['table'] = 'field_data_field_tura';
  $handler->display->display_options['filters']['field_tura_nid']['field'] = 'field_tura_nid';
  $handler->display->display_options['filters']['field_tura_nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tura_nid']['expose']['operator_id'] = 'field_tura_nid_op';
  $handler->display->display_options['filters']['field_tura_nid']['expose']['label'] = 'Wybierz turę';
  $handler->display->display_options['filters']['field_tura_nid']['expose']['operator'] = 'field_tura_nid_op';
  $handler->display->display_options['filters']['field_tura_nid']['expose']['identifier'] = 'field_tura_nid';

  /* Display: Tura */
  $handler = $view->new_display('page', 'Tura', 'page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'title_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['row_class'] = 'title';
  $handler->display->display_options['style_options']['columns'] = array(
    'type' => 'type',
    'title' => 'title',
    'field_zgloszeni' => 'field_zgloszeni',
    'field_poczatek_1' => 'field_poczatek_1',
    'title_1' => 'title_1',
    'ops' => 'ops',
  );
  $handler->display->display_options['style_options']['default'] = 'type';
  $handler->display->display_options['style_options']['info'] = array(
    'type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_zgloszeni' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_poczatek_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'ops' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* Wygląd przy braku wyników: Globalne: Pole tekstowe */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Tura';
  $handler->display->display_options['empty']['area']['content'] = 'Brak aktywnych tur';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Pole: Zawartość: typ */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Zadanie';
  /* Pole: Zawartość: Tytuł */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Opis';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['preserve_tags'] = 'a';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Pole: Flags: User uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'flag_content';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  /* Pole: Globalne: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Zgłosili się';
  $handler->display->display_options['fields']['php']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_value'] = '$user=user_load($row->uid);
$username=$user->name;

return $username;';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Pole: Zawartość: Zgloszeni */
  $handler->display->display_options['fields']['field_zgloszeni']['id'] = 'field_zgloszeni';
  $handler->display->display_options['fields']['field_zgloszeni']['table'] = 'field_data_field_zgloszeni';
  $handler->display->display_options['fields']['field_zgloszeni']['field'] = 'field_zgloszeni';
  $handler->display->display_options['fields']['field_zgloszeni']['label'] = '';
  $handler->display->display_options['fields']['field_zgloszeni']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_zgloszeni']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_zgloszeni']['alter']['text'] = '[php]';
  $handler->display->display_options['fields']['field_zgloszeni']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_zgloszeni']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_zgloszeni']['settings'] = array(
    'user_reference_view_mode' => 'full',
  );
  $handler->display->display_options['fields']['field_zgloszeni']['delta_offset'] = '0';
  /* Pole: Zawartość: Czas zapisów */
  $handler->display->display_options['fields']['field_poczatek_1']['id'] = 'field_poczatek_1';
  $handler->display->display_options['fields']['field_poczatek_1']['table'] = 'field_data_field_poczatek';
  $handler->display->display_options['fields']['field_poczatek_1']['field'] = 'field_poczatek';
  $handler->display->display_options['fields']['field_poczatek_1']['relationship'] = 'field_tura_nid';
  $handler->display->display_options['fields']['field_poczatek_1']['label'] = 'Czas zakupów';
  $handler->display->display_options['fields']['field_poczatek_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_poczatek_1']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Pole: Zawartość: Tytuł */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_tura_nid';
  $handler->display->display_options['fields']['title_1']['label'] = 'Tura';
  $handler->display->display_options['fields']['title_1']['exclude'] = TRUE;
  /* Pole: Flags: Flag link */
  $handler->display->display_options['fields']['ops']['id'] = 'ops';
  $handler->display->display_options['fields']['ops']['table'] = 'flag_content';
  $handler->display->display_options['fields']['ops']['field'] = 'ops';
  $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['fields']['ops']['label'] = 'Zgłoś się.';
  $handler->display->display_options['fields']['ops']['link_type'] = 'confirm';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Kryterium filtrowania: Zawartość: Do publikacji */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Kryterium filtrowania: Zawartość: typ */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'kasjer' => 'kasjer',
    'kasjer_2' => 'kasjer_2',
    'kasjer_3' => 'kasjer_3',
    'koordynacja' => 'koordynacja',
    'koordynacja_2' => 'koordynacja_2',
    'nadz_r_ksi_gowo_ci' => 'nadz_r_ksi_gowo_ci',
    'nadz_r_ksi_gowo_ci_2' => 'nadz_r_ksi_gowo_ci_2',
    'sprz_tanie' => 'sprz_tanie',
    'sprz_tanie_2' => 'sprz_tanie_2',
    'sprz_tanie_3' => 'sprz_tanie_3',
    'transport' => 'transport',
    'transport_2' => 'transport_2',
    'wazacy' => 'wazacy',
    'wazacy_2' => 'wazacy_2',
    'wazacy_3' => 'wazacy_3',
  );
  /* Kryterium filtrowania: Zawartość: Czas zapisów (field_poczatek) */
  $handler->display->display_options['filters']['field_poczatek_value']['id'] = 'field_poczatek_value';
  $handler->display->display_options['filters']['field_poczatek_value']['table'] = 'field_data_field_poczatek';
  $handler->display->display_options['filters']['field_poczatek_value']['field'] = 'field_poczatek_value';
  $handler->display->display_options['filters']['field_poczatek_value']['relationship'] = 'field_tura_nid';
  $handler->display->display_options['filters']['field_poczatek_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_poczatek_value']['granularity'] = 'hour';
  $handler->display->display_options['filters']['field_poczatek_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_poczatek_value']['default_date'] = 'now';
  $handler->display->display_options['filters']['field_poczatek_value']['year_range'] = '-0:+1';
  $handler->display->display_options['path'] = 'tura';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Aktywne tury';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-tury';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Tura';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'main-menu';

  /* Display: Procedury */
  $handler = $view->new_display('attachment', 'Procedury', 'attachment_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'field_poczatek_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
    1 => array(
      'field' => 'title_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'field_poczatek' => 'field_poczatek',
    'title' => 'title',
    'type' => 'type',
    'ops' => 'ops',
    'field_zgloszeni' => 'field_zgloszeni',
    'field_poczatek_1' => 'field_poczatek_1',
    'title_1' => 'title_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_poczatek' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'ops' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_zgloszeni' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_poczatek_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Pole: Globalne: Własny tekst */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<h2>
<a href="https://docs.google.com/document/d/1csIZcWUFN9GgEX_epAHJCVVLN3mYGFjWLu5TT8NkHIw/edit?hl=en&authkey=CJ_P8fgP&pli=1#" target="_blank">Link do procedur</a>
</h2>
';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Kryterium filtrowania: Zawartość: Do publikacji */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Kryterium filtrowania: Zawartość: typ */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'kasjer' => 'kasjer',
    'koordynacja' => 'koordynacja',
    'nadz_r_ksi_gowo_ci' => 'nadz_r_ksi_gowo_ci',
    'sprz_tanie' => 'sprz_tanie',
    'transport' => 'transport',
    'wazacy' => 'wazacy',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
    'block_1' => 0,
    'page_1' => 0,
  );

  /* Display: Tura Blok */
  $handler = $view->new_display('block', 'Tura Blok', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Pole: Zawartość: Czas zapisów */
  $handler->display->display_options['fields']['field_poczatek']['id'] = 'field_poczatek';
  $handler->display->display_options['fields']['field_poczatek']['table'] = 'field_data_field_poczatek';
  $handler->display->display_options['fields']['field_poczatek']['field'] = 'field_poczatek';
  $handler->display->display_options['fields']['field_poczatek']['relationship'] = 'field_tura_nid';
  $handler->display->display_options['fields']['field_poczatek']['label'] = 'Termin zakupów';
  $handler->display->display_options['fields']['field_poczatek']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Pole: Pole: Dostawca */
  $handler->display->display_options['fields']['field_dostawca']['id'] = 'field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['table'] = 'field_data_field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['field'] = 'field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['relationship'] = 'field_tura_nid';
  $handler->display->display_options['fields']['field_dostawca']['type'] = 'taxonomy_term_reference_plain';

  /* Display: Archiwum tur */
  $handler = $view->new_display('page', 'Archiwum tur', 'page_1');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '70';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Kryterium filtrowania: Zawartość: Do publikacji */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Kryterium filtrowania: Zawartość: typ */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'kasjer' => 'kasjer',
    'kasjer_2' => 'kasjer_2',
    'kasjer_3' => 'kasjer_3',
    'koordynacja' => 'koordynacja',
    'koordynacja_2' => 'koordynacja_2',
    'nadz_r_ksi_gowo_ci' => 'nadz_r_ksi_gowo_ci',
    'nadz_r_ksi_gowo_ci_2' => 'nadz_r_ksi_gowo_ci_2',
    'sprz_tanie' => 'sprz_tanie',
    'sprz_tanie_2' => 'sprz_tanie_2',
    'sprz_tanie_3' => 'sprz_tanie_3',
    'transport' => 'transport',
    'transport_2' => 'transport_2',
    'wazacy' => 'wazacy',
    'wazacy_2' => 'wazacy_2',
    'wazacy_3' => 'wazacy_3',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Kryterium filtrowania: Zawartość: Tura (field_tura) */
  $handler->display->display_options['filters']['field_tura_nid']['id'] = 'field_tura_nid';
  $handler->display->display_options['filters']['field_tura_nid']['table'] = 'field_data_field_tura';
  $handler->display->display_options['filters']['field_tura_nid']['field'] = 'field_tura_nid';
  $handler->display->display_options['filters']['field_tura_nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tura_nid']['expose']['operator_id'] = 'field_tura_nid_op';
  $handler->display->display_options['filters']['field_tura_nid']['expose']['label'] = 'Wybierz turę';
  $handler->display->display_options['filters']['field_tura_nid']['expose']['operator'] = 'field_tura_nid_op';
  $handler->display->display_options['filters']['field_tura_nid']['expose']['identifier'] = 'field_tura_nid';
  $handler->display->display_options['path'] = 'archiwum-tur';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Archiwum tur';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['name'] = 'menu-tury';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Tura';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'main-menu';
  $translatables['tura'] = array(
    t('Master'),
    t('więcej'),
    t('Zastosuj'),
    t('Przywróć'),
    t('Sortuj po'),
    t('Rosnąco'),
    t('Opis'),
    t('Brak atywnych tur'),
    t('field_tura'),
    t('flag'),
    t('Zadanie'),
    t('Realizacja zadania'),
    t('Złosili się'),
    t('Czas zakupów'),
    t('Tura'),
    t('Wybierz turę'),
    t('Brak aktywnych tur'),
    t('User uid'),
    t('Zgłosili się'),
    t('[php]'),
    t('Zgłoś się.'),
    t('Procedury'),
    t('<h2>
<a href="https://docs.google.com/document/d/1csIZcWUFN9GgEX_epAHJCVVLN3mYGFjWLu5TT8NkHIw/edit?hl=en&authkey=CJ_P8fgP&pli=1#" target="_blank">Link do procedur</a>
</h2>
'),
    t('Tura Blok'),
    t('Termin zakupów'),
    t('Dostawca'),
    t('Archiwum tur'),
  );
  $export['tura'] = $view;

  return $export;
}
