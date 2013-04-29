<?php

/**
 * @file
 * Hooks provided by Commerce Reports.
 */
 
/**
 * Add custom blocks to the dashboard provided by Commerce Reports.
 *
 * Modules that wish to add their own blocks to the dashboard can implement this hook
 * to provide metadata about this block. All content displayed on the dashboard is powered by
 * hook_block_info and hook_block_view. Each Drupal block is referenced as a section by the dashboard.
 *
 * @return
 *   An array whose keys are internal dashboard block names and whose values are arrays
 *   containing the keys:
 *     - title: A human readable, translated label for the block.
 *     - type: COMMERCE_REPORTS_DASHBOARD_BLOCK (default) if you want your block to take a third of the page,
 *       or COMMERCE_REPORTS_DASHBOARD_ROW if you want your block to stretch over a complete row.
 *     - switchSections (optional): A boolean that indicates if there should be the ability to switch between sections (see later).
 *       If TRUE, only one section will be shown at a time and there will be controls available to switch between these sections.
 *       If FALSE, all sections will be shown below each other.              
 *     - report (optional): The array describing which report or module this block belongs to. Known keys are:
 *         - title: A human readable, translated label for the module or report.
 *         - path (optional): The Drupal path to the module, as it should be given to url().
 *     - sections: The array describing which sections should be added to this block. This is where you reference the Drupal blocks
 *       that you wish to display. It returns an array whose keys are internal names for the sections and whose values are arrays
 *       containing the keys:
 *         - title: A human readable, translated label for the section.
 *         - module: The name of the module implementing the block.
 *         - block: The name of the block.
 *     - weight (optional): To determine where the block should be located.
 *
 * @see hook_block_info
 * @see hook_block_view
 */
function hook_commerce_reports_dashboard() {
  return array(
    'sales' => array(
      'title' => t('Sales'),
      'type' => COMMERCE_REPORTS_DASHBOARD_ROW,
      'switchSections' => TRUE,
      'report' => array(
        'title' => t('Sales reports'),
        'path' => 'admin/commerce/reports/sales',
      ),
      'sections' => array(
        'year' => array(
          'title' => 'Year',
          'module' => 'views',
          'block' => 'cc437fbe6b867b448dc946fd925800a3',
        ),
        'month' => array(
          'title' => 'Month',
          'module' => 'views',
          'block' => '1127e4706efe2c1eb8537a65a644e572',
        ),
        'week' => array(
          'title' => 'Week',
          'module' => 'views',
          'block' => 'd70fc459675538d56c73a9f90574211a',
        ),
      ),
      'weight' => 100,
    ),
  );
}

/**
 * Allow modules to alter the dashboard.
 */
function hook_commerce_reports_dashboard_alter(&$info) {
  $info['overview_today']['weight'] = 90;
}
