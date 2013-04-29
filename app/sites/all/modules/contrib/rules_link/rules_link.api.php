<?php
/**
 * @file
 * Hooks provided by this module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Acts on rules links being loaded from the database.
 *
 * This hook is invoked during rules link loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $rules_links
 *   An array of rules link entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_rules_link_load(array $rules_links) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a rules link is inserted.
 *
 * This hook is invoked after the rules link is inserted into the database.
 *
 * @param RulesLink $rules_link
 *   The rules link that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_rules_link_insert(RulesLink $rules_link) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('rules_link', $rules_link),
      'extra' => print_r($rules_link, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a rules link being inserted or updated.
 *
 * This hook is invoked before the rules link is saved to the database.
 *
 * @param RulesLink $rules_link
 *   The rules link that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_rules_link_presave(RulesLink $rules_link) {
  $rules_link->name = 'foo';
}

/**
 * Responds to a rules link being updated.
 *
 * This hook is invoked after the rules link has been updated in the database.
 *
 * @param RulesLink $rules_link
 *   The rules link that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_rules_link_update(RulesLink $rules_link) {
  db_update('mytable')
    ->fields(array('extra' => print_r($rules_link, TRUE)))
    ->condition('id', entity_id('rules_link', $rules_link))
    ->execute();
}

/**
 * Responds to rules link deletion.
 *
 * This hook is invoked after the rules link has been removed from the database.
 *
 * @param RulesLink $rules_link
 *   The rules link that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_rules_link_delete(RulesLink $rules_link) {
  db_delete('mytable')
    ->condition('pid', entity_id('rules_link', $rules_link))
    ->execute();
}

/**
 * @}
 */