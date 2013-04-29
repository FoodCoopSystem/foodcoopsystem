<?php
/**
 * @file
 * Rules hooks, callbacks and supporting functions for Commerce Product
 * Popularity.
 *
 * @todo Check if an access callback's needed for the action.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_productpopularity_rules_action_info() {
  $actions['commerce_productpopularity_process_radioactivity'] = array(
    'label' => t("Process Radioactivity fields"),
    'group' => t("Commerce Product Popularity"),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t("Order"),
      ),
      'energy' => array(
        'type' => 'integer',
        'label' => t("Energy"),
        'description' => t("Enter the amount of energy that should be added to the Radioactivity field per unit sold. Example: If you configure a value of 10, then purchasing 3 units of a product will increase its energy by 30."),
      ),
    ),
  );
  
  return $actions;
}

/**
 * Rules action callback: Updates any products and product displays from the
 * order if they have an appropriately configured radioactivity field.
 */
function commerce_productpopularity_process_radioactivity($commerce_order, $energy) {

  $order = entity_metadata_wrapper('commerce_order', $commerce_order);
  
  // Get array of radioactivity fields configured to work with this module.
  $active_instances = commerce_productpopularity_get_active_instances();
  
  foreach ($order->commerce_line_items as $line_item) {
    // Check this line item's associated with a product (not eg. shipping).
    if (isset($line_item->commerce_product)) {
    
      $quantity = $line_item->quantity->value();
      $extra_energy = $energy * $quantity;
      $product_type = $line_item->commerce_product->type->value();
      
      // Check if the product has an active field attached.
      if (!empty($active_instances['commerce_product'][$product_type])) {
        $product_id = $line_item->commerce_product->product_id->value();
        foreach ($active_instances['commerce_product'][$product_type] as $field_name) {
          commerce_productpopularity_add_incident('commerce_product', $product_type, $field_name, $product_id, $extra_energy);
        }
      }
      
      // Check if the product has Product Display details.
      $data = $line_item->value()->data;
      if (!empty($data['commerce_productpopularity'])) {
      
        // Double check the field's active in case it's changed since the line
        // item was created.
        $entity_type = $data['commerce_productpopularity']['entity_type'];
        $bundle = $data['commerce_productpopularity']['bundle'];
        if (!empty($active_instances[$entity_type][$bundle])) {
        
          $entity_id = $data['commerce_productpopularity']['entity_id'];
          
          foreach ($active_instances[$entity_type][$bundle] as $field_name) {
            commerce_productpopularity_add_incident($entity_type, $bundle, $field_name, $entity_id, $extra_energy);
          }
        }
      }
    }
  }
  
  // @todo This is only necessary for the live storage engine IIUC: Add check to
  //   only call when necessary.
  field_cache_clear();
}

/**
 * Registers a new Radioactivity incident with the details passed. If this is
 * used with the live field storage then field_cache_clear() should be called
 * afterwards.
 * 
 * @see radioactivity_rules_action_emit()
 */
function commerce_productpopularity_add_incident($entity_type, $bundle, $field_name, $entity_id, $energy) {
  if ($energy) {
  
    // Get the language.
    $info = field_info_field($field_name);
    if ($info['translatable']) {
      global $language;
      $language = $language->language;
    }
    else {
      $language = LANGUAGE_NONE;
    }
    
    // Add the incident.
    $profile = radioactivity_get_field_profile($entity_type, $bundle, $field_name);
    if ($profile && $profile->storageObject) {
      $profile->storageObject->addIncident(new RadioactivityIncident($entity_type, $bundle, $field_name, $language, $entity_id, $energy, time()));
    }
  }
}
