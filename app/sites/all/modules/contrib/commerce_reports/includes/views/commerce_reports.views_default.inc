<?php
/**
 * @file
 * Default views.
 */

/**
 * Views for commerce orders for report displays.
 */

function commerce_reports_visualization_display($view, $name, $title, $type = 'block', $base_display = 'default') {
  // Retrieve our base display
  $default = $view->display[$base_display];

  // Add the display
  $handler = $view->new_display($type, $title, $name);

  // Disable the pager (show all items)
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';

  // Setup the custom style plugin
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'visualization';

  // Set some general settings concerning fields
  $handler->display->display_options['defaults']['fields'] = FALSE;
  $handler->display->display_options['fields'] = $default->display_options['fields'];
  $handler->display->display_options['style_options']['info'] = $default->display_options['style_options']['info'];

  foreach ($handler->display->display_options['style_options']['info'] as &$field) {
    $field['separator'] = '';
  }

  foreach ($handler->display->display_options['fields'] as $name => &$field) {
    // All fields that are passed to the charting module should have at least a value.
    $field['hide_alter_empty'] = 1;
    $field['empty'] = '0';

    // Get rid of those nasty currency fields. We need raw data!
    if ((!empty($field['type'])) && strstr($field['type'], 'commerce_price')) {
      $field['type'] = 'commerce_reports_visualization';
    }
  }

  return $handler;
}

function commerce_reports_data_export($view) {
  if (empty($view->display['page']) || empty($view->display['page']->display_options['path'])) {
    return;
  }

  $handler = $view->new_display('views_data_export', 'Export to CSV', 'views_data_export');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = $view->display['page']->display_options['path'] . '/csv';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
  );
}

/**
 * Implements hook_views_default_views().
 */
function commerce_reports_views_default_views() {
  /**
   * Commerce reports: Products
   */
  $view = new view();
  $view->name = 'commerce_reports_products';
  $view->description = '';
  $view->tag = 'commerce_reports';
  $view->base_table = 'commerce_line_item';
  $view->human_name = 'Product reports';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Product reports';
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access commerce reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'sku' => 'sku',
    'title' => 'title',
    'quantity' => 'quantity',
    'commerce_total' => 'commerce_total',
  );
  $handler->display->display_options['style_options']['default'] = 'commerce_total';
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
    'quantity' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
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
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: Commerce Line Item: Order ID */
  $handler->display->display_options['relationships']['order_id']['id'] = 'order_id';
  $handler->display->display_options['relationships']['order_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['relationships']['order_id']['field'] = 'order_id';
  $handler->display->display_options['relationships']['order_id']['required'] = 1;
  /* Relationship: Commerce Line item: Referenced product */
  $handler->display->display_options['relationships']['commerce_product_product_id']['id'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['table'] = 'field_data_commerce_product';
  $handler->display->display_options['relationships']['commerce_product_product_id']['field'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['required'] = 1;
  /* Field: Commerce Product: SKU */
  $handler->display->display_options['fields']['sku']['id'] = 'sku';
  $handler->display->display_options['fields']['sku']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['sku']['field'] = 'sku';
  $handler->display->display_options['fields']['sku']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['sku']['label'] = 'Product';
  $handler->display->display_options['fields']['sku']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['external'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['sku']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['sku']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['html'] = 0;
  $handler->display->display_options['fields']['sku']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['sku']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['sku']['hide_empty'] = 0;
  $handler->display->display_options['fields']['sku']['empty_zero'] = 0;
  $handler->display->display_options['fields']['sku']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['sku']['link_to_product'] = 1;
  /* Field: Commerce Product: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_product'] = 0;
  /* Field: SUM(Commerce Line Item: Quantity) */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['group_type'] = 'sum';
  $handler->display->display_options['fields']['quantity']['label'] = 'Sold';
  $handler->display->display_options['fields']['quantity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['quantity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['quantity']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['quantity']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['quantity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['quantity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['quantity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['quantity']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['quantity']['set_precision'] = 0;
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  $handler->display->display_options['fields']['quantity']['format_plural'] = 0;
  /* Field: SUM(Commerce Line item: Total) */
  $handler->display->display_options['fields']['commerce_total']['id'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['table'] = 'field_data_commerce_total';
  $handler->display->display_options['fields']['commerce_total']['field'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['group_type'] = 'sum';
  $handler->display->display_options['fields']['commerce_total']['label'] = 'Revenue';
  $handler->display->display_options['fields']['commerce_total']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['external'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['commerce_total']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['commerce_total']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['html'] = 0;
  $handler->display->display_options['fields']['commerce_total']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['commerce_total']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['commerce_total']['hide_empty'] = 0;
  $handler->display->display_options['fields']['commerce_total']['empty_zero'] = 0;
  $handler->display->display_options['fields']['commerce_total']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['commerce_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_total']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_total']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_total']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_total']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );
  $handler->display->display_options['fields']['commerce_total']['field_api_classes'] = 0;
  /* Sort criterion: COUNT(Commerce Line Item: Line item ID) */
  $handler->display->display_options['sorts']['line_item_id']['id'] = 'line_item_id';
  $handler->display->display_options['sorts']['line_item_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['sorts']['line_item_id']['field'] = 'line_item_id';
  $handler->display->display_options['sorts']['line_item_id']['group_type'] = 'count';
  $handler->display->display_options['sorts']['line_item_id']['order'] = 'DESC';
  /* Filter criterion: Commerce Line Item: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'product' => 'product',
  );
  /* Filter criterion: Commerce Order: Order state */
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
  $handler->display->display_options['path'] = 'admin/commerce/reports/products';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Products';
  $handler->display->display_options['menu']['description'] = 'View product reports.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Commerce Line Item: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'product' => 'product',
  );
  /* Filter criterion: Commerce Order: Order state */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['relationship'] = 'order_id';
  $handler->display->display_options['filters']['state']['value'] = array(
    'pending' => 'pending',
    'completed' => 'completed',
  );
  /* Filter criterion: Commerce Product: SKU */
  $handler->display->display_options['filters']['sku']['id'] = 'sku';
  $handler->display->display_options['filters']['sku']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['sku']['field'] = 'sku';
  $handler->display->display_options['filters']['sku']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['filters']['sku']['operator'] = 'word';
  $handler->display->display_options['filters']['sku']['exposed'] = TRUE;
  $handler->display->display_options['filters']['sku']['expose']['operator_id'] = 'sku_op';
  $handler->display->display_options['filters']['sku']['expose']['label'] = 'Search products (SKU)';
  $handler->display->display_options['filters']['sku']['expose']['operator'] = 'sku_op';
  $handler->display->display_options['filters']['sku']['expose']['identifier'] = 'sku';
  $handler->display->display_options['filters']['sku']['expose']['required'] = 0;
  $handler->display->display_options['filters']['sku']['expose']['multiple'] = FALSE;
  $handler->display->display_options['path'] = 'admin/commerce/reports/products';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Products';
  $handler->display->display_options['menu']['description'] = 'View product reports.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  /**
   * Integration with commerce_reports_visualization.
   */
  /**
   * Top products by revenue.
   */
  $handler = commerce_reports_visualization_display($view, 'chart_revenue', 'Chart (by revenue)');

  // Chart settings
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['style_options']['type'] = 'column';
  $handler->display->display_options['style_options']['fields'] = array(
    'commerce_total' => array(
      'enabled' => TRUE,
      'sort' => 'DESC',
    ),
  );
  $handler->display->display_options['style_options']['yAxis'] = array(
    'title' => 'Amount',
  );
  $handler->display->display_options['style_options']['xAxis'] = array(
    'labelField' => 'sku',
    'invert' => 0,
  );

  // Only render 5 products
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';

  /**
   * Top products by quantity.
   */
  $handler = commerce_reports_visualization_display($view, 'chart_quantity', 'Chart (by quantity sold)', 'block', 'chart_revenue');

  // Chart settings
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['style_options']['type'] = 'column';
  $handler->display->display_options['style_options']['fields'] = array(
    'quantity' => array(
      'enabled' => TRUE,
      'sort' => 'DESC',
    ),
  );
  $handler->display->display_options['style_options']['yAxis'] = array(
    'title' => 'Quantity',
  );
  $handler->display->display_options['style_options']['xAxis'] = array(
    'labelField' => 'sku',
    'invert' => 0,
  );

  // Only render 5 products
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';

  /**
   * Integration with views data export.
   */
  if (module_exists('views_data_export')) {
    commerce_reports_data_export($view);
  }

  $views[$view->name] = $view;

  /**
   * Commerce reports: Customers
   */
  $view = new view();
  $view->name = 'commerce_reports_customers';
  $view->description = '';
  $view->tag = 'commerce_reports';
  $view->base_table = 'users';
  $view->human_name = 'Customer reports';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Customer reports';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access commerce reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'order_id' => 'order_id',
    'commerce_order_total_1' => 'commerce_order_total_1',
    'commerce_order_total' => 'commerce_order_total',
  );
  $handler->display->display_options['style_options']['default'] = 'commerce_order_total_1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_order_total_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_order_total' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: User: Orders */
  $handler->display->display_options['relationships']['uid_commerce_orders']['id'] = 'uid_commerce_orders';
  $handler->display->display_options['relationships']['uid_commerce_orders']['table'] = 'users';
  $handler->display->display_options['relationships']['uid_commerce_orders']['field'] = 'uid_commerce_orders';
  $handler->display->display_options['relationships']['uid_commerce_orders']['required'] = TRUE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Customer';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: COUNT(Commerce Order: Order ID) */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['fields']['order_id']['label'] = 'Orders';
  $handler->display->display_options['fields']['order_id']['group_type'] = 'count';
  /* Field: SUM(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total_1']['id'] = 'commerce_order_total_1';
  $handler->display->display_options['fields']['commerce_order_total_1']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_type'] = 'sum';
  $handler->display->display_options['fields']['commerce_order_total_1']['label'] = 'Total';
  $handler->display->display_options['fields']['commerce_order_total_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total_1']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total_1']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );
  /* Field: AVG(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['fields']['commerce_order_total']['group_type'] = 'avg';
  $handler->display->display_options['fields']['commerce_order_total']['label'] = 'Average';
  $handler->display->display_options['fields']['commerce_order_total']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_order_total']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_order_total']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );
  $handler->display->display_options['fields']['commerce_order_total']['field_api_classes'] = TRUE;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['filters']['status_1']['value'] = array(
    'pending' => 'pending',
    'completed' => 'completed',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['filters']['status_1']['value'] = array(
    'pending' => 'pending',
    'completed' => 'completed',
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Search customers';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['reduce'] = 0;
  $handler->display->display_options['path'] = 'admin/commerce/reports/customers';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Customers';
  $handler->display->display_options['menu']['description'] = 'View customer reports.';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  /**
   * Integration with commerce_reports_visualization.
   */
  /**
   * Top users by revenue.
   */
  $handler = commerce_reports_visualization_display($view, 'chart_revenue', 'Chart (by revenue)');

  // Chart settings
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['style_options']['type'] = 'pie';
  $handler->display->display_options['style_options']['fields'] = array(
    'commerce_order_total_1' => array(
      'enabled' => TRUE,
      'sort' => 'DESC',
    ),
  );

  $handler->display->display_options['style_options']['xAxis'] = array(
    'labelField' => 'name',
    'invert' => 0,
  );

  // Only render 10 customers
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';

  /**
   * Top users by amount of orders.
   */
  $handler = commerce_reports_visualization_display($view, 'chart_orders', 'Chart (by orders)', 'block', 'chart_revenue');

  // Chart settings
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['style_options']['type'] = 'pie';
  $handler->display->display_options['style_options']['fields'] = array(
    'order_id' => array(
      'enabled' => TRUE,
      'sort' => 'DESC',
    ),
  );

  $handler->display->display_options['style_options']['xAxis'] = array(
    'labelField' => 'name',
    'invert' => 0,
  );

  /**
   * Integration with views data export.
   */
  if (module_exists('views_data_export')) {
    commerce_reports_data_export($view);
  }

  $views[$view->name] = $view;

  /**
   * Commerce reports: Payment methods
   */
  $view = new view();
  $view->name = 'commerce_reports_payment_methods';
  $view->description = 'View payment methods usage breakdown.';
  $view->tag = 'commerce_reports';
  $view->base_table = 'commerce_payment_transaction';
  $view->human_name = 'Payment gateway reports';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Payment methods';
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access commerce reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'payment_method' => 'payment_method',
    'transaction_id' => 'transaction_id',
    'amount' => 'amount',
  );
  $handler->display->display_options['style_options']['default'] = 'amount';
  $handler->display->display_options['style_options']['info'] = array(
    'payment_method' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'transaction_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Field: Commerce Payment Transaction: Payment method */
  $handler->display->display_options['fields']['payment_method']['id'] = 'payment_method';
  $handler->display->display_options['fields']['payment_method']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['fields']['payment_method']['field'] = 'payment_method';
  $handler->display->display_options['fields']['payment_method']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['payment_method']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['payment_method']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['payment_method']['alter']['external'] = 0;
  $handler->display->display_options['fields']['payment_method']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['payment_method']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['payment_method']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['payment_method']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['payment_method']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['payment_method']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['payment_method']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['payment_method']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['payment_method']['alter']['html'] = 0;
  $handler->display->display_options['fields']['payment_method']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['payment_method']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['payment_method']['hide_empty'] = 0;
  $handler->display->display_options['fields']['payment_method']['empty_zero'] = 0;
  $handler->display->display_options['fields']['payment_method']['hide_alter_empty'] = 1;
  /* Field: COUNT(Commerce Payment Transaction: Transaction ID) */
  $handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['transaction_id']['label'] = 'Transactions';
  $handler->display->display_options['fields']['transaction_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['transaction_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['transaction_id']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['transaction_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['transaction_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['transaction_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['transaction_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['transaction_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['transaction_id']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['transaction_id']['format_plural'] = 0;
  /* Field: SUM(Commerce Payment Transaction: Amount) */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['group_type'] = 'sum';
  $handler->display->display_options['fields']['amount']['label'] = 'Revenue';
  $handler->display->display_options['fields']['amount']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['external'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['amount']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['amount']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['html'] = 0;
  $handler->display->display_options['fields']['amount']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['amount']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['amount']['hide_empty'] = 0;
  $handler->display->display_options['fields']['amount']['empty_zero'] = 0;
  $handler->display->display_options['fields']['amount']['hide_alter_empty'] = 1;
  /* Filter criterion: Commerce Payment Transaction: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    'success' => 'success',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/reports/payment-methods';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Payment methods';
  $handler->display->display_options['menu']['description'] = 'View breakdown of payment gateway usage.';
  $handler->display->display_options['menu']['weight'] = '20';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  /**
   * Integration with views data export.
   */
  if (module_exists('views_data_export')) {
    commerce_reports_data_export($view);
  }

  $views[$view->name] = $view;

  /**
   * Commerce reports: Sales reports
   */
  $view = new view();
  $view->name = 'commerce_reports_sales';
  $view->description = 'View commerce daily and monthly sales totals.';
  $view->tag = 'commerce_reports';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Sales reports';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sales report';
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access commerce reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Refresh report';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = 0;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = 1;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'sales_data' => 'sales_data',
    'order_id' => 'order_id',
    'commerce_order_total' => 'commerce_order_total',
    'commerce_order_total_1' => 'commerce_order_total_1',
  );
  $handler->display->display_options['style_options']['default'] = 'sales_data';
  $handler->display->display_options['style_options']['info'] = array(
    'sales_data' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_order_total' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_order_total_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: MIN(Commerce Order: Sales data) */
  $handler->display->display_options['fields']['sales_data']['id'] = 'sales_data';
  $handler->display->display_options['fields']['sales_data']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['sales_data']['field'] = 'sales_data';
  $handler->display->display_options['fields']['sales_data']['group_type'] = 'min';
  $handler->display->display_options['fields']['sales_data']['label'] = 'Date';
  $handler->display->display_options['fields']['sales_data']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['sales_data']['exposed'] = 1;
  $handler->display->display_options['fields']['sales_data']['default_period'] = '31556926';
  /* Field: COUNT(Commerce Order: Order ID) */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['order_id']['label'] = 'Number of Orders';
  /* Field: SUM(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['group_type'] = 'sum';
  $handler->display->display_options['fields']['commerce_order_total']['label'] = 'Total Revenue';
  $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );
  /* Field: AVG(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total_1']['id'] = 'commerce_order_total_1';
  $handler->display->display_options['fields']['commerce_order_total_1']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_type'] = 'avg';
  $handler->display->display_options['fields']['commerce_order_total_1']['label'] = 'Average Order';
  $handler->display->display_options['fields']['commerce_order_total_1']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total_1']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );
  /* Filter criterion: Commerce Order: Order state */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['value'] = array(
    'pending' => 'pending',
    'completed' => 'completed',
  );
  $handler->display->display_options['filters']['state']['exposed'] = TRUE;
  $handler->display->display_options['filters']['state']['expose']['operator_id'] = 'state_op';
  $handler->display->display_options['filters']['state']['expose']['label'] = 'Order statuses';
  $handler->display->display_options['filters']['state']['expose']['operator'] = 'state_op';
  $handler->display->display_options['filters']['state']['expose']['identifier'] = 'state';
  $handler->display->display_options['filters']['state']['expose']['multiple'] = TRUE;

  /* Display: Sales Reports */
  $handler = $view->new_display('page', 'Sales reports', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'sales_data' => 'sales_data',
    'order_id' => 'order_id',
    'commerce_order_total' => 'commerce_order_total',
    'commerce_order_total_1' => 'commerce_order_total_1',
  );
  $handler->display->display_options['style_options']['default'] = 'sales_data';
  $handler->display->display_options['style_options']['info'] = array(
    'sales_data' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_order_total' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_order_total_1' => array(
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
  $handler->display->display_options['path'] = 'admin/commerce/reports/sales';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Sales';
  $handler->display->display_options['menu']['description'] = 'View sales reports.';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Sales totals';
  $handler->display->display_options['tab_options']['description'] = 'View daily and monthly sales totals.';
  $handler->display->display_options['tab_options']['weight'] = '-10';
  $handler->display->display_options['tab_options']['name'] = 'management';

  /**
   * Integration with commerce_reports_visualization.
   */
  /**
   * 12 month chart.
   */
  $handler = commerce_reports_visualization_display($view, 'chart_year', 'Chart (one year)');

  // Chart settings
  $handler->display->display_options['defaults']['style_options'] = FALSE;

  $handler->display->display_options['style_options']['type'] = 'line';

  $handler->display->display_options['style_options']['fields'] = array(
    'sales_data' => array(
      'enabled' => FALSE,
      'sort' => 'ASC',
    ),
    'commerce_order_total' => array(
      'enabled' => TRUE,
      'sort' => '0',
    ),
    'commerce_order_total_1' => array(
      'enabled' => TRUE,
      'sort' => '0',
    ),
  );
  $handler->display->display_options['style_options']['yAxis'] = array(
    'title' => 'Amount',
  );
  $handler->display->display_options['style_options']['xAxis'] = array(
    'labelField' => 'sales_data',
    'invert' => 0,
  );

  // Date field: set period to one year and group monthly
  $handler->display->display_options['defaults']['fields'] = FALSE;

  $handler->display->display_options['fields']['sales_data']['default_period'] = '31556926';
  $handler->display->display_options['fields']['sales_data']['default_granularity'] = 'Y-m';

  /**
   * 1 month chart.
   */
  $handler = commerce_reports_visualization_display($view, 'chart_month', 'Chart (one month)', 'block', 'chart_year');

  $handler->display->display_options['defaults']['style_options'] = FALSE;

  $handler->display->display_options['style_options']['type'] = 'line';

  $handler->display->display_options['style_options']['fields'] = array(
    'sales_data' => array(
      'enabled' => FALSE,
      'sort' => 'ASC',
    ),
    'commerce_order_total' => array(
      'enabled' => TRUE,
      'sort' => '0',
    ),
    'commerce_order_total_1' => array(
      'enabled' => TRUE,
      'sort' => '0',
    ),
  );
  $handler->display->display_options['style_options']['yAxis'] = array(
    'title' => 'Amount',
  );
  $handler->display->display_options['style_options']['xAxis'] = array(
    'labelField' => 'sales_data',
    'invert' => 0,
  );

  $handler->display->display_options['defaults']['fields'] = FALSE;

  $handler->display->display_options['fields']['sales_data']['default_period'] = '2592000';
  $handler->display->display_options['fields']['sales_data']['default_granularity'] = 'Y-m-d';

  /**
   * 1 week chart.
   */
  $handler = commerce_reports_visualization_display($view, 'chart_week', 'Chart (one week)', 'block', 'chart_year');

  $handler->display->display_options['defaults']['style_options'] = FALSE;

  $handler->display->display_options['style_options']['type'] = 'line';

  $handler->display->display_options['style_options']['fields'] = array(
    'sales_data' => array(
      'enabled' => FALSE,
      'sort' => 'ASC',
    ),
    'commerce_order_total' => array(
      'enabled' => TRUE,
      'sort' => '0',
    ),
    'commerce_order_total_1' => array(
      'enabled' => TRUE,
      'sort' => '0',
    ),
  );
  $handler->display->display_options['style_options']['yAxis'] = array(
    'title' => 'Amount',
  );
  $handler->display->display_options['style_options']['xAxis'] = array(
    'labelField' => 'sales_data',
    'invert' => 0,
  );

  $handler->display->display_options['defaults']['fields'] = FALSE;

  $handler->display->display_options['fields']['sales_data']['default_period'] = '604800';
  $handler->display->display_options['fields']['sales_data']['default_granularity'] = 'Y-m-d';

  /**
   * Integration with views data export.
   */
  if (module_exists('views_data_export')) {
    commerce_reports_data_export($view);
  }

  $views[$view->name] = $view;

  /**
   * Commerce reports: Sales overview
   */
  $view = new view();
  $view->name = 'commerce_reports_sales_overview';
  $view->description = '';
  $view->tag = 'commerce_reports';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Sales reports overview';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'order_id' => 'order_id',
    'commerce_order_total' => 'commerce_order_total',
    'commerce_order_total_1' => 'commerce_order_total_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'order_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_order_total' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_order_total_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty text';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No orders.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Field: COUNT(Commerce Order: Order ID) */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['order_id']['label'] = 'Sales';
  $handler->display->display_options['fields']['order_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['order_id']['empty'] = 'None';
  $handler->display->display_options['fields']['order_id']['empty_zero'] = TRUE;
  /* Field: SUM(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['group_type'] = 'sum';
  $handler->display->display_options['fields']['commerce_order_total']['label'] = 'Gross';
  $handler->display->display_options['fields']['commerce_order_total']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total']['empty'] = 'None';
  $handler->display->display_options['fields']['commerce_order_total']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );
  /* Field: AVG(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total_1']['id'] = 'commerce_order_total_1';
  $handler->display->display_options['fields']['commerce_order_total_1']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_type'] = 'avg';
  $handler->display->display_options['fields']['commerce_order_total_1']['label'] = 'Average';
  $handler->display->display_options['fields']['commerce_order_total_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total_1']['empty'] = 'None';
  $handler->display->display_options['fields']['commerce_order_total_1']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['commerce_order_total_1']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total_1']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    'pending' => 'pending',
    'completed' => 'completed',
  );

  /* Display: Today */
  $handler = $view->new_display('block', 'Today', 'today');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Today';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    'pending' => 'pending',
    'completed' => 'completed',
  );
  /* Filter criterion: Date: Date (commerce_order) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>';
  $handler->display->display_options['filters']['date_filter']['expose']['operator_id'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Date (commerce_order)';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['granularity'] = 'second';
  $handler->display->display_options['filters']['date_filter']['default_date'] = 'today';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'commerce_order.created' => 'commerce_order.created',
  );
  $handler->display->display_options['block_description'] = 'Sales Reports: Overview (Today)';

  /* Display: Yesterday */
  $handler = $view->new_display('block', 'Yesterday', 'yesterday');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Yesterday';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    'pending' => 'pending',
    'completed' => 'completed',
  );
  /* Filter criterion: Date: Date (commerce_order) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = 'between';
  $handler->display->display_options['filters']['date_filter']['expose']['operator_id'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Date (commerce_order)';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['granularity'] = 'second';
  $handler->display->display_options['filters']['date_filter']['default_date'] = 'yesterday';
  $handler->display->display_options['filters']['date_filter']['default_to_date'] = 'today';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'commerce_order.created' => 'commerce_order.created',
  );
  $handler->display->display_options['block_description'] = 'Sales Reports: Overview (Yesterday)';

  /* Display: Month */
  $handler = $view->new_display('block', 'Month', 'month');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Month-to-date';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    'pending' => 'pending',
    'completed' => 'completed',
  );
  /* Filter criterion: Commerce Order: Created date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['operator'] = '>=';
  $handler->display->display_options['filters']['created']['value']['value'] = '-30 day';
  $handler->display->display_options['filters']['created']['value']['type'] = 'offset';
  $handler->display->display_options['block_description'] = 'Sales Reports: Overview (Month)';

  $views[$view->name] = $view;

  return $views;
}
