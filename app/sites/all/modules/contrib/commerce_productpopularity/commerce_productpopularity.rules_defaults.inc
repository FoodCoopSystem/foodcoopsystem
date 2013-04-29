<?php
/**
 * @file
 * commerce_productpopularity.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_productpopularity_default_rules_configuration() {
  $items = array();
  $items['rules_commerce_productpopularity'] = entity_import('rules_config', '{ "rules_commerce_productpopularity" : {
      "LABEL" : "Commerce Product Popularity",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "commerce_productpopularity", "commerce_payment" ],
      "ON" : [ "commerce_payment_order_paid_in_full" ],
      "DO" : [
        { "commerce_productpopularity_process_radioactivity" : { "commerce_order" : [ "commerce-order" ], "energy" : "10" } }
      ]
    }
  }');
  return $items;
}
