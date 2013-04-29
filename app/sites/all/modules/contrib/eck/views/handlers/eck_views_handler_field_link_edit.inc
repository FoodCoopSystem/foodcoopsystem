<?php
/**
 * Field handler to present a link to editt the entity content.
 *
 */
class eck_views_handler_field_link_edit extends eck_views_handler_field_link {
  
  function render_link($entity, $values) {
    $entity_type = $entity->entityType();
    $bundle = $entity->type;
    
    if (!user_access('eck administer entities') && !user_access('eck edit entities') && !user_access("eck administer {$entity_type} {$bundle} entities") && !user_access("eck delete {$entity_type} {$bundle} entities")) {
      return;
    }
    
    $crud_info = get_bundle_crud_info($entity_type, $bundle);
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = str_replace('%', $values->id, $crud_info['edit']['path']);
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    
    return $text;
  }
}