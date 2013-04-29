<?php
/**
 * @file
 * wks_commerce.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function wks_commerce_default_rules_configuration() {
  $items = array();
  $items['rules_dodaj_podatek'] = entity_import('rules_config', '{ "rules_dodaj_podatek" : {
      "LABEL" : "Dodaj podatek",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "add_fundusz_gromadzki" ],
      "REQUIRES" : [ "entity" ],
      "ON" : [ "commerce_product_view" ],
      "DO" : []
    }
  }');
  $items['rules_oblicz_podatki_op_ata_na_fundusz_gromadzki'] = entity_import('rules_config', '{ "rules_oblicz_podatki_op_ata_na_fundusz_gromadzki" : {
      "LABEL" : "Oblicz podatki: Op\\u0142ata na fundusz gromadzki",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "commerce_line_item", "commerce_product_reference" ],
      "ON" : [ "commerce_product_calculate_sell_price" ],
      "DO" : [
        { "commerce_line_item_unit_price_multiply" : {
            "commerce_line_item" : [ "commerce_line_item" ],
            "amount" : "1.10",
            "component_name" : "tax|fundusz_gomadzki",
            "round_mode" : "1"
          }
        }
      ]
    }
  }');
  $items['rules_provider_have_add_content'] = entity_import('rules_config', '{ "rules_provider_have_add_content" : {
      "LABEL" : "Provider have add content",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "typ_produktu" : "typ_produktu" } }
          }
        },
        { "user_has_role" : {
            "account" : [ "node:author" ],
            "roles" : { "value" : { "4" : "4", "8" : "8" } }
          }
        }
      ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "6" : "6", "3" : "3" } },
            "subject" : "Dostawca doda\\u0142 nowy produkt [node:title]",
            "message" : "Witaj,\\r\\n\\r\\nDostawca [node:author] doda\\u0142 nowy produkt [node:title], sprawd\\u017a jego poprawno\\u015b\\u0107 [node:url]\\r\\n"
          }
        }
      ]
    }
  }');
  $items['rules_return_products_to_stock'] = entity_import('rules_config', '{ "rules_return_products_to_stock" : {
      "LABEL" : "Return products to stock",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Stock" ],
      "REQUIRES" : [ "commerce_stock", "entity" ],
      "ON" : [ "commerce_line_item_delete" ],
      "DO" : [
        { "commerce_stock_increase_by_line_item" : { "commerce_line_item" : [ "commerce_line_item" ] } }
      ]
    }
  }');
  $items['rules_wy_lij_powiadomienie_o_zmianie_cen_produkt_w'] = entity_import('rules_config', '{ "rules_wy_lij_powiadomienie_o_zmianie_cen_produkt_w" : {
      "LABEL" : "Wy\\u015blij powiadomienie o zmianie cen produkt\\u00f3w",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "4",
      "REQUIRES" : [ "rules", "entity" ],
      "ON" : [ "commerce_product_update" ],
      "IF" : [
        { "NOT data_is" : {
            "data" : [ "commerce-product:commerce-price" ],
            "value" : [ "commerce-product-unchanged:commerce-price" ]
          }
        },
        { "user_has_role" : {
            "account" : [ "commerce-product:creator" ],
            "roles" : { "value" : { "8" : "8", "9" : "9" } }
          }
        }
      ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "7" : "7", "5" : "5", "6" : "6", "3" : "3" } },
            "subject" : "Informujemy, \\u017ce produkt zosta\\u0142 zaktualizowany",
            "message" : "Witaj,\\r\\n\\r\\nInformujemy \\u017ce w [site:name] zaktualizowano dane produktu - [commerce-product:title]\\r\\n\\r\\nWi\\u0119cej informacji znajdziesz na: [site:url]produkty"
          }
        }
      ]
    }
  }');
  return $items;
}
