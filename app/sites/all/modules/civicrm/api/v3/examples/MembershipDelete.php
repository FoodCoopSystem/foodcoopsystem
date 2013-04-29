<?php



/*
 
 */
function membership_delete_example(){
$params = array( 
  'id' => 1,
  'version' => 3,
);

  require_once 'api/api.php';
  $result = civicrm_api( 'membership','delete',$params );

  return $result;
}

/*
 * Function returns array of result expected from previous function
 */
function membership_delete_expectedresult(){

  $expectedResult = array( 
  'is_error' => 1,
  'error_message' => 'DB Error: syntax error',
);

  return $expectedResult  ;
}




/*
* This example has been generated from the API test suite. The test that created it is called
* 
* testMembershipDelete and can be found in 
* http://svn.civicrm.org/civicrm/branches/v3.4/tests/phpunit/CiviTest/api/v3/MembershipTest.php
* 
* You can see the outcome of the API tests at 
* http://tests.dev.civicrm.org/trunk/results-api_v3
* and review the wiki at
* http://wiki.civicrm.org/confluence/display/CRMDOC/CiviCRM+Public+APIs
* Read more about testing here
* http://wiki.civicrm.org/confluence/display/CRM/Testing
*/