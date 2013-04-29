<?php
/*
 * In a property behavior plugin we want to hijack drupal when it is relavant. In this case we
 * want to do it when an entity with a property using our behavior is being manipulated: created, updated, saved,
 * shown, etc.
 * 
 * So the plugin implementation in ECK gives an opportunity to do this in multiple places. Currelty this are the
 * places where we can do it
 * 
 * entity_save: save gets called everytime the entity is being saved.
 * 
 * entity_insert: insert is a subset of save, in which it only gets called on a new entity
 * entity_update: insert is a subset of save, in which it only gets called on an entity that was loaded (edit mode)
 * 
 * We are also allowed to give a default_widget for this behavior, and a default_formatter
 * 
 * As new use cases arise, I am sure that this would be expanded to many other cases.
 * 
 * THE PLUGIN
 * 
 * The plugin is a very simple array with a label, and with the different callbacks that we want to implement.
 * 
 * In this example we are using the default_widget, default_formatter, and the save options. 
 * After the plugin array we give the functions that we want call at these different stages.
 */

$plugin = array(
  'label' => "Title",
  //'entity_save' => 'eck_title_property_entity_save',
  'entity_view' => 'eck_title_property_entity_view',
  'entity_info' => 'eck_title_property_entity_info',
  'default_widget' => 'eck_title_property_widget',
  'default_formatter' => 'eck_title_property_formatter'
);

/**
 * Define a rendarble array to input a value for the property using this behavior 
 *
 * @param $entity: and entity object that contains the property for which this behavior is relevant
 * @param $property: the name of the property (string) that is using this behavior
 * 
 * @return a rendarable array use when the input form for this entity is displayed
 */
function eck_title_property_widget($property, $vars){
  $entity = $vars['entity'];
  $title = _eck_title_property_extract_title($entity, $property);
  return array(
    '#type' => 'textfield',
    '#title' => $vars['properties'][$property]['label'],
    '#default_value' => $title,
    '#required' => TRUE
  );
}

function eck_title_property_formatter($property, $vars){
  $entity = $vars['entity'];
  $title = _eck_title_property_extract_title($entity, $property);
  return array('#markup' => "<h1>{$title}</h1>");
}

function eck_title_property_entity_view($property, $vars){
   $entity = $vars['entity'];
   $title = _eck_title_property_extract_title($entity, $property);
   if(empty($title)){
     $title = "{$entity->entityType()} : {$entity->id}";
   }
   
   drupal_set_title($title);
}

function eck_title_property_entity_info($property, $var){
  $info = $var;
  unset($info['label callback']);
  $info['entity keys']['label'] = $property;
  
  return $info;
}

function _eck_title_property_extract_title($entity, $property){
  $title = "";
  if(isset($entity->{$property})){
    $title = $entity->{$property};
  }
  
  return $title;
}

