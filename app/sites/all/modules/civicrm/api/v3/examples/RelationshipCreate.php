<?php



/*
 
 */
function relationship_create_example(){
$params = array( 
  'contact_id_a' => 1,
  'contact_id_b' => 2,
  'relationship_type_id' => 16,
  'start_date' => '2010-10-30',
  'end_date' => '2010-12-30',
  'is_active' => 1,
  'note' => 'note',
  'version' => 3,
);

  require_once 'api/api.php';
  $result = civicrm_api( 'relationship','create',$params );

  return $result;
}

/*
 * Function returns array of result expected from previous function
 */
function relationship_create_expectedresult(){

  $expectedResult = array( 
  'is_error' => 0,
  'version' => 3,
  'count' => 1,
  'id' => 2,
  'values' => array( 
      '2' => array( 
          'id' => 2,
          'moreIDs' => '2',
        ),
    ),
);

  return $expectedResult  ;
}




/*
* This example has been generated from the API test suite. The test that created it is called
* 
* testRelationshipCreate and can be found in 
* http://svn.civicrm.org/civicrm/branches/v3.4/tests/phpunit/CiviTest/api/v3/RelationshipTest.php
* 
* You can see the outcome of the API tests at 
* http://tests.dev.civicrm.org/trunk/results-api_v3
* and review the wiki at
* http://wiki.civicrm.org/confluence/display/CRMDOC/CiviCRM+Public+APIs
* Read more about testing here
* http://wiki.civicrm.org/confluence/display/CRM/Testing
*/