<?php
/**
 * @file Main relationships listing block
 * List the relationships between the viewed user and the current user
 */
if ($relationships) {
  $the_other_uid = $settings->block_type == UR_BLOCK_MY ? $user->uid : $account->uid;
  $showing_all_types = $settings->rtid == UR_BLOCK_ALL_TYPES;
  $rows = array();
  foreach ($relationships as $rtid => $relationship) {
    $tt_rel_name = ur_tt("user_relationships:rtid:$rtid:name", $relationship->name);
    $tt_rel_plural_name = ur_tt("user_relationships:rtid:$rtid:plural_name", $relationship->plural_name); 
    if ($the_other_uid == $relationship->requester_id) {
      $rtype_heading = $relationship->is_oneway ? 
        t("@rel_name of", array('@rel_name' => $tt_rel_name, '@rel_plural_name' => $tt_rel_plural_name)) : 
        t("@rel_plural_name", array('@rel_name' => $tt_rel_name, '@rel_plural_name' => $tt_rel_plural_name));
      $relatee = $relationship->requestee;
    }
    else {
      $rtype_heading = t("@rel_plural_name", array('@rel_name' => $tt_rel_name, '@rel_plural_name' => $tt_rel_plural_name));
      $relatee = $relationship->requester;
    }

    $title = $rtype_heading;

    $username = theme('username', array('account' => $relatee));
    $rows[$title][] = $username;
  }

  foreach ($rows as $title => $users) {
    $variables = array('items' => ($rtid == UR_BLOCK_ALL_TYPES ? array($users) : $users));
    if ($showing_all_types) {
      $variables['title'] = $title;
    }
    $output[] = theme('item_list', $variables);
  }

  print implode('', $output);
}
/* removing printing out empty placeholder so the block is hidden when no data
// No relationships so figure out how we present that
else {
  if ($settings->rtid == UR_BLOCK_ALL_TYPES) {
    $rtype_name = 'relationships';
  }
  else {
    $rtype      = user_relationships_type_load($settings->rtid);
    $rtype_name = $rtype->plural_name;
  }

  if ($account->uid == $user->uid) {
    print t('You have no @rels', array('@rels' => $rtype_name));
  }
  else {
    print t('!name has no @rels', array('!name' => theme('username', $account), '@rels' => $rtype_name));
  }
}
*/
?>
