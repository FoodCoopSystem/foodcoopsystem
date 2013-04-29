<?php


/**
 * Implements hook_rules_action_info().
 *
 * Provides an action to adjust stock level of a certain product
 * by a certain quantity.
 */
function wks_commerce_rules_action_info() {
  $actions = array();


  $actions['wks_commerce_increase_stock'] = array(
    'label' => t('Increase the product stock'),
    'group' => t('Commerce Stock'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item')
      ),
    ),
  );

  return $actions;
}


/**
 * If the line item is stock-enabled, add the sold amount in a line item
 * as when items are added to and removed from cart.
 */
function wks_commerce_increase_stock($line_item) {
  
// The product SKU that will have its stock level adjusted.
    $sku = $line_item->line_item_label;
    $product = commerce_product_load_by_sku($sku);

    $product_wrapper = entity_metadata_wrapper('commerce_product', $product);
    if (commerce_stock_product_type_enabled($product->type)) {
      if (!(commerce_stock_product_type_override_enabled($product->type)
          && isset($product_wrapper->commerce_stock_override) && $product_wrapper->commerce_stock_override->value() == 1)) {

        $qty = (int) $line_item->quantity;
        // Subtract the sold amount from the available stock level.
        wks_commerce_stock_adjust($product, $qty);
      }
    }
}
