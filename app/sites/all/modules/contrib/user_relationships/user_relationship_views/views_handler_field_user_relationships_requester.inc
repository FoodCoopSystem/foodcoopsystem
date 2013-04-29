<?php

/**
 * @file
 * User Relationships Views integration.
 * Link to user profile of relationship requester
 */
class views_handler_field_user_relationships_requester extends views_handler_field_user_link {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'requester_id';
  }

  function render($values) {
    $uid = $values->{$this->aliases['requester_id']};
    $requester = user_load($uid);
    return theme('username', array('account' => $requester));
  }

}
