<?php
module_load_include('inc', 'eck', 'eck.classes');
/**
 * @file
 * All of the menu, pages, and forms related to bundle administration.
 */

/**
 * This function creates the menu items relevant to bundle administration
 *
 * @param $entity_type
 *  (Object) as returned by eck__entity_type__load()
 *
 * This function called is triggered from hook_menu()
 * @see eck_menu()
 */
function eck__bundle__menu($entity_type) {
  // Create the menus relavant to types.
  $path = eck__entity_type__path();
  $menu = array();

  // DELETE ENTITY TYPE
  $menu["{$path}/{$entity_type->name}/delete"] = array(
    'title' => "Delete",
    'description' => "Delete the '{$entity_type->label}' Entity Type",
    'page callback' => "drupal_get_form",//"eck__entity_type__delete",
    'page arguments' => array("eck__entity_type__delete_form", $entity_type->name),
    'access callback' => 'eck__multiple_access_check',
    'access arguments' => array( array('eck administer entity types', 'eck delete entity types') ),
    'file' => 'eck.entity_type.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
  );
  
  //MANAGE Entity Type Properties
  $menu["{$path}/{$entity_type->name}/properties"] = array(
    'title' => "Manage properties",
    'description' => "Manage the properties of the {$entity_type->label} entity type",
    'page callback' => "drupal_get_form",//"eck__entity_type__delete",
    'page arguments' => array("eck__properties__form", $entity_type->name),
    'access arguments' => array("manage {$entity_type->name} properties"),
    'file' => 'eck.properties.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 4,
  );
  
  // OVERVIEW Entity types.
  $menu["{$path}/{$entity_type->name}"] = array(
    'title' => "{$entity_type->label}" ,
    'description' => "View all the bundles for '{$entity_type->label}'",
    'page callback' => "eck__bundle__list",
    'page arguments' => array(3),
    'access callback' => 'eck__multiple_access_check',
    'access arguments' => array( array('eck administer entity types',
                                       'eck add entity types',
                                       'eck edit entity types',
                                       'eck administer bundles', 
                                       'eck list bundles', 
                                       "eck administer {$entity_type->name} bundles",
                                       "eck list {$entity_type->name} bundles"
                                       ) ),
    'file' => 'eck.bundle.inc'
  );
  
  $menu["{$path}/{$entity_type->name}/list"] = array(
    'title' => 'Bundle List',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 100,
  );
  
  
  $menu["{$path}/{$entity_type->name}/edit"] = array(
    'title' => "Edit",
    'description' => "Edit the '{$entity_type->label}' entity type",
    'page callback' => 'drupal_get_form',
    'page arguments' => array("eck__entity_type__form", $entity_type->name),
    'access callback' => 'eck__multiple_access_check',
    'access arguments' => array( array('eck administer entity types', 'eck edit entity types') ),
    'file' => 'eck.entity_type.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );


  $menu["{$path}/{$entity_type->name}/add"] = array(
    'title' => "Add bundle ",
    'description' => "Add a(n) new '{$entity_type->label} Bundle'",
    'page callback' => "drupal_get_form",
    'page arguments' => array('eck__bundle__add', 3),
    'access callback' => 'eck__multiple_access_check',
    'access arguments' => array( array('eck administer bundles', 
      'eck add bundles', "eck add {$entity_type->name} bundles") ),
    'type' => MENU_LOCAL_ACTION,
    'weight' => 0,
    'file' => 'eck.bundle.inc'
  );

  module_load_include('inc', 'eck', 'eck.entity');
  foreach (Bundle::loadByEntityType($entity_type) as $bundle) {
    $menu = array_merge($menu, eck__entity__menu($entity_type, $bundle));
  }

  return $menu;
}

/**
 * Page call back for the bundle overview table (to see and manipulate all created label of
 * a given type)
 *
 * @param entity_type
 *  (String) entity type
 */
function eck__bundle__list($entity_type) {
  $path = eck__entity_type__path();
  $entity_type = entity_type_load($entity_type);
  
  //Check that the user has permissions to view bundle lists:
  if( eck__multiple_access_check(
      array( 'eck administer bundles', 
             'eck list bundles',
             "eck administer {$entity_type->name} bundles",
             "eck list {$entity_type->name} bundles"
  ) ) )
  {
    
    $header = array(t('Type'), array('data' => t('Operations'), 'colspan' => '1'));
    $rows = array();

    foreach (Bundle::loadByEntityType($entity_type) as $bundle) {
      $bundle_label =  $bundle->label;

      $admin_info = get_bundle_admin_info($entity_type->name, $bundle->name);

      $uri = $admin_info['path'];
      
      $allowed_operations = '';
      //Check that the user has permissions to delete:
      if( eck__multiple_access_check(
        array( 'eck administer bundles', 
               'eck delete bundles',
               "eck administer {$entity_type->name} bundles",
               "eck delete {$entity_type->name} bundles"
              ) )
      ) $allowed_operations = l(t('delete'), $uri."/delete");
      
      $rows[] = 
      array(
        l($bundle_label, url($uri, array('absolute' => TRUE))),
        $allowed_operations
      );
    }
 
    $build['bundle_table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }



  return $build;
}

/**
 * ADD Entity types.
 *
 * @param $form
 *  Form array provided by the Form API
 * @param $form_state
 *  array provided by the Form API
 * @param entity_type
 *  (String) entity type
 */
function eck__bundle__add($form, &$form_state, $entity_type) {
  $entity_type = entity_type_load($entity_type);
  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $entity_type,
  );

  $form['bundle_label'] = array(
    '#type' => 'textfield',
    '#title' => "Type",
    '#description' => "A Human readable name for the bundle",
  );

  $form['bundle_name'] = array(
    '#type' => 'machine_name',
    '#required' => FALSE,
    '#machine_name' => array(
      'exists' => '_eck_fake_exists',
      'source' => array('bundle_label'),
    )
  );

  $form['#validate'][] = 'eck__bundle__add_validate';

  $form['submit'] = array(
    '#type' => 'submit',
    '#weight' => 10000,
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation for bundle creation (Make sure this bundle don't exist for thie entity type)
 *
 * @param $form
 *  Form array provided by the Form API
 * @param $form_state
 *  array provided by the Form API
 */
function eck__bundle__add_validate($form, &$form_state) {
  $entity_type = $form_state['values']['entity_type'];
 

  // The type does not have to be unique in the table, but it should be unique
  // to its entity so we will check that here.
  foreach (Bundle::loadByEntityType($entity_type) as $bundle) {
    
    if ($bundle->name == $form_state['values']['bundle_name']) {
      form_set_error('bundle', t("bundle {$bundle->label} already exists for this entity type"));
    }
  }
}

/**
 * Submit function for add form
 *
 * @param $form
 *  Form array provided by the Form API
 * @param $form_state
 *  array provided by the Form API
 */
function eck__bundle__add_submit($form, &$form_state) {
  $path = eck__entity_type__path();
  
  $entity_type = $form_state['values']['entity_type'];
  $bundle_name = $form_state['values']['bundle_name'];
  $bundle_label = $form_state['values']['bundle_label'];

  $bundle = new Bundle();
  $bundle->name = $bundle_name;
  $bundle->label = $bundle_label;
  $bundle->entity_type = $entity_type->name;
  $bundle->save();

  drupal_set_message(t('the %bundle for entity type %entity_type has been saved', 
  array('%bundle' => $bundle_name, '%entity_type' => $entity_type->name)));

  drupal_get_schema(NULL, TRUE);
  entity_info_cache_clear();
  menu_rebuild();

  drupal_goto("{$path}/{$entity_type->name}");
}

/**
 * Delete the bundle of a given entity type
 * @param $entity_type
 *  (EntityType) The entity type of the bundle that will be deleted
 *  the entity type object can be loaded by using EntityType::loadByName($entity_type_name)
 * @param  $bundle
 *  (Bundle) The bundle to be deleted, the Bundle object can be loaded by using 
 *  Bundle::loadByMachingName($bundle_machine_name)
 * 
 * for more information on the EntityType and Bundle classes look at eck.classes.inc
 */
function eck__bundle__delete($entity_type, $bundle){
  //first delete all of the entities of this bundle
  $query = new EntityFieldQuery();
  $query
  ->entityCondition('entity_type', $entity_type->name, '=')
  ->entityCondition('bundle', $bundle->name,'=');
  
  $results = $query->execute();
  
  if(!empty($results)){
    $ids = array_keys($results[$entity_type->name]);
    entity_delete($entity_type->name, $ids);
  }
  //then we delete the bundle (field_instances)
  field_attach_delete_bundle($entity_type->name, $bundle->name);
  
  //and finally we delete the bundle from the eck_type table
  $bundle->delete();
  
  menu_rebuild();
  drupal_flush_all_caches();
  
  drupal_set_message("The bundle '{$bundle->name}' from the entity type '{$entity_type->name}' has been deleted");
}

function eck__bundle__delete_form($form, &$form_state, $entity_type_name, $bundle_name) {
    $path = eck__entity_type__path();
    $entity_type = entity_type_load($entity_type_name);
    $bundle = bundle_load($entity_type_name, $bundle_name);
    
    /*dpm($entity_type);
    dpm($bundle);*/
    
  $form['entity_type'] =
      array(
        '#type' => 'value',
        '#value' => $entity_type,
  );

  $form['bundle'] =
      array(
        '#type' => 'value',
        '#value' => $bundle,
  );

  $form['submit_redirect'] =
  array(
    '#type' => 'value',
    '#value' => "{$path}/{$entity_type->name}",
  );

  $message = t("Are you sure that you want to delete the bundle '{$bundle->name}'");

  $caption = t("All of the data (entities) associated with this bundle
  will be deleted. This action cannot be undone.");

  return confirm_form($form, $message, "{$path}/{$entity_type->name}", $caption, t('Delete'));
}

/**
 * Sumbmit function for the delete functionality
 *
 * @param $form
 *  Form array provided by the Form API
 * @param $form_state
 *  array provided by the Form API
 */
function eck__bundle__delete_form_submit($form, &$form_state) {
  $entity_type = $form_state['values']['entity_type'];
  $bundle = $form_state['values']['bundle'];
  $submit_redirect = $form_state['values']['submit_redirect'];

  eck__bundle__delete($entity_type, $bundle);

  // Ok, lets delete the entity
  $form_state['redirect'] = $submit_redirect;
}

function eck__bundle__field_autocomplete($entity_type, $bundle, $string = ""){
  $field_label = get_bundle_field_label_info($entity_type, $bundle);
  $matches = array();
  
  if($field_label){
    $field = $field_label['field'];
    $language = $field_label['language'];
    $delta = $field_label['delta'];
    $column = $field_label['column'];
    $query = new EntityFieldQuery();
    
    $query->entityCondition('entity_type', $entity_type->name, '=')
    ->entityCondition('bundle', $bundle->name, '=')
    ->fieldCondition($field, $column, $string, 'CONTAINS');
    
    $results = $query->execute();
    
    $entities = entity_load($entity_type->name, array_keys($results[$entity_type->name]));
    foreach($entities as $id => $entity){
      $matches[$id] = $entity->{$field}[$language][$delta][$column];
    }
  }
  
  drupal_json_output($matches);
}

function get_bundle_field_label_info($entity_type, $bundle){
  
  $info = entity_get_info();
  
  if(array_key_exists('field label', $info[$entity_type->name]['bundles'][$bundle->name])){
    return $info[$entity_type->name]['bundles'][$bundle->name]['field label'];
  }
  else{
    return NULL;
  }
}
