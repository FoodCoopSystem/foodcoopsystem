<?php
/**
 * @file
 * Sort handler for views_calc module.
 */
class views_calc_handler_sort extends views_handler_sort {
  function query() {
    $result = _views_calc_fields();
    while ($result as $calc_field) {
      if ($this->field == "cid" . $calc_field->cid) {
        foreach (explode(',', $calc_field->tablelist) as $table) {
          $this->view->query->add_table($table);
        }
        $this->view->query->add_orderby(NULL, "({$calc_field->calc})", $this->options['order'], "cid" . $calc_field->cid);
        return;
      }
    }
  }
}
