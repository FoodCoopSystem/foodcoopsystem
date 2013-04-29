<?php
/**
 * @file
 * wks_commerce.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function wks_commerce_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'commerce_products_to_buy';
  $view->description = '';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_line_item';
  $view->human_name = 'Products to buy';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Kierowca - Produkty do kupienia';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'więcej';
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    5 => '5',
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
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elementów na stronę';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Wszystko -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Przesunięcie';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« pierwsza';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ poprzednia';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'następna ›';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'sku' => 'sku',
    'title' => 'title',
    'commerce_unit_price' => 'commerce_unit_price',
    'quantity' => 'quantity',
    'type' => 'type',
    'commerce_total' => 'commerce_total',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'sku' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_unit_price' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'quantity' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
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
    'commerce_total' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Zależność: Pozycja zamówienia Commerce: ID zamówienia */
  $handler->display->display_options['relationships']['order_id']['id'] = 'order_id';
  $handler->display->display_options['relationships']['order_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['relationships']['order_id']['field'] = 'order_id';
  $handler->display->display_options['relationships']['order_id']['required'] = TRUE;
  /* Zależność: Commerce Line item: Powiązany produkt */
  $handler->display->display_options['relationships']['commerce_product_product_id']['id'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['table'] = 'field_data_commerce_product';
  $handler->display->display_options['relationships']['commerce_product_product_id']['field'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['required'] = TRUE;
  /* Pole: Commerce Product: SKU */
  $handler->display->display_options['fields']['sku']['id'] = 'sku';
  $handler->display->display_options['fields']['sku']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['sku']['field'] = 'sku';
  $handler->display->display_options['fields']['sku']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['sku']['label'] = 'Produkt';
  $handler->display->display_options['fields']['sku']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['sku']['link_to_product'] = 1;
  /* Pole: Commerce Product: Tytuł */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_product'] = 0;
  /* Pole: Commerce Line item: Cena jednostkowa */
  $handler->display->display_options['fields']['commerce_unit_price']['id'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['table'] = 'field_data_commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['field'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_unit_price']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_unit_price']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_unit_price']['group_column'] = 'entity_id';
  /* Pole: SUM(Pozycja zamówienia Commerce: Ilość) */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['group_type'] = 'sum';
  $handler->display->display_options['fields']['quantity']['label'] = 'Sztuki/kg';
  $handler->display->display_options['fields']['quantity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  /* Pole: Commerce Product: typ */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['type']['label'] = 'Typ';
  $handler->display->display_options['fields']['type']['link_to_product'] = 0;
  $handler->display->display_options['fields']['type']['use_raw_value'] = 0;
  /* Pole: SUM(Commerce Line item: Razem) */
  $handler->display->display_options['fields']['commerce_total']['id'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['table'] = 'field_data_commerce_total';
  $handler->display->display_options['fields']['commerce_total']['field'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['group_type'] = 'sum';
  $handler->display->display_options['fields']['commerce_total']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_total']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_total']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_total']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_total']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );
  /* Kryterium sortowania: Commerce Product: Tytuł */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'commerce_product';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['sorts']['title']['order'] = 'DESC';
  /* Kryterium filtrowania: Pozycja zamówienia Commerce: typ */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'product' => 'product',
  );
  /* Kryterium filtrowania: Zamówienie Commerce: Stan zamówienia */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['relationship'] = 'order_id';
  $handler->display->display_options['filters']['state']['value'] = array(
    'pending' => 'pending',
    'completed' => 'completed',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Kryterium filtrowania: Pozycja zamówienia Commerce: typ */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'product' => 'product',
  );
  /* Kryterium filtrowania: Zamówienie Commerce: Stan zamówienia */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['relationship'] = 'order_id';
  $handler->display->display_options['filters']['state']['value'] = array(
    'checkout' => 'checkout',
  );
  $handler->display->display_options['path'] = 'admin/commerce/prints/products';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Products to buy';
  $handler->display->display_options['menu']['description'] = 'View product reports.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-menu-prints';
  $handler->display->display_options['menu']['context'] = 0;
  $translatables['commerce_products_to_buy'] = array(
    t('Master'),
    t('Kierowca - Produkty do kupienia'),
    t('więcej'),
    t('Zastosuj'),
    t('Przywróć'),
    t('Sortuj po'),
    t('Rosnąco'),
    t('Opis'),
    t('Elementów na stronę'),
    t('- Wszystko -'),
    t('Przesunięcie'),
    t('« pierwsza'),
    t('‹ poprzednia'),
    t('następna ›'),
    t('last »'),
    t('Zamówienie'),
    t('Produkt'),
    t('Tytuł'),
    t('Cena jednostkowa'),
    t('Sztuki/kg'),
    t('.'),
    t(','),
    t('Typ'),
    t('Razem'),
    t('Page'),
  );
  $export['commerce_products_to_buy'] = $view;

  $view = new view();
  $view->name = 'produkty';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Produkty';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'product';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'więcej';
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    5 => '5',
    7 => '7',
    6 => '6',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Zastosuj';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Przywróć';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortuj po';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Rosnąco';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Opis';
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'field_kategorie_produkt_w_tid' => array(
      'bef_format' => 'bef',
      'more_options' => array(
        'bef_select_all_none' => 0,
        'bef_collapsible' => 0,
        'bef_filter_description' => '',
      ),
    ),
  );
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Zależność: Flags: bookmarks */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'bookmarks';
  /* Zależność: Zawartość: Powiązany produkt */
  $handler->display->display_options['relationships']['field_nowy_produkt_product_id']['id'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['relationships']['field_nowy_produkt_product_id']['table'] = 'field_data_field_nowy_produkt';
  $handler->display->display_options['relationships']['field_nowy_produkt_product_id']['field'] = 'field_nowy_produkt_product_id';
  /* Pole: Pole: Dostawca */
  $handler->display->display_options['fields']['field_dostawca']['id'] = 'field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['table'] = 'field_data_field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['field'] = 'field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['field_dostawca']['label'] = '';
  $handler->display->display_options['fields']['field_dostawca']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_dostawca']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_dostawca']['type'] = 'taxonomy_term_reference_plain';
  /* Pole: Commerce Product: opis */
  $handler->display->display_options['fields']['field_opis']['id'] = 'field_opis';
  $handler->display->display_options['fields']['field_opis']['table'] = 'field_data_field_opis';
  $handler->display->display_options['fields']['field_opis']['field'] = 'field_opis';
  $handler->display->display_options['fields']['field_opis']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['field_opis']['label'] = '';
  $handler->display->display_options['fields']['field_opis']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_opis']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_opis']['alter']['text'] = '[[field_dostawca]] - <br/>
[field_opis]
';
  $handler->display->display_options['fields']['field_opis']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_opis']['hide_alter_empty'] = FALSE;
  /* Pole: Commerce Product: Magazyn */
  $handler->display->display_options['fields']['commerce_stock']['id'] = 'commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['table'] = 'field_data_commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['field'] = 'commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['commerce_stock']['label'] = '';
  $handler->display->display_options['fields']['commerce_stock']['exclude'] = TRUE;
  $handler->display->display_options['fields']['commerce_stock']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_stock']['settings'] = array(
    'thousand_separator' => ',',
    'prefix_suffix' => 1,
  );
  $handler->display->display_options['fields']['commerce_stock']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['commerce_stock']['delta_offset'] = '0';
  /* Pole: Zawartość: Tytuł */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Pole: Zawartość: Obraz */
  $handler->display->display_options['fields']['field_obraz']['id'] = 'field_obraz';
  $handler->display->display_options['fields']['field_obraz']['table'] = 'field_data_field_obraz';
  $handler->display->display_options['fields']['field_obraz']['field'] = 'field_obraz';
  $handler->display->display_options['fields']['field_obraz']['label'] = '';
  $handler->display->display_options['fields']['field_obraz']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_obraz']['alter']['path'] = '#';
  $handler->display->display_options['fields']['field_obraz']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['field_obraz']['alter']['alt'] = '[field_opis]';
  $handler->display->display_options['fields']['field_obraz']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_obraz']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_obraz']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
  );
  /* Pole: Commerce Product: Cena */
  $handler->display->display_options['fields']['commerce_price']['id'] = 'commerce_price';
  $handler->display->display_options['fields']['commerce_price']['table'] = 'field_data_commerce_price';
  $handler->display->display_options['fields']['commerce_price']['field'] = 'commerce_price';
  $handler->display->display_options['fields']['commerce_price']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['commerce_price']['label'] = '';
  $handler->display->display_options['fields']['commerce_price']['exclude'] = TRUE;
  $handler->display->display_options['fields']['commerce_price']['element_class'] = 'price';
  $handler->display->display_options['fields']['commerce_price']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_price']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_price']['settings'] = array(
    'calculation' => 'calculated_sell_price',
  );
  /* Pole: Zawartość: Tagi */
  $handler->display->display_options['fields']['field_tagi']['id'] = 'field_tagi';
  $handler->display->display_options['fields']['field_tagi']['table'] = 'field_data_field_tagi';
  $handler->display->display_options['fields']['field_tagi']['field'] = 'field_tagi';
  $handler->display->display_options['fields']['field_tagi']['label'] = '';
  $handler->display->display_options['fields']['field_tagi']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_tagi']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_tagi']['delta_offset'] = '0';
  /* Pole: Commerce Product: typ */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['type']['link_to_product'] = 0;
  $handler->display->display_options['fields']['type']['use_raw_value'] = 0;
  /* Pole: Globalne: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = '';
  $handler->display->display_options['fields']['php']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_value'] = '$static = commerce_product_load($row->field_opis);

if($static->field_paczka) {
$product_id = $static->sku;
$package = $static->field_paczka[\'und\'][0][\'value\'];

$orders = db_query(\'SELECT order_id FROM commerce_order WHERE status = :status\', array(\':status\' => \'cart\'))->fetchAll();

foreach($orders as $order) {

$quant = db_query(\'SELECT quantity FROM commerce_line_item WHERE order_id = :order_id AND line_item_label = :sku\', array(\':order_id\' => $order->order_id, \':sku\' => $product_id))->fetchAllKeyed();


}
$keys = array_keys($quant);
$sum = array_sum($keys);

return $sum;

}
';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Pole: Globalne: PHP */
  $handler->display->display_options['fields']['php_1']['id'] = 'php_1';
  $handler->display->display_options['fields']['php_1']['table'] = 'views';
  $handler->display->display_options['fields']['php_1']['field'] = 'php';
  $handler->display->display_options['fields']['php_1']['label'] = '';
  $handler->display->display_options['fields']['php_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php_1']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_1']['php_value'] = '$static = commerce_product_load($row->field_opis);

if($static->field_paczka) {

$proc = $row->php / $static->field_paczka[\'und\'][0][\'value\'] * 100;
return $proc;
}';
  $handler->display->display_options['fields']['php_1']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_1']['php_click_sortable'] = '';
  /* Pole: Commerce Product: Paczka */
  $handler->display->display_options['fields']['field_paczka']['id'] = 'field_paczka';
  $handler->display->display_options['fields']['field_paczka']['table'] = 'field_data_field_paczka';
  $handler->display->display_options['fields']['field_paczka']['field'] = 'field_paczka';
  $handler->display->display_options['fields']['field_paczka']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['field_paczka']['label'] = '';
  $handler->display->display_options['fields']['field_paczka']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_paczka']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_paczka']['alter']['text'] = '<div class="progress">
<a title="Paczka [field_paczka] kg zamówiono [php] kg" class="[php_1]" href="#"> 
<div class="s100"></div></a>
</div>';
  $handler->display->display_options['fields']['field_paczka']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_paczka']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_paczka']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Pole: Commerce Product: Quantity input field */
  $handler->display->display_options['fields']['add_to_cart_quantity']['id'] = 'add_to_cart_quantity';
  $handler->display->display_options['fields']['add_to_cart_quantity']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['add_to_cart_quantity']['field'] = 'add_to_cart_quantity';
  $handler->display->display_options['fields']['add_to_cart_quantity']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['add_to_cart_quantity']['label'] = '';
  $handler->display->display_options['fields']['add_to_cart_quantity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['add_to_cart_quantity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['add_to_cart_quantity']['default_quantity'] = '0.00';
  $handler->display->display_options['fields']['add_to_cart_quantity']['combine'] = 0;
  $handler->display->display_options['fields']['add_to_cart_quantity']['display_path'] = 0;
  /* Pole: Flags: Flag link */
  $handler->display->display_options['fields']['ops']['id'] = 'ops';
  $handler->display->display_options['fields']['ops']['table'] = 'flag_content';
  $handler->display->display_options['fields']['ops']['field'] = 'ops';
  $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['fields']['ops']['label'] = 'Lubię to';
  $handler->display->display_options['fields']['ops']['exclude'] = TRUE;
  $handler->display->display_options['fields']['ops']['hide_empty'] = TRUE;
  /* Pole: Globalne: Własny tekst */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div id="product">
<div id="product-name">[title]</div>
<div id="product-body">[[field_dostawca]] [field_opis]</div>
<div id="product-photo">[field_obraz]</div>
<div id="product-price">[commerce_price]</div>
<div id="product-tags">[field_tagi]</div>
<div id="product-quantity">[add_to_cart_quantity]</div>
<div id="product-type">[type]</div>
[field_paczka] 
</div>
<div id="like-flag">[ops]</div>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Kryterium sortowania: Zawartość: Data nadesłania */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
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
    'typ_produktu' => 'typ_produktu',
  );
  /* Kryterium filtrowania: Commerce Product: typ */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'na_sztuki' => 'na_sztuki',
    'product' => 'product',
  );
  /* Kryterium filtrowania: Commerce Product: Status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['status_1']['value'] = '1';

  /* Display: Produkty wszystkie */
  $handler = $view->new_display('page', 'Produkty wszystkie', 'page');
  $handler->display->display_options['path'] = 'produkty/wszystkie';

  /* Display: Produkty warzywa */
  $handler = $view->new_display('page', 'Produkty warzywa', 'page_1');
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Kryterium filtrowania: Zawartość: typ */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'typ_produktu' => 'typ_produktu',
  );
  /* Kryterium filtrowania: Commerce Product: Status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  /* Kryterium filtrowania: Commerce Product: typ */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'na_sztuki' => 'na_sztuki',
    'product' => 'product',
  );
  /* Kryterium filtrowania: Zawartość: Do publikacji */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Kryterium filtrowania: Zawartość: Posiada terminy kategorii (z głębokością) */
  $handler->display->display_options['filters']['term_node_tid_depth']['id'] = 'term_node_tid_depth';
  $handler->display->display_options['filters']['term_node_tid_depth']['table'] = 'node';
  $handler->display->display_options['filters']['term_node_tid_depth']['field'] = 'term_node_tid_depth';
  $handler->display->display_options['filters']['term_node_tid_depth']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['term_node_tid_depth']['reduce_duplicates'] = TRUE;
  $handler->display->display_options['filters']['term_node_tid_depth']['type'] = 'select';
  $handler->display->display_options['filters']['term_node_tid_depth']['vocabulary'] = 'tags';
  $handler->display->display_options['filters']['term_node_tid_depth']['hierarchy'] = 1;
  $handler->display->display_options['filters']['term_node_tid_depth']['depth'] = '0';
  /* Kryterium filtrowania: Zawartość: Do publikacji */
  $handler->display->display_options['filters']['status_2']['id'] = 'status_2';
  $handler->display->display_options['filters']['status_2']['table'] = 'node';
  $handler->display->display_options['filters']['status_2']['field'] = 'status';
  $handler->display->display_options['filters']['status_2']['value'] = '1';
  $handler->display->display_options['path'] = 'produkty/warzywa';

  /* Display: Produkty owoce */
  $handler = $view->new_display('page', 'Produkty owoce', 'page_2');
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
    'typ_produktu' => 'typ_produktu',
  );
  /* Kryterium filtrowania: Commerce Product: typ */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'na_sztuki' => 'na_sztuki',
    'product' => 'product',
  );
  /* Kryterium filtrowania: Zawartość: Kategoria produktów (field_kategoria_produkt) */
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['id'] = 'field_kategoria_produkt_tid';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['table'] = 'field_data_field_kategoria_produkt';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['field'] = 'field_kategoria_produkt_tid';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['value'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['vocabulary'] = 'tags';
  /* Kryterium filtrowania: Commerce Product: Status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['path'] = 'produkty/owoce';

  /* Display: Produkty */
  $handler = $view->new_display('page', 'Produkty', 'page_3');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'field_dostawca',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Pole: Pole: Dostawca */
  $handler->display->display_options['fields']['field_dostawca']['id'] = 'field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['table'] = 'field_data_field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['field'] = 'field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['relationship'] = 'field_produkt_product_id';
  $handler->display->display_options['fields']['field_dostawca']['label'] = '';
  $handler->display->display_options['fields']['field_dostawca']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_dostawca']['alter']['text'] = '<span></span>';
  $handler->display->display_options['fields']['field_dostawca']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_dostawca']['type'] = 'taxonomy_term_reference_plain';
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
    'typ_produktu' => 'typ_produktu',
  );
  $handler->display->display_options['path'] = 'produkty';

  /* Display: Produkty bakalie */
  $handler = $view->new_display('page', 'Produkty bakalie', 'page_4');
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
    'typ_produktu' => 'typ_produktu',
  );
  /* Kryterium filtrowania: Commerce Product: typ */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'na_sztuki' => 'na_sztuki',
    'product' => 'product',
  );
  /* Kryterium filtrowania: Zawartość: Kategoria produktów (field_kategoria_produkt) */
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['id'] = 'field_kategoria_produkt_tid';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['table'] = 'field_data_field_kategoria_produkt';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['field'] = 'field_kategoria_produkt_tid';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['value'] = array(
    4 => '4',
  );
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['vocabulary'] = 'tags';
  /* Kryterium filtrowania: Commerce Product: Status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['path'] = 'produkty/bakalie';

  /* Display: Produkty inne */
  $handler = $view->new_display('page', 'Produkty inne', 'page_5');
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
    'typ_produktu' => 'typ_produktu',
  );
  /* Kryterium filtrowania: Commerce Product: typ */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'na_sztuki' => 'na_sztuki',
    'product' => 'product',
  );
  /* Kryterium filtrowania: Zawartość: Kategoria produktów (field_kategoria_produkt) */
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['id'] = 'field_kategoria_produkt_tid';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['table'] = 'field_data_field_kategoria_produkt';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['field'] = 'field_kategoria_produkt_tid';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['value'] = array(
    3 => '3',
  );
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['vocabulary'] = 'tags';
  /* Kryterium filtrowania: Commerce Product: Status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['path'] = 'produkty/inne';

  /* Display: Produkty strączki */
  $handler = $view->new_display('page', 'Produkty strączki', 'page_6');
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
    'typ_produktu' => 'typ_produktu',
  );
  /* Kryterium filtrowania: Commerce Product: typ */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'na_sztuki' => 'na_sztuki',
    'product' => 'product',
  );
  /* Kryterium filtrowania: Zawartość: Kategoria produktów (field_kategoria_produkt) */
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['id'] = 'field_kategoria_produkt_tid';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['table'] = 'field_data_field_kategoria_produkt';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['field'] = 'field_kategoria_produkt_tid';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['value'] = array(
    5 => '5',
  );
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_kategoria_produkt_tid']['vocabulary'] = 'tags';
  /* Kryterium filtrowania: Commerce Product: Status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['path'] = 'produkty/straczki';

  /* Display: Magazyn */
  $handler = $view->new_display('page', 'Magazyn', 'page_7');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Magazyn';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Pole: Commerce Product: typ */
  $handler->display->display_options['fields']['type_1']['id'] = 'type_1';
  $handler->display->display_options['fields']['type_1']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['type_1']['field'] = 'type';
  $handler->display->display_options['fields']['type_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['type_1']['label'] = '';
  $handler->display->display_options['fields']['type_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['type_1']['link_to_product'] = 0;
  $handler->display->display_options['fields']['type_1']['use_raw_value'] = 0;
  /* Pole: Commerce Product: opis */
  $handler->display->display_options['fields']['field_opis']['id'] = 'field_opis';
  $handler->display->display_options['fields']['field_opis']['table'] = 'field_data_field_opis';
  $handler->display->display_options['fields']['field_opis']['field'] = 'field_opis';
  $handler->display->display_options['fields']['field_opis']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['field_opis']['label'] = '';
  $handler->display->display_options['fields']['field_opis']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_opis']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_opis']['alter']['text'] = '[[type_1]] - <br/>
[field_opis]
';
  $handler->display->display_options['fields']['field_opis']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_opis']['hide_alter_empty'] = FALSE;
  /* Pole: Commerce Product: Magazyn */
  $handler->display->display_options['fields']['commerce_stock']['id'] = 'commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['table'] = 'field_data_commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['field'] = 'commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['commerce_stock']['label'] = 'Zostało w magazynie:';
  $handler->display->display_options['fields']['commerce_stock']['exclude'] = TRUE;
  $handler->display->display_options['fields']['commerce_stock']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_stock']['settings'] = array(
    'thousand_separator' => ',',
    'prefix_suffix' => 1,
  );
  $handler->display->display_options['fields']['commerce_stock']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['commerce_stock']['delta_offset'] = '0';
  /* Pole: Zawartość: Tytuł */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Pole: Zawartość: Obraz */
  $handler->display->display_options['fields']['field_obraz']['id'] = 'field_obraz';
  $handler->display->display_options['fields']['field_obraz']['table'] = 'field_data_field_obraz';
  $handler->display->display_options['fields']['field_obraz']['field'] = 'field_obraz';
  $handler->display->display_options['fields']['field_obraz']['label'] = '';
  $handler->display->display_options['fields']['field_obraz']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_obraz']['alter']['path'] = '#';
  $handler->display->display_options['fields']['field_obraz']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['field_obraz']['alter']['alt'] = '[field_opis]';
  $handler->display->display_options['fields']['field_obraz']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_obraz']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_obraz']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
  );
  /* Pole: Commerce Product: Cena */
  $handler->display->display_options['fields']['commerce_price']['id'] = 'commerce_price';
  $handler->display->display_options['fields']['commerce_price']['table'] = 'field_data_commerce_price';
  $handler->display->display_options['fields']['commerce_price']['field'] = 'commerce_price';
  $handler->display->display_options['fields']['commerce_price']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['commerce_price']['label'] = '';
  $handler->display->display_options['fields']['commerce_price']['exclude'] = TRUE;
  $handler->display->display_options['fields']['commerce_price']['element_class'] = 'price';
  $handler->display->display_options['fields']['commerce_price']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_price']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_price']['settings'] = array(
    'calculation' => 'calculated_sell_price',
  );
  /* Pole: Zawartość: Tagi */
  $handler->display->display_options['fields']['field_tagi']['id'] = 'field_tagi';
  $handler->display->display_options['fields']['field_tagi']['table'] = 'field_data_field_tagi';
  $handler->display->display_options['fields']['field_tagi']['field'] = 'field_tagi';
  $handler->display->display_options['fields']['field_tagi']['label'] = '';
  $handler->display->display_options['fields']['field_tagi']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_tagi']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_tagi']['delta_offset'] = '0';
  /* Pole: Commerce Product: typ */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['type']['link_to_product'] = 0;
  $handler->display->display_options['fields']['type']['use_raw_value'] = 0;
  /* Pole: Commerce Product: Paczka */
  $handler->display->display_options['fields']['field_paczka']['id'] = 'field_paczka';
  $handler->display->display_options['fields']['field_paczka']['table'] = 'field_data_field_paczka';
  $handler->display->display_options['fields']['field_paczka']['field'] = 'field_paczka';
  $handler->display->display_options['fields']['field_paczka']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['field_paczka']['label'] = '';
  $handler->display->display_options['fields']['field_paczka']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_paczka']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_paczka']['alter']['text'] = '<div class="progress">
<a title="zamówiono [field_paczka] z [field_paczka] " href="#"> <div class="s100"></div></a>
</div>
';
  $handler->display->display_options['fields']['field_paczka']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_paczka']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_paczka']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Pole: Commerce Product: Quantity input field */
  $handler->display->display_options['fields']['add_to_cart_quantity']['id'] = 'add_to_cart_quantity';
  $handler->display->display_options['fields']['add_to_cart_quantity']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['add_to_cart_quantity']['field'] = 'add_to_cart_quantity';
  $handler->display->display_options['fields']['add_to_cart_quantity']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['add_to_cart_quantity']['label'] = '';
  $handler->display->display_options['fields']['add_to_cart_quantity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['add_to_cart_quantity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['add_to_cart_quantity']['default_quantity'] = '0.00';
  $handler->display->display_options['fields']['add_to_cart_quantity']['combine'] = 0;
  $handler->display->display_options['fields']['add_to_cart_quantity']['display_path'] = 0;
  /* Pole: Flags: Flag link */
  $handler->display->display_options['fields']['ops']['id'] = 'ops';
  $handler->display->display_options['fields']['ops']['table'] = 'flag_content';
  $handler->display->display_options['fields']['ops']['field'] = 'ops';
  $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['fields']['ops']['label'] = 'Lubię to';
  $handler->display->display_options['fields']['ops']['exclude'] = TRUE;
  $handler->display->display_options['fields']['ops']['hide_empty'] = TRUE;
  /* Pole: Globalne: Własny tekst */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div id="product">
<div id="product-name">[title]</div>
<div id="product-body">W magazynie zostało: [commerce_stock]</div>
<div id="product-photo">[field_obraz]</div>
<div id="product-price">[commerce_price]</div>
<div id="product-tags">[field_tagi]</div>
<div id="product-quantity">[add_to_cart_quantity]</div>
<div id="product-type">[type]</div>
[field_paczka] 
</div>
<div id="like-flag">[ops]</div>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
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
    'typ_produktu' => 'typ_produktu',
  );
  /* Kryterium filtrowania: Commerce Product: typ */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'magazyn' => 'magazyn',
    'sztuki_magazyn' => 'sztuki_magazyn',
  );
  /* Kryterium filtrowania: Commerce Product: Status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  /* Kryterium filtrowania: Commerce Product: Magazyn (commerce_stock) */
  $handler->display->display_options['filters']['commerce_stock_value']['id'] = 'commerce_stock_value';
  $handler->display->display_options['filters']['commerce_stock_value']['table'] = 'field_data_commerce_stock';
  $handler->display->display_options['filters']['commerce_stock_value']['field'] = 'commerce_stock_value';
  $handler->display->display_options['filters']['commerce_stock_value']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['commerce_stock_value']['operator'] = '>';
  $handler->display->display_options['filters']['commerce_stock_value']['value']['value'] = '0';
  $handler->display->display_options['path'] = 'produkty/magazyn';

  /* Display: Ulubione */
  $handler = $view->new_display('page', 'Ulubione', 'page_8');
  $handler->display->display_options['display_description'] = 'Strona z ulubionymi';
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
    'typ_produktu' => 'typ_produktu',
  );
  /* Kryterium filtrowania: Commerce Product: typ */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'na_sztuki' => 'na_sztuki',
    'product' => 'product',
  );
  /* Kryterium filtrowania: Flags: Flagged */
  $handler->display->display_options['filters']['flagged']['id'] = 'flagged';
  $handler->display->display_options['filters']['flagged']['table'] = 'flag_content';
  $handler->display->display_options['filters']['flagged']['field'] = 'flagged';
  $handler->display->display_options['filters']['flagged']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['filters']['flagged']['value'] = '1';
  /* Kryterium filtrowania: Commerce Product: Status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['path'] = 'produkty/ulubione';
  $translatables['produkty'] = array(
    t('Master'),
    t('więcej'),
    t('Zastosuj'),
    t('Przywróć'),
    t('Sortuj po'),
    t('Rosnąco'),
    t('Opis'),
    t('flag'),
    t('Produkt'),
    t('[[field_dostawca]] - <br/>
[field_opis]
'),
    t('[field_opis]'),
    t('<div class="progress">
<a title="Paczka [field_paczka] kg zamówiono [php] kg" class="[php_1]" href="#"> 
<div class="s100"></div></a>
</div>'),
    t('Lubię to'),
    t('<div id="product">
<div id="product-name">[title]</div>
<div id="product-body">[[field_dostawca]] [field_opis]</div>
<div id="product-photo">[field_obraz]</div>
<div id="product-price">[commerce_price]</div>
<div id="product-tags">[field_tagi]</div>
<div id="product-quantity">[add_to_cart_quantity]</div>
<div id="product-type">[type]</div>
[field_paczka] 
</div>
<div id="like-flag">[ops]</div>'),
    t('Produkty wszystkie'),
    t('Produkty warzywa'),
    t('Produkty owoce'),
    t('Produkty'),
    t('<span></span>'),
    t('Produkty bakalie'),
    t('Produkty inne'),
    t('Produkty strączki'),
    t('Magazyn'),
    t('[[type_1]] - <br/>
[field_opis]
'),
    t('Zostało w magazynie:'),
    t('<div class="progress">
<a title="zamówiono [field_paczka] z [field_paczka] " href="#"> <div class="s100"></div></a>
</div>
'),
    t('<div id="product">
<div id="product-name">[title]</div>
<div id="product-body">W magazynie zostało: [commerce_stock]</div>
<div id="product-photo">[field_obraz]</div>
<div id="product-price">[commerce_price]</div>
<div id="product-tags">[field_tagi]</div>
<div id="product-quantity">[add_to_cart_quantity]</div>
<div id="product-type">[type]</div>
[field_paczka] 
</div>
<div id="like-flag">[ops]</div>'),
    t('Ulubione'),
    t('Strona z ulubionymi'),
  );
  $export['produkty'] = $view;

  $view = new view();
  $view->name = 'shopping_cart_form_stock';
  $view->description = 'Display a shopping cart update form.';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Shopping cart form stock';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Koszyk przed zatwierdzeniem';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'więcej';
  $handler->display->display_options['access']['type'] = 'none';
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
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'commerce_display_path' => 'commerce_display_path',
    'line_item_title' => 'line_item_title',
    'commerce_unit_price' => 'commerce_unit_price',
    'edit_quantity' => 'edit_quantity',
    'edit_delete' => 'edit_delete',
    'commerce_total' => 'commerce_total',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'commerce_display_path' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'line_item_title' => array(
      'align' => '',
      'separator' => '',
    ),
    'commerce_unit_price' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'edit_quantity' => array(
      'align' => '',
      'separator' => '',
    ),
    'edit_delete' => array(
      'align' => '',
      'separator' => '',
    ),
    'commerce_total' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  /* Nagłówek: Globalne: Pole tekstowe */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'Dodanie produktów do koszyka jest równoważne z chęcią ich zamówienia. Produkty zamawiane są grupowo przez koordynatora podczas realizacji zakupów.';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  /* Stopka: Pozycja zamówienia Commerce: Podsumowanie pozycji zamówienia */
  $handler->display->display_options['footer']['line_item_summary']['id'] = 'line_item_summary';
  $handler->display->display_options['footer']['line_item_summary']['table'] = 'commerce_line_item';
  $handler->display->display_options['footer']['line_item_summary']['field'] = 'line_item_summary';
  $handler->display->display_options['footer']['line_item_summary']['label'] = 'Podsumowanie koszyka';
  $handler->display->display_options['footer']['line_item_summary']['info'] = array(
    'quantity' => 0,
    'total' => 'total',
  );
  /* Zależność: Zamówienie Commerce: Powiązana pozycja zamówienia */
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
  /* Pole: Commerce Line item: Ścieżka wyświetlania */
  $handler->display->display_options['fields']['commerce_display_path']['id'] = 'commerce_display_path';
  $handler->display->display_options['fields']['commerce_display_path']['table'] = 'field_data_commerce_display_path';
  $handler->display->display_options['fields']['commerce_display_path']['field'] = 'commerce_display_path';
  $handler->display->display_options['fields']['commerce_display_path']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_display_path']['label'] = '';
  $handler->display->display_options['fields']['commerce_display_path']['exclude'] = TRUE;
  /* Pole: Pozycja zamówienia Commerce: Tytuł */
  $handler->display->display_options['fields']['line_item_title']['id'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_title']['field'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['line_item_title']['label'] = 'Produkt';
  $handler->display->display_options['fields']['line_item_title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['line_item_title']['alter']['path'] = '[commerce_display_path]';
  /* Pole: Commerce Line item: Cena jednostkowa */
  $handler->display->display_options['fields']['commerce_unit_price']['id'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['table'] = 'field_data_commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['field'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_unit_price']['label'] = 'Cena';
  $handler->display->display_options['fields']['commerce_unit_price']['element_class'] = 'price';
  $handler->display->display_options['fields']['commerce_unit_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_unit_price']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_unit_price']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Pole: Pozycja zamówienia Commerce: Przycisk usuwania */
  $handler->display->display_options['fields']['edit_delete']['id'] = 'edit_delete';
  $handler->display->display_options['fields']['edit_delete']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['edit_delete']['field'] = 'edit_delete';
  $handler->display->display_options['fields']['edit_delete']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['edit_delete']['label'] = 'Usuń';
  /* Pole: Commerce Line item: Razem */
  $handler->display->display_options['fields']['commerce_total']['id'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['table'] = 'field_data_commerce_total';
  $handler->display->display_options['fields']['commerce_total']['field'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_total']['element_class'] = 'price';
  $handler->display->display_options['fields']['commerce_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_total']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_total']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Kryterium sortowania: Pozycja zamówienia Commerce: ID pozycji zamówienia */
  $handler->display->display_options['sorts']['line_item_id']['id'] = 'line_item_id';
  $handler->display->display_options['sorts']['line_item_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['sorts']['line_item_id']['field'] = 'line_item_id';
  $handler->display->display_options['sorts']['line_item_id']['relationship'] = 'commerce_line_items_line_item_id';
  /* Filtr kontekstowy: Zamówienie Commerce: ID zamówienia */
  $handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['order_id']['exception']['title'] = 'Wszystkie';
  $handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';
  /* Kryterium filtrowania: Pozycja zamówienia Commerce: Pozycja zamówienia jest typu produktowa pozycja zamówienia. */
  $handler->display->display_options['filters']['product_line_item_type']['id'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['product_line_item_type']['field'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['filters']['product_line_item_type']['value'] = '1';
  $handler->display->display_options['filters']['product_line_item_type']['group'] = 0;
  $translatables['shopping_cart_form_stock'] = array(
    t('Defaults'),
    t('Koszyk przed zatwierdzeniem'),
    t('więcej'),
    t('Zastosuj'),
    t('Przywróć'),
    t('Sortuj po'),
    t('Rosnąco'),
    t('Opis'),
    t('Dodanie produktów do koszyka jest równoważne z chęcią ich zamówienia. Produkty zamawiane są grupowo przez koordynatora podczas realizacji zakupów.'),
    t('Podsumowanie koszyka'),
    t('Pozycje zamówienia'),
    t('Produkt'),
    t('Cena'),
    t('Usuń'),
    t('Razem'),
    t('Wszystkie'),
  );
  $export['shopping_cart_form_stock'] = $view;

  $view = new view();
  $view->name = 'shopping_cart_wks_form';
  $view->description = 'Display a shopping cart update form.';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Shopping cart WKS form';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Koszyk przed zatwierdzeniem';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'więcej';
  $handler->display->display_options['access']['type'] = 'none';
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
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'commerce_display_path' => 'commerce_display_path',
    'line_item_title' => 'line_item_title',
    'commerce_unit_price' => 'commerce_unit_price',
    'edit_quantity' => 'edit_quantity',
    'edit_delete' => 'edit_delete',
    'commerce_total' => 'commerce_total',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'commerce_display_path' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'line_item_title' => array(
      'align' => '',
      'separator' => '',
    ),
    'commerce_unit_price' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'edit_quantity' => array(
      'align' => '',
      'separator' => '',
    ),
    'edit_delete' => array(
      'align' => '',
      'separator' => '',
    ),
    'commerce_total' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  /* Nagłówek: Globalne: Pole tekstowe */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = ' Dodanie produktów do koszyka jest równoważne z chęcią ich zamówienia. Produkty zamawiane są grupowo przez koordynatora podczas realizacji zakupów.';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  /* Stopka: Pozycja zamówienia Commerce: Podsumowanie pozycji zamówienia */
  $handler->display->display_options['footer']['line_item_summary']['id'] = 'line_item_summary';
  $handler->display->display_options['footer']['line_item_summary']['table'] = 'commerce_line_item';
  $handler->display->display_options['footer']['line_item_summary']['field'] = 'line_item_summary';
  $handler->display->display_options['footer']['line_item_summary']['label'] = 'Podsumowanie koszyka';
  $handler->display->display_options['footer']['line_item_summary']['info'] = array(
    'quantity' => 0,
    'total' => 'total',
  );
  /* Zależność: Zamówienie Commerce: Powiązana pozycja zamówienia */
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
  /* Pole: Commerce Line item: Ścieżka wyświetlania */
  $handler->display->display_options['fields']['commerce_display_path']['id'] = 'commerce_display_path';
  $handler->display->display_options['fields']['commerce_display_path']['table'] = 'field_data_commerce_display_path';
  $handler->display->display_options['fields']['commerce_display_path']['field'] = 'commerce_display_path';
  $handler->display->display_options['fields']['commerce_display_path']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_display_path']['label'] = '';
  $handler->display->display_options['fields']['commerce_display_path']['exclude'] = TRUE;
  /* Pole: Pozycja zamówienia Commerce: Tytuł */
  $handler->display->display_options['fields']['line_item_title']['id'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_title']['field'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['line_item_title']['label'] = 'Produkt';
  $handler->display->display_options['fields']['line_item_title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['line_item_title']['alter']['path'] = '[commerce_display_path]';
  /* Pole: Commerce Line item: Cena jednostkowa */
  $handler->display->display_options['fields']['commerce_unit_price']['id'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['table'] = 'field_data_commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['field'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_unit_price']['label'] = 'Cena';
  $handler->display->display_options['fields']['commerce_unit_price']['element_class'] = 'price';
  $handler->display->display_options['fields']['commerce_unit_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_unit_price']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_unit_price']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Pole: Pozycja zamówienia Commerce: Pole tekstowe ilości */
  $handler->display->display_options['fields']['edit_quantity']['id'] = 'edit_quantity';
  $handler->display->display_options['fields']['edit_quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['edit_quantity']['field'] = 'edit_quantity';
  $handler->display->display_options['fields']['edit_quantity']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['edit_quantity']['label'] = 'Ilość';
  $handler->display->display_options['fields']['edit_quantity']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['edit_quantity']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['edit_quantity']['element_label_colon'] = FALSE;
  /* Pole: Pozycja zamówienia Commerce: Przycisk usuwania */
  $handler->display->display_options['fields']['edit_delete']['id'] = 'edit_delete';
  $handler->display->display_options['fields']['edit_delete']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['edit_delete']['field'] = 'edit_delete';
  $handler->display->display_options['fields']['edit_delete']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['edit_delete']['label'] = 'Usuń';
  /* Pole: Commerce Line item: Razem */
  $handler->display->display_options['fields']['commerce_total']['id'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['table'] = 'field_data_commerce_total';
  $handler->display->display_options['fields']['commerce_total']['field'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_total']['element_class'] = 'price';
  $handler->display->display_options['fields']['commerce_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_total']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_total']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Kryterium sortowania: Pozycja zamówienia Commerce: ID pozycji zamówienia */
  $handler->display->display_options['sorts']['line_item_id']['id'] = 'line_item_id';
  $handler->display->display_options['sorts']['line_item_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['sorts']['line_item_id']['field'] = 'line_item_id';
  $handler->display->display_options['sorts']['line_item_id']['relationship'] = 'commerce_line_items_line_item_id';
  /* Filtr kontekstowy: Zamówienie Commerce: ID zamówienia */
  $handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['order_id']['exception']['title'] = 'Wszystkie';
  $handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';
  /* Kryterium filtrowania: Pozycja zamówienia Commerce: Pozycja zamówienia jest typu produktowa pozycja zamówienia. */
  $handler->display->display_options['filters']['product_line_item_type']['id'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['product_line_item_type']['field'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['filters']['product_line_item_type']['value'] = '1';
  $handler->display->display_options['filters']['product_line_item_type']['group'] = 0;
  $translatables['shopping_cart_wks_form'] = array(
    t('Defaults'),
    t('Koszyk przed zatwierdzeniem'),
    t('więcej'),
    t('Zastosuj'),
    t('Przywróć'),
    t('Sortuj po'),
    t('Rosnąco'),
    t('Opis'),
    t(' Dodanie produktów do koszyka jest równoważne z chęcią ich zamówienia. Produkty zamawiane są grupowo przez koordynatora podczas realizacji zakupów.'),
    t('Podsumowanie koszyka'),
    t('Pozycje zamówienia'),
    t('Produkt'),
    t('Cena'),
    t('Ilość'),
    t('Usuń'),
    t('Razem'),
    t('Wszystkie'),
  );
  $export['shopping_cart_wks_form'] = $view;

  $view = new view();
  $view->name = 'tagi';
  $view->description = 'A view to emulate Drupal core\'s handling of taxonomy/term.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Tagi';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Tagi';
  $handler->display->display_options['css_class'] = 'product';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'więcej';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Zastosuj';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Przywróć';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortuj po';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Rosnąco';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Opis';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elementów na stronę';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Wszystko -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Przesunięcie';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« pierwsza';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ poprzednia';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'następna ›';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['row_class'] = 'product';
  $handler->display->display_options['style_options']['fill_single_line'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  /* Zależność: Zawartość: Powiązany produkt */
  $handler->display->display_options['relationships']['field_nowy_produkt_product_id']['id'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['relationships']['field_nowy_produkt_product_id']['table'] = 'field_data_field_nowy_produkt';
  $handler->display->display_options['relationships']['field_nowy_produkt_product_id']['field'] = 'field_nowy_produkt_product_id';
  /* Zależność: Flags: bookmarks */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'bookmarks';
  /* Pole: Commerce Product: opis */
  $handler->display->display_options['fields']['field_opis']['id'] = 'field_opis';
  $handler->display->display_options['fields']['field_opis']['table'] = 'field_data_field_opis';
  $handler->display->display_options['fields']['field_opis']['field'] = 'field_opis';
  $handler->display->display_options['fields']['field_opis']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['field_opis']['exclude'] = TRUE;
  /* Pole: Pole: Dostawca */
  $handler->display->display_options['fields']['field_dostawca']['id'] = 'field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['table'] = 'field_data_field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['field'] = 'field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['field_dostawca']['label'] = '';
  $handler->display->display_options['fields']['field_dostawca']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_dostawca']['element_label_colon'] = FALSE;
  /* Pole: Commerce Product: Cena */
  $handler->display->display_options['fields']['commerce_price']['id'] = 'commerce_price';
  $handler->display->display_options['fields']['commerce_price']['table'] = 'field_data_commerce_price';
  $handler->display->display_options['fields']['commerce_price']['field'] = 'commerce_price';
  $handler->display->display_options['fields']['commerce_price']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['commerce_price']['label'] = '';
  $handler->display->display_options['fields']['commerce_price']['exclude'] = TRUE;
  $handler->display->display_options['fields']['commerce_price']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_price']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_price']['settings'] = array(
    'calculation' => '0',
  );
  /* Pole: Commerce Product: Magazyn */
  $handler->display->display_options['fields']['commerce_stock']['id'] = 'commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['table'] = 'field_data_commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['field'] = 'commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['commerce_stock']['label'] = '';
  $handler->display->display_options['fields']['commerce_stock']['exclude'] = TRUE;
  $handler->display->display_options['fields']['commerce_stock']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_stock']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Pole: Flags: Flag link */
  $handler->display->display_options['fields']['ops']['id'] = 'ops';
  $handler->display->display_options['fields']['ops']['table'] = 'flag_content';
  $handler->display->display_options['fields']['ops']['field'] = 'ops';
  $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['fields']['ops']['label'] = '';
  $handler->display->display_options['fields']['ops']['exclude'] = TRUE;
  $handler->display->display_options['fields']['ops']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['ops']['link_type'] = 'normal';
  /* Pole: Commerce Product: typ */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['type']['link_to_product'] = 0;
  $handler->display->display_options['fields']['type']['use_raw_value'] = 0;
  /* Pole: Commerce Product: Paczka */
  $handler->display->display_options['fields']['field_paczka']['id'] = 'field_paczka';
  $handler->display->display_options['fields']['field_paczka']['table'] = 'field_data_field_paczka';
  $handler->display->display_options['fields']['field_paczka']['field'] = 'field_paczka';
  $handler->display->display_options['fields']['field_paczka']['relationship'] = 'field_nowy_produkt_product_id';
  /* Pole: Commerce Product: Quantity input field */
  $handler->display->display_options['fields']['add_to_cart_quantity']['id'] = 'add_to_cart_quantity';
  $handler->display->display_options['fields']['add_to_cart_quantity']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['add_to_cart_quantity']['field'] = 'add_to_cart_quantity';
  $handler->display->display_options['fields']['add_to_cart_quantity']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['add_to_cart_quantity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['add_to_cart_quantity']['default_quantity'] = '0';
  $handler->display->display_options['fields']['add_to_cart_quantity']['combine'] = 1;
  $handler->display->display_options['fields']['add_to_cart_quantity']['display_path'] = 0;
  /* Pole: Zawartość: Tytuł */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Pole: Zawartość: Tagi */
  $handler->display->display_options['fields']['field_tagi']['id'] = 'field_tagi';
  $handler->display->display_options['fields']['field_tagi']['table'] = 'field_data_field_tagi';
  $handler->display->display_options['fields']['field_tagi']['field'] = 'field_tagi';
  $handler->display->display_options['fields']['field_tagi']['label'] = '';
  $handler->display->display_options['fields']['field_tagi']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_tagi']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_tagi']['delta_offset'] = '0';
  /* Pole: Zawartość: Obraz */
  $handler->display->display_options['fields']['field_obraz']['id'] = 'field_obraz';
  $handler->display->display_options['fields']['field_obraz']['table'] = 'field_data_field_obraz';
  $handler->display->display_options['fields']['field_obraz']['field'] = 'field_obraz';
  $handler->display->display_options['fields']['field_obraz']['label'] = '';
  $handler->display->display_options['fields']['field_obraz']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_obraz']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_obraz']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_obraz']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
  );
  /* Pole: Globalne: Własny tekst */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div id="product">
<div id="product-name">[title]</div>
<div id="product-body">[field_opis]</div>
<div id="product-photo">[field_obraz]</div>
<div id="product-price">[commerce_price]</div>
<div id="product-tags">[field_tagi]</div>
<div id="product-quantity">[add_to_cart_quantity]</div>
<div id="product-type">[type]</div>
[field_paczka] 
</div>
<div id="like-flag">[ops]</div>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Kryterium sortowania: Zawartość: Promowany */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Kryterium sortowania: Zawartość: Data nadesłania */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filtr kontekstowy: Zawartość: Posiada ID terminu kategorii (z głębokością) */
  $handler->display->display_options['arguments']['term_node_tid_depth']['id'] = 'term_node_tid_depth';
  $handler->display->display_options['arguments']['term_node_tid_depth']['table'] = 'node';
  $handler->display->display_options['arguments']['term_node_tid_depth']['field'] = 'term_node_tid_depth';
  $handler->display->display_options['arguments']['term_node_tid_depth']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['term_node_tid_depth']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['term_node_tid_depth']['exception']['title'] = 'Wszystkie';
  $handler->display->display_options['arguments']['term_node_tid_depth']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['term_node_tid_depth']['title'] = '%1';
  $handler->display->display_options['arguments']['term_node_tid_depth']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['term_node_tid_depth']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['term_node_tid_depth']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['term_node_tid_depth']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['term_node_tid_depth']['depth'] = '0';
  $handler->display->display_options['arguments']['term_node_tid_depth']['break_phrase'] = TRUE;
  /* Filtr kontekstowy: Zawartość: Modyfikator głębokości dla Posiada ID terminu kategorii */
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['id'] = 'term_node_tid_depth_modifier';
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['table'] = 'node';
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['field'] = 'term_node_tid_depth_modifier';
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['exception']['title'] = 'Wszystkie';
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['specify_validation'] = TRUE;
  /* Kryterium filtrowania: Zawartość: Opublikowany albo tryb administracji */
  $handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['table'] = 'node';
  $handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['group'] = 0;
  $handler->display->display_options['filters']['status_extra']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'taxonomy/term/%';

  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'feed');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 15;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elementów na stronę';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Wszystko -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Przesunięcie';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« pierwsza';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ poprzednia';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'następna ›';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['path'] = 'taxonomy/term/%/%/feed';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $translatables['tagi'] = array(
    t('Master'),
    t('Tagi'),
    t('więcej'),
    t('Zastosuj'),
    t('Przywróć'),
    t('Sortuj po'),
    t('Rosnąco'),
    t('Opis'),
    t('Elementów na stronę'),
    t('- Wszystko -'),
    t('Przesunięcie'),
    t('« pierwsza'),
    t('‹ poprzednia'),
    t('następna ›'),
    t('last »'),
    t('Produkt'),
    t('flag'),
    t('opis'),
    t('Paczka'),
    t('Quantity input field'),
    t('<div id="product">
<div id="product-name">[title]</div>
<div id="product-body">[field_opis]</div>
<div id="product-photo">[field_obraz]</div>
<div id="product-price">[commerce_price]</div>
<div id="product-tags">[field_tagi]</div>
<div id="product-quantity">[add_to_cart_quantity]</div>
<div id="product-type">[type]</div>
[field_paczka] 
</div>
<div id="like-flag">[ops]</div>'),
    t('Wszystkie'),
    t('%1'),
    t('Page'),
    t('Feed'),
  );
  $export['tagi'] = $view;

  $view = new view();
  $view->name = 'tags';
  $view->description = 'A view to emulate Drupal core\'s handling of taxonomy/term.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Tags';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Tagi';
  $handler->display->display_options['css_class'] = 'product';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'więcej';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Zastosuj';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Przywróć';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortuj po';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Rosnąco';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Opis';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elementów na stronę';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Wszystko -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Przesunięcie';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« pierwsza';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ poprzednia';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'następna ›';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['row_class'] = 'product';
  $handler->display->display_options['style_options']['fill_single_line'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  /* Zależność: Zawartość: Powiązany produkt */
  $handler->display->display_options['relationships']['field_nowy_produkt_product_id']['id'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['relationships']['field_nowy_produkt_product_id']['table'] = 'field_data_field_nowy_produkt';
  $handler->display->display_options['relationships']['field_nowy_produkt_product_id']['field'] = 'field_nowy_produkt_product_id';
  /* Zależność: Flags: bookmarks */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'bookmarks';
  /* Pole: Commerce Product: opis */
  $handler->display->display_options['fields']['field_opis']['id'] = 'field_opis';
  $handler->display->display_options['fields']['field_opis']['table'] = 'field_data_field_opis';
  $handler->display->display_options['fields']['field_opis']['field'] = 'field_opis';
  $handler->display->display_options['fields']['field_opis']['relationship'] = 'field_nowy_produkt_product_id';
  /* Pole: Pole: Dostawca */
  $handler->display->display_options['fields']['field_dostawca']['id'] = 'field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['table'] = 'field_data_field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['field'] = 'field_dostawca';
  $handler->display->display_options['fields']['field_dostawca']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['field_dostawca']['label'] = '';
  $handler->display->display_options['fields']['field_dostawca']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_dostawca']['element_label_colon'] = FALSE;
  /* Pole: Commerce Product: Cena */
  $handler->display->display_options['fields']['commerce_price']['id'] = 'commerce_price';
  $handler->display->display_options['fields']['commerce_price']['table'] = 'field_data_commerce_price';
  $handler->display->display_options['fields']['commerce_price']['field'] = 'commerce_price';
  $handler->display->display_options['fields']['commerce_price']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['commerce_price']['label'] = '';
  $handler->display->display_options['fields']['commerce_price']['exclude'] = TRUE;
  $handler->display->display_options['fields']['commerce_price']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_price']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_price']['settings'] = array(
    'calculation' => '0',
  );
  /* Pole: Commerce Product: Magazyn */
  $handler->display->display_options['fields']['commerce_stock']['id'] = 'commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['table'] = 'field_data_commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['field'] = 'commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['commerce_stock']['label'] = '';
  $handler->display->display_options['fields']['commerce_stock']['exclude'] = TRUE;
  $handler->display->display_options['fields']['commerce_stock']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_stock']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Pole: Flags: Flag link */
  $handler->display->display_options['fields']['ops']['id'] = 'ops';
  $handler->display->display_options['fields']['ops']['table'] = 'flag_content';
  $handler->display->display_options['fields']['ops']['field'] = 'ops';
  $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['fields']['ops']['label'] = '';
  $handler->display->display_options['fields']['ops']['exclude'] = TRUE;
  $handler->display->display_options['fields']['ops']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['ops']['link_type'] = 'normal';
  /* Pole: Commerce Product: typ */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['type']['link_to_product'] = 0;
  $handler->display->display_options['fields']['type']['use_raw_value'] = 0;
  /* Pole: Commerce Product: Paczka */
  $handler->display->display_options['fields']['field_paczka']['id'] = 'field_paczka';
  $handler->display->display_options['fields']['field_paczka']['table'] = 'field_data_field_paczka';
  $handler->display->display_options['fields']['field_paczka']['field'] = 'field_paczka';
  $handler->display->display_options['fields']['field_paczka']['relationship'] = 'field_nowy_produkt_product_id';
  /* Pole: Commerce Product: Quantity input field */
  $handler->display->display_options['fields']['add_to_cart_quantity']['id'] = 'add_to_cart_quantity';
  $handler->display->display_options['fields']['add_to_cart_quantity']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['add_to_cart_quantity']['field'] = 'add_to_cart_quantity';
  $handler->display->display_options['fields']['add_to_cart_quantity']['relationship'] = 'field_nowy_produkt_product_id';
  $handler->display->display_options['fields']['add_to_cart_quantity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['add_to_cart_quantity']['default_quantity'] = '0';
  $handler->display->display_options['fields']['add_to_cart_quantity']['combine'] = 1;
  $handler->display->display_options['fields']['add_to_cart_quantity']['display_path'] = 0;
  /* Pole: Zawartość: Tytuł */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Pole: Zawartość: Tagi */
  $handler->display->display_options['fields']['field_tagi']['id'] = 'field_tagi';
  $handler->display->display_options['fields']['field_tagi']['table'] = 'field_data_field_tagi';
  $handler->display->display_options['fields']['field_tagi']['field'] = 'field_tagi';
  $handler->display->display_options['fields']['field_tagi']['label'] = '';
  $handler->display->display_options['fields']['field_tagi']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_tagi']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_tagi']['delta_offset'] = '0';
  /* Pole: Zawartość: Obraz */
  $handler->display->display_options['fields']['field_obraz']['id'] = 'field_obraz';
  $handler->display->display_options['fields']['field_obraz']['table'] = 'field_data_field_obraz';
  $handler->display->display_options['fields']['field_obraz']['field'] = 'field_obraz';
  $handler->display->display_options['fields']['field_obraz']['label'] = '';
  $handler->display->display_options['fields']['field_obraz']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_obraz']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_obraz']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_obraz']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
  );
  /* Pole: Globalne: Własny tekst */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div id="product">
<div id="product-name">[title]</div>
<div id="product-body">[field_opis]</div>
<div id="product-photo">[field_obraz]</div>
<div id="product-price">[commerce_price]</div>
<div id="product-tags">[field_tagi]</div>
<div id="product-quantity">[add_to_cart_quantity]</div>
<div id="product-type">[type]</div>
[field_paczka] 
</div>
<div id="like-flag">[ops]</div>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Kryterium sortowania: Zawartość: Promowany */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Kryterium sortowania: Zawartość: Data nadesłania */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filtr kontekstowy: Zawartość: Posiada ID terminu kategorii (z głębokością) */
  $handler->display->display_options['arguments']['term_node_tid_depth']['id'] = 'term_node_tid_depth';
  $handler->display->display_options['arguments']['term_node_tid_depth']['table'] = 'node';
  $handler->display->display_options['arguments']['term_node_tid_depth']['field'] = 'term_node_tid_depth';
  $handler->display->display_options['arguments']['term_node_tid_depth']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['term_node_tid_depth']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['term_node_tid_depth']['exception']['title'] = 'Wszystkie';
  $handler->display->display_options['arguments']['term_node_tid_depth']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['term_node_tid_depth']['title'] = '%1';
  $handler->display->display_options['arguments']['term_node_tid_depth']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['term_node_tid_depth']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['term_node_tid_depth']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['term_node_tid_depth']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['term_node_tid_depth']['depth'] = '0';
  $handler->display->display_options['arguments']['term_node_tid_depth']['break_phrase'] = TRUE;
  /* Filtr kontekstowy: Zawartość: Modyfikator głębokości dla Posiada ID terminu kategorii */
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['id'] = 'term_node_tid_depth_modifier';
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['table'] = 'node';
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['field'] = 'term_node_tid_depth_modifier';
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['exception']['title'] = 'Wszystkie';
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['term_node_tid_depth_modifier']['specify_validation'] = TRUE;
  /* Kryterium filtrowania: Zawartość: Opublikowany albo tryb administracji */
  $handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['table'] = 'node';
  $handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['group'] = 0;
  $handler->display->display_options['filters']['status_extra']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'taxonomy/term/%';

  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'feed');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 15;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elementów na stronę';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Wszystko -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Przesunięcie';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« pierwsza';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ poprzednia';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'następna ›';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['path'] = 'taxonomy/term/%/%/feed';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $translatables['tags'] = array(
    t('Master'),
    t('Tagi'),
    t('więcej'),
    t('Zastosuj'),
    t('Przywróć'),
    t('Sortuj po'),
    t('Rosnąco'),
    t('Opis'),
    t('Elementów na stronę'),
    t('- Wszystko -'),
    t('Przesunięcie'),
    t('« pierwsza'),
    t('‹ poprzednia'),
    t('następna ›'),
    t('last »'),
    t('Produkt'),
    t('flag'),
    t('opis'),
    t('Paczka'),
    t('Quantity input field'),
    t('<div id="product">
<div id="product-name">[title]</div>
<div id="product-body">[field_opis]</div>
<div id="product-photo">[field_obraz]</div>
<div id="product-price">[commerce_price]</div>
<div id="product-tags">[field_tagi]</div>
<div id="product-quantity">[add_to_cart_quantity]</div>
<div id="product-type">[type]</div>
[field_paczka] 
</div>
<div id="like-flag">[ops]</div>'),
    t('Wszystkie'),
    t('%1'),
    t('Page'),
    t('Feed'),
  );
  $export['tags'] = $view;

  $view = new view();
  $view->name = 'wydruk_zamowien';
  $view->description = 'Display a list of orders for the store admin.';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Orders list with names';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Kasjer - zamówienia z imionami';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'więcej';
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    5 => '5',
    6 => '6',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Zastosuj';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Przywróć';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortuj po';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Rosnąco';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Opis';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 50;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elementów na stronę';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Wszystko -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Przesunięcie';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« pierwsza';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ poprzednia';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'następna ›';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'order_number' => 'order_number',
    'field_adressfield' => 'field_adressfield',
    'field_telefon' => 'field_telefon',
    'commerce_line_items' => 'commerce_line_items',
    'status' => 'status',
  );
  $handler->display->display_options['style_options']['default'] = 'field_adressfield';
  $handler->display->display_options['style_options']['info'] = array(
    'order_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_adressfield' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_telefon' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_line_items' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Stopka: Zamówienie Commerce: Łączna wartość zamówienia */
  $handler->display->display_options['footer']['order_total']['id'] = 'order_total';
  $handler->display->display_options['footer']['order_total']['table'] = 'commerce_order';
  $handler->display->display_options['footer']['order_total']['field'] = 'order_total';
  /* Stopka: Globalne: Result summary */
  $handler->display->display_options['footer']['result']['id'] = 'result';
  $handler->display->display_options['footer']['result']['table'] = 'views';
  $handler->display->display_options['footer']['result']['field'] = 'result';
  $handler->display->display_options['footer']['result']['content'] = 'Wyświetlone: @start z @end Pozycji: @total na stronę @per_page';
  /* Wygląd przy braku wyników: Globalne: Pole tekstowe */
  $handler->display->display_options['empty']['text']['id'] = 'text';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['content'] = 'Brak utworzonych zamówień.';
  $handler->display->display_options['empty']['text']['format'] = 'filtered_html';
  /* Zależność: Zamówienie Commerce: Powiązana pozycja zamówienia */
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
  /* Zależność: Commerce Line item: Powiązany produkt */
  $handler->display->display_options['relationships']['commerce_product_product_id']['id'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['table'] = 'field_data_commerce_product';
  $handler->display->display_options['relationships']['commerce_product_product_id']['field'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['relationship'] = 'commerce_line_items_line_item_id';
  /* Zależność: Zamówienie Commerce: Właściciel */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'commerce_order';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Pole: Zamówienie Commerce: Numer zamówienia */
  $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['link_to_order'] = 'admin';
  /* Pole: Pole: Imię i nazwisko */
  $handler->display->display_options['fields']['field_adressfield']['id'] = 'field_adressfield';
  $handler->display->display_options['fields']['field_adressfield']['table'] = 'field_data_field_adressfield';
  $handler->display->display_options['fields']['field_adressfield']['field'] = 'field_adressfield';
  $handler->display->display_options['fields']['field_adressfield']['relationship'] = 'commerce_customer_billing_profile_id_1';
  $handler->display->display_options['fields']['field_adressfield']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['field_adressfield']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Pole: Commerce Product: Tytuł */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['title']['label'] = 'Nazwa';
  $handler->display->display_options['fields']['title']['link_to_product'] = 0;
  /* Pole: Zamówienie Commerce: Status zamówienia */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Kryterium filtrowania: Zamówienie Commerce: Stan zamówienia */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['value'] = array(
    'pending' => 'pending',
  );
  $handler->display->display_options['filters']['state']['expose']['operator_id'] = 'state_op';
  $handler->display->display_options['filters']['state']['expose']['label'] = 'Status zamówienia';
  $handler->display->display_options['filters']['state']['expose']['operator'] = 'state_op';
  $handler->display->display_options['filters']['state']['expose']['identifier'] = 'state';
  $handler->display->display_options['filters']['state']['expose']['multiple'] = TRUE;

  /* Display: Orders with user names  */
  $handler = $view->new_display('page', 'Orders with user names ', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Kasjer - Zamówienia z imionami';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Pole: Zamówienie Commerce: Numer zamówienia */
  $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['link_to_order'] = 'admin';
  /* Pole: Pole: Imię i nazwisko */
  $handler->display->display_options['fields']['field_adressfield']['id'] = 'field_adressfield';
  $handler->display->display_options['fields']['field_adressfield']['table'] = 'field_data_field_adressfield';
  $handler->display->display_options['fields']['field_adressfield']['field'] = 'field_adressfield';
  $handler->display->display_options['fields']['field_adressfield']['relationship'] = 'uid';
  $handler->display->display_options['fields']['field_adressfield']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['field_adressfield']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Pole: Pole: Telefon */
  $handler->display->display_options['fields']['field_telefon']['id'] = 'field_telefon';
  $handler->display->display_options['fields']['field_telefon']['table'] = 'field_data_field_telefon';
  $handler->display->display_options['fields']['field_telefon']['field'] = 'field_telefon';
  $handler->display->display_options['fields']['field_telefon']['relationship'] = 'uid';
  /* Pole: Zamówienie Commerce: Pozycje zamówienia */
  $handler->display->display_options['fields']['commerce_line_items']['id'] = 'commerce_line_items';
  $handler->display->display_options['fields']['commerce_line_items']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['fields']['commerce_line_items']['field'] = 'commerce_line_items';
  $handler->display->display_options['fields']['commerce_line_items']['settings'] = array(
    'view' => 'commerce_line_item_table|default',
  );
  $handler->display->display_options['fields']['commerce_line_items']['delta_offset'] = '0';
  /* Pole: Zamówienie Commerce: Status zamówienia */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['path'] = 'admin/commerce/prints/print-name';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Orders with user names';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Prints';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Quantities of products with user names */
  $handler = $view->new_display('page', 'Quantities of products with user names', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Ważący - Zamówienia z imionami';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'order_number',
      'rendered' => 0,
      'rendered_strip' => 0,
    ),
    1 => array(
      'field' => 'field_adressfield',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'order_number' => 'order_number',
    'field_adressfield' => 'field_adressfield',
    'title' => 'title',
    'quantity' => 'quantity',
    'type' => 'type',
    'status' => 'status',
  );
  $handler->display->display_options['style_options']['default'] = 'field_adressfield';
  $handler->display->display_options['style_options']['info'] = array(
    'order_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_adressfield' => array(
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
    'quantity' => array(
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
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Pole: Zamówienie Commerce: Numer zamówienia */
  $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['exclude'] = TRUE;
  $handler->display->display_options['fields']['order_number']['link_to_order'] = 'admin';
  /* Pole: Pole: Imię i nazwisko */
  $handler->display->display_options['fields']['field_adressfield']['id'] = 'field_adressfield';
  $handler->display->display_options['fields']['field_adressfield']['table'] = 'field_data_field_adressfield';
  $handler->display->display_options['fields']['field_adressfield']['field'] = 'field_adressfield';
  $handler->display->display_options['fields']['field_adressfield']['relationship'] = 'uid';
  $handler->display->display_options['fields']['field_adressfield']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_adressfield']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['field_adressfield']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Pole: Commerce Product: Tytuł */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['title']['label'] = 'Nazwa';
  $handler->display->display_options['fields']['title']['link_to_product'] = 0;
  /* Pole: Pozycja zamówienia Commerce: Ilość */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  /* Pole: Commerce Product: typ */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['type']['label'] = 'Typ';
  $handler->display->display_options['fields']['type']['link_to_product'] = 0;
  $handler->display->display_options['fields']['type']['use_raw_value'] = 0;
  /* Pole: Zamówienie Commerce: Status zamówienia */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['path'] = 'admin/commerce/prints/quantieties-names';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Quantities of products with names';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Quantities of products */
  $handler = $view->new_display('page', 'Quantities of products', 'page_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Ważący - Zamówienia';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'title',
      'rendered' => 0,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'order_number' => 'order_number',
    'field_adressfield' => 'field_adressfield',
    'title' => 'title',
    'quantity' => 'quantity',
    'type' => 'type',
    'status' => 'status',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'order_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_adressfield' => array(
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
    'quantity' => array(
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
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Pole: Zamówienie Commerce: Numer zamówienia */
  $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['link_to_order'] = 'admin';
  /* Pole: Pole: Imię i nazwisko */
  $handler->display->display_options['fields']['field_adressfield']['id'] = 'field_adressfield';
  $handler->display->display_options['fields']['field_adressfield']['table'] = 'field_data_field_adressfield';
  $handler->display->display_options['fields']['field_adressfield']['field'] = 'field_adressfield';
  $handler->display->display_options['fields']['field_adressfield']['relationship'] = 'uid';
  $handler->display->display_options['fields']['field_adressfield']['label'] = '';
  $handler->display->display_options['fields']['field_adressfield']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_adressfield']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['field_adressfield']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Pole: Commerce Product: Tytuł */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_product'] = 0;
  /* Pole: Pozycja zamówienia Commerce: Ilość */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  /* Pole: Commerce Product: typ */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['type']['label'] = 'Typ';
  $handler->display->display_options['fields']['type']['link_to_product'] = 0;
  $handler->display->display_options['fields']['type']['use_raw_value'] = 0;
  /* Pole: Zamówienie Commerce: Status zamówienia */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['path'] = 'admin/commerce/prints/quantieties';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Quantities of products';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $translatables['wydruk_zamowien'] = array(
    t('Defaults'),
    t('Kasjer - zamówienia z imionami'),
    t('więcej'),
    t('Zastosuj'),
    t('Przywróć'),
    t('Sortuj po'),
    t('Rosnąco'),
    t('Opis'),
    t('Elementów na stronę'),
    t('- Wszystko -'),
    t('Przesunięcie'),
    t('« pierwsza'),
    t('‹ poprzednia'),
    t('następna ›'),
    t('last »'),
    t('Wyświetlone: @start z @end Pozycji: @total na stronę @per_page'),
    t('Brak utworzonych zamówień.'),
    t('Pozycje zamówienia'),
    t('Produkt'),
    t('Właściciel zamówienia'),
    t('Numer zamówienia'),
    t('Imię i nazwisko'),
    t('Nazwa'),
    t('Status zamówienia'),
    t('Orders with user names '),
    t('Kasjer - Zamówienia z imionami'),
    t('Telefon'),
    t('Quantities of products with user names'),
    t('Ważący - Zamówienia z imionami'),
    t('Ilość'),
    t('.'),
    t(','),
    t('Typ'),
    t('Quantities of products'),
    t('Ważący - Zamówienia'),
  );
  $export['wydruk_zamowien'] = $view;

  $view = new view();
  $view->name = 'zamowienie';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Zamówienie';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Zamówienie';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'więcej';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Zastosuj';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Przywróć';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Sortuj po';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Rosnąco';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Opis';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Elementów na stronę';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Wszystko -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Przesunięcie';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« pierwsza';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ poprzednia';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'następna ›';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Pole: Zamówienie Commerce: ID zamówienia */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  /* Pole: Zamówienie Commerce: Łączna wartość zamówienia */
  $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Pole: Zamówienie Commerce: Pozycje zamówienia (commerce_line_items:delta) */
  $handler->display->display_options['fields']['delta']['id'] = 'delta';
  $handler->display->display_options['fields']['delta']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['fields']['delta']['field'] = 'delta';
  /* Kryterium filtrowania: Zamówienie Commerce: Typ zamówienia */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'commerce_order' => 'commerce_order',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'zam-wienie';
  $translatables['zamowienie'] = array(
    t('Master'),
    t('Zamówienie'),
    t('więcej'),
    t('Zastosuj'),
    t('Przywróć'),
    t('Sortuj po'),
    t('Rosnąco'),
    t('Opis'),
    t('Elementów na stronę'),
    t('- Wszystko -'),
    t('Przesunięcie'),
    t('« pierwsza'),
    t('‹ poprzednia'),
    t('następna ›'),
    t('last »'),
    t('ID zamówienia'),
    t('Łączna wartość zamówienia'),
    t('Pozycje zamówienia (commerce_line_items:delta)'),
    t('.'),
    t(','),
    t('Page'),
  );
  $export['zamowienie'] = $view;

  return $export;
}
