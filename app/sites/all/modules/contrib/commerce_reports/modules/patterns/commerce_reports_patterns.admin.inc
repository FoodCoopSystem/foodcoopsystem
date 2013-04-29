<?php

function commerce_reports_patterns_form($form, &$form_state) {
  $form['min_support'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum support'),
    '#description' => t('The minimum support should be given as a percentage that indicates what the minimum relative amount of transactions an item set requires to be frequent.'),
    '#size' => 6,
    '#field_suffix' => '%',
  );
  $form['min_confidence'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum confidence'),
    '#description' => t('The minimum confidence should be given as a percentage that indicates the chance of occurence a rule should minimally have.'),
    '#size' => 6,
    '#field_suffix' => '%',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate'),
  );
  
  return $form;
}

function commerce_reports_patterns_form_validate($form, &$form_state) {
  $min_support = (float) $form_state['values']['min_support'];
  $min_confidence = (float) $form_state['values']['min_confidence'];
  
  if (($min_support < 0) || ($min_support > 100)) {
    form_set_error('min_support', t('The minimum support should be a value between 0 and 100.'));
  }
  if (($min_confidence < 0) || ($min_confidence > 100)) {
    form_set_error('min_confidence', t('The minimum confidence should be a value between 0 and 100.'));
  }
}

function commerce_reports_patterns_form_submit($form, &$form_state) {
  $min_support = (float) $form_state['values']['min_support'] / 100;
  $min_confidence = (float) $form_state['values']['min_confidence'] / 100;
  
  $batch = array(
    'title' => t('Association rule mining'),
    'operations' => array(
      array('commerce_reports_patterns_load', array($min_support)),
      array('commerce_reports_patterns_frequent_itemset', array()),
      array('commerce_reports_patterns_association_rules', array($min_confidence)),
    ),
    'finished' => 'commerce_reports_patterns_finished',
    'file' => drupal_get_path('module', 'commerce_reports_patterns') . '/commerce_reports_patterns.batch.inc',
  );
  
  batch_set($batch);
  
  batch_process('admin/commerce/reports/patterns/view');
}

function commerce_reports_patterns_view() {
  $maxSize = db_query("SELECT MAX(size) FROM {commerce_reports_patterns}")->fetchField();
  
  $return = array();
  for ($i = $maxSize; $i > 1; $i --) {
   $return[] = _commerce_reports_patterns_view($i);
  }
  
  if ($maxSize) {
    return $return;
  } else {
    return t('No association rules found using the specified parameters.');
  }
}

function _commerce_reports_patterns_view($size) {
  $query = db_query("SELECT if_clause, then_clause, confidence FROM {commerce_reports_patterns} WHERE size = :size ORDER BY confidence DESC LIMIT 10", array(':size' => $size));
  
  $rules = array();
  while ($result = $query->fetchAssoc()) {
    $rules[] = array(
      theme('item_list', array(
        'items' => unserialize($result['if_clause']),
      )),
      theme('item_list', array(
        'items' => unserialize($result['then_clause']),
      )),
      round($result['confidence'] * 100) . '%'
    );
  }
  
  return array(
    '#theme' => 'table',
    '#rows' => $rules,
    '#header' => array(t('Antecedent'), t('Consequent'), t('Chance')),
  );
}
