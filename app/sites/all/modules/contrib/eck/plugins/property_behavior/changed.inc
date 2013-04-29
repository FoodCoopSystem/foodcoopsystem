<?php
// Plugin definition
$plugin = array(
  'label' => "Changed",
  'entity_save' => 'eck_changed_property_entity_save',
  'property_info' => 'eck_changed_property_property_info'
);

function eck_changed_property_entity_save($property, $vars){
  $entity = $vars['entity'];
  $entity->{$property} = time();
}

function eck_changed_property_property_info($property, $vars){
  $vars['properties'][$property]['type'] = 'date';
  $vars['properties'][$property]['description'] = t("The last time this entity was updated.");
  return $vars;
}
