<?php
/**
 * @file
 * commerce_vbo_views.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_vbo_views_default_rules_configuration() {
  $items = array();
  $items['rules_disable_product'] = entity_import('rules_config', '{ "rules_disable_product" : {
      "LABEL" : "Wy\\u0142\\u0105cz produkt",
      "PLUGIN" : "action set",
      "TAGS" : [ "commerce_vbo_views" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "commerce_product" : { "label" : "Commerce Product", "type" : "commerce_product" } },
      "ACTION SET" : [
        { "data_set" : { "data" : [ "commerce-product:status" ], "value" : "0" } }
      ]
    }
  }');
  $items['rules_enable_product'] = entity_import('rules_config', '{ "rules_enable_product" : {
      "LABEL" : "W\\u0142\\u0105cz produkt",
      "PLUGIN" : "action set",
      "TAGS" : [ "commerce_vbo_views" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "commerce_product" : { "label" : "Commerce Product", "type" : "commerce_product" } },
      "ACTION SET" : [
        { "data_set" : { "data" : [ "commerce-product:status" ], "value" : "1" } }
      ]
    }
  }');
  return $items;
}
