<?php

class DBObject implements Iterator{
  //Wheteher this object was loaded or just created
  public $is_new;
  //Iterator variable
  private $position;
  
  //The database table where the objects exist
  private $table;
  private $vars;
  private $data;
  
  private $primary_keys;
  private $serialize;
  
  protected function __construct($table){
    $this->serialize = array();
    $this->is_new = TRUE;
    //Iterator variable
    $this->position = 0;
    
    $data = array();
    //is this a real table, check it
 
    
    $schemas = drupal_get_schema();
    $tables = array_keys($schemas);

    
    if(in_array($table, $tables)){
      $this->table = $table;
      
      $this->primary_keys = $schemas[$table]["primary key"];
      $this->vars = array_keys($schemas[$table]['fields']);
      
      //do we want to handle searialized variables by default? let's do it
      //and wait for some critizism
      foreach($schemas[$table]['fields'] as $name => $field){
        if(array_key_exists('serialize', $field) && $field['serialize']){
          $this->serialize[] = $name;
        }
      }
      foreach($this->vars as $var){
        $this->data[$var] = NULL;
      };
    }else{
      //@todo throw an exception
    }
  }
  
  function __set($var, $value){
    if(in_array($var, $this->vars)){
      $this->data[$var] = $value;
    }
  }
  
  function __get($var){
    if(property_exists($this, $var)){
      return $this->{$var};
    }
    
    return $this->data[$var];
  }
  
  public function __isset($name) {
    return isset($this->data[$name]);
  }

    /**  As of PHP 5.1.0  */
  public function __unset($name) {
    unset($this->data[$name]);
  }
  
  //DB Interaction Functions
  public function save(){
    
    //before we save, lets serialize the properties that require it
    foreach($this->serialize as $property){
   
      $this->{$property} = drupal_json_encode($this->{$property});
    }
    
   
    
    if($this->is_new){
      return 
      db_insert($this->table)
      ->fields($this->data)
      ->execute();
    }else{
      //well I need to know what the primary id is to set up the condition;
      $primary_key = $this->primary_keys[0];
      db_update($this->table)
      ->condition($primary_key, $this->{$primary_key}, '=')
      ->fields($this->data)
      ->execute();
    }
    
    //now that we are done saving lets deserialize in case that for some
    //reason we will continue manipulating the properties
    foreach($this->serialize as $property){
      $this->{$property} = drupal_json_decode($this->{$property});
    }
    
    $this->is_new = FALSE;
  }
  
  protected function load($property, $value){
    $result = 
    db_select($this->table, 't')
    ->fields('t')
    ->condition($property, $value,'=')
    ->execute()
    ->fetchAssoc();
    
    if($result){
      foreach($result as $property => $value){
        if(in_array($property, $this->serialize)){
          $value = drupal_json_decode($value);
        }
        $this->{$property} = $value;
      }
    }
    
    $this->is_new = FALSE;
  }
  
  public function delete(){
    //we can only deleted if its a loaded object, or if it has been saved
    if(!$this->is_new){
      $query = db_delete($this->table);
      $primary_key = $this->primary_keys[0];
      $query->condition($primary_key, $this->{$primary_key}, '=');
      $query->execute();
      //Should we delete the data from the object.. not sure.
      //for right now lets just set it back to new
      $this->is_new = TRUE;
    }
    
  }
  
  //Iterator Interface Functions
  function rewind() {
      $this->position = 0;
  }

  function current() {
      return $this->data[$this->key()];
  }

  function key() {
      return $this->vars[$this->position];
  }

  function next() {
      ++$this->position;
  }

  function valid() {
    if(in_array($this->position, array_keys($this->vars))){
      return TRUE;
    }else{
      return FALSE;
    }
  }
}

class EntityType extends DBObject{
  
  //If an entity type is new, we can create its table from the current data of
  //the object, but if this is a loaded object, we need to actually keep
  //track of the changes happening so we can modify the already existing table
  //appropiately.
  private $changes;
  
  public function __construct(){
    parent::__construct('eck_entity_type');
    $this->properties = array();
    $this->changes = array();
  }
  
  public function addProperty($name, $label, $type, $behavior = NULL){
    if(!$this->is_new){
      $this->recordFieldChange('add', $name);
    }
    
    $p = $this->properties;
    //@todo check that type is an actual type
    $p[$name] = array('label' => $label, 'type' => $type, 'behavior' => $behavior);
    
    $this->properties = $p;
  }
  
  public function removeProperty($name){
    $p = $this->properties;
    if(array_key_exists($name, $p)){
      unset($p[$name]);
      $this->properties = $p;
      if(!$this->is_new){
        $this->recordFieldChange('remove', $name);
      }
    }
  }
  
  public function changeBehavior($name, $behavior){
    $p = $this->properties;
    //@todo check that type is an actual type
    if(array_key_exists($name, $p)){
      $p[$name]['behavior'] = $behavior;
      //@todo look at this more closelly, does the behavior change really affect the property
      //cache?
      entity_property_info_cache_clear();
    }else{
      //@Todo add exception.. the property does not exist
    }
    
    $this->properties = $p;
  }
  
  public function removeBehavior($name){
    $this->changeBehavior($name, NULL);
  }
  
  private function recordFieldChange($op, $name){
    //If it is not new we need to keep track of stuff
    if(!$this->is_new){
      $p = $this->properties;
      $c = $this->changes;
      switch ($op) {
        case 'add':
          //if the property does not exist already add keep track
          if(!array_key_exists($name, $p)){
            $c[$op][] = $name;
          }
        break;
        
        case 'remove':
          //if there is an add in the changes take it out, otherwise add a
          //remove
          if(array_key_exists('add', $c)){
            
            $key = array_search($name, $c['add']);
            if($key != FALSE){
              unset($c['add'][$key]);
            }     
          }else{
            $c[$op][] = $name;
          }
        break;
      }
      $this->changes = $c;
    }
  }
  
  public function save(){
    if($this->is_new){
      module_load_include('inc', 'eck', 'eck.enttiy_type');
      $schema = eck__entity_type__schema($this);
      db_create_table("eck_{$this->name}", $schema);
      
    }else{
      //modify the already existing table in accordance with the recorded changes
      if(array_key_exists('add', $this->changes)){
        foreach($this->changes['add'] as $name){
          //first lets get the record 
          $properties = $this->properties;
          $property = $properties[$name];
          //now we check to see whether it is a default or a custom property
          //it is not custom so lets get the schema and add the field
          $schema = eck_property_type_schema($property['type']);
          db_add_field("eck_{$this->name}", $name, $schema);
          
        }
      }
      if(array_key_exists('remove', $this->changes)){
        foreach($this->changes['remove'] as $name){
          db_drop_field("eck_{$this->name}", $name);
        }
      }
    }
    
    parent::save();
    drupal_get_schema(NULL, TRUE);
  }
  
  public function delete(){
    parent::delete();
    db_drop_table('eck_'.$this->name);
    drupal_flush_all_caches();
  }
  
  public static function loadByName($name){
    $self = new EntityType();
    $self->load('name', $name);
    return $self;
  }
  
  public static function loadAll(){
    $results = db_select('eck_entity_type', 't')
    ->fields('t', array('name'))
    ->execute();
    
    $entity_types = array();
    
    foreach($results as $result){
      $name = $result->name;
      $entity_types[] = EntityType::loadByName($name);
    }
    return $entity_types;
  }
}

class Bundle extends DBObject{
  
  public function __construct(){
    parent::__construct('eck_bundle');
  }
  
  private function createMachineName(){
    $this->machine_name = "{$this->entity_type}_{$this->name}";
  }
  
  private function createLabel(){
    $name = $this->name;
    $pieces = explode("_", $name);
    $final = array();
    foreach($pieces as $piece){
      $final[] = ucfirst($piece);
    }
    
    $this->label = implode(" ", $final);
  }
  
  public function save(){
    //Lets do some checks before the bundle is saved
    if(isset($this->entity_type) && isset($this->name)){
      //we are good
      //@todo we should check that the entity type is a proper
      //entity type object
      
      //Lets set the machine name
      $this->createMachineName();
      
      if(!isset($this->label)){
        $this->createLabel();
      }
      
      parent::save();
      
    }else{
      //if the name an entity type are not set, we can not save
      //the bundle
      //@todo throw soem error or exception
    }
  }
  
  /**
   * This method returns a bundle object
   * @param $machine_name
   *  (String) A string composed of the entity type name and the bundle name
   *  "{$entity_type_name}_{$bundle_name}"
   */
  public static function loadByMachineName($machine_name){
    $self = new Bundle();
    $self->load('machine_name', $machine_name);
    return $self;
  }
  
  public static function loadAll(){
    //@todo move this to a general function
    $results = db_select('eck_bundle', 't')
    ->fields('t', array('machine_name'))
    ->execute();
    
    $bundles = array();
    
    foreach($results as $result){
      $name = $result->machine_name;
      $bundles[] = Bundle::loadByMachineName($name);
    }
    return $bundles;
  }
  
  public static function loadByEntityType($entity_type){
    //@todo move this to a general function
    $results = db_select('eck_bundle', 't')
    ->fields('t', array('name'))
    ->condition('entity_type', $entity_type->name, '=')
    ->execute();
    
    $bundles = array();
    
    foreach($results as $result){
      $name = $result->name;
   
      $bundles[] = Bundle::loadByMachineName("{$entity_type->name}_{$name}");
    }
    return $bundles;
  }
}
