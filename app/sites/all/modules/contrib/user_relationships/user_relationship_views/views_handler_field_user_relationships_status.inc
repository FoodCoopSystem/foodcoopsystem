<?php

/**
 * @file
 * User Relationships Views integration.
 * Map approved column to a text label
 */
class views_handler_field_user_relationships_status extends views_handler_field {
  function construct() {
    parent::construct();
    $this->status_map = array('0' => t('pending'), '1' => t('approved'));
  }

  function render($values) {
    return isset($this->status_map[$values->{$this->table . '_approved'}]) ? $this->status_map[$values->{$this->table . '_approved'}] : t('unknown');
  }
}
