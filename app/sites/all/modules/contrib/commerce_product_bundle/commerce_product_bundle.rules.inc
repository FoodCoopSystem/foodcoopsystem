<?php

/**
 * @file
 * Rules integration for product bundle.
 *
 * @ToDo Add a condition
 * 1) to check if a product is part of a bundle and
 * 2) to check if a product is a bundle.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_product_bundle_rules_event_info() {
  $events = array();

  $events['commerce_product_bundle_calc'] = array(
    'label' => t('Sub Product Price Calculation'),
    'group' => t('Commerce Product Bundle'),
    'variables' => array(
      'line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('sub line item'),
      ),
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_product_bundle_rules_action_info() {
  $actions = array();

  $actions['commerce_product_bundle_calculate_price'] = array(
    'label' => t('Calculate the price of a bundle product'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
      'multiply_by_quantity' => array(
        'type' => 'boolean',
        'label' => t('Multiply by Quantity'),
        'description' => t('Should the price be multiply by the added quantity of the sub product.'),
      ),
    ),
    'group' => t('Commerce Product Bundle'),
  );

  $actions['commerce_product_bundle_set_price'] = array(
    'label' => t('Set the price of a sub bundle.'),
    'parameter' => array(
      'line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Sub line item'),
      ),
      'amount' => array(
        'type' => 'decimal',
        'label' => t('Amount'),
      ),
    ),
    'group' => t('Commerce Product Bundle'),
  );

  $actions['commerce_product_bundle_load_sub_items'] = array(
    'label' => t('Load Sub Line Items.'),
    'parameter' => array(
      'line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Sub line item'),
      ),
    ),
    'provides' => array(
      'sub_line_items' => array(
        'type' => 'list<commerce_line_item>',
        'label' => t('sub line items'),
      ),
    ),
    'group' => t('Commerce Product Bundle'),
  );

  return $actions;
}

/**
 * Loads sub line items of a bundle product line item.
 *
 * @param obj $line_item
 *   The parent line item.
 *
 * @return array
 *   Array of all sub line item objects.
 */
function commerce_product_bundle_load_sub_items($line_item) {
  $sub_line_items = commerce_product_bundle_get_sub_line_items($line_item);
  return array('sub_line_items' => $sub_line_items);
}

/**
 * Calculates the price for a bundle line item.
 *
 * @param obj $line_item
 *   The parent line item object.
 * @param bool $multiply_by_quantity
 *   Indicates wether we should multiply the sub line items with the quantity
 *   of these items or not.
 */
function commerce_product_bundle_calculate_price($line_item, $multiply_by_quantity) {

  $original_line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  $original_line_item = $line_item;

  $unitprice = 0;

  $found_bundle = FALSE;

  $sub_items = commerce_product_bundle_get_sub_line_items($line_item);

  // If we do not find any sub line item. So we should try to find
  // the default sub products, to get a default price for this
  // product. This is primarly used for the product view, where we dont
  // get any line item with sub line items.
  if (count($sub_items) == 0) {

    $product = $original_line_item_wrapper->commerce_product;

    foreach (field_info_instances('commerce_product', $product->type->value()) as $product_field_name => $product_field) {
      $info = field_info_field($product_field_name);
      // If it is a product reference.
      if ($info['type'] == 'commerce_product_reference') {
        // Check if the reference is potential a bundle product:
        $form_in_full_node_display = (isset($product_field['display']['node_full'])
          && $product_field['display']['node_full']['type'] == 'commerce_bundle_product_add_to_cart_form');
        $form_in_default_display = (isset($product_field['display']['default'])
          && $product_field['display']['default']['type'] == 'commerce_bundle_product_add_to_cart_form');

        if ($form_in_full_node_display || ($form_in_default_display && !isset($product_field['display']['node_full']))) {

          // Get first sub product of this field, we assume that this is the
          // default product. (reset() or each() does not work, so use a foreach
          // loop.
          $product = $original_line_item_wrapper->commerce_product;
          foreach ($product->{$product_field_name} as $sub_product) {
            break;
          }

          // For single value product reference field, we got no
          // commerce_product object. This checks, if we have the correct
          // objects and create one if not.
          if ($sub_product->type() !== 'commerce_product' && is_numeric($sub_product->value())) {
            // Load the product.
            $product = commerce_product_load($sub_product->value());
            // Create commerce_product object instance.
            $sub_product = entity_metadata_wrapper('commerce_product', $product);
          }

          if ($form_in_full_node_display) {
            $settings = $product_field['display']['node_full']['settings'];
          }
          elseif ($form_in_default_display) {
            $settings = $product_field['display']['default']['settings'];
          }

          // Use the default value from the display settings to determine
          // the default quantity to use.
          $quantity = $settings['default_quantity'];

          if ($multiply_by_quantity) {
            $commerce_total = array();
            $commerce_total['amount'] = $quantity * $sub_product->commerce_price->amount->value();
            $commerce_total['currency_code'] = $sub_product->commerce_price->currency_code->value();

            // Add the components multiplied by the quantity to the data array.
            $data = $sub_product->commerce_price->data->value();

            if (empty($data['components'])) {
              $data['components'] = array();
            }
            else {
              foreach ($data['components'] as $key => $component) {
                $data['components'][$key]['price']['amount'] *= $quantity;
              }
            }

            // Set the updated data array to the total price.
            $commerce_total['data'] = $data;

            $component_total = $commerce_total;

          }
          else {
            $component_total['amount'] = $sub_product->commerce_price->amount->value();
            $component_total['currency_code'] = $sub_product->commerce_price->currency_code->value();
            $component_total['data'] = $sub_product->commerce_price->data->value();
            $component_total['data']['components'] += array();
          }

          $unitprice += commerce_currency_convert(
            $component_total['amount'],
            $component_total['currency_code'],
            $original_line_item_wrapper->commerce_unit_price->currency_code->value()
          );

          // Combine the line item total's component prices into the
          // unit price total.
          $original_line_item_wrapper->commerce_unit_price->data = commerce_price_components_combine(
            $original_line_item_wrapper->commerce_unit_price->value(),
            $component_total
          );
          $found_bundle = TRUE;

        }
      }
    }
  }

  // This is the normal way how we determine the bundle product price.
  else {
    // Reset the unit price to the product price.
    $original_line_item_wrapper->commerce_unit_price = $original_line_item_wrapper->commerce_product->commerce_price->value();

    foreach ($sub_items as $item) {
      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $item);

      if ($multiply_by_quantity) {
        $line_item_wrapper->commerce_total->amount = $item->quantity * $line_item_wrapper->commerce_unit_price->amount->value();
        $line_item_wrapper->commerce_total->currency_code = $line_item_wrapper->commerce_unit_price->currency_code->value();

        // Add the components multiplied by the quantity to the data array.
        $data = $line_item_wrapper->commerce_unit_price->data->value();

        if (empty($data['components'])) {
          $data['components'] = array();
        }
        else {
          foreach ($data['components'] as $key => $component) {
            $data['components'][$key]['price']['amount'] *= $item->quantity;
          }
        }

        // Set the updated data array to the total price.
        $line_item_wrapper->commerce_total->data = $data;

      }
      /*else {
        $component_total = commerce_price_component_total($line_item_wrapper->commerce_unit_price->value());
      }*/

      $line_item = $line_item_wrapper->value();
      rules_invoke_event('commerce_product_bundle_calc', $line_item, commerce_order_load($original_line_item->order_id));

      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

      $component_total = commerce_price_component_total($line_item_wrapper->commerce_total->value());

      $unitprice += commerce_currency_convert(
        $component_total['amount'],
        $component_total['currency_code'],
        $original_line_item_wrapper->commerce_unit_price->currency_code->value()
      );

      // Combine the line item total's component prices into the unit
      // price total.
      $original_line_item_wrapper->commerce_unit_price->data = commerce_price_components_combine(
        $original_line_item_wrapper->commerce_unit_price->value(),
        $line_item_wrapper->commerce_total->value()
      );
      $found_bundle = TRUE;
    }
  }

  if ($found_bundle) {
    $original_line_item_wrapper->commerce_unit_price->amount = $unitprice;
  }
}

/**
 * Sets the price of a line item.
 *
 * @param obj $line_item
 *   The line item object of which we set the price.
 * @param float $amount
 *   The value of the new price.
 */
function commerce_product_bundle_set_price($line_item, $amount) {
  if (is_numeric($amount)) {
    $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

    // Calculate the updated amount and create a price array representing the
    // difference between it and the current amount.
    $current_amount = $wrapper->commerce_unit_price->amount->value();
    $updated_amount = $amount;

    $wrapper->commerce_unit_price->data = commerce_price_component_delete(
      $wrapper->commerce_unit_price->value(),
      'base_price'
    );

    $difference = array(
      'amount' => $updated_amount,
      'currency_code' => $wrapper->commerce_unit_price->currency_code->value(),
      'data' => array(),
    );

    // Set the amount of the unit price and add the difference as a component.
    $wrapper->commerce_unit_price->amount = $updated_amount;

    $wrapper->commerce_unit_price->data = array();

    $wrapper->commerce_unit_price->data = commerce_price_component_add(
      $wrapper->commerce_unit_price->value(),
      'base_price',
      $difference,
      TRUE
    );

  }

}

/**
 * @}
 */
