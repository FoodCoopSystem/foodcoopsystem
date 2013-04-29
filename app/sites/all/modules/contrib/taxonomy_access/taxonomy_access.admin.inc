<?php
/**
 * @file
 * Administrative interface for taxonomy access control.
 */

/**
 * Renders the TAC permissions administration overview page.
 * @return
 *     Form to render.
 */
function taxonomy_access_admin() {
  $roles = _taxonomy_access_user_roles();
  $active_rids = db_query('SELECT rid FROM {taxonomy_access_default} WHERE vid = 0')->fetchCol();

  $header = array(t('Role'), array('data' => t('Operations'), 'colspan' => 2));
  $rows = array();

  foreach ($roles as $rid => $name) {
    $row = array();
    $row[] = $name;

    if (in_array($rid, $active_rids)) {
      // Add edit operation link for active roles.
      $row[] = array('data' => l(t("edit access rules"), TAXONOMY_ACCESS_CONFIG . "/role/$rid/edit"));

      // Add disable operation link for custom roles.
      if ($rid != DRUPAL_ANONYMOUS_RID && $rid != DRUPAL_AUTHENTICATED_RID) {
        $row[] = array('data' => l(t("disable access control"), TAXONOMY_ACCESS_CONFIG . "/role/$rid/delete"));
      }
      else {
        $row[] = array('data' => '');
      }
    }
    else {
      // Add enable link for unconfigured roles.
      $row[] = array('data' => '');
      $row[] = array('data' => l(t("enable access control"), TAXONOMY_ACCESS_CONFIG . "/role/$rid/edit"));
    }

    $rows[] = $row;
  }

  $build['role_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $build;
}

/**
 * Form callback to delete all access rules for a particular role.
 *
 * @param $rid
 *   The role ID to disable.
 */
function taxonomy_access_admin_delete_role($form, &$form_state, $rid) {
  $roles = _taxonomy_access_user_roles();
  $role_global_default = db_query(
    'SELECT grant_view, grant_update, grant_delete
     FROM {taxonomy_access_default}
     WHERE vid = 0 AND rid = :rid', array(':rid' => $rid))
    ->fetchAssoc();
  if ($role_global_default) {
    $form['rid'] = array(
      '#type' => 'value',
      '#value' => $rid,
    );

    return confirm_form($form,
      t('Are you sure you want to delete all taxonomy access rules for the role %role?',
        array('%role' => $roles[$rid])),
      TAXONOMY_ACCESS_CONFIG,
      t('This action cannot be undone.'),
      t('Delete all'),
      t('Cancel'));
  }
}

/**
 * Submit handler for taxonomy_access_admin_delete_role.
 *
 * @param $rid
 *   The role id to disable.
 *
 * @see
 *   taxonomy_access_admin_delete_role()
 */
function taxonomy_access_admin_delete_role_submit($form, &$form_state) {
  $roles = _taxonomy_access_user_roles();
  $rid = $form_state['values']['rid'];
  if (is_numeric($rid)
      && $rid != DRUPAL_ANONYMOUS_RID
      && $rid != DRUPAL_AUTHENTICATED_RID) {
    if ($form_state['values']['confirm']) {
      $form_state['redirect'] = TAXONOMY_ACCESS_CONFIG;
      taxonomy_access_delete_role_grants($rid);
      drupal_set_message(t('All taxonomy access rules deleted for role %role.',
          array('%role' => $roles[$rid])));
    }
  }
}

/**
 * Assembles a row of grant options for a term or default on the admin form.
 *
 * @param (array) $grants
 *   (optional) An array of grants to use as form defaults.  Defaults to NULL.
 */
function taxonomy_access_admin_build_row(array $grants = NULL) {
  $form['#tree'] = TRUE;
  foreach (array('view', 'update', 'delete') as $grant) {
    $form[$grant] = array(
      '#type' => 'radios',
      // 1: Allow, 0: Ignore, 2: Deny
      '#options' => array('1' => '', '0' => '', '2' => ''),
      '#default_value' => is_string($grants['grant_' . $grant]) ? $grants['grant_' . $grant] : '0',
      '#required' => TRUE,
    );
  }
  foreach (array('create', 'list') as $grant) {
    $form[$grant] = array(
      '#type' => 'checkbox',
      '#default_value' => is_string($grants['grant_' . $grant]) ? $grants['grant_' . $grant] : '0',
    );
  }
  return $form;
}

/**
 * Form for managing grants by role.
 *
 * @param $rid
 *   The role ID.
 */
function taxonomy_access_admin_form($form, $form_state, $rid) {
  // Disable list filtering while preparing this form.
  taxonomy_access_disable_list();

  $roles = _taxonomy_access_user_roles();

  // Fetch all default grants.
  $default_grants = db_query('SELECT * FROM {taxonomy_access_default} WHERE rid = :rid', array(':rid' => $rid))->fetchAllAssoc('vid', PDO::FETCH_ASSOC);

  // If we are adding a role, no global default is set yet, so insert it now.
  if (empty($default_grants[0]) && isset($rid)) {
    // Assemble a $row object for Schema API.
    $row = new stdClass();
    $row->vid = 0;
    $row->rid = $rid;

    // Insert the row with defaults for all grants.
    drupal_write_record('taxonomy_access_default', $row);

    drupal_set_message(t('Access control for role %role enabled.',
      array('%role' => $roles[$rid])));
  }

  // Fetch all grants.
  $grants = db_query('SELECT * FROM {taxonomy_access_term} WHERE rid = :rid', array(':rid' => $rid))->fetchAllAssoc('tid', PDO::FETCH_ASSOC);

  $form['instructions'] = array(
    '#markup' => _taxonomy_access_admin_instructions_html(),
    '#weight' => '20',
  );
  $form['rid'] = array('#type' => 'value', '#value' => $rid);
  $form['grants'] = $form['selected_terms'] = $form['selected_defaults'] = array('#tree' => TRUE);

  // Global default.
  $global_defaults = empty($default_grants[0]) ? NULL : $default_grants[0];
  $form['vocabs'][0]['#title'] = 'Global';
  $form['grants'][0][0] = taxonomy_access_admin_build_row($global_defaults);
  $form['selected_defaults'][0] = array(
     '#type' => 'checkbox',
     '#disabled' => TRUE,
     '#title' => t('<em>default</em>'),
     '#description' => t("can't be disabled without disabling TAC for this role"),
  );

  foreach (taxonomy_get_vocabularies() as $vid => $vocabulary) {
    $form['vocabs'][$vid]['#title'] = check_plain($vocabulary->name);
    if (isset($default_grants[$vid])) {
      $form['grants'][$vid][0] = taxonomy_access_admin_build_row($default_grants[$vid]);
      $form['selected_defaults'][$vid] = array(
         '#type' => 'checkbox',
         '#title' => t('<em>default</em>'),
      );
    }
    else {
      $add_items[$vocabulary->name]["default $vid"] = t('*default*');
    }

    if ($tree = taxonomy_get_tree($vid)) {
      foreach ($tree as $term) {
        if (isset($grants[$term->tid])) {
          $form['grants'][$vid][$term->tid] = taxonomy_access_admin_build_row($grants[$term->tid]);
          $form['selected_terms'][$term->tid] = array(
             '#type' => 'checkbox',
             '#title' => str_repeat('&nbsp;&nbsp;', $term->depth) . check_plain($term->name),
          );
        }
        else {
          $add_items[$vocabulary->name]["term $term->tid"] = str_repeat('-', $term->depth) . check_plain($term->name);
        }
      }
    }
  }
  // New grant row.
  if (isset($add_items)) {
    $form['new']['grants'] = taxonomy_access_admin_build_row();
    $form['new']['#tree'] = TRUE;
    $form['new']['item'] = array(
      '#type' => 'select',
      '#options' => $add_items,
    );
    $form['new']['recursive'] = array(
      '#type' => 'checkbox',
      '#title' => t('with children'),
      '#description' => t('Add child terms recursively with these values.'),
    );
    $form['new']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
    );
  }

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete selected'),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save all'),
  );

  return $form;
}

/**
 * Renders the permission matrix user form for a given user role.
 *
 * @return
 *     String containing rendered HTML form in table.
 */
function theme_taxonomy_access_admin_form($variables) {
  $form = $variables['form'];

  $roles = _taxonomy_access_user_roles();
  $header = array(
    array( 'data' => t('Category'), 'colspan' => 3),
    array( 'data' => t('View'), 'colspan' => 4),
    array( 'data' => t('Update'), 'colspan' => 4),
    array( 'data' => t('Delete'), 'colspan' => 4),
    array( 'data' => t('Add Tag')),
    array( 'data' => t('View Tag')),
  );
  $sub_header = array(
    '&nbsp;<strong>' . t('<acronym title="Allow">A</acronym>') . '</strong>',
    '&nbsp;<strong>' . t('<acronym title="Ignore">I</acronym>') . '</strong>',
    '&nbsp;<strong>' . t('<acronym title="Deny">D</acronym>') . '</strong>',
    '&nbsp;',
  );
  $sub_header = array_merge(array('&nbsp;'), $sub_header, $sub_header, $sub_header);
  $sub_header = array_pad($sub_header, 15, '&nbsp;');
  $node_grant_types = array('view', 'update', 'delete');

  $radios = array('1' => t('Allow'), '0' => t('Ignore'), '2' => t('Deny'));

  $rows = array();

  foreach (array_keys($form['vocabs']) as $vid) {
    if (is_numeric($vid) AND isset($form['grants'][$vid])) {
      $row = $sub_header;
      $row[0] = array('data' => '<h3>' . check_plain($form['vocabs'][$vid]['#title']) . '</h3>', 'colspan' => 3);
      $rows[] = $row;
      foreach (array_keys($form['grants'][$vid]) as $tid) {
        if (is_numeric($tid)) {
          $select_key = $tid? 'selected_terms' : 'selected_defaults';
          $select_id = $tid? $tid : $vid;
          $row = array(
            array('data' => drupal_render($form[$select_key][$select_id]), 'colspan' => 3),
          );
          foreach ($node_grant_types as $grant) {
            foreach (array_keys($radios) as $key) {
              // I need this hack to display radio buttons horizontally (instead of standard form 'radios')
              $row[] = array('data' => drupal_render($form['grants'][$vid][$tid][$grant][$key]));
            }
            $row[] = '&nbsp;';
          }
          foreach (array('create', 'list') as $grant) {
            $row[] = array('data' => drupal_render($form['grants'][$vid][$tid][$grant]));
          }
          $rows[] = $row;
        }
      }
    }
  }
  if (isset($form['new'])) {
    $row = $sub_header;
    $row[0] = array('data' => '<h3>' . t('New') . '</h3>', 'colspan' => 3);
    $rows[] = $row;

    $row = array(
      array(
        'data' => drupal_render($form['new']['item'])
        . drupal_render($form['new']['recursive']),
        'colspan' => '2',
      ),
      drupal_render($form['new']['add']),
    );
    foreach ($node_grant_types as $grant) {
      foreach (array_keys($radios) as $key) {
        // I need this hack to display radio buttons horizontally (instead of standard form 'radios')
        $row[] = array('data' => drupal_render($form['new']['grants'][$grant][$key]));
      }
      $row[] = '&nbsp;';
    }
    foreach (array('create', 'list') as $grant) {
      $row[] = array('data' => drupal_render($form['new']['grants'][$grant]));
    }
    $rows[] = $row;
    $row = array();
  }


  $output = '';
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Submit handler for the administration form at TAXONOMY_ACCESS_CONFIG.
 */
function taxonomy_access_admin_form_submit($form, &$form_state) {
  // Save some typing.
  $values = $form_state['values'];

  switch ($values['op']) {

    case t('Delete selected'):
      _taxonomy_access_admin_form_delete($values['rid'], $values);
      break;

    case t('Add'):
      _taxonomy_access_admin_form_add($values['rid'], $values);
      break;

    case t('Save all'):
      _taxonomy_access_admin_form_save_all($values['rid'], $values, $form);
      $form_state['redirect'] = TAXONOMY_ACCESS_CONFIG;
      break;
  }
}

/**
 * Helper to process the "Delete selected" button on the administration form.
 *
 * @param $rid
 *   The role id for the submission.
 * @param $values
 *   The $form_state['values'] from the form submission.
 */
function _taxonomy_access_admin_form_delete($rid, $values) {
  if (isset($values['selected_terms']) && is_array($values['selected_terms'])) {
    $disable_tids = array();
    foreach ($values['selected_terms'] as $tid => $enabled) {
      if ($enabled) {
        $disable_tids[] = $tid;
      }
    }

    if (!empty($disable_tids)) {
      taxonomy_access_delete_term_grants($disable_tids, $rid);
      unset($disable_tids);
    }
  }

  if (isset($values['selected_defaults']) && is_array($values['selected_defaults'])) {
    $disable_vids = array();
    foreach ($values['selected_defaults'] as $vid => $enabled) {
      if ($enabled) {
        $disable_vids[] = $vid;
      }
    }

    if (!empty($disable_vids)) {
      taxonomy_access_delete_default_grants($disable_vids, $rid);
      unset($disable_tids);
    }
  }
}

/**
 * Helper to process the "Add" button on the administration form.
 *
 * @param $rid
 *   The role id for the submission.
 * @param $values
 *   The $form_state['values'] from the form submission.
 */
function _taxonomy_access_admin_form_add($rid, $values) {
  $new = $values['new'];
  list($type, $id) = explode(' ', $new['item']);
  $rows = array();

  if ($type == 'term') {
    $rows[$id] =
      _taxonomy_access_format_grant_record($id, $rid, $new['grants']);

    // If we are adding children recursively, add those as well.
    if ($new['recursive'] == 1) {
      $children = _taxonomy_access_get_descendants($id);
      foreach ($children as $tid) {
        $rows[$tid] =
          _taxonomy_access_format_grant_record($tid, $rid, $new['grants']);
      }
    }

    // Set the grants for the row or rows.
    taxonomy_access_set_term_grants($rows);
  }

  elseif ($type == 'default') {
    $default[$id] =
      _taxonomy_access_format_grant_record($id, $rid, $new['grants'], TRUE);

    // For the vocabulary default, all terms in the vocabulary are children.
    if ($new['recursive'] == 1) {
      $children = _taxonomy_access_get_vocabulary_terms($id);
      foreach ($children as $tid) {
        $rows[$tid] =
          _taxonomy_access_format_grant_record($tid, $rid, $new['grants']);
      }
    }

    // Set the grants for the row or rows.
    taxonomy_access_set_default_grants($default);
    taxonomy_access_set_term_grants($rows);
  }
}

/**
 * Helper to process the "Save all" button on the administration form.
 *
 * @param $rid
 *   The role id for the submission.
 * @param $values
 *   The $form_state['values'] from the form submission.
 * @param $form
 *   The $form array for the form.  Used to compare default values.
 */
function _taxonomy_access_admin_form_save_all($rid, $values, $form) {
  // Create four lists of records to update.
  $update_terms = array();
  $skip_terms = array();
  $update_defaults = array();
  $skip_defaults = array();

  foreach ($values['grants'] as $vid => $rows) {
    foreach ($rows as $tid => $grants) {
      // Check the default values for this row.
      $defaults = array();
      foreach ($grants as $grant_name => $value) {
        $defaults[$grant_name] =
          $form['grants'][$vid][$tid][$grant_name]['#default_value'];
      }

      // Proceed if the user changed the row (values differ from defaults).
      if ($defaults != $grants) {
        // If the grants for node access match the defaults, then we
        // can skip updating node access records for this row.
        $update_nodes = FALSE;
        foreach (array('view', 'update', 'delete') as $op) {
          if ($defaults[$op] != $grants[$op]) {
            $update_nodes = TRUE;
          }
        }

        // Add the row to one of the four arrays.
        switch (TRUE) {
          // Term record with node grant changes.
          case ($tid && $update_nodes):
            $update_terms[$tid] =
              _taxonomy_access_format_grant_record($tid, $rid, $grants);
            break;

          // Term record and no node grant changes.
          case ($tid && !$update_nodes):
            $skip_terms[$tid] =
              _taxonomy_access_format_grant_record($tid, $rid, $grants);
            break;

          // Vocab record with node grant changes.
          case (!$tid && $update_nodes):
            $update_defaults[$vid] =
              _taxonomy_access_format_grant_record($vid, $rid, $grants, TRUE);
            break;

          // Vocab record and no node grant changes.
          case (!$tid && !$update_nodes):
            $skip_defaults[$vid] =
              _taxonomy_access_format_grant_record($vid, $rid, $grants, TRUE);
            break;
        }
      }
    }
  }

  // Process each set.
  if (!empty($update_terms)){
    taxonomy_access_set_term_grants($update_terms);
  }
  if (!empty($skip_terms)){
    taxonomy_access_set_term_grants($skip_terms, FALSE);
  }
  if (!empty($update_defaults)){
    taxonomy_access_set_default_grants($update_defaults);
  }
  if (!empty($skip_defaults)){
    taxonomy_access_set_default_grants($skip_defaults, FALSE);
  }
}


/**
 * Generates HTML markup with form instructions for the admin form.
 *
 * @return
 *     Instructions HTML string.
 */
function _taxonomy_access_admin_instructions_html() {
  $instructions = '';
  $instructions .= ''
    . "<br /><br />"
    . "<div class=\"instructions\">"
    . "<h2>" . t("Explanation of fields") . "</h2>"
    . _taxonomy_access_grant_help_table()
    . "<p>"
    . t('Options for View, Update, and Delete are <em>Allow</em> (<acronym title="Allow">A</acronym>), <em>Ignore</em> (<acronym title="Ignore">I</acronym>), and <em>Deny</em> (<acronym title="Deny">D</acronym>).')
    . "</p>\n\n"
    . "<ul>\n"
    . "<li>"
    . t('<em>Deny</em> (<acronym title="Deny">D</acronym>) overrides <em>Allow</em> (<acronym title="Allow">A</acronym>) within this role.')
    . "</li>"
    . "<li>"
    . t('Both <em>Allow</em> (<acronym title="Allow">A</acronym>) and <em>Deny</em> (<acronym title="Deny">D</acronym>) override <em>Ignore</em> (<acronym title="Ignore">I</acronym>) within this role.')
    . "</li>"
    . "<li>"
    . t('If a user has <strong>multiple roles</strong>, an <em>Allow</em> (<acronym title="Allow">A</acronym>) from another role <strong>will</strong> override a <em>Deny</em> (<acronym title="Deny">D</acronym>) here.')
    . "</li>"
    . "</ul>\n\n"
    ;
  if (arg(4) != DRUPAL_ANONYMOUS_RID && arg(4) != DRUPAL_AUTHENTICATED_RID) {
    // Role other than Anonymous or Authenticated
    $instructions .= ''
      . "<p>"
      . t('<strong>Remember:</strong> This role <strong>will</strong> inherit permissions from the <em>authenticated user</em> role.  Be sure to <a href="@url">configure the authenticated user</a> properly.',
        array("@url" => url(
            TAXONOMY_ACCESS_CONFIG
            . "/role/"
            .  DRUPAL_AUTHENTICATED_RID
            . '/edit')))
      . "</p>\n\n"
      ;
  }
  $instructions .= ''
    . "<p>"
    . t('For more information and for troubleshooting guidelines, see the <a href="@help">help page</a> and the !readme.',
      array(
        '@help' => url('admin/help/taxonomy_access'),
        '!readme' => "<code>README.txt</code>"
      ))
    . "</p>\n\n"
    . "</div>\n\n"
    ;

  return $instructions;

}
