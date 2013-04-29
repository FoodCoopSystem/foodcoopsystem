<?php
/**
 * @file
 * Provides all statistics and other features that are not powered by Views.
 */

/**
 * Implementation of table showing sales overview.
 */
function commerce_reports_sales() {
  $sales = array();

  if ($timestamp = strtotime('first day of this month midnight')) {
    $passed_days = format_date(REQUEST_TIME, 'custom', 'j');
    $month_raw = commerce_reports_sales_data($timestamp, $passed_days);

    $month = commerce_reports_sales_format($month_raw, t('Daily average for @month', array('@month' => format_date($timestamp, 'custom', 'F o'))));
    $sales = array_merge($sales, $month);

    $total_days = format_date($timestamp, 'custom', 't');
    $projected_raw = array();
    foreach ($month_raw as $currency => $row) {
      $projected_raw[$currency] = array(
        'count' => round($row['count'] * $total_days, 2),
        'sum' => round( $row['sum'] * $total_days),
      );
    }

    $projected = commerce_reports_sales_format($projected_raw, t('Projected totals for @month', array('@month' => format_date($timestamp, 'custom', 'F o'))));
    $sales = array_merge($sales, $projected);
  }

  return array(
    '#theme' => 'table',
    '#rows' => $sales,
    '#header' => array(array('data' => t('Sales Data')), array('data' => t('Number of Orders')), array('data' => t('Revenue'))),
  );
}

/**
 * Helper function that retrieves sales data from a certain starting point.
 */
function commerce_reports_sales_data($start = 0, $time_periods = 1) {
  $sales = array();

  $result = db_query('SELECT SUM(amount) AS sum, AVG(amount) AS average, COUNT(*) AS count, currency_code FROM {commerce_payment_transaction} cpt WHERE created >= :start  GROUP by currency_code', array(':start' => $start));

  while ($row = $result->fetchAssoc()) {
    $currency = $row['currency_code'];
    unset($row['currency_code']);

    $sales[$currency] = $row;
    $sales[$currency]['count'] = round($sales[$currency]['count'] / $time_periods, 2);
    $sales[$currency]['sum'] = round($sales[$currency]['sum'] / $time_periods);
  }

  return $sales;
}

/**
 * Helper function that formats the data retrieved from commerce_reports_sales_data() function.
 */
function commerce_reports_sales_format($data, $title) {
  $sales = array();

  foreach ($data as $currency => $row) {
    $sales[] = array($title, $row['count'], commerce_currency_format($row['sum'], $currency));
  }

  if (empty($sales)) {
    $sales[] = array($title, 0, commerce_currency_format(0, commerce_default_currency()));
  }

  return $sales;
}

/**
 * Implementation of table showing statistics about customers.
 */
function commerce_reports_customers() {
  $statistics = array();

  $total_revenue = db_query('SELECT currency_code, SUM(amount) AS amount FROM {commerce_payment_transaction} GROUP by currency_code');

  while ($result = $total_revenue->fetch()) {
    $statistics[] = array(t('Total revenue'), commerce_currency_format($result->amount, $result->currency_code));
  }

  $total_customers = db_query("SELECT COUNT(uid) FROM (SELECT u.uid AS uid FROM {users} u INNER JOIN {commerce_order} o ON u.uid = o.uid WHERE o.status IN ('completed', 'pending') GROUP BY o.uid) s")->fetchField();
  $new_customers_today = db_query("SELECT COUNT(uid) FROM (SELECT u.uid AS uid FROM {users} u INNER JOIN {commerce_order} o ON u.uid = o.uid WHERE o.status IN ('completed', 'pending') GROUP BY o.uid HAVING MIN(o.created) >= :created) s", array(':created' => strtotime('midnight', REQUEST_TIME)))->fetchField();
  $online_customers = db_query("SELECT COUNT(uid) FROM (SELECT u.uid AS uid FROM {users} u INNER JOIN {commerce_order} o ON u.uid = o.uid WHERE o.status IN ('completed', 'pending') GROUP BY o.uid HAVING o.uid <> 0) s WHERE uid IN (SELECT sess.uid FROM {sessions} sess)")->fetchField();

  $statistics = array_merge($statistics,
    array(
      array(t('Customers total'), $total_customers),
      array(t('New customers today'), $new_customers_today),
      array(t('Online customers'), $online_customers),
    )
  );

  return array(
    '#theme' => 'table',
    '#rows' => $statistics,
  );
}

/**
 * Implementation of billing information block.
 */
function commerce_reports_customer_map() {
  $data = array();

  $result = db_query("SELECT ca.commerce_customer_address_country AS country, COUNT(*) AS count FROM {field_data_commerce_customer_address} ca WHERE ca.bundle = 'billing' GROUP BY ca.commerce_customer_address_country");
  $result->setFetchMode(PDO::FETCH_ASSOC);

  foreach ($result as $record) {
    $data[] = $record;
  }

  $options = array(
    'title' => 'Customer map',
    'fields' => array(
      'country' => array(
        'label' => t('Country'),
      ),
      'count' => array(
        'enabled' => TRUE,
        'label' => t('Amount'),
      ),
    ),
    'xAxis' => array(
      'labelField' => 'country',
    ),
    'data' => $data,
    'type' => 'map',
    'height' => '400px',
  );

  return array(
    '#theme' => 'visualization',
    '#options' => $options,
  );
}
