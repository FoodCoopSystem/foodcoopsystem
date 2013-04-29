<?php



/*
 /*this demonstrates setting a custom field through the API 
 */
function contact_create_example(){
$params = array( 
  'first_name' => 'abc1',
  'contact_type' => 'Individual',
  'last_name' => 'xyz1',
  'version' => 3,
  'custom_1' => 'custom string',
);

  require_once 'api/api.php';
  $result = civicrm_api( 'contact','create',$params );

  return $result;
}

/*
 * Function returns array of result expected from previous function
 */
function contact_create_expectedresult(){

  $expectedResult = array( 
  'is_error' => 0,
  'version' => 3,
  'count' => 1,
  'id' => 1,
  'values' => array( 
      '1' => array( 
          'id' => 1,
          'contact_type' => 'Individual',
          'contact_sub_type' => 'null',
          'do_not_email' => '',
          'do_not_phone' => '',
          'do_not_mail' => '',
          'do_not_sms' => '',
          'do_not_trade' => '',
          'is_opt_out' => '',
          'legal_identifier' => '',
          'external_identifier' => '',
          'sort_name' => 'xyz1, abc1',
          'display_name' => 'abc1 xyz1',
          'nick_name' => '',
          'legal_name' => '',
          'image_URL' => '',
          'preferred_communication_method' => '',
          'preferred_language' => 'en_US',
          'preferred_mail_format' => '',
          'api_key' => '',
          'first_name' => 'abc1',
          'middle_name' => '',
          'last_name' => 'xyz1',
          'prefix_id' => '',
          'suffix_id' => '',
          'email_greeting_id' => '',
          'email_greeting_custom' => '',
          'email_greeting_display' => '',
          'postal_greeting_id' => '',
          'postal_greeting_custom' => '',
          'postal_greeting_display' => '',
          'addressee_id' => '',
          'addressee_custom' => '',
          'addressee_display' => '',
          'job_title' => '',
          'gender_id' => '',
          'birth_date' => '',
          'is_deceased' => '',
          'deceased_date' => '',
          'household_name' => '',
          'primary_contact_id' => '',
          'organization_name' => '',
          'sic_code' => '',
          'user_unique_id' => '',
        ),
    ),
);

  return $expectedResult  ;
}




/*
* This example has been generated from the API test suite. The test that created it is called
* 
* testCreateWithCustom and can be found in 
* http://svn.civicrm.org/civicrm/branches/v3.4/tests/phpunit/CiviTest/api/v3/ContactTest.php
* 
* You can see the outcome of the API tests at 
* http://tests.dev.civicrm.org/trunk/results-api_v3
* and review the wiki at
* http://wiki.civicrm.org/confluence/display/CRMDOC/CiviCRM+Public+APIs
* Read more about testing here
* http://wiki.civicrm.org/confluence/display/CRM/Testing
*/