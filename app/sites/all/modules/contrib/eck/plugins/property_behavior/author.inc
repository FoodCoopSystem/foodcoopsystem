<?php
// Plugin definition
$plugin = array(
  'label' => "Author",
  'entity_save' => 'eck_author_property_entity_save',
  'property_info' => 'eck_author_property_property_info'
);

function eck_author_property_entity_save($property, $vars){
  $entity = $vars['entity'];
  if(isset($entity->is_new) && $entity->is_new){
    global $user;
    $entity->{$property} = $user->uid;
  }
}

function eck_author_property_property_info($property, $vars){
  $vars['properties'][$property]['type'] = 'user';
  
  return $vars;
}
