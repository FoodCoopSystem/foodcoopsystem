<?php
/**
 * @file
 * wks_civicrm.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function wks_civicrm_default_rules_configuration() {
  $items = array();
  $items['rules_dodaj_nowe_zam_wienie_do_civicrm'] = entity_import('rules_config', '{ "rules_dodaj_nowe_zam_wienie_do_civicrm" : {
      "LABEL" : "Dodaj nowe zam\\u00f3wienie do CiviCRM",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "commerce_civicrm", "entity" ],
      "ON" : [ "commerce_order_insert" ],
      "DO" : [
        { "commerce_civicrm_create_order_civicrm" : { "commerce_order" : [ "commerce-order" ] } }
      ]
    }
  }');
  return $items;
}
