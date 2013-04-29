<?php
/**
 * @file
 * Handles properties attached to entites created through ECK.
 */

/**
 * Assigns fields to the db schema for an entity type.
 * @param $schema
 */

function eck_set_properties_schema(&$schema, $entity_type){
  $properties = $entity_type->properties;
  
  //dpm($properties, "Properties in schema");
  
  foreach($properties as $name => $info){
      $type = $info['type'];
      $schema['fields'][$name] = eck_property_type_schema($type);
  }
}

function eck_property_types() {
  //All types: serial, int, float, numeric, varchar, char, text, blob, datetime
  $default_types = array(
    'text' => t('Text'),
    'integer' => t('Integer'), 
    'decimal' => t('Decimal'), 
    'positive_integer' => t('Unsigned Integer'),
  );

  drupal_alter('eck_property_types', $default_types);

  // @TODO: As modules start to implement ECK hooks, this should be removed.
  if (module_exists('uuid') && !array_key_exists('uuid', $default_types)) {
    $default_types['uuid'] = t('UUID');
  }

  return $default_types;
}


function eck_property_type_schema($type) {
  $schema = array();
  
  switch ($type) {
    case 'text':
      $schema = array(
        'description' => 'Text',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      );
      break;

    case 'decimal':
      $schema = array(
        'description' => 'Decimal',
        'type' => 'float',
        'not null' => TRUE,
        'default' => 0,
      );
      break;

    case 'integer':
      $schema = array(
        'type' => 'int',
        'description' => "Integer",
        'not null' => TRUE,
        'default' => 0,
      );
      break;
    case 'positive_integer':
      $schema = array(
        'type' => 'int',
        'description' => "Integer",
        'not null' => TRUE,
        'unsigned' => TRUE,
        'default' => 0,
      );
      break;
      
      
    case 'uuid':
      $schema = array(
        'type' => 'char',
        'length' => 36,
        'not null' => TRUE,
        'default' => '',
        'description' => 'The Universally Unique Identifier.',
      );
      break;
      
    default:
      $schema = NULL;
      break;
  }
  
  drupal_alter('eck_property_type_schema', $schema);
  
  return $schema;
}


/**
 * Generates schema definition for additional ECK properties.
 *
 * @param $property_name
 *   Property to generate schema definition for.
 * @param $entity_type
 *   (Object) An object with and entity types info, as returned by entity_type__load 
 * This is only use to add stuff to the descriptions of the property, do we really need this?
 * Well, if people is already using the module it will be a pain in the butt to syncronize their schemas to use
 * indexes, and so on without the specifics of the entity type, so for the sake of moving forward to more important
 * things, lets leave the $entity_type as a parameter for this function.
 */
/*function eck__property__schema(&$schema, $entity_type, $property_name) {
}*/

/*function eck__property__form($entity_type){
  $form = array();
  // Enable/disable properties.
  $form['properties'] = array(
    '#type' => 'fieldset',
    '#title' => t('Properties'),
    '#description' => t('Note: unchecking will delete the property and all data stored within!'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );
  
  $properties = hook_eck_property_info();
 
  foreach ($properties as $property => $info) {
    
      $form['properties'][$property] = array(
        '#type' => 'checkbox',
        '#title' => $info['label'],
        '#default_value' => 
        isset($entity_type->properties) &&
        array_key_exists($property, $entity_type->properties) ? 1 : 0
      );
  }
  
  return $form;
}*/


function eck__properties__form($form, &$state, $entity_type_name){
  if (!isset($entity_type_name)) {
    $entity_type = new EntityType();
  }
  else {
    $entity_type = EntityType::loadByName($entity_type_name);
  }
  
  $form['entity_type'] = array('#type' => 'value', '#value' => $entity_type);
  
  //When the add button is clicked, the submit callback directs the form to be rebuilt. 
   //Here we check to see if a new property was added, and keep it in a a list of new properties
   //to add them to the table on each rebuild 
   $entity_type_properties = $entity_type->properties;
   
   if(!array_key_exists('values', $state) || !array_key_exists('new_properties', $state['values'])){
     $new_properties = eck_get_default_properties();
     $new_properties = array_merge($new_properties, $entity_type_properties);
   }else{
     $new_properties = $state['values']['new_properties'];
   }
  
   if(!empty($state['values']['property_name'])){
     $property_name = $state['values']['property_name'];
     $property_type = $state['values']['property_type'];
     //OK we need to do a little bit of filtering do the oddity of modifying existing
     //properties through the add Property form #fail @todo need to get a better interface.
     //But until then, we need to keep the form from changing the property_type or property_name of the already
     //existing properties
     
     if(!array_key_exists($property_name, $new_properties) || 
       $new_properties[$property_name]['type'] == $property_type){
     
       $new_properties[$property_name] =
       array(
         'label' => $state['values']['property_label'],
         'type' => $state['values']['property_type']
       );
       
       if(array_key_exists('property_behavior', $state['values'])){
         $new_properties[$state['values']['property_name']]['behavior'] = 
           $state['values']['property_behavior'];
       }
     }else{
       drupal_set_message("Can not modify the type of {$property_name}", "error");
     }
   }
   
   $form['new_properties'] = array('#type' => 'value', '#value' => $new_properties);
  
  //This is the select table where all the new properties are shown.
  $header = array(
    'machine_name' => t('Machine Name'),
    'name' => t('Name'),
    'type' => t('Type'),
    'behavior' => t('Behaviour')
  );
  
  $options = array();
  
  //@todo can we do this in a better way.. the only way I can think of is, load all defaults,
  //and then load all the properties in the entity type, while modifying the before added default
  //properties given the entity type settings
  
  $property_arrays = array('new_properties');
  
  foreach($property_arrays as $array_name){
    foreach(${$array_name} as $property_name => $property_info){
      $options[$property_name] = 
      array(
        'machine_name' => $property_name,
        'name' => $property_info['label'],
        'type' => $property_info['type'],
      );
      
      if(array_key_exists('behavior', $property_info)){
        $options[$property_name]['behavior'] = $property_info['behavior'];
      }else{
        $options[$property_name]['behavior'] = "";
      }
      
    }
  }
  
  $form["new_properties_table_label"] = array(
    '#markup' => '<h3>Properties</h3>'
  );
  
  $defaults = array();
  foreach($entity_type_properties as $property => $info){
    $defaults[$property] = 1;
  }
  
  $form['new_properties_table'] = 
  array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No other properties for this entity type.'),
    '#default_value' => $defaults
  );

  //Add new property
  $types[t('Generic')] = eck_property_types();
 
  $form["add_new_property"] = array(
    '#markup' => '<h3>Add new property</h3>'
  );

   $form['property_type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array('' => t('- Please choose -')) + $types,
    '#required' => TRUE,
    '#after_build' => array('eck_deactivate_on_save')
  );

  $form["property_label"] = array(
    '#type' => 'textfield',
    '#title' => t("Name"),
    '#description' => t("A human readable name for the property."),
    '#required' => TRUE,
    '#after_build' => array('eck_deactivate_on_save')
  );

  $form["property_name"] = array(
    '#type' => 'machine_name',
    '#machine_name' => array(
      'exists' => '_eck_fake_exists',
      'source' => array('property_label'),
    ),
   '#after_build' => array('eck_deactivate_on_save')
  );
  
  $behavior_plugins = ctools_get_plugins('eck', 'property_behavior');
  $options = array();
  
  foreach($behavior_plugins as $behavior => $info){
    $options[$behavior] = $info['label'];
  }
  
  
  $form['property_behavior'] = array(
    '#type' => 'select',
    '#title' => t('Behavior'),
    '#options' => array('' => t('- Please choose -')) + $options
  );
  
  $form['property_add'] = array(
    '#type' => 'submit',
    '#value' => t('Add Property'),
  );
  
  $form['line_break'] = array(
    '#markup' => '<br><hr/><br>'
  );
  
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  return $form;
}

function eck_deactivate_on_save($element, &$state){
  if(array_key_exists('input', $state) && array_key_exists('op', $state['input']) && 
    $state['input']['op'] == 'Save'){
    isset($element['#element_validate']) ? $element['#element_validate'] = NULL : NULL;
    isset($element['#needs_validation']) ? $element['#needs_validation'] = NULL : NULL;
  }
  
  return $element;
}

function eck__properties__form_submit($form, &$state){
  if($state['values']['op'] == "Add Property"){
    $state['rebuild'] = TRUE;
  }
  else if($state['values']['op'] == "Save"){
    //Here we want to add the properties to the entity type and save it
    $entity_type = $state['values']['entity_type'];
    //dpm($entity_type, "Entity Type");
    
    foreach($state['values']['new_properties_table'] as $property => $active){
      if($active){
        $info = $state['values']['new_properties'][$property];
        if(array_key_exists('behavior', $info)){
          $entity_type->addProperty($property, $info['label'], $info['type'], $info['behavior']);
        }else{
          $entity_type->addProperty($property, $info['label'], $info['type']);
        }
      }else{
        $entity_type->removeProperty($property);
      }
    }
    
    $entity_type->save();
    
    //lets flush the cache so new behaviors and properties will get set up correctly
    drupal_flush_all_caches();
  }
}





