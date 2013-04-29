<?php
/**
 * @file
 * Functions for batch processing tax reports.
 */
 
/**
 * Build the table of tax information.
 */
function _commerce_reports_tax_generate(&$context) {
  if (empty($context['sandbox'])) {
    $context['results'] = array(
      'orders' => array()
    );
  
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current'] = 0;
    $context['sandbox']['max'] = db_query("SELECT COUNT(order_id) FROM {commerce_order} WHERE type = 'commerce_order' AND status IN (:order_statuses)ORDER BY order_id ASC", array(':order_statuses' => commerce_reports_tax_order_statuses()))->fetchColumn();
  }
    
  $pending_orders = db_query("SELECT co.order_id AS order_id, COUNT(crt.tax_rate) AS should_update FROM {commerce_order} co LEFT JOIN {commerce_reports_tax} crt ON co.order_id = crt.order_id WHERE co.type = 'commerce_order' AND co.status IN (:order_statuses) AND co.order_id > :current_order_id GROUP BY crt.order_id ORDER BY co.order_id ASC LIMIT 10", array(':order_statuses' => commerce_reports_tax_order_statuses(), ':current_order_id' => $context['sandbox']['current']))->fetchAllAssoc('order_id', PDO::FETCH_ASSOC);
    
  if ($pending_orders) {
    $orders = entity_load('commerce_order', array_keys($pending_orders));

    // Iterate the orders and break each order down into its components.
    foreach ($orders as $order) {
      if ($pending_orders[$order->order_id]['should_update']) {
        $update = TRUE;
      }
      else {
        $update = FALSE;
      }
      
      commerce_reports_tax_order_save($order, $update);
      
      $context['results']['orders'][] = $order->order_id;
      $context['sandbox']['progress']++;
      $context['sandbox']['current'] = $order->order_id;
    }
  }
  
  // Report back to the user.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Finishes the batch process and stores any remaining information.
 */
function _commerce_reports_tax_finished($success, $results, $operations) {
  if ($success) {
  	variable_set('commerce_reports_tax_batch_finished', REQUEST_TIME);
  	
    drupal_set_message(t('Successfully generated tax report.'));
  }
  else {
    drupal_set_message(t('There was a problem whilst generating the tax report. The information might be incomplete or erroneous.'), 'error');
  }
}
