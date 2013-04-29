<?php

/**
 * @file
 * API documentation for user_relationship_blocks module.
 */

/**
 * Return the user that is currently displayed.
 *
 * @param $delta
 *   The delta of the currently viewed block.
 *
 * @return
 *   The uid of the user currently displayed on this page, if any.
 *
 */
function hook_user_relationship_blocks_get_uid($delta) {
  if (arg(0) == 'mypath') {
    return arg(1);
  }
}
