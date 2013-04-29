<?php
/**
 * @file
 * Administrative forms.
 */
 
 /**
  * Configuration form.
  */
function commerce_reports_tax_form($form, &$form_state) {
  $form['generate'] = array(
    '#type' => 'submit',
    '#value' => t('Generate all tax information'),
    '#submit' => array('commerce_reports_tax_form_submit_generate'),
  );

  return $form;
}

/**
 * Submit handler that generates a tax report.
 */
function commerce_reports_tax_form_submit_generate($form, &$form_state) {
  commerce_reports_tax_generate();
}

/**
 * Callback for tax report generation.
 */
function commerce_reports_tax_generate_callback() {
	commerce_reports_tax_generate();
	
	batch_process('admin/commerce/reports/tax');
}

/**
 * Returns the table containing the tax report.
 */
function commerce_reports_tax_table() {
	if (!variable_get('commerce_reports_tax_batch_finished', FALSE)) {
		drupal_set_message();
	}
	
  $rows = commerce_reports_tax();
  
  if (!variable_get('commerce_reports_tax_batch_finished', FALSE)) {
  	$rows[] = array(
      'data' => array(array('data' => t("It seems you have not yet extracted tax information from orders that occurred before enabling the tax reporting module. You will have to <a href='@generate-url'>generate</a> past data before anything will show up.", array('@generate-url' => url('admin/commerce/config/tax-reports/generate'))), 'colspan' => 3)),
    );
  }
  elseif (!$rows) {
    $rows[] = array(
      'data' => array(array('data' => t("There's currently no tax data available. You might want to <a href='@generate-url'>generate</a> past data.", array('@generate-url' => url('admin/commerce/config/tax-reports/generate'))), 'colspan' => 3)),
    );
  }
  
  return array(
    '#theme' => 'table',
    '#header' => array(t('Tax rate'), t('Taxable'), t('Taxed')),
    '#rows' => $rows,
  );
}