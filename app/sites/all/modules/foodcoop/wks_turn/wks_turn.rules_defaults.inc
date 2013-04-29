<?php
/**
 * @file
 * wks_turn.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function wks_turn_default_rules_configuration() {
  $items = array();
  $items['rules_new_turn'] = entity_import('rules_config', '{ "rules_new_turn" : {
      "LABEL" : "Create turn event",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Turn" ],
      "REQUIRES" : [ "rules", "wks_addtask" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "tura" : "tura" } } } }
      ],
      "DO" : [ { "add_task" : { "node" : [ "node" ] } } ]
    }
  }');
  $items['rules_remove_signup'] = entity_import('rules_config', '{ "rules_remove_signup" : {
      "LABEL" : "Signup in turn",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Turn" ],
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "flag_flagged_signup" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "flagged-node" ],
            "type" : { "value" : {
                "kasjer" : "kasjer",
                "koordynacja" : "koordynacja",
                "nadz_r_ksi_gowo_ci" : "nadz_r_ksi_gowo_ci",
                "sprz_tanie" : "sprz_tanie",
                "transport" : "transport",
                "wazacy" : "wazacy"
              }
            }
          }
        }
      ],
      "DO" : [
        { "flag_fetch_users_node" : {
            "USING" : { "flag" : "signup", "node" : [ "flagged-node" ] },
            "PROVIDE" : { "users" : { "users" : "Users who signup" } }
          }
        },
        { "list_add" : {
            "list" : [ "flagged-node:field-zgloszeni" ],
            "item" : [ "flagging-user" ],
            "unique" : 1
          }
        },
        { "user_add_role" : { "account" : [ "users:0" ], "roles" : { "value" : { "5" : "5" } } } }
      ]
    }
  }');
  $items['rules_remove_signups'] = entity_import('rules_config', '{ "rules_remove_signups" : {
      "LABEL" : "Remove signup",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Turn" ],
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "flag_unflagged_signup" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "flagged-node" ],
            "type" : { "value" : {
                "kasjer" : "kasjer",
                "koordynacja" : "koordynacja",
                "nadz_r_ksi_gowo_ci" : "nadz_r_ksi_gowo_ci",
                "sprz_tanie" : "sprz_tanie",
                "transport" : "transport",
                "wazacy" : "wazacy"
              }
            }
          }
        }
      ],
      "DO" : [
        { "flag_fetch_users_node" : {
            "USING" : { "flag" : "signup", "node" : [ "flagged-node" ] },
            "PROVIDE" : { "users" : { "users" : "Users who flagged" } }
          }
        },
        { "list_remove" : {
            "list" : [ "flagged-node:field-zgloszeni" ],
            "item" : [ "flagging-user" ]
          }
        },
        { "user_remove_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "5" : "5" } }
          }
        }
      ]
    }
  }');
  $items['rules_wy_lij_powiadomienie_e_mail_o_nowej_turze'] = entity_import('rules_config', '{ "rules_wy_lij_powiadomienie_e_mail_o_nowej_turze" : {
      "LABEL" : "Wy\\u015blij powiadomienie e-mail o nowej turze",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "4",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "tura" : "tura" } } } }
      ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "7" : "7", "5" : "5", "6" : "6", "3" : "3" } },
            "subject" : "Informujemy, \\u017ce stworzono now\\u0105 tur\\u0119 zakup\\u00f3w - [node:title]",
            "message" : "Witaj,\\r\\n\\r\\nInformujemy \\u017ce w [site:name] stworzono now\\u0105 tur\\u0119 zakup\\u00f3w - [node:title]\\r\\n\\r\\nWi\\u0119cej informacji znajdziesz na: [site:url]tura"
          }
        }
      ]
    }
  }');
  $items['rules_wy_lij_powiadomienie_e_mail_o_zapisaniu_si_do_zadania'] = entity_import('rules_config', '{ "rules_wy_lij_powiadomienie_e_mail_o_zapisaniu_si_do_zadania" : {
      "LABEL" : "Wy\\u015blij powiadomienie e-mail o zapisaniu si\\u0119 do zadania",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "4",
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "flag_flagged_signup" ],
      "DO" : [
        { "mail" : {
            "to" : [ "flagging-user:mail" ],
            "subject" : "Zapisa\\u0142e\\u015b si\\u0119 do roli [flagged-node:content-type] w turze [flagged-node:field-tura]",
            "message" : "Dzi\\u0119kujemy za zapisanie si\\u0119 do [flagged-node:content-type] w turze [flagged-node:field-tura]\\r\\n\\r\\nProcedury co powiniene\\u015b dalej robi\\u0107 znajdziesz na stronie: https:\\/\\/docs.google.com\\/document\\/d\\/1csIZcWUFN9GgEX_epAHJCVVLN3mYGFjWLu5TT8NkHIw\\/edit?hl=en\\u0026authkey=CJ_P8fgP\\u0026pli=1#",
            "language" : [ "" ]
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "6" : "6" } },
            "subject" : "U\\u017cytkownik [flagging-user:name] zapisa\\u0142 si\\u0119 do roli [flagged-node:content-type] w turze [flagged-node:field-tura]",
            "message" : "Witaj,\\r\\n\\r\\nInformujemy \\u017ce u\\u017cytkownik [flagging-user:name] zapisa\\u0142 si\\u0119 do roli [flagged-node:content-type] dla tury [flagged-node:field-tura].\\r\\n\\r\\nWi\\u0119cej informacji znajdziesz w zak\\u0142adce Tura systemu zam\\u00f3wie\\u0144: [site:url]tura oraz na naszej stronie z procedurami https:\\/\\/docs.google.com\\/document\\/d\\/1csIZcWUFN9GgEX_epAHJCVVLN3mYGFjWLu5TT8NkHIw\\/edit?hl=en\\u0026authkey=CJ_P8fgP\\u0026pli=1# \\r\\n\\r\\nPozdrawiamy,"
          }
        }
      ]
    }
  }');
  $items['rules_wy_lij_powiadomienie_e_mail_o_zrezygnowaniu_z_roli_zadania'] = entity_import('rules_config', '{ "rules_wy_lij_powiadomienie_e_mail_o_zrezygnowaniu_z_roli_zadania" : {
      "LABEL" : "Wy\\u015blij powiadomienie e-mail o zrezygnowaniu z roli zadania",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "4",
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "flag_unflagged_signup" ],
      "DO" : [
        { "mail" : {
            "to" : [ "flagging-user:mail" ],
            "subject" : "Rezygnacja z [flagged-node:content-type] w turze [flagged-node:field-tura]",
            "message" : "Zrezygnowa\\u0142e\\u015b z roli [flagged-node:content-type] w turze [flagged-node:field-tura]\\r\\n\\r\\nProcedury co powiniene\\u015b dalej robi\\u0107 znajdziesz na stronie: https:\\/\\/docs.google.com\\/document\\/d\\/1csIZcWUFN9GgEX_epAHJCVVLN3mYGFjWLu5TT8NkHIw\\/edit?hl=en\\u0026authkey=CJ_P8fgP\\u0026pli=1#",
            "language" : [ "" ]
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "6" : "6" } },
            "subject" : "U\\u017cytkownik [flagging-user:name] zrezygnowa\\u0142 z roli [flagged-node:content-type] w turze [flagged-node:field-tura]",
            "message" : "Witaj,\\r\\n\\r\\nInformujemy \\u017ce u\\u017cytkownik [flagging-user:name] zrezygnowa\\u0142 z roli [flagged-node:content-type] dla tury [flagged-node:field-tura].\\r\\n\\r\\nWi\\u0119cej informacji znajdziesz w zak\\u0142adce Tura systemu zam\\u00f3wie\\u0144: [site:url]tura"
          }
        }
      ]
    }
  }');
  return $items;
}
