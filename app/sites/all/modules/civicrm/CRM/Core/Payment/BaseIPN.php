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
class CRM_Core_Payment_BaseIPN {

  static $_now = NULL;
  function __construct() {
    self::$_now = date('YmdHis');
  }

  function validateData(&$input, &$ids, &$objects, $required = TRUE) {

    // make sure contact exists and is valid
    require_once 'CRM/Contact/DAO/Contact.php';
    $contact = new CRM_Contact_DAO_Contact();
    $contact->id = $ids['contact'];
    if (!$contact->find(TRUE)) {
      CRM_Core_Error::debug_log_message("Could not find contact record: {$ids['contact']}");
      echo "Failure: Could not find contact record: {$ids['contact']}<p>";
      return FALSE;
    }

    // make sure contribution exists and is valid
    require_once 'CRM/Contribute/DAO/Contribution.php';
    $contribution = new CRM_Contribute_DAO_Contribution();
    $contribution->id = $ids['contribution'];
    if (!$contribution->find(TRUE)) {
      CRM_Core_Error::debug_log_message("Could not find contribution record: $contributionID");
      echo "Failure: Could not find contribution record for $contributionID<p>";
      return FALSE;
    }
    $contribution->receive_date = CRM_Utils_Date::isoToMysql($contribution->receive_date);

    $objects['contact'] = &$contact;
    $objects['contribution'] = &$contribution;
    if (!$this->loadObjects($input, $ids, $objects, $required)) {
      return FALSE;
    }

    return TRUE;
  }

  function createContact(&$input, &$ids, &$objects) {
    $params    = array();
    $billingID = $ids['billing'];
    $lookup    = array(
      'first_name',
      'last_name',
      "street_address-{$billingID}",
      "city-{$billingID}",
      "state-{$billingID}",
      "postal_code-{$billingID}",
      "country-{$billingID}",
    );
    foreach ($lookup as $name) {
      $params[$name] = $input[$name];
    }
    if (!empty($params)) {
      // update contact record
      require_once 'CRM/Contact/BAO/Contact.php';
      $contact = CRM_Contact_BAO_Contact::createProfileContact($params, CRM_Core_DAO::$_nullArray, $ids['contact']);
    }

    return TRUE;
  }

  function loadObjects(&$input, &$ids, &$objects, $required) {
    $contribution = &$objects['contribution'];

    $objects['membership'] = NULL;
    $objects['contributionRecur'] = NULL;
    $objects['contributionType'] = NULL;
    $objects['event'] = NULL;
    $objects['participant'] = NULL;
    $objects['pledge_payment'] = NULL;

    require_once 'CRM/Contribute/DAO/ContributionType.php';
    $contributionType = new CRM_Contribute_DAO_ContributionType();
    $contributionType->id = $contribution->contribution_type_id;
    if (!$contributionType->find(TRUE)) {
      CRM_Core_Error::debug_log_message("Could not find contribution type record: $contributionTypeID");
      echo "Failure: Could not find contribution type record for $contributionTypeID<p>";
      return FALSE;
    }
    $objects['contributionType'] = $contributionType;


    $paymentProcessorID = NULL;
    if ($input['component'] == 'contribute') {

      // retrieve the other optional objects first so
      // stuff down the line can use this info and do things
      // CRM-6056
      if (isset($ids['membership'])) {
        require_once 'CRM/Member/DAO/Membership.php';
        if (is_numeric($ids['membership'])) {
          // see if there are any other memberships to be considered for same contribution.
          $query = "
SELECT membership_id 
FROM   civicrm_membership_payment 
WHERE  contribution_id = %1 AND membership_id != %2";
          $dao = CRM_Core_DAO::executeQuery($query,
            array(1 => array($contribution->id, 'Integer'),
              2 => array($ids['membership'], 'Integer'),
            )
          );

          $ids['membership'] = array($ids['membership']);
          while ($dao->fetch()) {
            $ids['membership'][] = $dao->membership_id;
          }
        }

        if (is_array($ids['membership'])) {
          foreach ($ids['membership'] as $id) {
            if (!empty($id)) {
              $membership = new CRM_Member_DAO_Membership();
              $membership->id = $id;
              if (!$membership->find(TRUE)) {
                CRM_Core_Error::debug_log_message("Could not find membership record: $id");
                echo "Failure: Could not find membership record: $id<p>";
                return FALSE;
              }
              $membership->join_date = CRM_Utils_Date::isoToMysql($membership->join_date);
              $membership->start_date = CRM_Utils_Date::isoToMysql($membership->start_date);
              $membership->end_date = CRM_Utils_Date::isoToMysql($membership->end_date);
              $membership->reminder_date = CRM_Utils_Date::isoToMysql($membership->reminder_date);

              $objects['membership'][] = $membership;
              $membership->free();
            }
          }
        }
      }

      if (isset($ids['pledge_payment'])) {
        require_once 'CRM/Pledge/DAO/PledgePayment.php';

        $objects['pledge_payment'] = array();
        foreach ($ids['pledge_payment'] as $key => $paymentID) {
          $payment = new CRM_Pledge_DAO_PledgePayment();
          $payment->id = $paymentID;
          if (!$payment->find(TRUE)) {
            CRM_Core_Error::debug_log_message("Could not find pledge payment record: $pledge_paymentID");
            echo "Failure: Could not find pledge payment record: $pledge_paymentID<p>";
            return FALSE;
          }
          $objects['pledge_payment'][] = $payment;
        }
      }

      if (isset($ids['contributionRecur'])) {
        require_once 'CRM/Contribute/DAO/ContributionRecur.php';
        $recur = new CRM_Contribute_DAO_ContributionRecur();
        $recur->id = $ids['contributionRecur'];
        if (!$recur->find(TRUE)) {
          CRM_Core_Error::debug_log_message("Could not find recur record: $contributionRecurID");
          echo "Failure: Could not find recur record: $contributionRecurID<p>";
          return FALSE;
        }
        $objects['contributionRecur'] = &$recur;

        //get payment processor id from recur object.
        $paymentProcessorID = $recur->payment_processor_id;
      }

      //for normal contribution get the payment processor id.
      if (!$paymentProcessorID) {
        if ($contribution->contribution_page_id) {
          // get the payment processor id from contribution page
          $paymentProcessorID = CRM_Core_DAO::getFieldValue('CRM_Contribute_DAO_ContributionPage',
            $contribution->contribution_page_id,
            'payment_processor_id'
          );
        }

        //fail to load payment processor id.
        if (!$paymentProcessorID &&
          !$contribution->contribution_page_id &&
          !CRM_Utils_Array::value('pledge_payment', $ids)
        ) {
          $loadObjectSuccess = TRUE;
          if ($required) {
            $loadObjectSuccess = FALSE;
            CRM_Core_Error::debug_log_message("Could not find contribution page for contribution record: $contributionID");
            echo "Failure: Could not find contribution page for contribution record: $contributionID<p>";
          }
          return $loadObjectSuccess;
        }
      }
    }
    else {
      // we are in event mode
      // make sure event exists and is valid
      require_once 'CRM/Event/DAO/Event.php';
      $event = new CRM_Event_DAO_Event();
      $event->id = $ids['event'];
      if ($ids['event'] &&
        !$event->find(TRUE)
      ) {
        CRM_Core_Error::debug_log_message("Could not find event: $eventID");
        echo "Failure: Could not find event: $eventID<p>";
        return FALSE;
      }

      $objects['event'] = &$event;

      require_once 'CRM/Event/DAO/Participant.php';
      $participant = new CRM_Event_DAO_Participant();
      $participant->id = $ids['participant'];
      if ($ids['participant'] &&
        !$participant->find(TRUE)
      ) {
        CRM_Core_Error::debug_log_message("Could not find participant: $participantID");
        echo "Failure: Could not find participant: $participantID<p>";
        return FALSE;
      }
      $participant->register_date = CRM_Utils_Date::isoToMysql($participant->register_date);

      $objects['participant'] = &$participant;

      if (!$paymentProcessorID) {
        $paymentProcessorID = $objects['event']->payment_processor_id;
      }
    }

    $loadObjectSuccess = TRUE;
    if ($paymentProcessorID) {
      require_once 'CRM/Core/BAO/PaymentProcessor.php';
      $paymentProcessor = CRM_Core_BAO_PaymentProcessor::getPayment($paymentProcessorID,
        $contribution->is_test ? 'test' : 'live'
      );

      $ids['paymentProcessor'] = $paymentProcessorID;
      $objects['paymentProcessor'] = &$paymentProcessor;
    }
    elseif ($required) {
      $loadObjectSuccess = FALSE;
      CRM_Core_Error::debug_log_message("Could not find payment processor for contribution record: $contributionID");
      echo "Failure: Could not find payment processor for contribution record: $contributionID<p>";
    }

    return $loadObjectSuccess;
  }

  function failed(&$objects, &$transaction) {
    $contribution = &$objects['contribution'];
    $memberships = &$objects['membership'];
    if (is_numeric($memberships)) {
      $memberships = array($objects['membership']);
    }
    $participant = &$objects['participant'];

    require_once 'CRM/Contribute/PseudoConstant.php';
    $contributionStatus = CRM_Contribute_PseudoConstant::contributionStatus(NULL, 'name');

    $contribution->contribution_status_id = array_search('Failed', $contributionStatus);
    $contribution->save();
    foreach ($memberships as $membership) {
      if ($membership) {
        $membership->status_id = 4;
        $membership->save();

        //update related Memberships.
        require_once 'CRM/Member/BAO/Membership.php';
        $params = array('status_id' => 4);
        CRM_Member_BAO_Membership::updateRelatedMemberships($membership->id, $params);
      }
    }
    if ($participant) {
      $participant->status_id = 4;
      $participant->save();
    }

    $transaction->commit();
    CRM_Core_Error::debug_log_message("Setting contribution status to failed");
    //echo "Success: Setting contribution status to failed<p>";
    return TRUE;
  }

  function pending(&$objects, &$transaction) {
    $transaction->commit();
    CRM_Core_Error::debug_log_message("returning since contribution status is pending");
    echo "Success: Returning since contribution status is pending<p>";
    return TRUE;
  }

  function cancelled(&$objects, &$transaction) {
    $contribution = &$objects['contribution'];
    $memberships = &$objects['membership'];
    if (is_numeric($memberships)) {
      $memberships = array($objects['membership']);
    }

    $participant = &$objects['participant'];

    $contribution->contribution_status_id = 3;
    $contribution->cancel_date = self::$_now;
    $contribution->cancel_reason = CRM_Utils_Array::value('reasonCode', $input);
    $contribution->save();

    foreach ($memberships as $membership) {
      if ($membership) {
        $membership->status_id = 6;
        $membership->save();

        //update related Memberships.
        require_once 'CRM/Member/BAO/Membership.php';
        $params = array('status_id' => 6);
        CRM_Member_BAO_Membership::updateRelatedMemberships($membership->id, $params);
      }
    }

    if ($participant) {
      $participant->status_id = 4;
      $participant->save();
    }

    $transaction->commit();
    CRM_Core_Error::debug_log_message("Setting contribution status to cancelled");
    //echo "Success: Setting contribution status to cancelled<p>";
    return TRUE;
  }

  function unhandled(&$objects, &$transaction) {
    $transaction->rollback();
    // we dont handle this as yet
    CRM_Core_Error::debug_log_message("returning since contribution status: $status is not handled");
    echo "Failure: contribution status $status is not handled<p>";
    return FALSE;
  }

  function completeTransaction(&$input, &$ids, &$objects, &$transaction, $recur = FALSE) {
    $contribution = &$objects['contribution'];
    $memberships = &$objects['membership'];
    if (is_numeric($memberships)) {
      $memberships = array($objects['membership']);
    }
    $participant  = &$objects['participant'];
    $event        = &$objects['event'];
    $changeToday  = CRM_Utils_Array::value('trxn_date', $input, self::$_now);
    $recurContrib = &$objects['contributionRecur'];

    $values = array();
    if ($input['component'] == 'contribute') {
      if ($contribution->contribution_page_id) {
        require_once 'CRM/Contribute/BAO/ContributionPage.php';
        CRM_Contribute_BAO_ContributionPage::setValues($contribution->contribution_page_id, $values);
        $source = ts('Online Contribution') . ': ' . $values['title'];
      }
      elseif ($recurContrib->id) {
        $contribution->contribution_page_id = NULL;
        $values['amount'] = $recurContrib->amount;
        $values['contribution_type_id'] = $objects['contributionType']->id;
        $values['title'] = $source = ts('Offline Recurring Contribution');
        $values['is_email_receipt'] = $recurContrib->is_email_receipt;
        require_once 'CRM/Core/BAO/Domain.php';
        $domainValues = CRM_Core_BAO_Domain::getNameAndEmail();
        $values['receipt_from_name'] = $domainValues[0];
        $values['receipt_from_email'] = $domainValues[1];
      }
      $contribution->source = $source;
      if (CRM_Utils_Array::value('is_email_receipt', $values)) {
        $contribution->receipt_date = self::$_now;
      }
      if (!empty($memberships)) {
        foreach ($memberships as $membership) {
          if ($membership) {
            $format = '%Y%m%d';

            require_once 'CRM/Member/BAO/MembershipType.php';
            require_once 'CRM/Member/BAO/Membership.php';
            $currentMembership = CRM_Member_BAO_Membership::getContactMembership($membership->contact_id,
              $membership->membership_type_id,
              $membership->is_test, $membership->id
            );

            // CRM-8141 update the membership type with the value recorded in log when membership created/renewed
            // this picks up membership type changes during renewals
            $sql = "
SELECT    membership_type_id 
FROM      civicrm_membership_log 
WHERE     membership_id=$membership->id 
ORDER BY  id DESC 
LIMIT 1;";
            require_once 'CRM/Core/DAO.php';
            $dao = new CRM_Core_DAO;
            $dao->query($sql);
            if ($dao->fetch()) {
              if (!empty($dao->membership_type_id)) {
                $membership->membership_type_id = $dao->membership_type_id;
                $membership->save();
              }
              // else fall back to using current membership type
            }
            // else fall back to using current membership type
            $dao->free();

            if ($currentMembership) {
              /*
                             * Fixed FOR CRM-4433
                             * In BAO/Membership.php(renewMembership function), we skip the extend membership date and status 
                             * when Contribution mode is notify and membership is for renewal ) 
                             */

              CRM_Member_BAO_Membership::fixMembershipStatusBeforeRenew($currentMembership, $changeToday);

              $dates = CRM_Member_BAO_MembershipType::getRenewalDatesForMembershipType($membership->id,
                $changeToday
              );
              $dates['join_date'] = CRM_Utils_Date::customFormat($currentMembership['join_date'], $format);
            }
            else {
              $dates = CRM_Member_BAO_MembershipType::getDatesForMembershipType($membership->membership_type_id);
            }

            //get the status for membership.
            require_once 'CRM/Member/BAO/MembershipStatus.php';
            $calcStatus = CRM_Member_BAO_MembershipStatus::getMembershipStatusByDate($dates['start_date'],
              $dates['end_date'],
              $dates['join_date'],
              'today',
              TRUE
            );

            $formatedParams = array('status_id' => CRM_Utils_Array::value('id', $calcStatus, 2),
              'join_date' => CRM_Utils_Date::customFormat($dates['join_date'], $format),
              'start_date' => CRM_Utils_Date::customFormat($dates['start_date'], $format),
              'end_date' => CRM_Utils_Date::customFormat($dates['end_date'], $format),
              'reminder_date' => CRM_Utils_Date::customFormat($dates['reminder_date'], $format),
            );
            //we might be renewing membership,
            //so make status override false.
            $formatedParams['is_override'] = FALSE;
            $membership->copyValues($formatedParams);
            $membership->save();

            //updating the membership log
            $membershipLog = array();
            $membershipLog = $formatedParams;

            $logStartDate = $formatedParams['start_date'];
            if (CRM_Utils_Array::value('log_start_date', $dates)) {
              $logStartDate = CRM_Utils_Date::customFormat($dates['log_start_date'], $format);
              $logStartDate = CRM_Utils_Date::isoToMysql($logStartDate);
            }

            $membershipLog['start_date'] = $logStartDate;
            $membershipLog['membership_id'] = $membership->id;
            $membershipLog['modified_id'] = $membership->contact_id;
            $membershipLog['modified_date'] = date('Ymd');
            $membershipLog['membership_type_id'] = $membership->membership_type_id;

            require_once 'CRM/Member/BAO/MembershipLog.php';
            CRM_Member_BAO_MembershipLog::add($membershipLog, CRM_Core_DAO::$_nullArray);

            //update related Memberships.
            CRM_Member_BAO_Membership::updateRelatedMemberships($membership->id, $formatedParams);
          }
        }
      }
    }
    else {
      // event
      $eventParams = array('id' => $objects['event']->id);
      $values['event'] = array();

      require_once 'CRM/Event/BAO/Event.php';
      CRM_Event_BAO_Event::retrieve($eventParams, $values['event']);

      $eventParams = array('id' => $objects['event']->id);
      $values['event'] = array();

      require_once 'CRM/Event/BAO/Event.php';
      CRM_Event_BAO_Event::retrieve($eventParams, $values['event']);

      //get location details
      $locationParams = array('entity_id' => $objects['event']->id, 'entity_table' => 'civicrm_event');
      require_once 'CRM/Core/BAO/Location.php';
      require_once 'CRM/Event/Form/ManageEvent/Location.php';
      $values['location'] = CRM_Core_BAO_Location::getValues($locationParams);

      require_once 'CRM/Core/BAO/UFJoin.php';
      $ufJoinParams = array(
        'entity_table' => 'civicrm_event',
        'entity_id' => $ids['event'],
        'weight' => 1,
      );

      $values['custom_pre_id'] = CRM_Core_BAO_UFJoin::findUFGroupId($ufJoinParams);

      $ufJoinParams['weight'] = 2;
      $values['custom_post_id'] = CRM_Core_BAO_UFJoin::findUFGroupId($ufJoinParams);

      $contribution->source = ts('Online Event Registration') . ': ' . $values['event']['title'];

      if ($values['event']['is_email_confirm']) {
        $contribution->receipt_date = self::$_now;
        $values['is_email_receipt'] = 1;
      }

      $participant->status_id = 1;
      $participant->save();
    }

    if (CRM_Utils_Array::value('net_amount', $input, 0) == 0 &&
      CRM_Utils_Array::value('fee_amount', $input, 0) != 0
    ) {
      $input['net_amount'] = $input['amount'] - $input['fee_amount'];
    }

    $contribution->contribution_status_id = 1;
    $contribution->is_test = $input['is_test'];
    $contribution->fee_amount = CRM_Utils_Array::value('fee_amount', $input, 0);
    $contribution->net_amount = CRM_Utils_Array::value('net_amount', $input, 0);
    $contribution->trxn_id = $input['trxn_id'];
    $contribution->receive_date = CRM_Utils_Date::isoToMysql($contribution->receive_date);
    $contribution->thankyou_date = CRM_Utils_Date::isoToMysql($contribution->thankyou_date);
    $contribution->cancel_date = 'null';

    if (CRM_Utils_Array::value('check_number', $input)) {
      $contribution->check_number = $input['check_number'];
    }

    if (CRM_Utils_Array::value('payment_instrument_id', $input)) {
      $contribution->payment_instrument_id = $input['payment_instrument_id'];
    }

    $contribution->save();

    // next create the transaction record
    $paymentProcessor = '';
    if (isset($objects['paymentProcessor'])) {
      if (is_array($objects['paymentProcessor'])) {
        $paymentProcessor = $objects['paymentProcessor']['payment_processor_type'];
      }
      else {
        $paymentProcessor = $objects['paymentProcessor']->payment_processor_type;
      }
    }

    if ($contribution->trxn_id) {

      $trxnParams = array(
        'contribution_id' => $contribution->id,
        'trxn_date' => isset($input['trxn_date']) ? $input['trxn_date'] : self::$_now,
        'trxn_type' => 'Debit',
        'total_amount' => $input['amount'],
        'fee_amount' => $contribution->fee_amount,
        'net_amount' => $contribution->net_amount,
        'currency' => $contribution->currency,
        'payment_processor' => $paymentProcessor,
        'trxn_id' => $contribution->trxn_id,
      );

      require_once 'CRM/Core/BAO/FinancialTrxn.php';
      $trxn = CRM_Core_BAO_FinancialTrxn::create($trxnParams);
    }

    self::updateRecurLinkedPledge($contribution);

    // create an activity record
    require_once 'CRM/Activity/BAO/Activity.php';
    if ($input['component'] == 'contribute') {
      //CRM-4027
      $targetContactID = NULL;
      if (CRM_Utils_Array::value('related_contact', $ids)) {
        $targetContactID = $contribution->contact_id;
        $contribution->contact_id = $ids['related_contact'];
      }
      CRM_Activity_BAO_Activity::addActivity($contribution, NULL, $targetContactID);
      // event
    }
    else {
      CRM_Activity_BAO_Activity::addActivity($participant);
    }

    CRM_Core_Error::debug_log_message("Contribution record updated successfully");
    $transaction->commit();

    // CRM-9132 legacy behaviour was that receipts were sent out in all instances. Still sending
    // when array_key 'is_email_receipt doesn't exist in case some instances where is needs setting haven't been set
    if (!array_key_exists('is_email_receipt', $values) ||
      $values['is_email_receipt'] == 1
    ) {
      self::sendMail($input, $ids, $objects, $values, $recur, FALSE);
    }

    CRM_Core_Error::debug_log_message("Success: Database updated and mail sent");
  }

  function getBillingID(&$ids) {
    // get the billing location type
    require_once 'CRM/Core/PseudoConstant.php';
    $locationTypes = CRM_Core_PseudoConstant::locationType();
    // CRM-8108 remove the ts around the Billing locationtype
    //$ids['billing'] =  array_search( ts('Billing'),  $locationTypes );
    $ids['billing'] = array_search('Billing', $locationTypes);
    if (!$ids['billing']) {
      CRM_Core_Error::debug_log_message(ts('Please set a location type of %1', array(1 => 'Billing')));
      echo "Failure: Could not find billing location type<p>";
      return FALSE;
    }
    return TRUE;
  }

  function sendMail(&$input, &$ids, &$objects, &$values, $recur = FALSE, $returnMessageText = FALSE) {
    $contribution = &$objects['contribution'];
    $participant  = &$objects['participant'];
    $event        = &$objects['event'];
    $memberships  = &$objects['membership'];
    $contact      = &$objects['contact'];
    if (empty($values)) {
      $values = array();
      $contribID = $ids['contribution'];
      if ($input['component'] == 'contribute') {
        require_once 'CRM/Contribute/BAO/ContributionPage.php';
        if (isset($contribution->contribution_page_id)) {
          CRM_Contribute_BAO_ContributionPage::setValues($contribution->contribution_page_id, $values);

          if ($contribution->contribution_page_id) {
            // CRM-8254
            $config = CRM_Core_Config::singleton();
            $config->defaultCurrency = CRM_Utils_Array::value('currency',
              $values,
              $config->defaultCurrency
            );
          }
        }
        else {
          // Handle re-print receipt for offline contributions (call from PDF.php - no contribution_page_id)
          $values['is_email_receipt'] = 1;
          $values['title'] = 'Contribution';
        }
        // set lineItem for contribution
        require_once 'CRM/Price/BAO/Set.php';
        if ($contribID && $pId = CRM_Price_BAO_Set::getFor('civicrm_contribution', $contribID)) {
          require_once 'CRM/Price/BAO/LineItem.php';
          $values['lineItem'][0] = CRM_Price_BAO_LineItem::getLineItems($contribID, 'contribution');
          $values['priceSetID'] = $pId;
        }
        require_once 'CRM/Contribute/BAO/Contribution.php';
        $relatedContact = CRM_Contribute_BAO_Contribution::getOnbehalfIds($contribID,
          $contribution->contact_id
        );
        // if this is onbehalf of contribution then set related contact
        if ($relatedContactId = CRM_Utils_Array::value('individual_id', $relatedContact)) {
          $values['related_contact'] = $ids['related_contact'] = $relatedContactId;
        }
      }
      else {
        // event
        $eventParams = array('id' => $objects['event']->id);
        $values['event'] = array();

        require_once 'CRM/Event/BAO/Event.php';
        CRM_Event_BAO_Event::retrieve($eventParams, $values['event']);

        //get location details
        $locationParams = array('entity_id' => $objects['event']->id, 'entity_table' => 'civicrm_event');
        require_once 'CRM/Core/BAO/Location.php';
        require_once 'CRM/Event/Form/ManageEvent/Location.php';
        $values['location'] = CRM_Core_BAO_Location::getValues($locationParams);

        require_once 'CRM/Core/BAO/UFJoin.php';
        $ufJoinParams = array(
          'entity_table' => 'civicrm_event',
          'entity_id' => $ids['event'],
          'weight' => 1,
        );

        $values['custom_pre_id'] = CRM_Core_BAO_UFJoin::findUFGroupId($ufJoinParams);

        $ufJoinParams['weight'] = 2;
        $values['custom_post_id'] = CRM_Core_BAO_UFJoin::findUFGroupId($ufJoinParams);

        // set lineItem for event contribution
        if ($contribID) {
          require_once 'CRM/Event/BAO/Participant.php';
          $participantIds = CRM_Event_BAO_Participant::getParticipantIds($contribID);
          require_once 'CRM/Price/BAO/LineItem.php';
          if (!empty($participantIds)) {
            foreach ($participantIds as $pIDs) {
              $lineItem = CRM_Price_BAO_LineItem::getLineItems($pIDs);
              if (!CRM_Utils_System::isNull($lineItem)) {
                $values['lineItem'][] = $lineItem;
              }
            }
          }
        }
      }

      // set receipt from e-mail and name in value
      if (!$returnMessageText) {
        require_once 'CRM/Contact/BAO/Contact/Location.php';
        $session = CRM_Core_Session::singleton();
        $userID = $session->get('userID');
        list($userName, $userEmail) = CRM_Contact_BAO_Contact_Location::getEmailDetails($userID);
        $values['receipt_from_email'] = $userEmail;
        $values['receipt_from_name'] = $userName;
      }

      // set display address of contributor
      if ($contribution->address_id) {
        require_once 'CRM/Core/BAO/Address.php';
        $addressParams     = array('id' => $contribution->address_id);
        $addressDetails    = CRM_Core_BAO_Address::getValues($addressParams, FALSE, 'id');
        $addressDetails    = array_values($addressDetails);
        $values['address'] = $addressDetails[0]['display'];
      }
    }

    $template = CRM_Core_Smarty::singleton();
    $template->assign('first_name', $contact->first_name);
    $template->assign('last_name', $contact->last_name);
    $template->assign('displayName', $contact->display_name);
    // CRM_Core_Error::debug('tpl',$template);
    //assign honor infomation to receiptmessage
    if ($honarID = CRM_Core_DAO::getFieldValue('CRM_Contribute_DAO_Contribution',
        $contribution->id,
        'honor_contact_id'
      )) {
      $honorDefault = array();
      $honorIds = array();
      $honorIds['contribution'] = $contribution->id;

      $idParams = array('id' => $honarID, 'contact_id' => $honarID);

      require_once 'CRM/Contact/BAO/Contact.php';
      CRM_Contact_BAO_Contact::retrieve($idParams, $honorDefault, $honorIds);

      require_once 'CRM/Core/PseudoConstant.php';
      $honorType = CRM_Core_PseudoConstant::honor();

      $template->assign('honor_block_is_active', 1);
      if (CRM_Utils_Array::value('prefix_id', $honorDefault)) {
        $prefix = CRM_Core_PseudoConstant::individualPrefix();
        $template->assign('honor_prefix', $prefix[$honorDefault['prefix_id']]);
      }
      $template->assign('honor_first_name', CRM_Utils_Array::value('first_name', $honorDefault));
      $template->assign('honor_last_name', CRM_Utils_Array::value('last_name', $honorDefault));
      $template->assign('honor_email', CRM_Utils_Array::value('email', $honorDefault['email'][1]));
      $template->assign('honor_type', $honorType[$contribution->honor_type_id]);
    }

    require_once 'CRM/Contribute/DAO/ContributionProduct.php';
    $dao = new CRM_Contribute_DAO_ContributionProduct();
    $dao->contribution_id = $contribution->id;
    if ($dao->find(TRUE)) {
      $premiumId = $dao->product_id;
      $template->assign('option', $dao->product_option);

      require_once 'CRM/Contribute/DAO/Product.php';
      $productDAO = new CRM_Contribute_DAO_Product();
      $productDAO->id = $premiumId;
      $productDAO->find(TRUE);
      $template->assign('selectPremium', TRUE);
      $template->assign('product_name', $productDAO->name);
      $template->assign('price', $productDAO->price);
      $template->assign('sku', $productDAO->sku);
    }

    // add the new contribution values
    if ($input['component'] == 'contribute') {
      $template->assign('title', $values['title']);
      $template->assign('amount', $input['amount']);

      //PCP Info
      require_once 'CRM/Contribute/DAO/ContributionSoft.php';
      $softDAO = new CRM_Contribute_DAO_ContributionSoft();
      $softDAO->contribution_id = $contribution->id;
      if ($softDAO->find(TRUE)) {
        $template->assign('pcpBlock', TRUE);
        $template->assign('pcp_display_in_roll', $softDAO->pcp_display_in_roll);
        $template->assign('pcp_roll_nickname', $softDAO->pcp_roll_nickname);
        $template->assign('pcp_personal_note', $softDAO->pcp_personal_note);

        //assign the pcp page title for email subject
        require_once 'CRM/PCP/DAO/PCP.php';
        $pcpDAO = new CRM_PCP_DAO_PCP();
        $pcpDAO->id = $softDAO->pcp_id;
        if ($pcpDAO->find(TRUE)) {
          $template->assign('title', $pcpDAO->title);
        }
      }
    }
    else {
      $template->assign('title', $values['event']['title']);
      $template->assign('totalAmount', $input['amount']);
    }

    if ($contribution->contribution_type_id) {
      $values['contribution_type_id'] = $contribution->contribution_type_id;
    }

    $template->assign('trxn_id', $contribution->trxn_id);
    $template->assign('receive_date',
      CRM_Utils_Date::mysqlToIso($contribution->receive_date)
    );
    $template->assign('contributeMode', 'notify');
    $template->assign('action', $contribution->is_test ? 1024 : 1);
    $template->assign('receipt_text',
      CRM_Utils_Array::value('receipt_text',
        $values
      )
    );
    $template->assign('is_monetary', 1);
    $template->assign('is_recur', $recur);
    $template->assign('currency', $contribution->currency);
    if ($recur) {
      require_once 'CRM/Core/Payment.php';
      $paymentObject = &CRM_Core_Payment::singleton($contribution->is_test ? 'test' : 'live',
        $objects['paymentProcessor']
      );
      $url = $paymentObject->cancelSubscriptionURL();
      $template->assign('cancelSubscriptionUrl', $url);
      if ($objects['paymentProcessor']['billing_mode'] & CRM_Core_Payment::BILLING_MODE_FORM) {
        //direct mode showing billing block, so use directIPN for temporary
        $template->assign('contributeMode', 'directIPN');
      }
    }

    require_once 'CRM/Utils/Address.php';
    $template->assign('address', CRM_Utils_Address::format($input));
    if ($input['component'] == 'event') {
      require_once 'CRM/Core/OptionGroup.php';
      $participantRoles = CRM_Event_PseudoConstant::participantRole();
      $viewRoles = array();
      foreach (explode(CRM_Core_DAO::VALUE_SEPARATOR, $participant->role_id) as $k => $v) {
        $viewRoles[] = $participantRoles[$v];
      }
      $values['event']['participant_role'] = implode(', ', $viewRoles);
      $template->assign('event', $values['event']);
      $template->assign('location', $values['location']);
      $template->assign('customPre', $values['custom_pre_id']);
      $template->assign('customPost', $values['custom_post_id']);

      $isTest = FALSE;
      if ($participant->is_test) {
        $isTest = TRUE;
      }

      $values['params'] = array();
      require_once 'CRM/Event/BAO/Event.php';
      //to get email of primary participant.
      $primaryEmail = CRM_Core_DAO::getFieldValue('CRM_Core_DAO_Email', $participant->contact_id, 'email', 'contact_id');
      $primaryAmount[] = array('label' => $participant->fee_level . ' - ' . $primaryEmail, 'amount' => $participant->fee_amount);
      //build an array of cId/pId of participants
      $additionalIDs = CRM_Event_BAO_Event::buildCustomProfile($participant->id, NULL, $ids['contact'], $isTest, TRUE);
      unset($additionalIDs[$participant->id]);
      //send receipt to additional participant if exists
      if (count($additionalIDs)) {
        $template->assign('isPrimary', 0);
        $template->assign('customProfile', NULL);
        //set additionalParticipant true
        $values['params']['additionalParticipant'] = TRUE;
        foreach ($additionalIDs as $pId => $cId) {
          $amount = array();
          //to change the status pending to completed
          $additional = new CRM_Event_DAO_Participant();
          $additional->id = $pId;
          $additional->contact_id = $cId;
          $additional->find(TRUE);
          $additional->register_date = $participant->register_date;
          $additional->status_id = 1;
          $additionalParticipantInfo = CRM_Core_DAO::getFieldValue('CRM_Core_DAO_Email', $additional->contact_id, 'email', 'contact_id');
          //if additional participant dont have email
          //use display name.
          if (!$additionalParticipantInfo) {
            require_once 'CRM/Contact/BAO/Contact.php';
            $additionalParticipantInfo = CRM_Contact_BAO_Contact::displayName($additional->contact_id);
          }
          $amount[0] = array('label' => $additional->fee_level, 'amount' => $additional->fee_amount);
          $primaryAmount[] = array('label' => $additional->fee_level . ' - ' . $additionalParticipantInfo, 'amount' => $additional->fee_amount);
          $additional->save();
          $additional->free();
          $template->assign('amount', $amount);
          CRM_Event_BAO_Event::sendMail($cId, $values, $pId, $isTest, $returnMessageText);
        }
      }

      //build an array of custom profile and assigning it to template
      $customProfile = CRM_Event_BAO_Event::buildCustomProfile($participant->id, $values, NULL, $isTest);

      if (count($customProfile)) {
        $template->assign('customProfile', $customProfile);
      }

      // for primary contact
      $values['params']['additionalParticipant'] = FALSE;
      $template->assign('isPrimary', 1);
      $template->assign('amount', $primaryAmount);
      $template->assign('register_date', CRM_Utils_Date::isoToMysql($participant->register_date));
      if ($contribution->payment_instrument_id) {
        require_once 'CRM/Contribute/PseudoConstant.php';
        $paymentInstrument = CRM_Contribute_PseudoConstant::paymentInstrument();
        $template->assign('paidBy', $paymentInstrument[$contribution->payment_instrument_id]);
      }
      // carry paylater, since we did not created billing,
      // so need to pull email from primary location, CRM-4395
      $values['params']['is_pay_later'] = $participant->is_pay_later;

      return CRM_Event_BAO_Event::sendMail($ids['contact'], $values, $participant->id, $isTest, $returnMessageText);
    }
    else {
      $values['contribution_id'] = $contribution->id;
      if (CRM_Utils_Array::value('related_contact', $ids)) {
        $values['related_contact'] = $ids['related_contact'];
        if (isset($ids['onbehalf_dupe_alert'])) {
          $values['onbehalf_dupe_alert'] = $ids['onbehalf_dupe_alert'];
        }

        require_once 'CRM/Core/BAO/Address.php';
        $entityBlock = array(
          'contact_id' => $ids['contact'],
          'location_type_id' => CRM_Core_DAO::getFieldValue('CRM_Core_DAO_LocationType',
            'Home', 'id', 'name'
          ),
        );
        $address = CRM_Core_BAO_Address::getValues($entityBlock);
        $template->assign('onBehalfAddress', $address[$entityBlock['location_type_id']]['display']);
      }

      $isTest = FALSE;
      if ($contribution->is_test) {
        $isTest = TRUE;
      }
      // CRM_Core_Error::debug('val',$values);

      require_once 'CRM/Contribute/BAO/ContributionPage.php';

      if (!empty($memberships)) {
        foreach ($memberships as $membership) {
          if ($membership->id) {
            $values['membership_id'] = $membership->id;

            // need to set the membership values here
            $template->assign('membership_assign', 1);
            require_once 'CRM/Member/PseudoConstant.php';
            $template->assign('membership_name',
              CRM_Member_PseudoConstant::membershipType($membership->membership_type_id)
            );
            $template->assign('mem_start_date', $membership->start_date);
            $template->assign('mem_join_date', $membership->join_date);
            $template->assign('mem_end_date', $membership->end_date);
            $membership_status = CRM_Member_PseudoConstant::membershipStatus($membership->status_id, NULL, 'label');
            $template->assign('mem_status', $membership_status);
            if ($membership_status == 'Pending' && $membership->is_pay_later == 1) {
              $template->assign('is_pay_later', 1);
            }

            // if separate payment there are two contributions recorded and the
            // admin will need to send a receipt for each of them separately.
            // we dont link the two in the db (but can potentially infer it if needed)
            $template->assign('is_separate_payment', 0);

            if ($recur && $paymentObject) {
              $url = $paymentObject->cancelSubscriptionURL($membership->id, 'membership');
              $template->assign('cancelSubscriptionUrl', $url);
            }
            $result = CRM_Contribute_BAO_ContributionPage::sendMail($ids['contact'], $values,
              $isTest, $returnMessageText
            );
            if ($returnMessageText) {
              return $result;
            }
            // otherwise if its about sending emails, continue sending without return, as we
            // don't want to exit the loop.
          }
        }
      }
      else {
        return CRM_Contribute_BAO_ContributionPage::sendMail($ids['contact'], $values, $isTest, $returnMessageText);
      }
    }
  }

  function updateContributionStatus(&$params) {
    // get minimum required values.
    $statusId       = CRM_Utils_Array::value('contribution_status_id', $params);
    $componentId    = CRM_Utils_Array::value('component_id', $params);
    $componentName  = CRM_Utils_Array::value('componentName', $params);
    $contributionId = CRM_Utils_Array::value('contribution_id', $params);

    if (!$contributionId || !$componentId || !$componentName || !$statusId) {
      return;
    }

    $input = $ids = $objects = array();

    //get the required ids.
    $ids['contribution'] = $contributionId;

    if (!$ids['contact'] = CRM_Utils_Array::value('contact_id', $params)) {
      $ids['contact'] = CRM_Core_DAO::getFieldValue('CRM_Contribute_DAO_Contribution',
        $contributionId,
        'contact_id'
      );
    }

    if ($componentName == 'Event') {
      $name = 'event';
      $ids['participant'] = $componentId;

      if (!$ids['event'] = CRM_Utils_Array::value('event_id', $params)) {
        $ids['event'] = CRM_Core_DAO::getFieldValue('CRM_Event_DAO_Participant',
          $componentId,
          'event_id'
        );
      }
    }

    if ($componentName == 'Membership') {
      $name = 'contribute';
      $ids['membership'] = $componentId;
    }
    $ids['contributionPage'] = NULL;
    $ids['contributionRecur'] = NULL;
    $input['component'] = $name;

    require_once 'CRM/Core/Payment/BaseIPN.php';
    require_once 'CRM/Core/Transaction.php';

    $baseIPN = new CRM_Core_Payment_BaseIPN();
    $transaction = new CRM_Core_Transaction();

    // reset template values.
    $template = CRM_Core_Smarty::singleton();
    $template->clearTemplateVars();

    if (!$baseIPN->validateData($input, $ids, $objects, FALSE)) {
      CRM_Core_Error::fatal();
    }

    $contribution = &$objects['contribution'];

    require_once 'CRM/Contribute/PseudoConstant.php';
    $contributionStatuses = CRM_Contribute_PseudoConstant::contributionStatus(NULL, 'name');

    if ($statusId == array_search('Cancelled', $contributionStatuses)) {
      $baseIPN->cancelled($objects, $transaction);
      $transaction->commit();
      return $statusId;
    }
    elseif ($statusId == array_search('Failed', $contributionStatuses)) {
      $baseIPN->failed($objects, $transaction);
      $transaction->commit();
      return $statusId;
    }

    // status is not pending
    if ($contribution->contribution_status_id != array_search('Pending', $contributionStatuses)) {
      $transaction->commit();
      return;
    }

    //set values for ipn code.
    foreach (array(
      'fee_amount', 'check_number', 'payment_instrument_id') as $field) {
      if (!$input[$field] = CRM_Utils_Array::value($field, $params)) {
        $input[$field] = $contribution->$field;
      }
    }
    if (!$input['trxn_id'] = CRM_Utils_Array::value('trxn_id', $params)) {
      $input['trxn_id'] = $contribution->invoice_id;
    }
    if (!$input['amount'] = CRM_Utils_Array::value('total_amount', $params)) {
      $input['amount'] = $contribution->total_amount;
    }
    $input['is_test'] = $contribution->is_test;
    $input['net_amount'] = $contribution->net_amount;
    if (CRM_Utils_Array::value('fee_amount', $input) && CRM_Utils_Array::value('amount', $input)) {
      $input['net_amount'] = $input['amount'] - $input['fee_amount'];
    }

    //complete the contribution.
    $baseIPN->completeTransaction($input, $ids, $objects, $transaction, FALSE);

    // reset template values before processing next transactions
    $template->clearTemplateVars();

    return $statusId;
  }

  /* 
     * Update pledge associated with a recurring contribution
     * 
     * If the contribution has a pledge_payment record pledge, then update the pledge_payment record & pledge based on that linkage.
     * 
     * If a previous contribution in the recurring contribution sequence is linked with a pledge then we assume this contribution
     * should be  linked with the same pledge also. Currently only back-office users can apply a recurring payment to a pledge & 
     * it should be assumed they
     * do so with the intention that all payments will be linked
     * 
     * The pledge payment record should already exist & will need to be updated with the new contribution ID.
     * If not the contribution will also need to be linked to the pledge
     */
  function updateRecurLinkedPledge(&$contribution) {
    $returnProperties = array('id', 'pledge_id');
    $paymentDetails   = array();
    $paymentIDs       = array();

    require_once 'CRM/Pledge/BAO/PledgePayment.php';
    if (CRM_Core_DAO::commonRetrieveAll('CRM_Pledge_DAO_PledgePayment', 'contribution_id', $contribution->id,
        $paymentDetails, $returnProperties
      )) {
      foreach ($paymentDetails as $key => $value) {
        $paymentIDs[] = $value['id'];
        $pledgeId = $value['pledge_id'];
      }
    }
    else {
      //payment is not already linked - if it is linked with a pledge we need to create a link.
      // return if it is not recurring contribution
      if (!$contribution->contribution_recur_id) {
        return;
      }

      require_once 'CRM/Contribute/DAO/Contribution.php';
      $relatedContributions = new CRM_Contribute_DAO_Contribution();
      $relatedContributions->contribution_recur_id = $contribution->contribution_recur_id;
      $relatedContributions->find();

      while ($relatedContributions->fetch()) {
        CRM_Core_DAO::commonRetrieveAll('CRM_Pledge_DAO_PledgePayment', 'contribution_id', $relatedContributions->id,
          $paymentDetails, $returnProperties
        );
      }

      if (empty($paymentDetails)) {
        // payment is not linked with a pledge and neither are any other contributions on this
        return;
      }

      foreach ($paymentDetails as $key => $value) {
        $pledgeId = $value['pledge_id'];
      }

      // we have a pledge now we need to get the oldest unpaid payment
      $paymentDetails = CRM_Pledge_BAO_PledgePayment::getOldestPledgePayment($pledgeId);
      $paymentDetails['contribution_id'] = $contribution->id;
      $paymentDetails['status_id'] = $contribution->contribution_status_id;
      $paymentDetails['actual_amount'] = $contribution->total_amount;

      // put contribution against it
      $payment = CRM_Pledge_BAO_PledgePayment::add($paymentDetails);
      $paymentIDs[] = $payment->id;
    }

    // update pledge and corresponding payment statuses
    CRM_Pledge_BAO_PledgePayment::updatePledgePaymentStatus($pledgeId, $paymentIDs, $contribution->contribution_status_id,
      NULL, $contribution->total_amount
    );
  }
}

