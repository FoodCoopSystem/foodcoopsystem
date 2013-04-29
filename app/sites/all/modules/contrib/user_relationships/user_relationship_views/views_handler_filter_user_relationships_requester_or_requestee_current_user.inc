<?php

/**
 * @file
 * Filter handler for the current user
 * @author pedrofaria http://drupal.org/user/27805
 */
class views_handler_filter_user_relationships_requester_or_requestee_current_user extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t('Is the logged in user');
  }

  function query() {
    $this->ensure_my_table();
    $op = empty($this->value) ? '<>' : '=';
    $where = "($this->table_alias.requester_id $op ***CURRENT_USER*** OR $this->table_alias.requestee_id $op ***CURRENT_USER***)";
    $this->query->add_where_expression($this->options['group'], $where);
  }
}

