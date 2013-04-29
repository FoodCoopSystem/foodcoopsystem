<?php

/**
 * @file
 * Default rule configurations for Commerce Stock.
 */
function commerce_stock_default_rules_configuration() {
  $rules_decrement_stock_completing_order = '{ "rules_decrement_stock_completing_order" : {
      "LABEL" : "Decrease stock when completing the order process",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "commerce_stock", "commerce_checkout" ],
      "ON" : [ "commerce_checkout_complete" ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "commerce_order:commerce-line-items" ] },
            "DO" : [
              { "commerce_stock_decrease_by_line_item" : { "commerce_line_item" : [ "list-item" ] } }
            ]
          }
        }
      ]
    }
  }';
  $configs['rules_decrement_stock_completing_order'] = rules_import($rules_decrement_stock_completing_order);

  return $configs;
}
