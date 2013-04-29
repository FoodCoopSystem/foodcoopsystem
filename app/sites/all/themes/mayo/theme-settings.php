<?php

/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * Custom theme settings
 */
function mayo_form_system_theme_settings_alter(&$form, &$form_state) {

  drupal_add_js(drupal_get_path('theme', 'mayo') . '/js/mayo.js');

  /*--------------- Font settings --------------*/
  $form['font'] = array(
    '#type' => 'fieldset',
    '#title' => t('Font settings'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  $form['font']['base_font_size'] = array(
    '#type' => 'select',
    '#title' => t('Base font size'),
    '#default_value' => theme_get_setting('base_font_size'),
    '#options' => array(
      '75%'    => '75% (=12px)',
      '81.25%' => '81.25% (=13px)',
      '87.5%'  => '87.5% (=14px)',
      '93.75%' => '93.75% (=15px)',
      '100%'   => '100% (=16px)',
      '112.5%' => '112.5% (=18px)'
    ),
    '#description' => t('To support text size enlargement/reduction, percent ratio based on the browser\'s regular font size (which is mostly 16px) is used.'),
  );
  $form['font']['base_font_family'] = array(
    '#type' => 'select',
    '#title' => t('Base font family'),
    '#default_value' => theme_get_setting('base_font_family'),
    '#options' => array(
      0 => t('Serif: Georgia, Palatino Linotype, Book Antiqua, URW Palladio L, Baskerville, serif'),
      1 => t('Sans-Serif: Verdana, Geneva, Arial, Bitstream Vera Sans, DejaVu Sans, sans-serif'),
      2 => t('Custom'),
    ),
    '#description' => t('Font used for most part of the contents.'),
  );
  $form['font']['base_custom_font_family'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom base font family'),
    '#default_value' => theme_get_setting('base_custom_font_family'),
    '#size' => 80,
    '#description' => t('Enter the base font-family you want to use. No need to start with <b>font-family:</b> and end with <b>;</b>. Just enter comma separated font names.'),
    '#prefix' => '<div id="base-custom-font-family-wrapper">',
    '#suffix' => '</div>',
  );
  $form['font']['heading_font_family'] = array(
    '#type' => 'select',
    '#title' => t('Heading font family (except for the site name and slogan)'),
    '#default_value' => theme_get_setting('heading_font_family'),
    '#options' => array(
      0 => t('Serif: Georgia, Palatino Linotype, Book Antiqua, URW Palladio L, Baskerville, serif'),
      1 => t('Sans-Serif: Verdana, Geneva, Arial, Bitstream Vera Sans, DejaVu Sans, sans-serif'),
      2 => t('Custom'),
    ),
    '#description' => t('Font used for the headings (h1, h2, h3, h4, h5). Font used for the site name and slogan can not be changed here. If you want to change it, please manually edit style.css in the theme\'s css subdirectory.'),
  );
  $form['font']['heading_custom_font_family'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom heading font family'),
    '#default_value' => theme_get_setting('heading_custom_font_family'),
    '#size' => 80,
    '#description' => t('Enter the font-family you want to use for the headings. No need to start with <b>font-family:</b> and end with <b>;</b>. Just enter comma separated font names.'),
    '#prefix' => '<div id="heading-custom-font-family-wrapper">',
    '#suffix' => '</div>',
  );

  /*--------------- Layout settings --------------*/
  $form['layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout settings'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  $form['layout']['base_vmargin'] = array(
    '#type' => 'textfield',
    '#title' => t('Base vertical (top/bottom) margin'),
    '#default_value' => theme_get_setting('base_vmargin'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the base vertical (top/bottom) margin which is vertical spaces between page edge and browser screen in px.'),
    '#prefix' => '<img src="/' . drupal_get_path('theme', 'mayo') . '/images/base-layout.png" /><br />',
  );
  $form['layout']['page_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Page width'),
    '#default_value' => theme_get_setting('page_width'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the page width including sidebars either in percent ratio (50-100%) for liquid layout, or in px (700-1600px) for fixed layout. If an invalid value is specified, the default value (90%) is used instead. You can leave this field blank to use the default value. Do not forget to add either % or px after the number.'),
    '#element_validate' => array('mayo_page_width_validate'),
  );
  $form['layout']['page_margin'] = array(
    '#type' => 'textfield',
    '#title' => t('Page margin'),
    '#default_value' => theme_get_setting('page_margin'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the page margin which is spaces between page edge and contents in px.'),
  );
  $form['layout']['layout_style'] = array(
    '#type' => 'radios',
    '#title' => t('Layout style'),
    '#default_value' => theme_get_setting('layout_style'),
    '#options' => array(
      1 => t('1. Apply page margin to all (header, footer and main contents).'),
      2 => t('2. Apply page margin to main contents only.'),
    ),
    '#description' => '<img src="/' . drupal_get_path('theme', 'mayo') . '/images/page-layout.png" /><br />' . t('When the layout 2 is selected, or header background image is selected, header borders are not drawn to make it looks better.'),
  );

  /*--------------- Advanced sidebar settings --------------*/
  $form['layout']['sidebar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sidebar layout settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['layout']['sidebar']['sidebar_layout_style'] = array(
    '#type' => 'radios',
    '#title' => t('Sidebar layout style'),
    '#default_value' => theme_get_setting('sidebar_layout_style'),
    '#options' => array(
      1 => t('1. Sidebar first comes left, sidebar second comes right.'),
      2 => t('2. Both sidebars come left.'),
      3 => t('3. Both sidebars come right.'),
    ),
    '#prefix' => '<img src="/' . drupal_get_path('theme', 'mayo') . '/images/sidebar-layout.png" />',
  );
  $form['layout']['sidebar']['sidebar_first_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Sidebar first width'),
    '#default_value' => theme_get_setting('sidebar_first_width'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the width of the sidebar first in % (15-40%). px value can not be used.'),
    '#element_validate' => array('mayo_sidebar_width_validate'),
  );
  $form['layout']['sidebar']['sidebar_second_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Sidebar second width'),
    '#default_value' => theme_get_setting('sidebar_second_width'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the width of the sidebar first in % (15-40%). px value can not be used.'),
    '#element_validate' => array('mayo_sidebar_width_validate'),
  );
  $form['layout']['sidebar']['note'] = array(
    '#type' => 'item',
    '#title' => t('Note:'),
    '#markup' => t('Main contents width is automatically determined based on the width of the sidebar and number of sidebar used.'),
  );


  /*--------------- Style settings --------------*/
  $form['style'] = array(
    '#type' => 'fieldset',
    '#title' => t('Style settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['style']['round_corners'] = array(
    '#type' => 'select',
    '#title' => t('Content box round corners'),
    '#default_value' => theme_get_setting('round_corners'),
    '#description' => t('Make the corner of sidebar block and/or node rounded.<br/><b>Note:</b> This feature does not work with IE. Currently, it works with Safari, Firefox, Opera, Google Chrome.'),
    '#options' => array(
      0 => t('No round corners'),
      1 => t('Sidebar block only'),
      2 => t('Node only'),
      3 => t('Both sidebar block and node'),
    ),
    '#suffix' => '<img src="/' . drupal_get_path('theme', 'mayo') . '/images/round-corners.png" /><br />',
  );

  $form['style']['menubar_style'] = array(
    '#type' => 'radios',
    '#title' => t('Menubar style'),
    '#default_value' => theme_get_setting('menubar_style'),
    '#options' => array(
      1 => t('1. Normal (based on the colors specified by the color set)'),
      2 => t('2. Gloss black image background.'),
    ),
    '#suffix' => '<img src="/' . drupal_get_path('theme', 'mayo') . '/images/menubar-type.png" />',
  );
  $form['style']['note'] = array(
    '#type' => 'item',
    '#title' => t('Note:'),
    '#markup' => t('When the menubar type 2 is selected, the menu text color, menu highlight color, menu divier color from the color set are ignored and the fixed colors that match to the menubar are used instead.  Besides, highlight color and menu divider color from the color set are still used for other places such as tabs and sub-menubar for superfish and nice_menus menu.'),
  );

  /*--------------- Advanced header settings --------------*/
  $form['adv_header'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced header settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['adv_header']['header_searchbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add search form to the header'),
    '#default_value' => theme_get_setting('header_searchbox'),
    '#description' => t('Check here if you want to add search form block to the right side of the header.'),
  );
  $form['adv_header']['header_fontsizer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add font resizing controls'),
    '#default_value' => theme_get_setting('header_fontsizer'),
    '#description' => t('Check here if you want to add font resizing controls at side of the header.'),
  );
  $form['adv_header']['header_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Header height'),
    '#default_value' => theme_get_setting('header_height'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the header height in px.'),
    '#prefix' => '<img src="/' . drupal_get_path('theme', 'mayo') . '/images/header-layout.png" /><br />',
  );
  $form['adv_header']['header_border_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Header border width'),
    '#default_value' => theme_get_setting('header_border_width'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the header border width in px. Note that header border is not drawn when you use header background image or when you use layout style 2.'),
  );
  $form['adv_header']['logo_left_margin'] = array(
    '#type' => 'textfield',
    '#title' => t('Logo left margin'),
    '#default_value' => theme_get_setting('logo_left_margin'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the left margin of the logo in px. This setting is used only when the logo option is enabled.'),
  );
  $form['adv_header']['logo_top_margin'] = array(
    '#type' => 'textfield',
    '#title' => t('Logo top margin'),
    '#default_value' => theme_get_setting('logo_top_margin'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the top margin of the logo in px. This setting is used only when the logo option is enabled.'),
  );
  $form['adv_header']['sitename_left_margin'] = array(
    '#type' => 'textfield',
    '#title' => t('Site name left margin'),
    '#default_value' => theme_get_setting('sitename_left_margin'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the left margin of the site name in px. This setting is used only when the sitename option is enabled.'),
  );
  $form['adv_header']['sitename_top_margin'] = array(
    '#type' => 'textfield',
    '#title' => t('Site name top margin'),
    '#default_value' => theme_get_setting('sitename_top_margin'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the top margin of the site name in px. This setting is used only when the sitename option is enabled.'),
  );
  $form['adv_header']['searchbox_right_margin'] = array(
    '#type' => 'textfield',
    '#title' => t('Search form right margin'),
    '#default_value' => theme_get_setting('searchbox_right_margin'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the right margin of the search form in px. This setting is used only when the header search form option is enabled.'),
  );
  $form['adv_header']['searchbox_top_margin'] = array(
    '#type' => 'textfield',
    '#title' => t('Search form top margin'),
    '#default_value' => theme_get_setting('searchbox_top_margin'),
    '#size' => 12,
    '#maxlength' => 8,
    '#description' => t('Specify the right margin of the search form in px. This setting is used only when the header search form option is enabled.'),
  );
  $form['adv_header']['searchbox_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Search form textfield width'),
    '#default_value' => theme_get_setting('searchbox_size'),
    '#size' => 10,
    '#maxlength' => 6,
    '#description' => t('Specify the width of the text field of the search forms in characters. This size is also applied for the search form in a block. NOTE: do not add px since this is not px size.'),
  );
  $form['adv_header']['header_bg_file'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the header background image'),
    '#default_value' => theme_get_setting('header_bg_file'),
    '#description' => t('If the background image is bigger than the header area, it is clipped. If it\'s smaller than the header area, it is tiled to fill the header area. To remove the background image, blank this field and save the settings.'),
    '#size' => 40,
    '#maxlength' => 120,
  );
  $form['adv_header']['header_bg'] = array(
    '#type' => 'file',
    '#title' => t('Upload header background image'),
    '#size' => 40,
    '#attributes' => array('enctype' => 'multipart/form-data'),
    '#description' => t('If you don\'t jave direct access to the server, use this field to upload your header background image'),
    '#element_validate' => array('mayo_header_bg_validate'),
  );
  $form['adv_header']['header_bg_alignment'] = array(
    '#type' => 'select',
    '#title' => t('Header backgeround image alignment'),
    '#default_value' => theme_get_setting('header_bg_alignment'),
    '#description' => t('Select the alignment of the header background image.'),
    '#options' => array(
      'top left' => t('Top left'),
      'top center' => t('Top center'),
      'top right' => t('Top right'),
      'center left' => t('Center left'),
      'center center' => t('Center center'),
      'center right' => t('Center right'),
      'bottom left' => t('Bottom left'),
      'bottom center' => t('Bottom center'),
      'bottom right' => t('Bottom right'),
    ),
  );
  $form['adv_header']['header_watermark'] = array(
    '#type' => 'select',
    '#title' => t('Header watermark'),
    '#default_value' => theme_get_setting('header_watermark'),
    '#description' => t('Select the watermark you want from the list below. The sample below is scaled down and the actual size of the watermark is bigger.'),
    '#options' => array(
      0 => t('-None-'),
      1 => t('Pixture'),
      2 => t('Wave'),
      3 => t('Bubble'),
      4 => t('Flower'),
      5 => t('Star'),
      6 => t('Metal'),
    ),
    '#suffix' => '<img src="/' . drupal_get_path('theme', 'mayo') . '/images/watermark-sample.png" /><br />',
  );

  /*--------------- Misellanenous settings --------------*/
  $form['misc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Miscellaneous settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['misc']['display_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('display_breadcrumb'),
    '#description' => t('Check here if you want to display breadcrumb.'),
  );
  $form['misc']['dark_messages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use dark message colors'),
    '#default_value' => theme_get_setting('dark_messages'),
    '#description' => t('Check here if you use the dark color set. Colors for the status/warning/error messages are adjusted.'),
  );

}

/**
 * Check and save the uploaded header background image
 */
function mayo_header_bg_validate($element, &$form_state) {
  global $base_url;

  $validators = array('file_validate_is_image' => array());
  $file = file_save_upload('header_bg', $validators, "public://", FILE_EXISTS_REPLACE);

  if ($file) {
    // change file's status from temporary to permanent and update file database
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);

    $file_url = file_create_url($file->uri);
    $file_url = str_ireplace($base_url, '', $file_url);

    // set to form
    $form_state['values']['header_bg_file'] = $file_url;
  }
}

/**
 * Validate page width
 */
function mayo_page_width_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    $width = $element['#value'];

    // check if it is liquid (%) or fixed width (px)
    if(preg_match("/(\d+)\s*%/", $width, $match)) {
      $num = intval($match[0]);
      if(50 <= $num && $num <= 100) {
        return;
      }
      else {
        form_error($element, t('The width for the liquid layout must be a value between 50% and 100%.'));
      }
    }
    else if(preg_match("/(\d+)\s*px/", $width, $match)) {
      $num = intval($match[0]);
      if(700 <= $num && $num < 1600) {
        return;
      }
      else {
        form_error($element, t('The width for the fixed layout must be a value between 700px and 1600px.'));
      }
    }
  }
}

/**
 * Validate sidebar width
 */
function mayo_sidebar_width_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    $width = $element['#value'];

    // check if it is liquid (%) or fixed width (px)
    if(preg_match("/(\d+)\s*%/", $width, $match)) {
      $num = intval($match[0]);
      if(15 <= $num && $num <= 40) {
        return;
      }
      else {
        form_error($element, t('The width of the sidebar must be a value between 15% and 40%.'));
      }
    }
    else if(preg_match("/(\d+)\s*px/", $width, $match)) {
      form_error($element, t('The width of the sidebar must be a value between 15% and 40%. Do not forget to add % character.'));
    }
  }
}
