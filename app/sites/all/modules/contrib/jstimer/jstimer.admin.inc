<?php


/**
 * @file
 * countdowntimer.admin.inc -contains all admin pages, settings, and validate.
 */


/**
 * Settings form for menu callback
 */
function jstimer_admin_settings() {

  $form = array();

  $form['jstimer_js_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Javascript'),
    '#collapsible' => TRUE,
  );
  $form['jstimer_js_settings']['jstimer_js_load_option'] = array(
    '#type' => 'radios',
    '#title' => t('Javascript load options'),
    '#default_value' => variable_get('jstimer_js_load_option', DEFAULT_JS_LOAD),
    '#options' => array(0 => t('Nodes only'), 1 => t('Every page'), 2 => t('Page lists')),
    '#description' => t("Node view will only load when a single node is viewed.  Every page, is well, EVERY PAGE.  Page list will use the lists below."),
  );
  $form['jstimer_js_settings']['jstimer_js_load_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Javascript include page list'),
    '#default_value' => variable_get('jstimer_js_load_pages', ''),
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#rows' => 1,
  );
  $form['jstimer_js_settings']['jstimer_js_exclude_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Javascript exclude page list'),
    '#default_value' => variable_get('jstimer_js_exclude_pages', ''),
    '#description' => t("The pages listed here will not load the jstimer code regardless of the settings above. Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#rows' => 1,
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'jstimer_admin_settings_submit';  // add submit handler
  return $form;
}

function jstimer_admin_settings_submit($form, &$form_state) {
  // build timer.js file with new settings.
  jstimer_build_js_cache();
}

function jstimer_admin_settings_validate($form, &$form_state) {
  // Form submitted with single quotes
  foreach ($form_state['values'] as $key => $value) {
    if ( substr($key, 0, 15) == "countdowntimer_" ) {
      if ( ! strpos($value, "'") === FALSE ) {
        form_set_error($key, t("Javascript Timer admin settings may not contain single quotes(')."));
        drupal_goto('admin/settings/countdowntimer');
      }
    }
  }
}
