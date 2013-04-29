<?php

/**
 * @file
 * Role Expire module migration support
 */

function role_expire_migrate_fields_role($type) {
  $fields = array(
    'duration' => t('Role Expire: Duration (in days)'),
  );

  return $fields;
}

/**
 * Implementation of hook_migrate_prepare_role().
 */
function role_expire_migrate_complete_role(&$newrole, $tblinfo, $row) {
  if (isset($newrole['duration'])) {
    role_expire_set_default_duration($newrole['rid'], $newrole['duration']);
  }
}

/**
 * Implementation of hook_migrate_delete_role().
 */
function role_expire_migrate_delete_role($rid) {
  role_expire_delete_default_duration($rid);
}
