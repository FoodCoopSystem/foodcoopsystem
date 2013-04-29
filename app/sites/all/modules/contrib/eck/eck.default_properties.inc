<?php

function  eck__default_properties__form($form, &$state, $entity_type){
   //DEFAULT PROPERTIES
  //Now we want to display the default properties
  $default_properties = eck_get_default_properties();
  $options = array();
  foreach($default_properties as $property_name => $property_info){
    $options[$property_name] = $property_info['label'];
  }
  
  $form['default_properties'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('Default Properties')
  );
  
  //Now we need to set the defaults if the entity_type has any
  $properties = $entity_type->properties;
  
  $defaults = array();
  foreach($properties as $property => $info){
    $defaults[$property] = $property;
  }
  
  $form['default_properties']['#default_value'] = $defaults;
  // :S
  
  return $form;
}

function  eck__default_properties__form_submit($form, &$state, $entity_type){
  
  $entity_type = $state['values']['entity_type'];
  $dp = $state['values']['default_properties'];
  foreach($dp as $property => $active){
    if($active){
      $info = eck_get_default_property($property);
      $entity_type->addProperty($property, $info['label'], $info['type'], $info['behavior']);
    }else{
      $entity_type->removeProperty($property);
    }
  }
  
  $state['values']['entity_type'] = $entity_type;
}

function eck_get_default_properties(){
  return module_invoke_all('eck_default_properties');
}

function eck_get_default_property($name){
  $default_properties = eck_get_default_properties();
  return $default_properties[$name];
}