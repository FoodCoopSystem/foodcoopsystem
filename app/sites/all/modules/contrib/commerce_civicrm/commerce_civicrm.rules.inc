<?php

/**
 * @file
 * This file contains the Rules hooks and functions necessary to make the order
 * related entity, conditions, events, and actions work.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_civicrm_rules_action_info() {
  $actions['commerce_civicrm_create_order_civicrm'] = array(
    'group' => t('Commerce CiviCRM'),
    'label' => t('Add and update CiviCRM contact record'),
    'base' => 'commerce_civicrm_action_order_civicrm',
    'parameter' => array(
      'commerce_order' => array('type' => 'commerce_order', 'label' => t('Order')),
    ),
  );

  return $actions;
}

