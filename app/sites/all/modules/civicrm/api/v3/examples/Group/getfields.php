<?php



/*
 demonstrate use of getfields to interogate api
 */
function group_getfields_example(){
$params = array( 
  'version' => 3,
  'action' => 'create',
);

  require_once 'api/api.php';
  $result = civicrm_api( 'group','getfields',$params );

  return $result;
}

/*
 * Function returns array of result expected from previous function
 */
function group_getfields_expectedresult(){

  $expectedResult = array( 
  'is_error' => 0,
  'version' => 3,
  'count' => 9,
  'id' => array( 
      'name' => 'id',
      'type' => 1,
      'required' => true,
    ),
  'values' => array( 
      'id' => array( 
          'name' => 'id',
          'type' => 1,
          'required' => true,
        ),
      'mailing_id' => array( 
          'name' => 'mailing_id',
          'type' => 1,
          'required' => true,
          'FKClassName' => 'CRM_Mailing_DAO_Mailing',
        ),
      'group_type' => array( 
          'name' => 'group_type',
          'type' => 2,
          'title' => 'Group Type',
          'enumValues' => 'Include, Exclude, Base',
        ),
      'entity_table' => array( 
          'name' => 'entity_table',
          'type' => 2,
          'title' => 'Entity Table',
          'required' => true,
          'maxlength' => 64,
          'size' => 30,
        ),
      'entity_id' => array( 
          'name' => 'entity_id',
          'type' => 1,
          'required' => true,
        ),
      'search_id' => array( 
          'name' => 'search_id',
          'type' => 1,
        ),
      'search_args' => array( 
          'name' => 'search_args',
          'type' => 32,
          'title' => 'Search Args',
        ),
      'is_active' => array( 
          'api.default' => 1,
        ),
      'title' => array( 
          'api.required' => 1,
        ),
    ),
);

  return $expectedResult  ;
}




/*
* This example has been generated from the API test suite. The test that created it is called
* 
* testgetfields and can be found in 
* http://svn.civicrm.org/civicrm/branches/v3.4/tests/phpunit/CiviTest/api/v3/GroupTest.php
* 
* You can see the outcome of the API tests at 
* http://tests.dev.civicrm.org/trunk/results-api_v3
* and review the wiki at
* http://wiki.civicrm.org/confluence/display/CRMDOC/CiviCRM+Public+APIs
* Read more about testing here
* http://wiki.civicrm.org/confluence/display/CRM/Testing
*/