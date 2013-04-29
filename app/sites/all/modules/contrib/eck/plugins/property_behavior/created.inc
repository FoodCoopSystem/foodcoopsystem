<?php 
// Plugin definition
$plugin = array(
  'label' => 'Created',
  'entity_save' => 'eck_created_property_entity_insert',
  'property_info' => 'eck_created_property_property_info'
);


function eck_created_property_entity_insert($property, $vars){
  $entity = $vars['entity'];
  if(isset($entity->is_new) && $entity->is_new){
    $entity->{$property} = time();
  }
}

function eck_created_property_property_info($property, $vars){
  $vars['properties'][$property]['type'] = 'date';
  $vars['properties'][$property]['description'] = t("The date the entity was created.");
  return $vars;
}
