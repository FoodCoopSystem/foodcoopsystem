<?php

/**
 * @file
 * integration with Author Pane for User Relationships API
 * @author Alex Karshakevich http://drupal.org/user/183217
 */

/**
 * Implements hook_preprocess_author_pane().
 */
function user_relationships_preprocess_author_pane(&$variables) {
  // Check if this preprocess needs to be run given who's calling it.
  if (function_exists('author_pane_run_preprocess') && !author_pane_run_preprocess('user_relationships', $variables['caller'])) {
    return;
  }

  global $user;
  $account = $variables['account'];

  // Anonymous users and users viewing their own account won't get a link.
  if (!$user->uid || !$account->uid || $user->uid == $account->uid) {
    return;
  }

  $rtids = variable_get('user_relationships_author_pane_rtids', array());
  if (!is_array($rtids)) {
    $rtids = array($rtids);//wrap into array for now, while selection is single
  }
  if (!count($rtids)) {
    return;
  }
  // Get a list of selected relationship types
  $all_rtypes = user_relationships_types_load();
  $rtypes = array();
  foreach ($rtids as $rtid) {
    $rtypes[] = $all_rtypes[$rtid];
  }

  foreach ($rtypes as $rtype) {
    $relationships = user_relationships_load(array('requester_id' => $user->uid, 'requestee_id' => $account->uid, 'rtid' => $rtype->rtid));

    if (count($relationships)) {
      // Existing relationship; need remove icon/link. (Despite the foreach,
      // there should only be one.)
      foreach ($relationships as $relationship) {
        $link_label = t('Remove @name from @rel_plural_name', array('@name' => format_username($account), '@rel_plural_name' => $relationship->plural_name));
        $css_class = array(str_replace(' ', '-', $relationship->name), 'author-pane-link', 'user_relationships_popup_link', 'author-relationship-remove-icon');
        //link to remove
        if (!isset($variables['user_relationships'])) {
          $variables['user_relationships'] = '';
        }
        $variables['user_relationships'] .= '<div class="author-pane-ur-link-item">';
        $variables['user_relationships'] .=
          l('<span>' . t('Remove @rel_name', array('@name' => format_username($account), '@rel_name' => ur_tt("user_relationships:rtid:$relationship->rtid:name", $relationship->name))) . '</span>',
          "user/{$user->uid}/relationships/{$relationship->rid}/remove",
          array(
                'query' => drupal_get_destination(),
                'html' => TRUE,
                'attributes'  => array('title' => $link_label, 'class' => $css_class),
          ));
        $variables['user_relationships'] .= '</div>';
      }
    }
    else {
      //No existing relationship; need an add icon/link.
      if (!user_relationships_can_request($user, $rtype) || !user_relationships_can_receive($account, $rtype)) {
        continue;
      }
      $css_class = array(str_replace(' ', '-', $rtype->name), 'author-pane-link', 'user_relationships_popup_link', 'author-relationship-remove-icon');
      //add link
      if (!isset($variables['user_relationships'])) {
        $variables['user_relationships'] = '';
      }
      $variables['user_relationships'] .= '<div class="author-pane-ur-link-item">';
      $variables['user_relationships'] .=
        l('<span>' . t('Add @rel_name', array('@name' => format_username($account), '@rel_name' => ur_tt("user_relationships:rtid:$rtype->rtid:name", $rtype->name))) . '</span>',
          "relationship/{$account->uid}/request/{$rtype->rtid}",
          array(
            'query' => drupal_get_destination(),
            'html' => TRUE,
            'attributes'  => array(
              'title' => t('Become @name\'s @rel_name', array('@name' => format_username($account), '@rel_name' => ur_tt("user_relationships:rtid:$rtype->rtid:name", $rtype->name))),
              'class' => $css_class
            ),
          )
        );
      $variables['user_relationships'] .= '</div>';
    }
  }
}

/**
 * Implements hook_author_pane_allow_preprocess_disable().
 */
function user_relationships_author_pane_allow_preprocess_disable() {
  return array('user_relationships' => 'User Relationships');
}
