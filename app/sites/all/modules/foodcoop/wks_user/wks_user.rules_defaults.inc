<?php
/**
 * @file
 * wks_user.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function wks_user_default_rules_configuration() {
  $items = array();
  $items['rules_przekierowanie_u_ytkownika_po_wylogowaniu'] = entity_import('rules_config', '{ "rules_przekierowanie_u_ytkownika_po_wylogowaniu" : {
      "LABEL" : "Przekierowanie u\\u017cytkownika po wylogowaniu",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "user_logout" ],
      "DO" : [
        { "redirect" : { "url" : "user\\/login" } },
        { "drupal_message" : { "message" : "Powodzenie, wylogowa\\u0142e\\u015b si\\u0119 prawid\\u0142owo!" } }
      ]
    }
  }');
  $items['rules_wy_lij_powiadomienie_e_mail_o_do_czeniu_nowego_kooperatyst'] = entity_import('rules_config', '{ "rules_wy_lij_powiadomienie_e_mail_o_do_czeniu_nowego_kooperatyst" : {
      "LABEL" : "Wy\\u015blij powiadomienie e-mail o do\\u0142\\u0105czeniu nowego kooperatysty",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "4",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "user_insert" ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "3" : "3" } },
            "subject" : "U\\u017cytkownik [account:name] do\\u0142\\u0105czy\\u0142 do [site:name]",
            "message" : "Witaj,\\r\\n\\r\\nInformujemy \\u017ce u\\u017cytkownik [account:name] do\\u0142\\u0105czy\\u0142 do kooperatywy [site:name].\\r\\nDane:\\r\\n[account:field_adressfield]\\r\\n[account:mail]\\r\\n[account:url]\\r\\n"
          }
        }
      ]
    }
  }');
  return $items;
}
