<?php

/**
 * @file
 * Contains Drupal Commerce specific hook implementations.
 * Included automatically.
 */

/**
 * Implements hook_commerce_product_type_update().
 */
function commerce_autosku_commerce_product_type_update($type) {
  $existing = db_query("SELECT 1 FROM {commerce_autosku_patterns} WHERE product_type = :type", array(':type' => $type['type']))->fetch();
  $record = new stdClass();
  $record->product_type = $type['type'];
  $record->pattern = $type['autosku_status'] ? $type['autosku']['pattern'] : FALSE;
  $record->advanced = $type['autosku']['advanced'];
  if ($existing) {
    drupal_write_record('commerce_autosku_patterns', $record, 'product_type');
  }
  else {
    drupal_write_record('commerce_autosku_patterns', $record);
  }
}

/**
 * Implements hook_commerce_product_type_insert().
 */
function commerce_autosku_commerce_product_type_insert($type) {
  $record = new stdClass();
  $record->product_type = $type['type'];
  $record->pattern = !empty($type['autosku_status']) ? $type['autosku']['pattern'] : FALSE;
  $record->advanced = isset($type['autosku']['advanced']) ? $type['autosku']['advanced'] : array();

  drupal_write_record('commerce_autosku_patterns', $record);
}

/**
 * Implements hook_commerce_product_type_delete().
 */
function commerce_autosku_commerce_product_type_delete($type) {
  db_query("DELETE FROM {commerce_autosku_patterns} WHERE product_type = :type", array(':type' => $type['type']));
}

/**
 * Implements hook_commerce_product_insert().
 */
function commerce_autosku_commerce_product_insert($product) {
  if (_commerce_autosku_is_temporary_sku($product->sku)) {
    unset($product->is_new);
    //Trigger immediate resave now that we have the ID.
    commerce_product_save($product);
    //Reset is_new in case anyone needs it after us.
    $product->is_new = TRUE;
  }
}

/**
 * Implements hook_commerce_product_presave().
 */
function commerce_autosku_commerce_product_presave($product) {
  $settings = commerce_autosku_get_settings($product);

  if ($settings) {
    if (_commerce_autosku_needs_temporary_sku($product, $settings)) {
      $product->sku = _commerce_autosku_get_temporary_sku();
    }
    //@todo: Replace TRUE with a check if we should update existing SKUS.
    elseif (empty($product->sku) || _commerce_autosku_is_temporary_sku($product->sku) || $settings['advanced']['update_existing']) {
      $data = array('commerce-product' => $product);
      $sku = token_replace($settings['pattern'], $data);
      $sku = commerce_autosku_cleanstring($sku);
      if ($settings['advanced']['case']) {
        switch ($settings['advanced']['case']) {
          case 'uppercase':
            $sku = drupal_strtoupper($sku);
            break;
          case 'lowercase':
            $sku = drupal_strtolower($sku);
        }
      }

      //Borrowed from pathauto...
      if (!commerce_product_validate_sku_unique($sku, $product->product_id)) {
        $original_sku = $sku;
        $i = 0;

        do {
          $sku = commerce_autosku_cleanstring($original_sku . COMMERCE_AUTOSKU_REPLACEMENT . $i);
          $i++;
        } while (!commerce_product_validate_sku_unique($sku, $product->product_id));
      }
      $product->sku = $sku;
    }
  }
}

/**
 * Implements hook_commerce_product_type_info_alter().
 */
function commerce_autosku_commerce_product_type_info_alter(&$types) {
  $patterns = array();

  if (!empty($types)) {
    //Fetch all the records at once before merging into the types array
    $result = db_query("SELECT product_type, pattern, advanced FROM {commerce_autosku_patterns} WHERE product_type IN (:types)", array(':types' => array_keys($types)), array('fetch' => PDO::FETCH_ASSOC));

    foreach ($result as $record) {
      $record['advanced'] = unserialize($record['advanced']);
      if (!is_array($record['advanced'])) {
        $record['advanced'] = _commerce_autosku_default_settings();
      }
      $patterns[$record['product_type']] = $record;
    }
  }

  foreach ($types as $name => &$type) {
    if (!empty($patterns[$name])) {
      $type['autosku'] = $patterns[$name];
    }
    //Modules implementing hook_product_type_info() can
    //declare autosku patterns in code if they wish.
    elseif (empty($type['autosku'])) {
      $type['autosku'] = FALSE;
    }
  }
}