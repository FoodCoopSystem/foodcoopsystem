<?php



/*
 
 */
function survey_get_example(){
$params = array( 
  'version' => 3,
  'title' => 'survey title',
  'activity_type_id' => '30',
  'max_number_of_contacts' => 12,
  'instructions' => 'Call people, ask for money',
);

  require_once 'api/api.php';
  $result = civicrm_api( 'survey','get',$params );

  return $result;
}

/*
 * Function returns array of result expected from previous function
 */
function survey_get_expectedresult(){

  $expectedResult = array( 
  'is_error' => 0,
  'version' => 3,
  'count' => 1,
  'id' => 1,
  'values' => array( 
      '1' => array( 
          'id' => '1',
          'title' => 'survey title',
          'activity_type_id' => '30',
          'instructions' => 'Call people, ask for money',
          'max_number_of_contacts' => '12',
          'is_active' => '1',
          'is_default' => 0,
          'created_date' => '2011-10-31 16:56:53',
        ),
    ),
);

  return $expectedResult  ;
}




/*
* This example has been generated from the API test suite. The test that created it is called
* 
* testGetSurvey and can be found in 
* http://svn.civicrm.org/civicrm/branches/v3.4/tests/phpunit/CiviTest/api/v3/SurveyTest.php
* 
* You can see the outcome of the API tests at 
* http://tests.dev.civicrm.org/trunk/results-api_v3
* and review the wiki at
* http://wiki.civicrm.org/confluence/display/CRMDOC/CiviCRM+Public+APIs
* Read more about testing here
* http://wiki.civicrm.org/confluence/display/CRM/Testing
*/