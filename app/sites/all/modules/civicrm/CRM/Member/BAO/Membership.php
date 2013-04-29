<?php
/*
 +--------------------------------------------------------------------+
 | CiviCRM version 4.1                                                |
 +--------------------------------------------------------------------+
 | Copyright CiviCRM LLC (c) 2004-2011                                |
 +--------------------------------------------------------------------+
 | This file is a part of CiviCRM.                                    |
 |                                                                    |
 | CiviCRM is free software; you can copy, modify, and distribute it  |
 | under the terms of the GNU Affero General Public License           |
 | Version 3, 19 November 2007 and the CiviCRM Licensing Exception.   |
 |                                                                    |
 | CiviCRM is distributed in the hope that it will be useful, but     |
 | WITHOUT ANY WARRANTY; without even the implied warranty of         |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               |
 | See the GNU Affero General Public License for more details.        |
 |                                                                    |
 | You should have received a copy of the GNU Affero General Public   |
 | License and the CiviCRM Licensing Exception along                  |
 | with this program; if not, contact CiviCRM LLC                     |
 | at info[AT]civicrm[DOT]org. If you have questions about the        |
 | GNU Affero General Public License or the licensing of CiviCRM,     |
 | see the CiviCRM license FAQ at http://civicrm.org/licensing        |
 +--------------------------------------------------------------------+
*/

/**
 *
 * @package CRM
 * @copyright CiviCRM LLC (c) 2004-2011
 * $Id$
 *
 */

require_once 'CRM/Member/DAO/Membership.php';
require_once 'CRM/Member/BAO/MembershipType.php';

require_once 'CRM/Core/BAO/CustomField.php';
require_once 'CRM/Core/BAO/CustomValue.php';
class CRM_Member_BAO_Membership extends CRM_Member_DAO_Membership {

  /**
   * static field for all the membership information that we can potentially import
   *
   * @var array
   * @static
   */
  static $_importableFields = NULL;
  function __construct() {
    parent::__construct();
  }

  /**
   * takes an associative array and creates a membership object
   *
   * the function extracts all the params it needs to initialize the created
   * membership object. The params array could contain additional unused name/value
   * pairs
   *
   * @param array  $params (reference ) an assoc array of name/value pairs
   * @param array $ids    the array that holds all the db ids
   *
   * @return object CRM_Member_BAO_Membership object
   * @access public
   * @static
   */
  static
  function &add(&$params, &$ids) {
    require_once 'CRM/Utils/Hook.php';

    if (CRM_Utils_Array::value('membership', $ids)) {
      CRM_Utils_Hook::pre('edit', 'Membership', $ids['membership'], $params);
    }
    else {
      CRM_Utils_Hook::pre('create', 'Membership', NULL, $params);
    }

    if (!CRM_Utils_Array::value('reminder_date', $params)) {
      $params['reminder_date'] = 'null';
    }

    if (!CRM_Utils_Array::value('is_override', $params)) {
      $params['is_override'] = 'null';
    }

    $membership = new CRM_Member_BAO_Membership();
    $membership->copyValues($params);

    $membership->id = CRM_Utils_Array::value('membership', $ids);

    $membership->save();
    $membership->free();

    $session = CRM_Core_Session::singleton();
    if (empty($membership->contact_id) || empty($membership->status_id)) {
      // this means we are in renewal mode and are just updating the membership
      // record or this is an API update call and all fields are not present in the update record
      // however the hooks dont care and want all data CRM-7784
      $tempMembership = new CRM_Member_DAO_Membership();
      $tempMembership->id = $membership->id;
      $tempMembership->find(TRUE);
      $membership = $tempMembership;
    }

    //get the log start date.
    //it is set during renewal of membership.
    $logStartDate = CRM_Utils_array::value('log_start_date', $params);
    $logStartDate = ($logStartDate) ? CRM_Utils_Date::isoToMysql($logStartDate) : CRM_Utils_Date::isoToMysql($membership->start_date);
    $values       = self::getStatusANDTypeValues($membership->id);

    $membershipLog = array(
      'membership_id' => $membership->id,
      'status_id' => $membership->status_id,
      'start_date' => $logStartDate,
      'end_date' => CRM_Utils_Date::isoToMysql($membership->end_date),
      'renewal_reminder_date' => CRM_Utils_Date::isoToMysql($membership->reminder_date),
      'modified_date' => date('Ymd'),
      'membership_type_id' => $values[$membership->id]['membership_type_id'],
    );

    $session = CRM_Core_Session::singleton();
    // If we have an authenticated session, set modified_id to that user's contact_id, else set to membership.contact_id
    if ($session->get('userID')) {
      $membershipLog['modified_id'] = $session->get('userID');
    }
    elseif (!empty($ids['userId'])) {
      $membershipLog['modified_id'] = $ids['userId'];
    }
    else {
      $membershipLog['modified_id'] = $membership->contact_id;
    }

    require_once 'CRM/Member/BAO/MembershipLog.php';
    CRM_Member_BAO_MembershipLog::add($membershipLog, CRM_Core_DAO::$_nullArray);

    // reset the group contact cache since smart groups might be affected due to this
    require_once 'CRM/Contact/BAO/GroupContactCache.php';
    CRM_Contact_BAO_GroupContactCache::remove();

    if (CRM_Utils_Array::value('membership', $ids)) {
      CRM_Utils_Hook::post('edit', 'Membership', $membership->id, $membership);
    }
    else {
      CRM_Utils_Hook::post('create', 'Membership', $membership->id, $membership);
    }

    return $membership;
  }

  /**
   * Given the list of params in the params array, fetch the object
   * and store the values in the values array
   *
   * @param array   $params input parameters to find object
   * @param array   $values output values of the object
   * @param boolean $active do you want only active memberships to
   *                        be returned
   *
   * @return CRM_Member_BAO_Membership|null the found object or null
   * @access public
   * @static
   */
  static
  function &getValues(&$params, &$values, $active = FALSE) {
    if (empty($params)) {
      return NULL;
    }
    $membership = new CRM_Member_BAO_Membership();

    $membership->copyValues($params);
    $membership->find();
    $memberships = array();
    while ($membership->fetch()) {
      if ($active &&
        (!CRM_Core_DAO::getFieldValue('CRM_Member_DAO_MembershipStatus',
            $membership->status_id,
            'is_current_member'
          ))
      ) {
        continue;
      }

      CRM_Core_DAO::storeValues($membership, $values[$membership->id]);
      $memberships[$membership->id] = $membership;
    }

    return $memberships;
  }

  /**
   * takes an associative array and creates a membership object
   *
   * @param array    $params      (reference ) an assoc array of name/value pairs
   * @param array    $ids         the array that holds all the db ids
   * @param boolean  $callFromAPI Is this function called from API?
   *
   * @return object CRM_Member_BAO_Membership object
   * @access public
   * @static
   */
  static
  function &create(&$params, &$ids, $skipRedirect = FALSE, $activityType = 'Membership Signup') {
    // always calculate status if is_override/skipStatusCal is not true.
    // giving respect to is_override during import.  CRM-4012

    // To skip status calculation we should use 'skipStatusCal'.
    // eg pay later membership, membership update cron CRM-3984

    if (!CRM_Utils_Array::value('is_override', $params) &&
      !CRM_Utils_Array::value('skipStatusCal', $params)
    ) {
      require_once 'CRM/Utils/Date.php';
      $startDate = $endDate = $joinDate = NULL;
      if (isset($params['start_date'])) {
        $startDate = $params['start_date'];
      }

      if (array_key_exists('end_date', $params)) {
        $endDate = $params['end_date'];
        $params['end_date'] = CRM_Utils_Date::processDate($endDate, NULL, TRUE, 'Ymd');
      }

      if (isset($params['join_date'])) {
        $joinDate = $params['join_date'];
      }

      require_once 'CRM/Member/BAO/MembershipStatus.php';
      //fix for CRM-3570, during import exclude the statuses those having is_admin = 1
      $excludeIsAdmin = CRM_Utils_Array::value('exclude_is_admin', $params, FALSE);

      //CRM-3724 always skip is_admin if is_override != true.
      if (!$excludeIsAdmin &&
        !CRM_Utils_Array::value('is_override', $params)
      ) {
        $excludeIsAdmin = TRUE;
      }

      $calcStatus = CRM_Member_BAO_MembershipStatus::getMembershipStatusByDate($startDate, $endDate, $joinDate,
        'today', $excludeIsAdmin
      );
      if (empty($calcStatus)) {
        if (!$skipRedirect) {
          // Redirect the form in case of error
          CRM_Core_Session::setStatus(ts('The membership cannot be saved.') .
            '<br/>' .
            ts('No valid membership status for given dates.')
          );
          return CRM_Utils_System::redirect(CRM_Utils_System::url('civicrm/contact/view',
              "reset=1&force=1&cid={$params['contact_id']}&selectedChild=member"
            ));
        }
        // Return the error message to the api
        $error = array();
        $error['is_error'] = ts('The membership cannot be saved. No valid membership status for given dates');
        return $error;
      }
      $params['status_id'] = $calcStatus['id'];
    }

    require_once 'CRM/Core/Transaction.php';
    $transaction = new CRM_Core_Transaction();

    $membership = self::add($params, $ids);

    if (is_a($membership, 'CRM_Core_Error')) {
      $transaction->rollback();
      return $membership;
    }


    // add custom field values
    if (CRM_Utils_Array::value('custom', $params)
      && is_array($params['custom'])
    ) {
      require_once 'CRM/Core/BAO/CustomValueTable.php';
      CRM_Core_BAO_CustomValueTable::store($params['custom'], 'civicrm_membership', $membership->id);
    }

    $params['membership_id'] = $membership->id;
    if (isset($ids['membership'])) {
      $ids['contribution'] = CRM_Core_DAO::getFieldValue('CRM_Member_DAO_MembershipPayment',
        $ids['membership'],
        'contribution_id',
        'membership_id'
      );
    }
    //record contribution for this membership
    if (CRM_Utils_Array::value('contribution_status_id', $params) && !CRM_Utils_Array::value('relate_contribution_id', $params)) {
      $contributionParams = array();
      $config = CRM_Core_Config::singleton();
      $contributionParams['currency'] = $config->defaultCurrency;
      $contributionParams['receipt_date'] = (CRM_Utils_Array::value('receipt_date', $params)) ? $params['receipt_date'] : 'null';
      $contributionParams['source'] = CRM_Utils_Array::value('contribution_source', $params);
      $contributionParams['non_deductible_amount'] = 'null';
      $recordContribution = array(
        'contact_id', 'total_amount', 'receive_date', 'contribution_type_id',
        'payment_instrument_id', 'trxn_id', 'invoice_id', 'is_test',
        'contribution_status_id', 'check_number', 'campaign_id', 'is_pay_later',
      );
      foreach ($recordContribution as $f) {
        $contributionParams[$f] = CRM_Utils_Array::value($f, $params);
      }

      require_once 'CRM/Contribute/BAO/Contribution.php';
      $contribution = CRM_Contribute_BAO_Contribution::create($contributionParams, $ids);

      if (CRM_Utils_Array::value('processPriceSet', $params) &&
        !empty($params['lineItems'])
      ) {
        require_once 'CRM/Contribute/Form/AdditionalInfo.php';
        CRM_Contribute_Form_AdditionalInfo::processPriceSet($contribution->id, $params['lineItems']);
      }

      //insert payment record for this membership
      if (!CRM_Utils_Array::value('contribution', $ids) ||
        CRM_Utils_Array::value('is_recur', $params)
      ) {
        require_once 'CRM/Member/DAO/MembershipPayment.php';
        $mpDAO = new CRM_Member_DAO_MembershipPayment();
        $mpDAO->membership_id = $membership->id;
        $mpDAO->contribution_id = $contribution->id;
        if (CRM_Utils_Array::value('is_recur', $params)) {
          $mpDAO->find();
        }

        CRM_Utils_Hook::pre('create', 'MembershipPayment', NULL, $mpDAO);
        $mpDAO->save();
        CRM_Utils_Hook::post('create', 'MembershipPayment', $mpDAO->id, $mpDAO);
      }
    }

    if (CRM_Utils_Array::value('relate_contribution_id', $params)) {
      require_once 'CRM/Member/DAO/MembershipPayment.php';
      $mpDAO = new CRM_Member_DAO_MembershipPayment();
      $mpDAO->membership_id = $membership->id;
      $mpDAO->contribution_id = $params['relate_contribution_id'];
      if (!($mpDAO->find(TRUE))) {
        CRM_Utils_Hook::pre('create', 'MembershipPayment', NULL, $mpDAO);
        $mpDAO->save();
        CRM_Utils_Hook::post('create', 'MembershipPayment', $mpDAO->id, $mpDAO);
      }
    }

    // add activity record only during create mode and renew mode
    // also add activity if status changed CRM-3984 and CRM-2521
    if (!CRM_Utils_Array::value('membership', $ids) ||
      $activityType == 'Membership Renewal' ||
      CRM_Utils_Array::value('createActivity', $params)
    ) {

      if (CRM_Utils_Array::value('membership', $ids)) {
        CRM_Core_DAO::commonRetrieveAll('CRM_Member_DAO_Membership',
          'id',
          $membership->id,
          $data,
          array('contact_id', 'membership_type_id', 'source')
        );

        $membership->contact_id = $data[$membership->id]['contact_id'];
        $membership->membership_type_id = $data[$membership->id]['membership_type_id'];
        $membership->source = CRM_Utils_Array::value('source', $data[$membership->id]);
      }

      // since we are going to create activity record w/
      // individual contact as a target in case of on behalf signup,
      // so get the copy of organization id, CRM-5551
      $realMembershipContactId = $membership->contact_id;

      // create activity source = individual, target = org CRM-4027
      $targetContactID = NULL;
      if (CRM_Utils_Array::value('is_for_organization', $params)) {
        $targetContactID = $membership->contact_id;
        $membership->contact_id = CRM_Utils_Array::value('userId', $ids);
      }

      if (empty($membership->contact_id) && (!empty($membership->owner_membership_id))) {
        $membership->contact_id = $realMembershipContactId;
      }
      require_once 'CRM/Activity/BAO/Activity.php';
      CRM_Activity_BAO_Activity::addActivity($membership, $activityType, $targetContactID);

      // we might created activity record w/ individual
      // contact as target so update membership object w/
      // original organization id, CRM-5551
      $membership->contact_id = $realMembershipContactId;
    }

    $transaction->commit();

    self::createRelatedMemberships($params, $membership);

    // do not add to recent items for import, CRM-4399
    if (!CRM_Utils_Array::value('skipRecentView', $params)) {
      require_once 'CRM/Utils/Recent.php';
      require_once 'CRM/Member/PseudoConstant.php';
      require_once 'CRM/Contact/BAO/Contact.php';
      $url = CRM_Utils_System::url('civicrm/contact/view/membership',
        "action=view&reset=1&id={$membership->id}&cid={$membership->contact_id}&context=home"
      );

      $membershipTypes = CRM_Member_PseudoConstant::membershipType();
      $title = CRM_Contact_BAO_Contact::displayName($membership->contact_id) . ' - ' . ts('Membership Type:') . ' ' . $membershipTypes[$membership->membership_type_id];

      require_once 'CRM/Core/Permission.php';
      $recentOther = array();
      if (CRM_Core_Permission::checkActionPermission('CiviMember', CRM_Core_Action::UPDATE)) {
        $recentOther['editUrl'] = CRM_Utils_System::url('civicrm/contact/view/membership',
          "action=update&reset=1&id={$membership->id}&cid={$membership->contact_id}&context=home"
        );
      }
      if (CRM_Core_Permission::checkActionPermission('CiviMember', CRM_Core_Action::DELETE)) {
        $recentOther['deleteUrl'] = CRM_Utils_System::url('civicrm/contact/view/membership',
          "action=delete&reset=1&id={$membership->id}&cid={$membership->contact_id}&context=home"
        );
      }

      // add the recently created Membership
      CRM_Utils_Recent::add($title,
        $url,
        $membership->id,
        'Membership',
        $membership->contact_id,
        NULL,
        $recentOther
      );
    }

    return $membership;
  }

  /**
   * Function to check the membership extended through relationship
   *
   * @param int $membershipId membership id
   * @param int $contactId    contact id
   *
   * @return Array    array of contact_id of all related contacts.
   * @static
   */
  static
  function checkMembershipRelationship($membershipId, $contactId, $action = CRM_Core_Action::ADD) {
    $contacts = array();
    $membershipTypeID = CRM_Core_DAO::getFieldValue('CRM_Member_DAO_Membership', $membershipId, 'membership_type_id');

    $membershipType = CRM_Member_BAO_MembershipType::getMembershipTypeDetails($membershipTypeID);
    require_once 'CRM/Contact/BAO/Relationship.php';
    $relationships = array();
    if (isset($membershipType['relationship_type_id'])) {
      $relationships = CRM_Contact_BAO_Relationship::getRelationship($contactId,
        CRM_Contact_BAO_Relationship::CURRENT
      );
      if ($action & CRM_Core_Action::UPDATE) {
        $pastRelationships = CRM_Contact_BAO_Relationship::getRelationship($contactId,
          CRM_Contact_BAO_Relationship::PAST
        );
        $relationships = array_merge($relationships, $pastRelationships);
      }
    }

    if (!empty($relationships)) {
      require_once 'CRM/Contact/BAO/RelationshipType.php';
      // check for each contact relationships
      foreach ($relationships as $values) {
        //get details of the relationship type
        $relType = array('id' => $values['civicrm_relationship_type_id']);
        $relValues = array();
        CRM_Contact_BAO_RelationshipType::retrieve($relType, $relValues);
        // Check if contact's relationship type exists in membership type
        $relTypeDirs   = array();
        $relTypeIds    = explode(CRM_Core_DAO::VALUE_SEPARATOR, $membershipType['relationship_type_id']);
        $relDirections = explode(CRM_Core_DAO::VALUE_SEPARATOR, $membershipType['relationship_direction']);
        $bidirectional = FALSE;
        foreach ($relTypeIds as $key => $value) {
          $relTypeDirs[] = $value . '_' . $relDirections[$key];
          if (in_array($value, $relType) &&
            $relValues['name_a_b'] == $relValues['name_b_a']
          ) {
            $bidirectional = TRUE;
            break;
          }
        }
        $relTypeDir = $values['civicrm_relationship_type_id'] . '_' . $values['rtype'];
        if ($bidirectional || in_array($relTypeDir, $relTypeDirs)) {
          // $values['status'] is going to have value for
          // current or past relationships.
          $contacts[$values['cid']] = $values['status'];
        }
      }
    }

    return $contacts;
  }

  /**
   * Takes a bunch of params that are needed to match certain criteria and
   * retrieves the relevant objects. We'll tweak this function to be more
   * full featured over a period of time. This is the inverse function of
   * create.  It also stores all the retrieved values in the default array
   *
   * @param array $params   (reference ) an assoc array of name/value pairs
   * @param array $defaults (reference ) an assoc array to hold the name / value pairs
   *                        in a hierarchical manner
   * @param array $ids      (reference) the array that holds all the db ids
   *
   * @return object CRM_Member_BAO_Membership object
   * @access public
   * @static
   */
  static
  function retrieve(&$params, &$defaults) {
    $membership = new CRM_Member_DAO_Membership();

    $membership->copyValues($params);

    if ($membership->find(TRUE)) {
      CRM_Core_DAO::storeValues($membership, $defaults);

      //get the membership status and type values.
      $statusANDType = self::getStatusANDTypeValues($membership->id);
      foreach (array(
        'status', 'membership_type') as $fld) {
        $defaults[$fld] = CRM_Utils_Array::value($fld, $statusANDType[$membership->id]);
      }
      if (CRM_Utils_Array::value('is_current_member', $statusANDType[$membership->id])) {
        $defaults['active'] = TRUE;
      }

      $membership->free();

      return $membership;
    }

    return NULL;
  }

  /**
   *
   * Function to get membership status and membership type values
   *
   * @param int $membershipId membership id of values to return
   *
   * @return array of key value pairs
   * @access public
   */
  function getStatusANDTypeValues($membershipId) {
    $values = array();
    if (!$membershipId) {
      return $values;
    }
    $sql = '
    SELECT  membership.id as id,
            status.id as status_id,
            status.label as status,
            status.is_current_member as is_current_member,
            type.id as membership_type_id,
            type.name as membership_type
      FROM  civicrm_membership membership
INNER JOIN  civicrm_membership_status status ON ( status.id = membership.status_id )
INNER JOIN  civicrm_membership_type type ON ( type.id = membership.membership_type_id )
     WHERE  membership.id = %1';
    $dao = CRM_Core_DAO::executeQuery($sql, array(1 => array($membershipId, 'Positive')));
    $properties = array('status', 'status_id', 'membership_type', 'membership_type_id', 'is_current_member');
    while ($dao->fetch()) {
      foreach ($properties as $property) {
        $values[$dao->id][$property] = $dao->$property;
      }
    }

    return $values;
  }

  /**
   * Function to delete membership.
   *
   * @param int $membershipId membership id that needs to be deleted
   *
   * @static
   *
   * @return $results   no of deleted Membership on success, false otherwise
   * @access public
   */
  static
  function deleteMembership($membershipId) {
    require_once 'CRM/Utils/Hook.php';
    CRM_Utils_Hook::pre('delete', 'Membership', $membershipId, CRM_Core_DAO::$_nullArray);

    require_once 'CRM/Core/Transaction.php';
    $transaction = new CRM_Core_Transaction();

    $results = NULL;
    //delete activity record
    $activityTypes = CRM_Core_PseudoConstant::activityType(TRUE, FALSE, FALSE, 'name');

    require_once 'CRM/Activity/BAO/Activity.php';
    $params = array(
      'source_record_id' => $membershipId,
      'activity_type_id' => array(array_search('Membership Signup', $activityTypes),
        array_search('Membership Renewal', $activityTypes),
      ),
    );

    CRM_Activity_BAO_Activity::deleteActivity($params);

    self::deleteMembershipPayment($membershipId);

    require_once 'CRM/Member/DAO/Membership.php';
    $membership     = new CRM_Member_DAO_Membership();
    $membership->id = $membershipId;
    $results        = $membership->delete();
    $transaction->commit();

    CRM_Utils_Hook::post('delete', 'Membership', $membership->id, $membership);

    // delete the recently created Membership
    require_once 'CRM/Utils/Recent.php';
    $membershipRecent = array(
      'id' => $membershipId,
      'type' => 'Membership',
    );
    CRM_Utils_Recent::del($membershipRecent);

    return $results;
  }

  /**
   * Function to obtain active/inactive memberships from the list of memberships passed to it.
   *
   * @param array  $memberships membership records
   * @param string $status      active or inactive
   *
   * @return array $actives array of memberships based on status
   * @static
   * @access public
   */
  static
  function activeMembers($memberships, $status = 'active') {
    $actives = array();
    if ($status == 'active') {
      foreach ($memberships as $f => $v) {
        if (CRM_Utils_Array::value('active', $v)) {
          $actives[$f] = $v;
        }
      }
      return $actives;
    }
    elseif ($status == 'inactive') {
      foreach ($memberships as $f => $v) {
        if (!CRM_Utils_Array::value('active', $v)) {
          $actives[$f] = $v;
        }
      }
      return $actives;
    }
    return NULL;
  }

  /**
   * Function to build Membership  Block in Contribution Pages
   *
   * @param object  $form                      form object
   * @param int     $pageId                    contribution page id
   * @param boolean $formItems
   * @param int     $selectedMembershipTypeID  selected membership id
   * @param boolean $thankPage                 thank you page
   * @param boolean $memContactId              contact who is to be
   * checked for having a current membership for a particular membership
   *
   * @static
   */
  static
  function buildMembershipBlock(&$form,
    $pageID,
    $formItems = FALSE,
    $selectedMembershipTypeID = NULL,
    $thankPage = FALSE,
    $isTest = NULL,
    $memberContactId = NULL
  ) {
    require_once 'CRM/Member/DAO/MembershipBlock.php';

    $separateMembershipPayment = FALSE;
    if ($form->_membershipBlock) {
      $form->_currentMemberships = array();
      if (!$memberContactId) {
        $session = CRM_Core_Session::singleton();
        $cid = $session->get('userID');
      }
      else {
        $cid = $memberContactId;
      }

      $membershipBlock    = $form->_membershipBlock;
      $membershipTypeIds  = array();
      $membershipTypes    = array();
      $radio              = array();
      $membershipPriceset = (!empty($form->_priceSetId) && $form->_useForMember) ? TRUE : FALSE;

      $allowAutoRenewMembership = $autoRenewOption = FALSE;
      $autoRenewMembershipTypeOptions = array();

      $separateMembershipPayment = CRM_Utils_Array::value('is_separate_payment', $membershipBlock);

      if ($membershipPriceset) {
        foreach ($form->_priceSet['fields'] as $pField) {
          if (empty($pField['options'])) {
            continue;
          }
          foreach ($pField['options'] as $opId => $opValues) {
            if (!CRM_Utils_Array::value('membership_type_id', $opValues)) {
              continue;
            }
            $membershipTypeIds[$opValues['membership_type_id']] = $opValues['membership_type_id'];
          }
        }
      }
      elseif (CRM_Utils_Array::value('membership_types', $membershipBlock)) {
        $membershipTypeIds = explode(',', $membershipBlock['membership_types']);
      }

      if (!empty($membershipTypeIds)) {
        //set status message if wrong membershipType is included in membershipBlock
        if (isset($form->_mid) && !$membershipPriceset) {
          $membershipTypeID = CRM_Core_DAO::getFieldValue('CRM_Member_DAO_Membership',
            $form->_mid,
            'membership_type_id'
          );
          if (!in_array($membershipTypeID, $membershipTypeIds)) {
            CRM_Core_Session::setStatus(ts("Oops. The membership you're trying to renew appears to be invalid. Contact your site administrator if you need assistance. If you continue, you will be issued a new membership."));
          }
        }

        $membershipTypeValues = self::buildMembershipTypeValues($form, $membershipTypeIds);
        $form->_membershipTypeValues = $membershipTypeValues;
        $endDate = NULL;
        foreach ($membershipTypeIds as $value) {
          $memType = $membershipTypeValues[$value];
          if ($selectedMembershipTypeID != NULL) {
            if ($memType['id'] == $selectedMembershipTypeID) {
              $form->assign('minimum_fee',
                CRM_Utils_Array::value('minimum_fee', $memType)
              );
              $form->assign('membership_name', $memType['name']);
              if (!$thankPage && $cid) {
                $membership = new CRM_Member_DAO_Membership();
                $membership->contact_id = $cid;
                $membership->membership_type_id = $memType['id'];
                if ($membership->find(TRUE)) {
                  $form->assign('renewal_mode', TRUE);
                  $memType['current_membership'] = $membership->end_date;
                  $form->_currentMemberships[$membership->membership_type_id] = $membership->membership_type_id;
                }
              }
              $membershipTypes[] = $memType;
            }
          }
          elseif ($memType['is_active']) {
            $javascriptMethod = NULL;
            $allowAutoRenewOpt = CRM_Utils_Array::value($value, $form->_membershipBlock['auto_renew']);
            if (!CRM_Utils_Array::value('is_recur', $form->_paymentProcessor)) {
              $allowAutoRenewOpt = 0;
            }
            $javascriptMethod = array('onclick' => "return showHideAutoRenew( this.value );");
            $autoRenewMembershipTypeOptions["autoRenewMembershipType_{$value}"] = (int)$allowAutoRenewOpt;
            if ($allowAutoRenewOpt) {
              $allowAutoRenewMembership = TRUE;
            }

            //add membership type.
            $radio[$memType['id']] = $form->createElement('radio', NULL, NULL, NULL,
              $memType['id'], $javascriptMethod
            );
            if ($cid) {
              $membership = new CRM_Member_DAO_Membership();
              $membership->contact_id = $cid;
              $membership->membership_type_id = $memType['id'];

              //show current membership, skip pending and cancelled membership records,
              //because we take first membership record id for renewal
              $membership->whereAdd('status_id != 5 AND status_id !=6');

              if (!is_null($isTest)) {
                $membership->is_test = $isTest;
              }

              //CRM-4297
              $membership->orderBy('end_date DESC');

              if ($membership->find(TRUE)) {
                if (!$membership->end_date) {
                  unset($radio[$memType['id']]);
                  $form->assign('islifetime', TRUE);
                  continue;
                }
                $form->assign('renewal_mode', TRUE);
                $form->_currentMemberships[$membership->membership_type_id] = $membership->membership_type_id;
                $memType['current_membership'] = $membership->end_date;
                if (!$endDate) {
                  $endDate = $memType['current_membership'];
                  $form->_defaultMemTypeId = $memType['id'];
                }
                if ($memType['current_membership'] < $endDate) {
                  $endDate = $memType['current_membership'];
                  $form->_defaultMemTypeId = $memType['id'];
                }
              }
            }
            $membershipTypes[] = $memType;
          }
        }
      }

      $form->assign('showRadio', $formItems);
      if ($formItems) {
        if (!$membershipPriceset) {
          if (!$membershipBlock['is_required']) {
            $form->assign('showRadioNoThanks', TRUE);
            $radio[''] = $form->createElement('radio', NULL, NULL, NULL, 'no_thanks', NULL);
            $form->addGroup($radio, 'selectMembership', NULL);
          }
          elseif ($membershipBlock['is_required'] && count($radio) == 1) {
            $temp = array_keys($radio);
            $form->add('hidden', 'selectMembership', $temp[0], array('id' => 'selectMembership'));
            $form->assign('singleMembership', TRUE);
            $form->assign('showRadio', FALSE);
          }
          else {
            $form->addGroup($radio, 'selectMembership', NULL);
          }

          $form->addRule('selectMembership', ts('Please select one of the memberships.'), 'required');
        }
        else {
          require_once 'CRM/Price/BAO/Set.php';
          $autoRenewOption = CRM_Price_BAO_Set::checkAutoRenewForPriceSet($form->_priceSetId);
          $form->assign('autoRenewOption', $autoRenewOption);
        }

        if (CRM_Utils_Array::value('is_recur', $form->_paymentProcessor) &&
          ($allowAutoRenewMembership || $autoRenewOption)
        ) {
          $form->addElement('checkbox', 'auto_renew', ts('Please renew my membership automatically.'));
        }
      }

      $form->assign('membershipBlock', $membershipBlock);
      $form->assign('membershipTypes', $membershipTypes);
      $form->assign('allowAutoRenewMembership', $allowAutoRenewMembership);
      $form->assign('autoRenewMembershipTypeOptions', json_encode($autoRenewMembershipTypeOptions));

      //give preference to user submitted auto_renew value.
      $takeUserSubmittedAutoRenew = (!empty($_POST) || $form->isSubmitted()) ? TRUE : FALSE;
      $form->assign('takeUserSubmittedAutoRenew', $takeUserSubmittedAutoRenew);
    }

    return $separateMembershipPayment;
  }

  /**
   * Function to return Membership Block info in Contribution Pages
   *
   * @param int $pageId contribution page id
   *
   * @static
   */
  static
  function getMembershipBlock($pageID) {
    $membershipBlock = array();
    require_once 'CRM/Member/DAO/MembershipBlock.php';
    $dao = new CRM_Member_DAO_MembershipBlock();
    $dao->entity_table = 'civicrm_contribution_page';

    $dao->entity_id = $pageID;
    $dao->is_active = 1;
    if ($dao->find(TRUE)) {
      CRM_Core_DAO::storeValues($dao, $membershipBlock);
      if (CRM_Utils_Array::value('membership_types', $membershipBlock)) {
        $membershipTypes = unserialize($membershipBlock['membership_types']);
        if (!is_array($membershipTypes)) {
          return $membershipBlock;
        }
        foreach ($membershipTypes as $key => $value) {
          $membershipBlock['auto_renew'][$key] = $value;
          $memTypes[$key] = $key;
        }
        $membershipBlock['membership_types'] = implode(',', $memTypes);
      }
    }
    else {
      return NULL;
    }

    return $membershipBlock;
  }

  /**
   * Function to return a current membership of given contact
   *        NB: if more than one membership meets criteria, a randomly selected one is returned.
   *
   * @param int $contactID  contact id
   * @param int $memType membership type, null to retrieve all types
   * @param int $isTest
   * @param int $membershipID if provided, then determine if it is current
   * @param boolean $onlySameParentOrg true if only Memberships with same parent org as the $memType wanted, false otherwise
   * @static
   */
  static
  function getContactMembership($contactID, $memType, $isTest, $membershipId = NULL, $onlySameParentOrg = FALSE) {
    $dao = new CRM_Member_DAO_Membership();
    if ($membershipId) {
      $dao->id = $membershipId;
    }
    $dao->contact_id = $contactID;
    $dao->membership_type_id = $memType;

    //fetch proper membership record.
    if ($isTest) {
      $dao->is_test = $isTest;
    }
    else {
      $dao->whereAdd('is_test IS NULL OR is_test = 0');
    }

    //avoid pending membership as current membership: CRM-3027
    require_once 'CRM/Member/PseudoConstant.php';
    $pendingStatusId = array_search('Pending', CRM_Member_PseudoConstant::membershipStatus());
    $dao->whereAdd("status_id != $pendingStatusId");

    // order by start date to find most recent membership first, CRM-4545
    $dao->orderBy('start_date DESC');

    // CRM-8141
    if ($onlySameParentOrg && $memType) {
      // require the same parent org as the $memType
      $params = array('id' => $memType);
      $defaults = array();
      require_once 'CRM/Member/BAO/MembershipType.php';
      if (CRM_Member_BAO_MembershipType::retrieve($params, $membershipType)) {
        $memberTypesSameParentOrg = CRM_Member_BAO_MembershipType::getMembershipTypesByOrg($membershipType['member_of_contact_id']);
        $memberTypesSameParentOrgList = implode(',', array_keys($memberTypesSameParentOrg));
        $dao->whereAdd('membership_type_id IN (' . $memberTypesSameParentOrgList . ')');
      }
    }

    if ($dao->find(TRUE)) {
      $membership = array();
      CRM_Core_DAO::storeValues($dao, $membership);
      $membership['is_current_member'] = CRM_Core_DAO::getFieldValue('CRM_Member_DAO_MembershipStatus',
        $membership['status_id'],
        'is_current_member', 'id'
      );
      return $membership;
    }

    // CRM-8141
    if ($onlySameParentOrg && $memType) {
      // see if there is a membership that has same parent as $memType but different parent than $membershipID

      if ($dao->id && CRM_Core_Permission::check('edit memberships')) {
        // CRM-10016, This is probably a backend renewal, and make sure we return the same membership thats being renewed.
        $dao->whereAdd();
      }
      else {
        unset($dao->id);
      }

      unset($dao->membership_type_id);
      if ($dao->find(TRUE)) {
        $membership = array();
        CRM_Core_DAO::storeValues($dao, $membership);
        $membership['is_current_member'] = CRM_Core_DAO::getFieldValue('CRM_Member_DAO_MembershipStatus',
          $membership['status_id'],
          'is_current_member', 'id'
        );
        return $membership;
      }
    }
    return FALSE;
  }

  /**
   * Combine all the importable fields from the lower levels object
   *
   * @param string  $contactType contact type
   * @param boolean $status
   *
   * @return array array of importable Fields
   * @access public
   */
  function &importableFields($contactType = 'Individual', $status = TRUE) {
    if (!self::$_importableFields) {
      if (!self::$_importableFields) {
        self::$_importableFields = array();
      }

      if (!$status) {
        $fields = array('' => array('title' => '- ' . ts('do not import') . ' -'));
      }
      else {
        $fields = array('' => array('title' => '- ' . ts('Membership Fields') . ' -'));
      }

      $tmpFields = CRM_Member_DAO_Membership::import();
      require_once 'CRM/Contact/BAO/Contact.php';
      $contactFields = CRM_Contact_BAO_Contact::importableFields($contactType, NULL);

      // Using new Dedupe rule.
      $ruleParams = array(
        'contact_type' => $contactType,
        'level' => 'Strict',
      );
      require_once 'CRM/Dedupe/BAO/Rule.php';
      $fieldsArray = CRM_Dedupe_BAO_Rule::dedupeRuleFields($ruleParams);

      $tmpContactField = array();
      if (is_array($fieldsArray)) {
        foreach ($fieldsArray as $value) {
          $customFieldId = CRM_Core_DAO::getFieldValue('CRM_Core_DAO_CustomField',
            $value,
            'id',
            'column_name'
          );
          $value = $customFieldId ? 'custom_' . $customFieldId : $value;
          $tmpContactField[trim($value)] = CRM_Utils_Array::value(trim($value), $contactFields);
          if (!$status) {
            $title = $tmpContactField[trim($value)]['title'] . " " . ts('(match to contact)');
          }
          else {
            $title = $tmpContactField[trim($value)]['title'];
          }
          $tmpContactField[trim($value)]['title'] = $title;
        }
      }
      $tmpContactField['external_identifier'] = $contactFields['external_identifier'];
      $tmpContactField['external_identifier']['title'] = $contactFields['external_identifier']['title'] . " " . ts('(match to contact)');

      $tmpFields['membership_contact_id']['title'] = $tmpFields['membership_contact_id']['title'] . " " . ts('(match to contact)');;

      $fields = array_merge($fields, $tmpContactField);
      $fields = array_merge($fields, $tmpFields);
      $fields = array_merge($fields, CRM_Core_BAO_CustomField::getFieldsForImport('Membership'));
      self::$_importableFields = $fields;
    }
    return self::$_importableFields;
  }

  /**
   * function to get all exportable fields
   *
   * @retun array return array of all exportable fields
   */
  function &exportableFields() {
    $expFieldMembership = CRM_Member_DAO_Membership::export();
    //campaign fields.
    if (isset($expFieldMembership['member_campaign_id'])) {
      $expFieldMembership['member_campaign'] = array('title' => ts('Campaign Title'));
    }

    $expFieldsMemType = CRM_Member_DAO_MembershipType::export();
    $fields           = array_merge($expFieldMembership, $expFieldsMemType);
    $fields           = array_merge($fields, $expFieldMembership);
    $membershipStatus = array(
      'membership_status' => array('title' => 'Membership Status',
        'name' => 'membership_status',
        'type' => CRM_Utils_Type::T_STRING,
        'where' => 'civicrm_membership_status.name',
      ));
    //CRM-6161 fix for customdata export
    $fields = array_merge($fields, $membershipStatus, CRM_Core_BAO_CustomField::getFieldsForImport('Membership'));
    return $fields;
  }

  /**
   * Function to get membership joins/renewals for a specified membership
   * type.  Specifically, retrieves a count of memberships whose start_date
   * is within a specified date range.  Dates match the regexp
   * "yyyy(mm(dd)?)?".  Omitted portions of a date match the earliest start
   * date or latest end date, i.e., 200803 is March 1st as a start date and
   * March 31st as an end date.
   *
   * @param int    $membershipTypeId  membership type id
   * @param int    $startDate         date on which to start counting
   * @param int    $endDate           date on which to end counting
   * @param bool   $isTest            if true, membership is for a test site
   * @param bool   $isOwner           if true, only retrieve membership records for owners //LCD
   *
   * @return returns the number of members of type $membershipTypeId whose
   *         start_date is between $startDate and $endDate
   */
  //LCD
  function getMembershipStarts($membershipTypeId, $startDate, $endDate, $isTest = 0, $isOwner = 0) {
    $query = "SELECT count(civicrm_membership.id) as member_count
  FROM   civicrm_membership left join civicrm_membership_status on ( civicrm_membership.status_id = civicrm_membership_status.id )
WHERE  membership_type_id = %1 AND start_date >= '$startDate' AND start_date <= '$endDate' 
AND civicrm_membership_status.is_current_member = 1
AND civicrm_membership.contact_id NOT IN (SELECT id FROM civicrm_contact WHERE is_deleted = 1)
AND is_test = %2";
    // LCD
    $query .= ($isOwner) ? ' AND owner_membership_id IS NULL' : '';
    $params = array(1 => array($membershipTypeId, 'Integer'),
      2 => array($isTest, 'Boolean'),
    );
    $memberCount = CRM_Core_DAO::singleValueQuery($query, $params);
    return (int)$memberCount;
  }

  /**
   * Function to get a count of membership for a specified membership type,
   * optionally for a specified date.  The date must have the form yyyymmdd.
   *
   * If $date is omitted, this function counts as a member anyone whose
   * membership status_id indicates they're a current member.
   * If $date is given, this function counts as a member anyone who:
   *  -- Has a start_date before $date and end_date after $date, or
   *  -- Has a start_date before $date and is currently a member, as indicated
   *     by the the membership's status_id.
   * The second condition takes care of records that have no end_date.  These
   * are assumed to be lifetime memberships.
   *
   * @param int    $membershipTypeId   membership type id
   * @param string $date               the date for which to retrieve the count
   * @param bool   $isTest             if true, membership is for a test site
   * @param bool   $isOwner           if true, only retrieve membership records for owners //LCD
   *
   * @return returns the number of members of type $membershipTypeId as of
   *         $date.
   */
  function getMembershipCount($membershipTypeId, $date = NULL, $isTest = 0, $isOwner = 0) {
    if (!is_null($date) && !preg_match('/^\d{8}$/', $date)) {
      CRM_Core_Error::fatal(ts('Invalid date "%1" (must have form yyyymmdd).', array(1 => $date)));
    }

    $params = array(1 => array($membershipTypeId, 'Integer'),
      2 => array($isTest, 'Boolean'),
    );
    $query = "SELECT  count(civicrm_membership.id ) as member_count
FROM   civicrm_membership left join civicrm_membership_status on ( civicrm_membership.status_id = civicrm_membership_status.id  )
WHERE  civicrm_membership.membership_type_id = %1 
AND civicrm_membership.contact_id NOT IN (SELECT id FROM civicrm_contact WHERE is_deleted = 1)
AND civicrm_membership.is_test = %2";
    if (!$date) {
      $query .= " AND civicrm_membership_status.is_current_member = 1";
    }
    else {
      $date = substr($date, 0, 4) . '-' . substr($date, 4, 2) . '-' . substr($date, 6, 2);
      $query .= " AND civicrm_membership.start_date <= '$date' AND civicrm_membership_status.is_current_member = 1";
    }
    // LCD
    $query .= ($isOwner) ? ' AND owner_membership_id IS NULL' : '';
    $memberCount = CRM_Core_DAO::singleValueQuery($query, $params);
    return (int)$memberCount;
  }

  /**
   * Function check the status of the membership before adding membership for a contact
   *
   * @param int $contactId contact id
   *
   * @return
   */
  function statusAvilability($contactId) {
    require_once 'CRM/Member/DAO/MembershipStatus.php';
    $membership = new CRM_Member_DAO_MembershipStatus();
    $membership->whereAdd('is_active=1');
    $count = $membership->count();

    if (!$count) {
      $session = CRM_Core_Session::singleton();
      CRM_Core_Session::setStatus(ts('There are no status present, You cannot add membership.'));
      return CRM_Utils_System::redirect(CRM_Utils_System::url('civicrm/contact/view', "reset=1&force=1&cid={$contactId}&selectedChild=member"));
    }
  }

  /**
   * Process the Memberships
   *
   * @param array  $membershipParams array of membership fields
   * @param int    $contactID        contact id
   * @param object $form             form object
   *
   * @return void
   * @access public
   */
  public function postProcessMembership($membershipParams, $contactID, &$form, &$premiumParams,
    $customFieldsFormatted = NULL, $includeFieldTypes = NULL
  ) {
    $tempParams  = $membershipParams;
    $paymentDone = FALSE;
    $result      = NULL;
    $isTest      = CRM_Utils_Array::value('is_test', $membershipParams, FALSE);
    $form->assign('membership_assign', TRUE);

    $form->set('membershipTypeID', $membershipParams['selectMembership']);

    $membershipTypeID = $membershipParams['selectMembership'];
    $membershipDetails = self::buildMembershipTypeValues($form, $membershipTypeID);

    $form->assign('membership_name', CRM_Utils_Array::value('name', $membershipDetails));

    $minimumFee = CRM_Utils_Array::value('minimum_fee', $membershipDetails);

    $contributionTypeId = NULL;

    if ($form->_values['amount_block_is_active']) {
      $contributionTypeId = $form->_values['contribution_type_id'];
    }
    else {
      $paymentDone        = TRUE;
      $params['amount']   = $minimumFee;
      $contributionTypeId = CRM_Utils_Array::value('contribution_type_id', $membershipDetails);
      if (!$contributionTypeId) {
        $contributionTypeId = $membershipParams['contribution_type_id'];
      }
    }

    //amount must be greater than zero for
    //adding contribution record  to contribution table.
    //this condition arises when separate membership payment is
    //enabled and contribution amount is not selected. fix for CRM-3010
    require_once 'CRM/Contribute/BAO/Contribution/Utils.php';
    if ($form->_amount > 0.0) {
      $result = CRM_Contribute_BAO_Contribution_Utils::processConfirm($form, $membershipParams,
        $premiumParams, $contactID,
        $contributionTypeId,
        'membership'
      );

      // Save the contribution ID so that I can be used in email receipts
      // For example, if you need to generate a tax receipt for the donation only.
      $form->_values['contribution_other_id'] = $result[1]->id;
    }
    else {
      // create the CMS contact here since we normally do this under processConfirm
      CRM_Contribute_BAO_Contribution_Utils::createCMSUser($membershipParams,
        $membershipParams['cms_contactID'],
        'email-' . $form->_bltID
      );
    }

    $errors = array();
    if (is_a($result[1], 'CRM_Core_Error')) {
      $errors[1] = CRM_Core_Error::getMessages($result[1]);
    }
    else {
      $contribution[1] = $result[1];
    }


    $memBlockDetails = CRM_Member_BAO_Membership::getMembershipBlock($form->_id);
    if (CRM_Utils_Array::value('is_separate_payment', $memBlockDetails) && !$paymentDone) {
      require_once 'CRM/Contribute/DAO/ContributionType.php';
      $contributionType = new CRM_Contribute_DAO_ContributionType();
      $contributionType->id = $membershipDetails['contribution_type_id'];
      if (!$contributionType->find(TRUE)) {
        CRM_Core_Error::fatal(ts("Could not find a system table"));
      }
      $tempParams['amount'] = $minimumFee;
      $invoiceID = md5(uniqid(rand(), TRUE));
      $tempParams['invoiceID'] = $invoiceID;

      //we don't allow recurring membership.CRM-3781.
      if (CRM_Utils_Array::value('is_recur', $tempParams)) {
        $tempParams['is_recur'] = 0;
      }

      $result = NULL;
      if ($form->_values['is_monetary'] && !$form->_params['is_pay_later'] && $minimumFee > 0.0) {
        require_once 'CRM/Core/Payment.php';
        $payment = CRM_Core_Payment::singleton($form->_mode, $form->_paymentProcessor, $form);

        if ($form->_contributeMode == 'express') {
          $result = &$payment->doExpressCheckout($tempParams);
        }
        else {
          $result = &$payment->doDirectPayment($tempParams);
        }
      }

      if (is_a($result, 'CRM_Core_Error')) {
        $errors[2] = CRM_Core_Error::getMessages($result);
      }
      else {
        //assign receive date when separate membership payment
        //and contribution amount not selected.
        if ($form->_amount == 0) {
          $now = date('YmdHis');
          $form->_params['receive_date'] = $now;
          $receiveDate = CRM_Utils_Date::mysqlToIso($now);
          $form->set('params', $form->_params);
          $form->assign('receive_date', $receiveDate);
        }

        $form->set('membership_trx_id', $result['trxn_id']);
        $form->set('membership_amount', $minimumFee);

        $form->assign('membership_trx_id', $result['trxn_id']);
        $form->assign('membership_amount', $minimumFee);

        // we dont need to create the user twice, so lets disable cms_create_account
        // irrespective of the value, CRM-2888
        $tempParams['cms_create_account'] = 0;

        $pending = $form->_params['is_pay_later'] ? ((CRM_Utils_Array::value('minimum_fee', $membershipDetails, 0) > 0.0) ? TRUE : FALSE) : FALSE;

        //set this variable as we are not creating pledge for
        //separate membership payment contribution.
        //so for differentiating membership contributon from
        //main contribution.
        $form->_params['separate_membership_payment'] = 1;

        $contribution[2] = CRM_Contribute_Form_Contribution_Confirm::processContribution($form,
          $tempParams,
          $result,
          $contactID,
          $contributionType,
          FALSE,
          $pending
        );
      }
    }

    $index = CRM_Utils_Array::value('is_separate_payment', $memBlockDetails) ? 2 : 1;

    if (!CRM_Utils_Array::value($index, $errors)) {
      if (CRM_Utils_Array::value('member_campaign_id', $membershipParams['onbehalf'])) {
        $form->_params['campaign_id'] = $membershipParams['onbehalf']['member_campaign_id'];
      }
      if (is_array($membershipTypeID)) {
        $createdMemberships = array();
        foreach ($membershipTypeID as $memType) {
          $membership = self::renewMembership($contactID, $memType,
            $isTest, $form, NULL,
            CRM_Utils_Array::value('cms_contactID', $membershipParams),
            $customFieldsFormatted
          );
          $createdMemberships[$memType] = $membership;
          if (isset($contribution[$index])) {
            //insert payment record
            require_once 'CRM/Member/DAO/MembershipPayment.php';
            $dao = new CRM_Member_DAO_MembershipPayment();
            $dao->membership_id = $membership->id;
            $dao->contribution_id = $contribution[$index]->id;
            //Fixed for avoiding duplicate entry error when user goes
            //back and forward during payment mode is notify
            if (!$dao->find(TRUE)) {
              CRM_Utils_Hook::pre('create', 'MembershipPayment', NULL, $dao);
              $dao->save();
              CRM_Utils_Hook::post('create', 'MembershipPayment', $dao->id, $dao);
            }
          }
        }
        if ($form->_priceSetId && !empty($form->_useForMember) && !empty($form->_lineItem)) {
          foreach ($form->_lineItem[$form->_priceSetId] as & $priceFieldOp) {
            if (CRM_Utils_Array::value('membership_type_id', $priceFieldOp) &&
              isset($createdMemberships[$priceFieldOp['membership_type_id']])
            ) {
              $membershipOb = $createdMemberships[$priceFieldOp['membership_type_id']];
              $priceFieldOp['start_date'] = $membershipOb->start_date ? CRM_Utils_Date::customFormat($membershipOb->start_date, '%d%f %b, %Y') : '-';
              $priceFieldOp['end_date'] = $membershipOb->end_date ? CRM_Utils_Date::customFormat($membershipOb->end_date, '%d%f %b, %Y') : '-';
            }
            else {
              $priceFieldOp['start_date'] = $priceFieldOp['end_date'] = 'N/A';
            }
          }
          $form->_values['lineItem'] = $form->_lineItem;
          $form->assign('lineItem', $form->_lineItem);
        }
      }
      else {
        $membership = self::renewMembership($contactID, $membershipTypeID,
          $isTest, $form, NULL,
          CRM_Utils_Array::value('cms_contactID', $membershipParams),
          $customFieldsFormatted
        );
        if (isset($contribution[$index])) {
          //insert payment record
          require_once 'CRM/Member/DAO/MembershipPayment.php';
          $dao = new CRM_Member_DAO_MembershipPayment();
          $dao->membership_id = $membership->id;
          $dao->contribution_id = $contribution[$index]->id;
          //Fixed for avoiding duplicate entry error when user goes
          //back and forward during payment mode is notify
          if (!$dao->find(TRUE)) {
            CRM_Utils_Hook::pre('create', 'MembershipPayment', NULL, $dao);
            $dao->save();
            CRM_Utils_Hook::post('create', 'MembershipPayment', $dao->id, $dao);
          }
        }
      }
    }

    if (!empty($errors)) {
      foreach ($errors as $error) {
        if (is_string($error)) {
          $message[] = $error;
        }
      }
      $message = ts('Payment Processor Error message') . ': ' . implode('<br/>', $message);
      $session = CRM_Core_Session::singleton();
      $session->setStatus($message);
      CRM_Utils_System::redirect(CRM_Utils_System::url('civicrm/contribute/transact',
          "_qf_Main_display=true&qfKey={$form->_params['qfKey']}"
        ));
    }

    // CRM-7851
    require_once 'CRM/Core/BAO/CustomValueTable.php';
    CRM_Core_BAO_CustomValueTable::postProcess($form->_params,
      CRM_Core_DAO::$_nullArray,
      'civicrm_membership',
      $membership->id,
      'Membership'
    );

    $form->_params['membershipID'] = $membership->id;
    if ($form->_contributeMode == 'notify') {
      if ($form->_values['is_monetary'] && $form->_amount > 0.0 && !$form->_params['is_pay_later']) {
        // this does not return
        require_once 'CRM/Core/Payment.php';
        $payment = CRM_Core_Payment::singleton($form->_mode, $form->_paymentProcessor, $form);
        $payment->doTransferCheckout($form->_params, 'contribute');
      }
    }

    $form->_values['membership_id'] = $membership->id;
    if (isset($contribution[$index]->id)) {
      $form->_values['contribution_id'] = $contribution[$index]->id;
    }

    // Do not send an email if Recurring transaction is done via Direct Mode
    // Email will we sent when the IPN is received.
    if (CRM_Utils_Array::value('is_recur', $form->_params) && $form->_contributeMode == 'direct') {
      return TRUE;
    }

    //finally send an email receipt
    require_once 'CRM/Contribute/BAO/ContributionPage.php';
    CRM_Contribute_BAO_ContributionPage::sendMail($contactID,
      $form->_values,
      $isTest, FALSE,
      $includeFieldTypes
    );
  }

  /**
   * This method will renew / create the membership depending on
   * whether the given contact has a membership or not. And will add
   * the modified dates for membership and in the log table.
   *
   * @param int     $contactID           id of the contact
   * @param int     $membershipTypeID    id of the new membership type
   * @param boolean $is_test             if this is test contribution or live contribution
   * @param object  $form                form object
   * @param array   $ipnParams           array of name value pairs, to be used (for e.g source) when $form not present
   * @param int     $modifiedID          individual contact id in case of On Behalf signup (CRM-4027 )
   * @param int     $numRenewTerms       how many membership terms are being added to end date (default is 1)
   *
   * @return object $membership          object of membership
   *
   * @static
   * @access public
   *
   **/
  static
  function renewMembership($contactID, $membershipTypeID, $is_test,
    &$form, $changeToday = NULL, $modifiedID = NULL,
    $customFieldsFormatted = NULL, $numRenewTerms = 1
  ) {
    require_once 'CRM/Utils/Hook.php';
    $statusFormat = '%Y-%m-%d';
    $format       = '%Y%m%d';
    $ids          = array();

    //get all active statuses of membership.
    require_once 'CRM/Member/PseudoConstant.php';
    $allStatus = CRM_Member_PseudoConstant::membershipStatus();

    $membershipTypeDetails = CRM_Member_BAO_MembershipType::getMembershipTypeDetails($membershipTypeID);

    // check is it pending. - CRM-4555
    $pending = FALSE;
    if (CRM_Utils_Array::value('minimum_fee', $membershipTypeDetails) > 0.0) {
      if (((isset($form->_contributeMode) && $form->_contributeMode == 'notify') ||
          CRM_Utils_Array::value('is_pay_later', $form->_params) ||
          (CRM_Utils_Array::value('is_recur', $form->_params)
            && $form->_contributeMode == 'direct'
          )
        ) &&
        (($form->_values['is_monetary'] && $form->_amount > 0.0) ||
          CRM_Utils_Array::value('separate_membership_payment', $form->_params)
        )
      ) {
        $pending = TRUE;
      }
    }

    //decide status here, if needed.
    $updateStatusId = NULL;

    // CRM-7297 - allow membership type to be be changed during renewal so long as the parent org of new membershipType
    // is the same as the parent org of an existing membership of the contact
    $currentMembership = CRM_Member_BAO_Membership::getContactMembership($contactID, $membershipTypeID,
      $is_test, $form->_membershipId, TRUE
    );
    if ($currentMembership) {
      $activityType = 'Membership Renewal';
      $form->set('renewal_mode', TRUE);

      // Do NOT do anything.
      //1. membership with status : PENDING/CANCELLED (CRM-2395)
      //2. Paylater/IPN renew. CRM-4556.
      if ($pending || in_array($currentMembership['status_id'], array(array_search('Pending', $allStatus),
            array_search('Cancelled', $allStatus),
          ))) {
        $membership = new CRM_Member_DAO_Membership();
        $membership->id = $currentMembership['id'];
        $membership->find(TRUE);

        // CRM-8141 create a membership_log entry so that we will know the membership_type_id to change to when payment completed
        $format = '%Y%m%d';
        // note that we are logging the requested new membership_type_id that may be different than current membership_type_id
        // it will be used when payment is received to update the membership_type_id to what was paid for
        $logParams = array(
          'membership_id' => $membership->id,
          'status_id' => $membership->status_id,
          'start_date' => CRM_Utils_Date::customFormat(
            $membership->start_date,
            $format
          ),
          'end_date' => CRM_Utils_Date::customFormat(
            $membership->end_date,
            $format
          ),
          'modified_date' => CRM_Utils_Date::customFormat(
            date('Ymd'),
            $format
          ),
          'renewal_reminder_date' => CRM_Utils_Date::customFormat(
            $membership->reminder_date,
            $format
          ),
          'membership_type_id' => $membershipTypeID,
        );
        $session = CRM_Core_Session::singleton();
        // If we have an authenticated session, set modified_id to that user's contact_id, else set to membership.contact_id
        if ($session->get('userID')) {
          $logParams['modified_id'] = $session->get('userID');
        }
        else {
          $logParams['modified_id'] = $membership->contact_id;
        }
        require_once 'CRM/Member/BAO/MembershipLog.php';
        CRM_Member_BAO_MembershipLog::add($logParams, CRM_Core_DAO::$_nullArray);

        if (CRM_Utils_Array::value('contributionRecurID', $form->_params)) {
          CRM_Core_DAO::setFieldValue('CRM_Member_DAO_Membership', $membership->id,
            'contribution_recur_id', $form->_params['contributionRecurID']
          );
        }

        return $membership;
      }

      //we renew expired membership, CRM-6277
      if (!$changeToday) {
        if ($form->get('renewDate')) {
          $changeToday = $form->get('renewDate');
        }
        elseif (get_class($form) == 'CRM_Contribute_Form_Contribution_Confirm') {
          $changeToday = date('YmdHis');
        }
      }

      // Check and fix the membership if it is STALE
      self::fixMembershipStatusBeforeRenew($currentMembership, $changeToday);

      // Now Renew the membership
      if (!$currentMembership['is_current_member']) {
        // membership is not CURRENT

        // CRM-7297 Membership Upsell - calculate dates based on new membership type
        $dates = CRM_Member_BAO_MembershipType::getRenewalDatesForMembershipType($currentMembership['id'],
          $changeToday,
          $membershipTypeID,
          $numRenewTerms
        );

        $currentMembership['join_date'] = CRM_Utils_Date::customFormat($currentMembership['join_date'], $format);
        $currentMembership['start_date'] = CRM_Utils_Array::value('start_date', $dates);
        $currentMembership['end_date'] = CRM_Utils_Array::value('end_date', $dates);
        $currentMembership['reminder_date'] = CRM_Utils_Array::value('reminder_date', $dates);
        $currentMembership['is_test'] = $is_test;

        if (CRM_Utils_Array::value('membership_source', $form->_params)) {
          $currentMembership['source'] = $form->_params['membership_source'];
        }
        elseif (isset($form->_values['title']) && !empty($form->_values['title'])) {
          $currentMembership['source'] = ts('Online Contribution:') . ' ' . $form->_values['title'];
        }
        else {
          $currentMembership['source'] = CRM_Core_DAO::getFieldValue('CRM_Member_DAO_Membership',
            $currentMembership['id'],
            'source'
          );
        }

        if (CRM_Utils_Array::value('id', $currentMembership)) {
          $ids['membership'] = $currentMembership['id'];
        }
        $memParams = $currentMembership;
        $memParams['membership_type_id'] = $membershipTypeID;

        //set the log start date.
        $memParams['log_start_date'] = CRM_Utils_Date::customFormat($dates['log_start_date'], $format);
      }
      else {

        // CURRENT Membership
        $membership = new CRM_Member_DAO_Membership();
        $membership->id = $currentMembership['id'];
        $membership->find(TRUE);
        // CRM-7297 Membership Upsell - calculate dates based on new membership type
        $dates = CRM_Member_BAO_MembershipType::getRenewalDatesForMembershipType($membership->id,
          $changeToday,
          $membershipTypeID,
          $numRenewTerms
        );

        // Insert renewed dates for CURRENT membership
        $memParams = array();
        $memParams['join_date'] = CRM_Utils_Date::isoToMysql($membership->join_date);
        $memParams['start_date'] = CRM_Utils_Date::isoToMysql($membership->start_date);
        $memParams['end_date'] = CRM_Utils_Array::value('end_date', $dates);
        $memParams['reminder_date'] = CRM_Utils_Array::value('reminder_date', $dates);
        $memParams['membership_type_id'] = $membershipTypeID;

        //set the log start date.
        $memParams['log_start_date'] = CRM_Utils_Date::customFormat($dates['log_start_date'], $format);
        if (empty($membership->source)) {
          if (CRM_Utils_Array::value('membership_source', $form->_params)) {
            $memParams['source'] = $form->_params['membership_source'];
          }
          elseif (CRM_Utils_Array::value('title', $form->_values)) {
            $memParams['source'] = ts('Online Contribution:') . ' ' . $form->_values['title'];
          }
          else {
            $memParams['source'] = CRM_Core_DAO::getFieldValue('CRM_Member_DAO_Membership',
              $currentMembership['id'],
              'source'
            );
          }
        }

        if (CRM_Utils_Array::value('id', $currentMembership)) {
          $ids['membership'] = $currentMembership['id'];
        }
      }
      //CRM-4555
      if ($pending) {
        $updateStatusId = array_search('Pending', $allStatus);
      }
    }
    else {
      // NEW Membership

      $activityType = 'Membership Signup';
      $memParams = array(
        'contact_id' => $contactID,
        'membership_type_id' => $membershipTypeID,
      );

      if (!$pending) {
        $dates = CRM_Member_BAO_MembershipType::getDatesForMembershipType($membershipTypeID);

        $memParams['join_date'] = CRM_Utils_Array::value('join_date', $dates);
        $memParams['start_date'] = CRM_Utils_Array::value('start_date', $dates);
        $memParams['end_date'] = CRM_Utils_Array::value('end_date', $dates);
        $memParams['reminder_date'] = CRM_Utils_Array::value('reminder_date', $dates);

        require_once 'CRM/Member/BAO/MembershipStatus.php';
        $status = CRM_Member_BAO_MembershipStatus::getMembershipStatusByDate(CRM_Utils_Date::customFormat($dates['start_date'],
            $statusFormat
          ),
          CRM_Utils_Date::customFormat($dates['end_date'],
            $statusFormat
          ),
          CRM_Utils_Date::customFormat($dates['join_date'],
            $statusFormat
          ),
          'today', TRUE
        );
        $updateStatusId = CRM_Utils_Array::value('id', $status);
      }
      else {
        // if IPN/Pay-Later set status to: PENDING
        $updateStatusId = array_search('Pending', $allStatus);
      }

      if (CRM_Utils_Array::value('membership_source', $form->_params)) {
        $memParams['source'] = $form->_params['membership_source'];
      }
      else {
        $memParams['source'] = ts('Online Contribution:') . ' ' . $form->_values['title'];
      }
      $memParams['contribution_recur_id'] = CRM_Utils_Array::value('contributionRecurID', $form->_params);

      $memParams['is_test'] = $is_test;
      $memParams['is_pay_later'] = $form->_params['is_pay_later'];
    }

    //CRM-4555
    //if we decided status here and want to skip status
    //calculation in create( ); then need to pass 'skipStatusCal'.
    if ($updateStatusId) {
      $memParams['status_id'] = $updateStatusId;
      $memParams['skipStatusCal'] = TRUE;
    }

    //since we are renewing,
    //make status override false.
    $memParams['is_override'] = FALSE;

    //CRM-4027, create log w/ individual contact.
    if ($modifiedID) {
      $ids['userId'] = $modifiedID;
      $memParams['is_for_organization'] = TRUE;
    }
    else {
      $ids['userId'] = $contactID;
    }

    //inherit campaign from contrib page.
    if (isset($form->_values) && is_array($form->_values) && !empty($form->_values)) {
      $campaignId = CRM_Utils_Array::value('campaign_id', $form->_params);
      if (!array_key_exists('campaign_id', $form->_params)) {
        $campaignId = CRM_Utils_Array::value('campaign_id', $form->_values);
      }
      $memParams['campaign_id'] = $campaignId;
    }

    $memParams['custom'] = $customFieldsFormatted;
    $membership = self::create($memParams, $ids, FALSE, $activityType);
    // not sure why this statement is here, seems quite odd :( - Lobo: 12/26/2010
    // related to: http://forum.civicrm.org/index.php/topic,11416.msg49072.html#msg49072
    $membership->find(TRUE);
    if (!empty($dates)) {
      $form->assign('mem_start_date',
        CRM_Utils_Date::customFormat($dates['start_date'], $format)
      );
      $form->assign('mem_end_date',
        CRM_Utils_Date::customFormat($dates['end_date'], $format)
      );
    }

    return $membership;
  }

  /**
   * Method to fix membership status of stale membership
   *
   * This method first checks if the membership is stale. If it is,
   * then status will be updated based on existing start and end
   * dates and log will be added for the status change.
   *
   * @param  array  $currentMembership   referance to the array
   *                                     containing all values of
   *                                     the current membership
   * @param  array  $changeToday         array of month, day, year
   *                                     values in case today needs
   *                                     to be customised, null otherwise
   *
   * @return void
   * @static
   */
  static
  function fixMembershipStatusBeforeRenew(&$currentMembership, $changeToday) {
    $today = NULL;
    if ($changeToday) {
      $today = CRM_Utils_Date::processDate($changeToday, NULL, FALSE, 'Y-m-d');
    }

    require_once 'CRM/Member/BAO/MembershipStatus.php';
    $status = CRM_Member_BAO_MembershipStatus::getMembershipStatusByDate(
      $currentMembership['start_date'],
      $currentMembership['end_date'],
      $currentMembership['join_date'],
      $today
    );


    if (empty($status) ||
      empty($status['id'])
    ) {
      CRM_Core_Error::fatal(ts('Oops, it looks like there is no valid membership status corresponding to the membership start and end dates for this membership. Contact the site administrator for assistance.'));
    }

    $currentMembership['today_date'] = $today;

    if ($status['id'] !== $currentMembership['status_id']) {
      $memberDAO = new CRM_Member_DAO_Membership();
      $memberDAO->id = $currentMembership['id'];
      $memberDAO->find(TRUE);

      $memberDAO->status_id = $status['id'];
      $memberDAO->join_date = CRM_Utils_Date::isoToMysql($memberDAO->join_date);
      $memberDAO->start_date = CRM_Utils_Date::isoToMysql($memberDAO->start_date);
      $memberDAO->end_date = CRM_Utils_Date::isoToMysql($memberDAO->end_date);
      $memberDAO->save();
      CRM_Core_DAO::storeValues($memberDAO, $currentMembership);
      $memberDAO->free();

      $currentMembership['is_current_member'] = CRM_Core_DAO::getFieldValue(
        'CRM_Member_DAO_MembershipStatus',
        $currentMembership['status_id'],
        'is_current_member'
      );
      $format = '%Y%m%d';

      $logParams = array(
        'membership_id' => $currentMembership['id'],
        'status_id' => $status['id'],
        'start_date' => CRM_Utils_Date::customFormat(
          $currentMembership['start_date'],
          $format
        ),
        'end_date' => CRM_Utils_Date::customFormat(
          $currentMembership['end_date'],
          $format
        ),
        'modified_date' => CRM_Utils_Date::customFormat(
          $currentMembership['today_date'],
          $format
        ),
        'renewal_reminder_date' => CRM_Utils_Date::customFormat(
          $currentMembership['reminder_date'],
          $format
        ),
        'membership_type_id' => $currentMembership['membership_type_id'],
      );

      $session = CRM_Core_Session::singleton();
      // If we have an authenticated session, set modified_id to that user's contact_id, else set to membership.contact_id
      if ($session->get('userID')) {
        $logParams['modified_id'] = $session->get('userID');
      }
      else {
        $logParams['modified_id'] = $currentMembership['contact_id'];
      }
      require_once 'CRM/Member/BAO/MembershipLog.php';
      CRM_Member_BAO_MembershipLog::add($logParams, CRM_Core_DAO::$_nullArray);
    }
  }

  /**
   * Function to get the contribution page id from the membership record
   *
   * @param int membershipId membership id
   *
   * @return int $contributionPageId contribution page id
   * @access public
   * @static
   */
  static
  function getContributionPageId($membershipID) {
    $query = "
SELECT c.contribution_page_id as pageID
  FROM civicrm_membership_payment mp, civicrm_contribution c
 WHERE mp.contribution_id = c.id
   AND mp.membership_id = " . CRM_Utils_Type::escape($membershipID, 'Integer');

    return CRM_Core_DAO::singleValueQuery($query,
      CRM_Core_DAO::$_nullArray
    );
  }

  /**
   * Function to delete related memberships
   *
   * @param int $ownerMembershipId
   * @param int $contactId
   *
   * @return null
   * @static
   */
  static
  function deleteRelatedMemberships($ownerMembershipId, $contactId = NULL) {
    if (!$ownerMembershipId && !$contactId) {
      return;
    }

    $membership = new CRM_Member_DAO_Membership();
    $membership->owner_membership_id = $ownerMembershipId;

    if ($contactId) {
      $membership->contact_id = $contactId;
    }

    $membership->find();
    while ($membership->fetch()) {
      //delete related first and then delete parent.
      self::deleteRelatedMemberships($membership->id);
      self::deleteMembership($membership->id);
    }
    $membership->free();
  }

  /**
   * Function to updated related memberships
   *
   * @param int   $ownerMembershipId owner Membership Id
   * @param array $params            formatted array of key => value..
   * @static
   */
  static
  function updateRelatedMemberships($ownerMembershipId, $params) {
    $membership = new CRM_Member_DAO_Membership();
    $membership->owner_membership_id = $ownerMembershipId;
    $membership->find();

    while ($membership->fetch()) {
      $relatedMembership = new CRM_Member_DAO_Membership();
      $relatedMembership->id = $membership->id;
      $relatedMembership->copyValues($params);
      $relatedMembership->save();
      $relatedMembership->free();
    }

    $membership->free();
  }

  /**
   * Function to get list of membership fields for profile
   * For now we only allow custom membership fields to be in
   * profile
   *
   * @return return the list of membership fields
   * @static
   * @access public
   */
  static
  function getMembershipFields($mode = NULL) {
    $fields = CRM_Member_DAO_Membership::export();

    //campaign fields.
    if (isset($fields['member_campaign_id'])) {
      require_once 'CRM/Export/Form/Select.php';
      if ($mode == CRM_Export_Form_Select::MEMBER_EXPORT) {
        $fields['member_campaign'] = array('title' => ts('Campaign Title'));
      }
      else {
        $fields['member_campaign_id']['title'] = ts('Campaign');
      }
    }

    unset($fields['membership_contact_id']);
    $fields = array_merge($fields, CRM_Core_BAO_CustomField::getFieldsForImport('Membership'));

    $membershipType = CRM_Member_DAO_MembershipType::export();

    require_once 'CRM/Member/DAO/MembershipStatus.php';
    $membershipStatus = CRM_Member_DAO_MembershipStatus::export();

    $fields = array_merge($fields, $membershipType, $membershipStatus);

    return $fields;
  }

  /**
   * function to get the sort name of a contact for a particular membership
   *
   * @param  int    $id      id of the membership
   *
   * @return null|string     sort name of the contact if found
   * @static
   * @access public
   */
  static
  function sortName($id) {
    $id = CRM_Utils_Type::escape($id, 'Integer');

    $query = "
SELECT civicrm_contact.sort_name
FROM   civicrm_membership, civicrm_contact
WHERE  civicrm_membership.contact_id = civicrm_contact.id
  AND  civicrm_membership.id = {$id}
";
    return CRM_Core_DAO::singleValueQuery($query, CRM_Core_DAO::$_nullArray);
  }

  /**
   * function to create memberships for related contacts
   *
   * @param  array      $params       array of key - value pairs
   * @param  object     $membership   membership object
   *
   * @return null|relatedMembership     array of memberships if created
   * @static
   * @access public
   */
  static
  function createRelatedMemberships(&$params, &$dao) {
    static $relatedContactIds = array();

    $membership = new CRM_Member_DAO_Membership();
    $membership->id = $dao->id;

    // required since create method doesn't return all the
    // parameters in the returned membership object
    if (!$membership->find(TRUE)) {
      return;
    }
    require_once 'CRM/Member/PseudoConstant.php';
    $deceasedStatusId = array_search('Deceased', CRM_Member_PseudoConstant::membershipStatus());

    $allRelatedContacts = array();
    $relatedContacts = array();
    if (!is_a($membership, 'CRM_Core_Error')) {
      $allRelatedContacts = CRM_Member_BAO_Membership::checkMembershipRelationship($membership->id,
        $membership->contact_id,
        CRM_Utils_Array::value('action', $params)
      );
    }

    // check for loops. CRM-4213
    // remove repeated related contacts, which already inherited membership.
    $relatedContactIds[$membership->contact_id] = TRUE;
    foreach ($allRelatedContacts as $cid => $status) {
      if (!CRM_Utils_Array::value($cid, $relatedContactIds)) {
        $relatedContactIds[$cid] = TRUE;

        //don't create membership again for owner contact.
        $nestedRelationship = FALSE;
        if ($membership->owner_membership_id) {
          $nestedRelMembership = new CRM_Member_DAO_Membership();
          $nestedRelMembership->id = $membership->owner_membership_id;
          $nestedRelMembership->contact_id = $cid;
          $nestedRelationship = $nestedRelMembership->find(TRUE);
          $nestedRelMembership->free();
        }
        if (!$nestedRelationship) {
          $relatedContacts[$cid] = $status;
        }
      }
    }

    //lets cleanup related membership if any.
    if (empty($relatedContacts)) {
      require_once 'CRM/Member/BAO/Membership.php';
      CRM_Member_BAO_Membership::deleteRelatedMemberships($membership->id);
    }
    else {
      // Edit the params array
      unset($params['id']);
      // Reminder should be sent only to the direct membership
      unset($params['reminder_date']);
      // unset the custom value ids
      if (is_array(CRM_Utils_Array::value('custom', $params))) {
        foreach ($params['custom'] as $k => $v) {
          unset($params['custom'][$k]['id']);
        }
      }
      if (!isset($params['membership_type_id'])) {
        $params['membership_type_id'] = $membership->membership_type_id;
      }

      foreach ($relatedContacts as $contactId => $relationshipStatus) {
        //use existing membership record.
        $relMembership = new CRM_Member_DAO_Membership();
        $relMembership->contact_id = $contactId;
        $relMembership->owner_membership_id = $membership->id;
        $relMemIds = array();
        if ($relMembership->find(TRUE)) {
          $params['id'] = $relMemIds['membership'] = $relMembership->id;
        }
        $params['contact_id'] = $contactId;
        $params['owner_membership_id'] = $membership->id;

        // set status_id as it might have been changed for
        // past relationship
        $params['status_id'] = $membership->status_id;

        if ($deceasedStatusId &&
          CRM_Core_DAO::getFieldValue('CRM_Contact_DAO_Contact', $contactId, 'is_deceased')
        ) {
          $params['status_id'] = $deceasedStatusId;
        }
        elseif ((CRM_Utils_Array::value('action', $params) & CRM_Core_Action::UPDATE) &&
          ($relationshipStatus == CRM_Contact_BAO_Relationship::PAST)
        ) {
          // FIXME : While updating/ renewing the
          // membership, if the relationship is PAST then
          // the membership of the related contact must be
          // expired.
          // For that, getting Membership Status for which
          // is_current_member is 0. It works for the
          // generated data as there is only one membership
          // status having is_current_member = 0.
          // But this wont work exactly if there will be
          // more than one status having is_current_member = 0.
          require_once 'CRM/Member/DAO/MembershipStatus.php';
          $membershipStatus = new CRM_Member_DAO_MembershipStatus();
          $membershipStatus->is_current_member = 0;
          if ($membershipStatus->find(TRUE)) {
            $params['status_id'] = $membershipStatus->id;
          }
        }

        //don't calculate status again in create( );
        $params['skipStatusCal'] = TRUE;

        //do create activity if we changed status.
        if ($params['status_id'] != $relMembership->status_id) {
          $params['createActivity'] = TRUE;
        }

        // we should not created contribution record for related contacts, CRM-3371
        unset($params['contribution_status_id']);

        CRM_Member_BAO_Membership::create($params, $relMemIds);
      }
    }
  }

  /**
   * Delete the record that are associated with this Membership Payment
   *
   * @param  int  $membershipId  membsership id.
   *
   * @return boolean  true if deleted false otherwise
   * @access public
   */
  static
  function deleteMembershipPayment($membershipId) {

    require_once 'CRM/Member/DAO/MembershipPayment.php';
    $membesrshipPayment = new CRM_Member_DAO_MembershipPayment();
    $membesrshipPayment->membership_id = $membershipId;
    $membesrshipPayment->find();

    while ($membesrshipPayment->fetch()) {
      require_once 'CRM/Contribute/BAO/Contribution.php';
      CRM_Contribute_BAO_Contribution::deleteContribution($membesrshipPayment->contribution_id);
      CRM_Utils_Hook::pre('delete', 'MembershipPayment', $membesrshipPayment->id, $membesrshipPayment);
      $membesrshipPayment->delete();
      CRM_Utils_Hook::post('delete', 'MembershipPayment', $membesrshipPayment->id, $membesrshipPayment);
    }
    return $membesrshipPayment;
  }

  static
  function &buildMembershipTypeValues(&$form, $membershipTypeID = NULL) {
    $whereClause = NULL;

    if (is_array($membershipTypeID)) {
      $allIDs = implode(',', $membershipTypeID);
      $whereClause = "WHERE id IN ( $allIDs )";
    }
    elseif (is_numeric($membershipTypeID) &&
      $membershipTypeID > 0
    ) {
      $whereClause = "WHERE id = $membershipTypeID";
    }

    $query = "
SELECT *
FROM   civicrm_membership_type
       $whereClause;
";
    $dao = CRM_Core_DAO::executeQuery($query);

    $membershipTypeValues = array();
    $membershipTypeFields = array(
      'id', 'minimum_fee', 'name', 'is_active',
      'description', 'contribution_type_id', 'auto_renew',
    );

    while ($dao->fetch()) {
      $membershipTypeValues[$dao->id] = array();
      foreach ($membershipTypeFields as $mtField) {
        $membershipTypeValues[$dao->id][$mtField] = $dao->$mtField;
      }
    }
    $dao->free();

    require_once 'CRM/Utils/Hook.php';
    CRM_Utils_Hook::membershipTypeValues($form, $membershipTypeValues);

    if (is_numeric($membershipTypeID) &&
      $membershipTypeID > 0
    ) {
      return $membershipTypeValues[$membershipTypeID];
    }
    else {
      return $membershipTypeValues;
    }
  }

  /**
   * Function to get membership record count for a Contact
   *
   * @param int $contactId Contact ID
   * @param boolean $activeOnly
   *
   * @return int count of membership records
   * @access public
   * @static
   */
  static
  function getContactMembershipCount($contactID, $activeOnly = FALSE) {
    $query = "SELECT count(*) FROM civicrm_membership WHERE civicrm_membership.contact_id = {$contactID} AND civicrm_membership.is_test = 0 ";

    // CRM-6627, all status below 3 (active, pending, grace) are considered active
    if ($activeOnly) {
      $query .= " and status_id <= 3";
    }
    return CRM_Core_DAO::singleValueQuery($query);
  }

  /**
   * Function to check whether payment processor supports
   * cancellation of membership subscription
   *
   * @param int $mid membership id
   *
   * @return boolean
   * @access public
   * @static
   */
  static
  function isCancelSubscriptionSupported($mid, $isNotCancelled = TRUE) {
    $cacheKeyString = "$mid";
    $cacheKeyString .= $isNotCancelled ? '_1' : '_0';

    static $supportsCancel = array();

    if (!array_key_exists($cacheKeyString, $supportsCancel)) {
      $supportsCancel[$cacheKeyString] = FALSE;
      $isCancelled = FALSE;

      if ($isNotCancelled) {
        $isCancelled = self::isSubscriptionCancelled($mid);
      }

      require_once 'CRM/Core/BAO/PaymentProcessor.php';
      require_once 'CRM/Core/Payment.php';
      $paymentObject = CRM_Core_BAO_PaymentProcessor::getProcessorForEntity($mid, 'membership', 'obj');
      if (!empty($paymentObject)) {
        $supportsCancel[$cacheKeyString] = CRM_Core_Payment::isCancelSupported($paymentObject) && !$isCancelled;
      }
    }
    return $supportsCancel[$cacheKeyString];
  }

  /**
   * Function to check whether subscription is already cancelled
   *
   * @param int $mid membership id
   *
   * @return string $status contribution status
   * @access public
   * @static
   */
  static
  function isSubscriptionCancelled($mid) {
    $sql = "
   SELECT cr.contribution_status_id 
     FROM civicrm_contribution_recur cr
LEFT JOIN civicrm_membership mem ON ( cr.id = mem.contribution_recur_id )
    WHERE mem.id = %1 LIMIT 1";
    $params = array(1 => array($mid, 'Integer'));
    $statusId = CRM_Core_DAO::singleValueQuery($sql, $params);
    require_once 'CRM/Contribute/PseudoConstant.php';
    $status = CRM_Contribute_PseudoConstant::contributionStatus($statusId);
    if ($status == 'Cancelled') {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Function to get membership joins for a specified membership
   * type.  Specifically, retrieves a count of still current memberships whose
   * join_date and start_date
   * are within a specified date range.  Dates match the regexp
   * "yyyy(mm(dd)?)?".  Omitted portions of a date match the earliest start
   * date or latest end date, i.e., 200803 is March 1st as a start date and
   * March 31st as an end date.
   *
   * @param int    $membershipTypeId  membership type id
   * @param int    $startDate         date on which to start counting
   * @param int    $endDate           date on which to end counting
   * @param bool   $isTest            if true, membership is for a test site
   *
   * @return returns the number of members of type $membershipTypeId
   *         whose join_date is between $startDate and $endDate and
   *         whose start_date is between $startDate and $endDate
   */
  function getMembershipJoins($membershipTypeId, $startDate, $endDate, $isTest = 0) {
    $testClause = 'membership.is_test = 1';
    if (!$isTest) {
      $testClause = '( membership.is_test IS NULL OR membership.is_test = 0 )';
    }

    $query = "
    SELECT  count( membership.id ) as member_count
      FROM  civicrm_membership membership
INNER JOIN  civicrm_membership_status status ON ( membership.status_id = status.id AND status.is_current_member = 1 )
INNER JOIN  civicrm_contact contact ON ( membership.contact_id = contact.id AND contact.is_deleted = 0 )
     WHERE  membership.membership_type_id = %1
       AND  membership.join_date >= '$startDate'  AND membership.join_date <= '$endDate' 
       AND  membership.start_date >= '$startDate' AND membership.start_date <= '$endDate' 
       AND  {$testClause}";

    $params = array(1 => array($membershipTypeId, 'Integer'));
    $memberCount = CRM_Core_DAO::singleValueQuery($query, $params);

    return (int)$memberCount;
  }

  /**
   * Function to get membership renewals for a specified membership
   * type.  Specifically, retrieves a count of still current memberships whose
   * join_date is before and start_date
   * is within a specified date range.  Dates match the regexp
   * "yyyy(mm(dd)?)?".  Omitted portions of a date match the earliest start
   * date or latest end date, i.e., 200803 is March 1st as a start date and
   * March 31st as an end date.
   *
   * @param int    $membershipTypeId  membership type id
   * @param int    $startDate         date on which to start counting
   * @param int    $endDate           date on which to end counting
   * @param bool   $isTest            if true, membership is for a test site
   *
   * @return returns the number of members of type $membershipTypeId
   *         whose join_date is before $startDate and
   *         whose start_date is between $startDate and $endDate
   */
  function getMembershipRenewals($membershipTypeId, $startDate, $endDate, $isTest = 0) {
    $testClause = 'membership.is_test = 1';
    if (!$isTest) {
      $testClause = '( membership.is_test IS NULL OR membership.is_test = 0 )';
    }

    $query = "
    SELECT  count(membership.id) as member_count
      FROM  civicrm_membership membership 
INNER JOIN  civicrm_membership_status status ON ( membership.status_id = status.id AND status.is_current_member = 1 )
INNER JOIN  civicrm_contact contact ON ( contact.id = membership.contact_id AND contact.is_deleted = 0 ) 
     WHERE  membership.membership_type_id = %1
       AND  membership.join_date < '$startDate'
       AND  membership.start_date >= '$startDate' AND membership.start_date <= '$endDate' 
       AND  {$testClause}";

    $params = array(1 => array($membershipTypeId, 'Integer'));
    $memberCount = CRM_Core_DAO::singleValueQuery($query, $params);

    return (int)$memberCount;
  }

  /**
   * Function to process price set and line items.
   *
   * @access public
   *
   * @return None
   */
  function processPriceSet($membershipId, $lineItem) {
    if (!$membershipId || !is_array($lineItem)
      || CRM_Utils_system::isNull($lineItem)
    ) {
      return;
    }

    require_once 'CRM/Price/BAO/Set.php';
    require_once 'CRM/Price/BAO/LineItem.php';
    foreach ($lineItem as $priceSetId => $values) {
      if (!$priceSetId) {
        continue;
      }
      foreach ($values as $line) {
        $line['entity_table'] = 'civicrm_membership';
        $line['entity_id'] = $membershipId;
        CRM_Price_BAO_LineItem::create($line);
      }
      CRM_Price_BAO_Set::addTo('civicrm_membership', $membershipId, $priceSetId);
    }
  }

  /**
   * retrieve the contribution record for the associated Membership id
   *
   * @param  int  $membershipId  membsership id.
   *
   * @return contribution id
   * @access public
   */
  static
  function getMembershipContributionId($membershipId) {
    require_once 'CRM/Member/DAO/MembershipPayment.php';

    $membesrshipPayment = new CRM_Member_DAO_MembershipPayment();
    $membesrshipPayment->membership_id = $membershipId;
    if ($membesrshipPayment->find(TRUE)) {
      return $membesrshipPayment->contribution_id;
    }
    return NULL;
  }

  /*
     * The function checks and updates the status of all membership records for a given domain using the 
     * calc_membership_status and update_contact_membership APIs.
     *
     * IMPORTANT: 
     * It uses the default Domain FROM Name and FROM Email Address as the From email address for emails sent by this api.  
     * Verify that this value has been properly set from Administer > Configure > Domain Information
     * If you want to use some other FROM email address, modify line 125 and set your valid email address.
     *
     * @return void
     * @access public
     */

  static
  function updateAllMembershipStatus() {
    require_once 'CRM/Member/BAO/MembershipLog.php';
    require_once 'CRM/Member/BAO/Membership.php';
    require_once 'CRM/Core/BAO/MessageTemplates.php';
    require_once 'CRM/Member/BAO/MembershipType.php';
    require_once 'CRM/Utils/Date.php';
    require_once 'CRM/Utils/System.php';
    require_once 'CRM/Member/PseudoConstant.php';
    require_once 'CRM/Contact/BAO/Contact.php';
    require_once 'CRM/Activity/BAO/Activity.php';
    require_once 'CRM/Contribute/PseudoConstant.php';
    require_once 'api/api.php';

    //get all active statuses of membership, CRM-3984
    $allStatus     = CRM_Member_PseudoConstant::membershipStatus();
    $statusLabels  = CRM_Member_PseudoConstant::membershipStatus(NULL, NULL, 'label');
    $allTypes      = CRM_Member_PseudoConstant::membershipType();
    $contribStatus = CRM_Contribute_PseudoConstant::contributionStatus(NULL, 'name');

    $query = "
SELECT     civicrm_membership.id                    as membership_id,
           civicrm_membership.is_override           as is_override,
           civicrm_membership.reminder_date         as reminder_date,
           civicrm_membership.membership_type_id    as membership_type_id,
           civicrm_membership.status_id             as status_id,
           civicrm_membership.join_date             as join_date,
           civicrm_membership.start_date            as start_date,
           civicrm_membership.end_date              as end_date,
           civicrm_membership.source                as source,
           civicrm_contact.id                       as contact_id,
           civicrm_contact.is_deceased              as is_deceased,
           civicrm_membership.owner_membership_id   as owner_membership_id,
           civicrm_membership.contribution_recur_id as recur_id
FROM       civicrm_membership
INNER JOIN civicrm_contact ON ( civicrm_membership.contact_id = civicrm_contact.id )
WHERE      civicrm_membership.is_test = 0";

    $params = array();
    $dao = CRM_Core_DAO::executeQuery($query, $params);

    $today         = date("Y-m-d");
    $processCount  = 0;
    $updateCount   = 0;
    $reminderCount = 0;

    require_once 'CRM/Core/Smarty.php';
    $smarty = CRM_Core_Smarty::singleton();

    require_once 'CRM/Core/BAO/Domain.php';
    $domainValues = CRM_Core_BAO_Domain::getNameAndEmail();
    $domainFromEmail = "$domainValues[0] <$domainValues[1]>";

    //use domain email address as a default From email.
    $fromEmailAddress = $domainFromEmail;

    while ($dao->fetch()) {
      // echo ".";
      $processCount++;

      /**
       $count++;
       echo $dao->contact_id . ', '. CRM_Utils_System::memory( ) . "<p>\n";

       CRM_Core_Error::debug( 'fBegin', count( $GLOBALS['_DB_DATAOBJECT']['RESULTS'] ) );
       if ( $count > 2 ) {
       foreach ( $GLOBALS['_DB_DATAOBJECT']['RESULTS'] as $r ) {
       CRM_Core_Error::debug( 'r', $r->query );
       }
       // CRM_Core_Error::debug( 'f', $GLOBALS['_DB_DATAOBJECT']['RESULTS'] );
       exit( );
       }
       **/

      // Put common parameters into array for easy access
      $memberParams = array(
        'id' => $dao->membership_id,
        'status_id' => $dao->status_id,
        'contact_id' => $dao->contact_id,
        'membership_type_id' => $dao->membership_type_id,
        'membership_type' => $allTypes[$dao->membership_type_id],
        'join_date' => $dao->join_date,
        'start_date' => $dao->start_date,
        'end_date' => $dao->end_date,
        'reminder_date' => $dao->reminder_date,
        'source' => $dao->source,
        'skipStatusCal' => TRUE,
        'skipRecentView' => TRUE,
      );

      $smarty->assign_by_ref('memberParams', $memberParams);

      //update membership record to Deceased if contact is deceased
      if ($dao->is_deceased) {
        // check for 'Deceased' membership status, CRM-5636
        $deceaseStatusId = array_search('Deceased', $allStatus);
        if (!$deceaseStatusId) {
          CRM_Core_Error::fatal(ts("Deceased Membership status is missing or not active. <a href='%1'>Click here to check</a>.", array(1 => CRM_Utils_System::url('civicrm/admin/member/membershipStatus', 'reset=1'))));
        }

        //process only when status change.
        if ($dao->status_id != $deceaseStatusId) {
          //take all params that need to save.
          $deceasedMembership = $memberParams;
          $deceasedMembership['status_id'] = $deceaseStatusId;
          $deceasedMembership['createActivity'] = TRUE;
          $deceasedMembership['version'] = 3;

          //since there is change in status.
          $statusChange = array('status_id' => $deceaseStatusId);
          $smarty->append_by_ref('memberParams', $statusChange, TRUE);

          //process membership record.
          civicrm_api('membership', 'create', $deceasedMembership);
        }
        continue;
      }

      //we fetch related, since we need to check for deceased
      //now further processing is handle w/ main membership record.
      if ($dao->owner_membership_id) {
        continue;
      }

      //update membership records where status is NOT - Pending OR Cancelled.
      //as well as membership is not override.
      //skipping Expired membership records -> reduced extra processing( kiran )
      if (!$dao->is_override &&
        !in_array($dao->status_id, array(array_search('Pending', $allStatus),
            array_search('Cancelled', $allStatus),
            array_search('Expired', $allStatus),
          ))
      ) {

        // CRM-7248: added excludeIsAdmin param to the following fn call to prevent moving to admin statuses
        //get the membership status as per id.
        $newStatus = civicrm_api('membership_status', 'calc',
          array(
            'membership_id' => $dao->membership_id, 'version' => 3), TRUE
        );
        $statusId = CRM_Utils_Array::value('id', $newStatus);

        //process only when status change.
        if ($statusId &&
          $statusId != $dao->status_id
        ) {
          //take all params that need to save.
          $memParams = $memberParams;
          $memParams['status_id'] = $statusId;
          $memParams['createActivity'] = TRUE;
          $memParams['version'] = 3;

          //since there is change in status.
          $statusChange = array('status_id' => $statusId);
          $smarty->append_by_ref('memberParams', $statusChange, TRUE);

          //process member record.
          civicrm_api('membership', 'create', $memParams);
          $updateCount++;
        }
      }

      //convert date from string format to timestamp format
      $reminder_date = CRM_Utils_DATE::unixTime($dao->reminder_date);
      $today_date = CRM_Utils_DATE::unixTime($today);

      //send reminder for membership renewal
      if ($dao->reminder_date &&
        $dao->reminder_date != '0000-00-00' &&
        ($reminder_date <= $today_date)
      ) {
        $memType = new CRM_Member_BAO_MembershipType();

        $memType->id = $dao->membership_type_id;
        $memType->find(TRUE);
        $renewalMsgId = $memType->renewal_msg_id;

        if ($memType->autorenewal_msg_id && $dao->recur_id) {
          $contribStatusId = CRM_Core_DAO::getFieldValue('CRM_Contribute_DAO_ContributionRecur',
            $dao->recur_id, 'contribution_status_id'
          );
          if ($contribStatusId != array_search('Cancelled', $contribStatus)) {
            $renewalMsgId = $memType->autorenewal_msg_id;
          }
        }
        if ($renewalMsgId) {
          $toEmail = CRM_Contact_BAO_Contact::getPrimaryEmail($dao->contact_id);

          if ($toEmail) {
            $sendResult = CRM_Core_BAO_MessageTemplates::sendReminder($dao->contact_id,
              $toEmail,
              $renewalMsgId,
              $fromEmailAddress
            );
            if (!$sendResult ||
              is_a($sendResult, 'PEAR_Error')
            ) {
              // we could not send an email, for now we ignore
              // CRM-3406
              // at some point we might decide to do something
            }
            else {
              $reminderCount++;
            }

            //set membership reminder date to NULL since we've sent the reminder.
            CRM_Core_DAO::setFieldValue('CRM_Member_DAO_Membership', $dao->membership_id, 'reminder_date', 'null');

            // insert the activity log record.
            $config = CRM_Core_Config::singleton();
            $activityParams = array();
            $activityParams['subject'] = $allTypes[$dao->membership_type_id] . ": Status - " . $statusLabels[$newStatus['id']] . ", End Date - " . CRM_Utils_Date::customFormat(CRM_Utils_Date::isoToMysql($dao->end_date), $config->dateformatFull);
            $activityParams['source_record_id'] = $dao->membership_id;

            $session = CRM_Core_Session::singleton();
            $activityParams['source_contact_id'] = $session->get('userID') ? $session->get('userID') : $dao->contact_id;
            $activityParams['assignee_contact_id'] = $dao->contact_id;

            $activityParams['activity_date_time'] = date('YmdHis');

            static $actRelIds = array();
            if (!isset($actRelIds['activity_type_id'])) {
              $actRelIds['activity_type_id'] = CRM_Core_OptionGroup::getValue('activity_type',
                'Membership Renewal Reminder', 'name'
              );
            }
            $activityParams['activity_type_id'] = $actRelIds['activity_type_id'];

            if (!isset($actRelIds['activity_status_id'])) {
              $actRelIds['activity_status_id'] = CRM_Core_OptionGroup::getValue('activity_status', 'Completed', 'name');
            }
            $activityParams['status_id'] = $actRelIds['activity_status_id'];

            static $msgTpl = array();
            if (!isset($msgTpl[$memType->renewal_msg_id])) {
              $msgTpl[$memType->renewal_msg_id] = array();

              $messageTemplate = new CRM_Core_DAO_MessageTemplates();
              $messageTemplate->id = $memType->renewal_msg_id;
              if ($messageTemplate->find(TRUE)) {
                $msgTpl[$memType->renewal_msg_id]['subject'] = $messageTemplate->msg_subject;
                $msgTpl[$memType->renewal_msg_id]['details'] = $messageTemplate->msg_text;
              }
              $messageTemplate->free();
            }
            $activityParams['details'] = "Subject: {$msgTpl[$memType->renewal_msg_id]['subject']}
Message: {$msgTpl[$memType->renewal_msg_id]['details']}
";
            $activity = CRM_Activity_BAO_Activity::create($activityParams);
          }
        }
        $memType->free();
      }
      // CRM_Core_Error::debug( 'fEnd', count( $GLOBALS['_DB_DATAOBJECT']['RESULTS'] ) );
    }
    $result['is_error'] = 0;
    $result['messages'] = ts('Processed %1 membership records. Updated %2 records. Sent %3 renewal reminders.', array(1 => $processCount, 2 => $updateCount, 3 => $reminderCount));
    return $result;
  }

  /** The function returns the membershiptypes for a particular contact
   * who has lifetime membership without end date.
   *
   * @param  array      $contactMembershipType       array of allMembershipTypes Key - value pairs
   *
   */

  static
  function getAllContactMembership($contactID, $isTest = FALSE, $onlyLifeTime = FALSE) {
    $contactMembershipType = array();
    if (!$contactID) {
      return $contactMembershipType;
    }

    $dao = new CRM_Member_DAO_Membership();
    $dao->contact_id = $contactID;
    require_once 'CRM/Member/PseudoConstant.php';
    $pendingStatusId = array_search('Pending', CRM_Member_PseudoConstant::membershipStatus());
    $dao->whereAdd("status_id != $pendingStatusId");

    if ($isTest) {
      $dao->is_test = $isTest;
    }
    else {
      $dao->whereAdd('is_test IS NULL OR is_test = 0');
    }

    if ($onlyLifeTime) {
      $dao->whereAdd('end_date IS NULL');
    }

    $dao->find();
    while ($dao->fetch()) {
      $membership = array();
      CRM_Core_DAO::storeValues($dao, $membership);
      $contactMembershipType[$dao->membership_type_id] = $membership;
    }
    return $contactMembershipType;
  }

  public function updateMembershipReminderDate($params) {
    require_once 'CRM/Member/PseudoConstant.php';

    //get all active statuses of membership.
    $allStatuses = CRM_Member_PseudoConstant::membershipStatus();

    //set membership reminder date if membership
    //record has one of the following status.
    $validStatus = array('New', 'Current', 'Grace');

    $statusIds = array();
    foreach ($validStatus as $status) {
      $statusId = array_search($status, $allStatuses);
      if ($statusId) {
        $statusIds[$statusId] = $statusId;
      }
    }

    //we don't have valid status to check,
    //therefore no need to proceed further.
    if (empty($statusIds)) {
      return;
    }

    //set reminder date for all memberships,
    //in case reminder date is missing and
    //membership type has reminder day set.

    $query = '
    UPDATE  civicrm_membership membership 
INNER JOIN  civicrm_contact contact ON ( contact.id = membership.contact_id ) 
INNER JOIN  civicrm_membership_type type ON ( type.id = membership.membership_type_id )
       SET  membership.reminder_date = DATE_SUB( membership.end_date, INTERVAL type.renewal_reminder_day + 1 DAY ) 
     WHERE  membership.reminder_date IS NULL 
       AND  contact.is_deleted = 0
       AND  ( contact.is_deceased IS NULL OR contact.is_deceased = 0 )
       AND  type.renewal_reminder_day IS NOT NULL 
       AND  membership.status_id IN ( ' . implode(' , ', $statusIds) . ' )';

    CRM_Core_DAO::executeQuery($query);

    return array(
      'is_error' => 0,
      'messages' => 'Membership(s) reminder date updated. (Done)',
    );
  }
}

