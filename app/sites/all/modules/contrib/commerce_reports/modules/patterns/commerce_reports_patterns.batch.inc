<?php

function commerce_reports_patterns_load($min_support, &$context) {
  if (empty($context['sandbox'])) {
    $total_transactions = db_query("SELECT COUNT(order_id) FROM {commerce_order} WHERE status IN ('completed', 'pending')")->fetchField();
    $min_transactions = (int) floor($total_transactions * $min_support);
    
    $context['results'] = array(
      'total_transactions' => $total_transactions,
      'minimum_transactions' => $min_transactions,
      'frequent_sets' => array(
        1 => array(
          array(
            'prefix' => array(),
            'sets' => array(),
          ),
        ),
      ),
    );
    
    $context['sandbox']['products'] = db_query('SELECT sku, COUNT(li.line_item_id) AS occurences FROM {commerce_product} p LEFT JOIN {commerce_line_item} li ON p.sku = li.line_item_label GROUP BY sku HAVING occurences >= :min_occurences ORDER BY occurences DESC', array(':min_occurences' => $min_transactions))->fetchAll();
    
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($context['sandbox']['products']);
    
    if (!$context['sandbox']['max']) {
      drupal_set_message(t('The value of your minimum support is too high to include any of the available transactions.'), 'error');
      return;
    }
  }
  
  $product = $context['sandbox']['products'][$context['sandbox']['progress']];
  
  if ($product) {
    $context['results']['frequent_sets'][1][0]['sets'][] = array(
      'items' => array($product->sku),
      'tidlist' => db_query("SELECT DISTINCT order_id FROM {commerce_line_item} WHERE line_item_label = :sku", array(':sku' => $product->sku))->fetchCol()
    );
  }

  $context['sandbox']['progress'] ++;
  
  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  } else {
    if (!count($context['results']['frequent_sets'][1][0]['sets'])) {
      unset($context['results']['frequent_sets'][1]);
    }
  }
}

function commerce_reports_patterns_frequent_itemset(&$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['set_length'] = 2;
    
    $context['sandbox']['current_partition'] = 0;
    $context['sandbox']['first_itemset'] = 0;
    $context['sandbox']['second_itemset'] = 1;
  }
  
  $length = $context['sandbox']['set_length'];
  
  if (empty($context['results']['frequent_sets'][$length - 1])) {
    return;
  }
  if (empty($context['results']['frequent_sets'][$length])) {
    $context['results']['frequent_sets'][$length] = array();
  }
  
  $prev_partitions =& $context['results']['frequent_sets'][$length - 1];
  $prev_partitions_count = count($prev_partitions);
  
  for ($x = 0; $x < 50; $x ++) {
    if (!empty($prev_partitions[$context['sandbox']['current_partition']])) {
      $partition_itemset_count = count($prev_partitions[$context['sandbox']['current_partition']]['sets']);
    } else {
      $partition_itemset_count = 0;
    }
    
    $a = $context['sandbox']['first_itemset'];
    $b = $context['sandbox']['second_itemset'];
    
    if ($context['sandbox']['second_itemset'] >= $partition_itemset_count) {
      if ($context['sandbox']['first_itemset'] >= $partition_itemset_count) {
        if ($context['sandbox']['current_partition'] >= $prev_partitions_count) {
          $context['sandbox']['current_partition'] = 0;
          $context['sandbox']['set_length'] ++;
          break;
        }
        else {
          $context['sandbox']['current_partition'] ++;
        }
        
        $context['sandbox']['first_itemset'] = 0;
      }
      else {
        $context['sandbox']['first_itemset'] ++;
      }
      
      $context['sandbox']['second_itemset'] = $context['sandbox']['first_itemset'] + 1;
      continue;
    }
    else {
      $context['sandbox']['second_itemset'] ++;
    }
    
    $partition =& $prev_partitions[$context['sandbox']['current_partition']];
    $partition_prefix =& $partition['prefix'];
    $partition_itemset =& $partition['sets'];
    
    $a_set =& $partition_itemset[$a];
    $b_set =& $partition_itemset[$b];
    
    $candidate = array(
      'items' => array_values(array_unique(array_merge($a_set['items'], $b_set['items']))),
      'tidlist' => array_intersect($a_set['tidlist'], $b_set['tidlist']),
    );
    
    $occurences = max(count($candidate['tidlist']), 0);
    
    if ($occurences >= $context['results']['minimum_transactions']) {
      $new_items = array_diff($candidate['items'], $partition_prefix);
      $prefix = array_merge($partition_prefix, array(reset($new_items)));

      _commerce_reports_patterns_add_set($context['results']['frequent_sets'][$length], $prefix, $candidate);
    }
  }
  
  if ($count = count($context['results']['frequent_sets'][$length])) {
    $context['finished'] = 0;
    $context['message'] = t('Generated %count frequent item sets with length %length', array('%count' => $count, '%length' => $length));
  } else {
    unset($context['results']['frequent_sets'][$length]);
  }
}

function _commerce_reports_patterns_add_set(&$partitions, $prefix, $set) {
  sort($prefix);
  
  foreach ($partitions as &$partition) {
    if ($partition['prefix'] == $prefix) {
      $partition['sets'][] = $set;
      
      return;
    }
  }
  
  $partitions[] = array(
    'prefix' => $prefix,
    'sets' => array($set),
  );
}

function commerce_reports_patterns_association_rules($min_confidence, &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['length'] = 2;
    $context['sandbox']['partition'] = 0;
    $context['sandbox']['set'] = 0;
    
    $context['results']['count'] = 0;
    
    $context['results']['rules'] = array();
    
    db_query('TRUNCATE {commerce_reports_patterns}');
  }
  
  if ($context['sandbox']['set'] > count($context['results']['frequent_sets'][$context['sandbox']['length']][$context['sandbox']['partition']]['sets']) - 1) {
    $context['sandbox']['partition'] ++;
    $context['sandbox']['set'] = 0;
  }
  
  if ($context['sandbox']['partition'] > count($context['results']['frequent_sets'][$context['sandbox']['length']]) - 1) {
    $context['sandbox']['length'] ++;
    $context['sandbox']['partition'] = 0;
  }
  
  if ($context['sandbox']['length'] > count($context['results']['frequent_sets'])) {
    $context['finished'] = 1;
    
    return;
  }
  else {
    $context['finished'] = 0;
  }
  
  $set =& $context['results']['frequent_sets'][$context['sandbox']['length']][$context['sandbox']['partition']]['sets'][$context['sandbox']['set']];
  $items =& $set['items'];
  $set_support_count = count($set['tidlist']);
  
  $count = count($items);
  $members = pow(2, $count);
  
  for ($i = 0; $i < $members; $i ++) {
    $b = sprintf('%0' . $count . 'b', $i);
    
    $rule = new stdClass();
    $rule->size = $count;
    $rule->if_clause = array();
    $rule->then_clause = array();
    
    for ($j = 0; $j < $count; $j++) {
      if ($b{$j} == '1') {
        $rule->if_clause[] = $items[$j];
      }
      else {
        $rule->then_clause[] = $items[$j];
      }
    }
    
    if (!empty($rule->then_clause) && ($if_count = count($rule->if_clause))) {
      $if_support_count = db_query("SELECT COUNT(*) FROM (SELECT order_id FROM {commerce_line_item} WHERE line_item_label IN (:sku) GROUP BY order_id HAVING COUNT(line_item_id) = :count) cli", array(':sku' => $rule->if_clause, ':count' => $if_count))->fetchField();
    
      if ($if_support_count && (($confidence = ($set_support_count / $if_support_count)) >= $min_confidence)) {
        $rule->confidence = $confidence;
        
        drupal_write_record('commerce_reports_patterns', $rule);
        $context['results']['count'] ++;
      }
    }
    
    // Clear memory after set processing.
    unset($set);
  }
  
  $context['sandbox']['set'] ++;
  
  $context['message'] = t('Found %count association rules', array('%count' => $context['results']['count']));
}

function commerce_reports_patterns_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('Successfully finished mining frequent itemsets, the largest set contained %length items.', array('%length' => count($results['frequent_sets']))));
    drupal_set_message(t('Successfully finished generating %count association rules.', array('%count' => $results['count'])));
  }
  else {
    drupal_set_message(t('There was a problem generating frequent itemsets.'), 'error');
  }
}
