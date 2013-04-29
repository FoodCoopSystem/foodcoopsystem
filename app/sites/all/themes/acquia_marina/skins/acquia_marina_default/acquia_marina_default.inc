<?php
/**
 * Implements hook_fusion_apply_group_PLUGIN_info().
 */
 
 function acquia_marina_fusion_apply_group_acquia_marina_default_info() {
   $groups = array();
   
   $groups['acquia_marina'] = array(
     'title' => t('Acquia Marina'),
     'description' => t('Styles for the Acquia Marina theme.'),
   );
 
   return $groups;
 }
 
 /**
  * Implements hook_fusion_apply_skin_PLUGIN_info().
  */
function acquia_marina_fusion_apply_skin_acquia_marina_default_info() {

$skins = array();

  $skins['acquia_marina_block_titles'] = array(
    'title' => t('Block Titles'),
    'type' => 'select',
    'attached' => array('css' => array('acquia-marina-default.css')),
    'default status' => TRUE,
    'group' => 'acquia_marina',
    'theme hooks' => array('block'),
    'options' => array(
      'block_title_1' => array('title' => t('Blue rounded title background with white text'), 'class' => array('marina-title-rounded-blue')),
      'block_title_2' => array('title' => t('Green rounded title background with white text'), 'class' => array('marina-title-rounded-green')),
      'block_title_3' => array('title' => t('Green title'), 'class' => array('marina-title-green')),
    ),
  );
  
  $skins['acquia_marina_block_title_icon_styles'] = array(
    'title' => t('Block Title Icons'),
    'description' => t('Place an icon in the top corner of your block title'),
    'type' => 'select',
    'attached' => array('css' => array('acquia-marina-default.css')),
    'default status' => TRUE,
    'group' => 'acquia_marina',
    'theme hooks' => array('block'),
    'options' => array(
      'icon_1' => array('title' => t('Bubble'), 'class' => array('icon-bubble', 'title-icon')),
      'icon_2' => array('title' => t('Calendar'), 'class' => array('icon-calendar', 'title-icon')),
      'icon_3' => array('title' => t('Chalkboard'), 'class' => array('icon-chalkboard', 'title-icon')),
      'icon_4' => array('title' => t('Chat bubbles'), 'class' => array('icon-chatbubbles', 'title-icon')),
      'icon_5' => array('title' => t('Color chart'), 'class' => array('icon-colorchart', 'title-icon')),
      'icon_6' => array('title' => t('Flag'), 'class' => array('icon-flag', 'title-icon')),
      'icon_7' => array('title' => t('Gears'), 'class' => array('icon-gears', 'title-icon')),
      'icon_8' => array('title' => t('Home'), 'class' => array('icon-home', 'title-icon')),
      'icon_9' => array('title' => t('Keys'), 'class' => array('icon-keys', 'title-icon')),
      'icon_10' => array('title' => t('Newspaper'), 'class' => array('icon-newspaper', 'title-icon')),
      'icon_11' => array('title' => t('Notepad'), 'class' => array('icon-notepad', 'title-icon')),
      'icon_12' => array('title' => t('Photo'), 'class' => array('icon-photo', 'title-icon')),
      'icon_13' => array('title' => t('RSS'), 'class' => array('icon-rss', 'title-icon')),
      'icon_14' => array('title' => t('Search'), 'class' => array('icon-search', 'title-icon')),
      'icon_15' => array('title' => t('Star'), 'class' => array('icon-star', 'title-icon')),
    ),
  );  

  $skins['acquia_marina_general_styles'] = array(
    'title' => t('Block Containers'),
    'type' => 'radios',
    'attached' => array('css' => array('acquia-marina-default.css')),
    'default status' => TRUE,
    'group' => 'acquia_marina',
    'theme hooks' => array('block'),
    'options' => array(
      'gradient_rounded_corner' => array('title' => t('Rounded corners with border and gradient background'), 'class' => array('marina-gradient-rounded-corners')),
      'gray_rounded_corner' => array('title' => t('Rounded corners with flat gray background'), 'class' => array('marina-gray-rounded-corners')),
    ),
  );

  $skins['acquia_marina_lists'] = array(
    'title' => t('Lists Styles'),
    'type' => 'select',
    'attached' => array('css' => array('acquia-marina-default.css')),
    'default status' => TRUE,
    'group' => 'acquia_marina',
    'options' => array(
      'lists_1' => array('title' => t('Small text list with >>'), 'class' => array('marina-list-raquo')),
      'lists_2' => array('title' => t('Large text list with green checkmarks (for Preface Top region)'), 'class' => array('marina-list-arrow-green')),  
    ),
  );  


  return $skins;
}