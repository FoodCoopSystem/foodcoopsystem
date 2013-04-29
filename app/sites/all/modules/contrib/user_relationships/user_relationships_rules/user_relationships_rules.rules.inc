<?php


/**
 * @file
 * Rules integration for the system module
 */

/**
 * Implements hook_rules_action_info().
 *
 * @ingroup rules
 */
function user_relationships_rules_rules_action_info() {
  return array(
    'user_relationships_rules_action_create_relationship' => array(
      'label' => t('Request, create or approve relationships between users'),
      'group' => t('Relationships'),
      'named parameter' => TRUE,
      'parameter' => array(
        'relationship_type' => array(
          'type' => 'integer',
          'label' => t('Relationship type'),
          'options list' => 'user_relationships_rules_types_list',
        ),
        'requester' => array(
          'type' => 'user',
          'label' => t('Requester'),
        ),
        'requestee' => array(
          'type' => 'user',
          'label' => t('Requestee'),
        ),
        'approve' => array(
          'type' => 'boolean',
          'label' => t('Approve relationship'),
        )
      ),
    ),
    'user_relationships_rules_action_delete_relationship' => array(
      'label' => t('Delete, cancel or disapprove relationships between users'),
      'group' => t('Relationships'),
      'named parameter' => TRUE,
      'parameter' => array(
        'relationship_type' => array(
          'type' => 'integer',
          'label' => t('Relationship type'),
          'options list' => 'user_relationships_rules_types_list',
        ),
        'requester' => array(
          'type' => 'user',
          'label' => t('Requester'),
        ),
        'requestee' => array(
          'type' => 'user',
          'label' => t('Requestee'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 *
 * @ingroup rules
 */
function user_relationships_rules_rules_condition_info() {
  return array(
    'user_relationships_rules_condition_relationship_has_type' => array(
      'label' => t('Relationship has type'),
      'named parameter' => TRUE,
      'parameter' => array(
        'relationship' => array(
          'type' => 'relationship',
          'label' => t('Relationship'),
        ),
        'relationship_type' => array(
          'type' => 'list<integer>',
          'label' => t('Relationship type'),
          'options list' => 'user_relationships_rules_types_list',
        ),
      ),
      'group' => t('Relationships'),
    ),
    'user_relationships_rules_condition_users_are_related' => array(
      'label' => t('Check if two users are related'),
      'named parameter' => TRUE,
      'parameter' => array(
        'user1' => array('type' => 'user', 'label' => t('User A')),
        'user2' => array('type' => 'user', 'label' => t('User B')),
      ),
      'group' => t('Relationships'),
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 */
function user_relationships_rules_rules_event_info() {
  return array(
    'user_relationships_request' => array(
      'label' => t('A user relationship has been requested'),
      'group' => t('Relationships'),
      'variables' => array(
        'requester' => array('type' => 'user', 'label' => t('User who initiated the request')),
        'requestee' => array('type' => 'user', 'label' => t('User whose relationship is requested')),
        'relationship' => array('type' => 'relationship', 'label' => t('The relationship object')),
      ),
    ),
    'user_relationships_cancel' => array(
      'label' => t('A user relationship has been cancelled'),
      'group' => t('Relationships'),
      'variables' => array(
        'requester' => array('type' => 'user', 'label' => t('User who initiated the request')),
        'requestee' => array('type' => 'user', 'label' => t('User whose relationship is requested')),
        'relationship' => array('type' => 'relationship', 'label' => t('The relationship object')),
      ),
    ),
    'user_relationships_approve' => array(
      'label' => t('A user relationship has been approved'),
      'group' => t('Relationships'),
      'variables' => array(
        'requester' => array('type' => 'user', 'label' => t('User who initiated the request')),
        'requestee' => array('type' => 'user', 'label' => t('User whose relationship is requested')),
        'relationship' => array('type' => 'relationship', 'label' => t('The relationship object')),
      ),
    ),
    'user_relationships_disapprove' => array(
      'label' => t('A user relationship has been disapproved'),
      'group' => t('Relationships'),
      'arguments' => array(
        'requester' => array('type' => 'user', 'label' => t('User who initiated the request')),
        'requestee' => array('type' => 'user', 'label' => t('User whose relationship is requested')),
        'relationship' => array('type' => 'relationship', 'label' => t('The relationship object')),
      ),
    ),
    'user_relationships_remove' => array(
      'label' => t('A user relationship has been removed'),
      'group' => t('Relationships'),
      'arguments' => array(
        'requester'   => array('type' => 'user',  'label' => t('User who initiated the request')),
        'requestee'   => array('type' => 'user',  'label' => t('User whose relationship is requested')),
        'relationship' => array('type' => 'relationship',  'label' => t('The relationship object'))
      )
    ),
  );
}

/**
 * Returns a list of relationships that can be used in a options list callback.
 */
function user_relationships_rules_types_list() {
  $options = array();
  $relationship_types = user_relationships_types_load();
  foreach ($relationship_types as $relationship_type) {
    $options[$relationship_type->rtid] = $relationship_type->name;
  }
  return $options;
}

/**
 * Action Implementation: Create relationship.
 */
function user_relationships_rules_action_create_relationship($settings) {
  $args = array(
    'requester_id' => $settings['requester'] instanceof EntityDrupalWrapper ? $settings['requester']->getIdentifier() : $settings['requester']->uid,
    'requestee_id' => $settings['requestee'] instanceof EntityDrupalWrapper ? $settings['requestee']->getIdentifier() : $settings['requestee']->uid,
    'rtid' => $settings['relationship_type'],
  );
  $existing = user_relationships_load($args);

  if (empty($existing)) {
    user_relationships_request_relationship($args['requester_id'], $args['requestee_id'], $args['rtid'], $settings['approve']);
  }
  elseif ($settings['approve']) {
    $relationship = reset($existing);
    user_relationships_save_relationship($relationship, 'approve');
  }
}

/**
 * Action Implementation: Delete relationship.
 */
function user_relationships_rules_action_delete_relationship($settings) {
  $args = array(
    'requester_id' => $settings['requester'] instanceof EntityDrupalWrapper ? $settings['requester']->getIdentifier() : $settings['requester']->uid,
    'requestee_id' => $settings['requestee'] instanceof EntityDrupalWrapper ? $settings['requestee']->getIdentifier() : $settings['requestee']->uid,
    'rtid' => $settings['relationship_type'],
  );
  $existing = user_relationships_load($args);// finding rid - this method can probably be made better
  if ($relationship = reset($existing)) {
    $deleted_by = $settings['requester'] instanceof EntityDrupalWrapper ? user_load($settings['requester']->getIdentifier()) : $settings['requester'];
    user_relationships_delete_relationship($relationship, $deleted_by, 'remove');
  }
}

/**
 * Condition Implementation: Relationship has type.
 */
function user_relationships_rules_condition_relationship_has_type($settings) {
  return in_array($settings['relationship']->rtid, $settings['relationship_type']);
}

/**
 * Condition Implementation: Users are related.
 */
function user_relationships_rules_condition_users_are_related($user1, $user2, $relationship_types) {
  // Get the approved relationships between the two users and the specified
  // relationship types.
  $relationships = user_relationships_load(array('approved' => 1, 'rtid' => $relationship_types, 'between' => array($user1 instanceof EntityDrupalWrapper ? $user1->getIdentifier() : $user1->uid, $user2 instanceof EntityDrupalWrapper ? $user2->getIdentifier() : $user2->uid)));
  return !empty($relationships);
}
