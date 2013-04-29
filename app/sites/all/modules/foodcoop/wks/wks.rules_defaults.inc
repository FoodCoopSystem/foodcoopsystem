<?php
/**
 * @file
 * wks.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function wks_default_rules_configuration() {
  $items = array();
  $items['rules_dostawca'] = entity_import('rules_config', '{ "rules_dostawca" : {
      "LABEL" : "Dostawca",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Commerce" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "user_login" ],
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "4" : "4", "8" : "8" } },
            "operation" : "OR"
          }
        }
      ],
      "DO" : [ { "redirect" : { "url" : "admin\\/commerce\\/products" } } ]
    }
  }');
  return $items;
}
