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

require_once 'CRM/Contribute/DAO/ContributionRecur.php';
class CRM_Contribute_BAO_ContributionRecur extends CRM_Contribute_DAO_ContributionRecur {

  /**
   * takes an associative array and creates a contribution object
   *
   * the function extract all the params it needs to initialize the create a
   * contribution object. the params array could contain additional unused name/value
   * pairs
   *
   * @param array  $params (reference ) an assoc array of name/value pairs
   * @param array $ids    the array that holds all the db ids
   *
   * @return object CRM_Contribute_BAO_Contribution object
   * @access public
   * @static
   */
  static
  function add(&$params, &$ids) {

    require_once 'CRM/Utils/Hook.php';
    if (CRM_Utils_Array::value('id', $params)) {
      CRM_Utils_Hook::pre('edit', 'ContributionRecur', $params['id'], $params);
    }
    else {
      CRM_Utils_Hook::pre('create', 'ContributionRecur', NULL, $params);
    }

    $duplicates = array();
    if (self::checkDuplicate($params, $duplicates)) {
      $error = CRM_Core_Error::singleton();
      $d = implode(', ', $duplicates);
      $error->push(CRM_Core_Error::DUPLICATE_CONTRIBUTION,
        'Fatal',
        array($d),
        "Found matching contribution(s): $d"
      );
      return $error;
    }

    $recurring = new CRM_Contribute_BAO_ContributionRecur();
    $recurring->copyValues($params);
    $recurring->id = CRM_Utils_Array::value('contribution', $ids);

    // set currency for CRM-1496
    if (!isset($recurring->currency)) {
      $config = CRM_Core_Config::singleton();
      $recurring->currency = $config->defaultCurrency;
    }
    $result = $recurring->save();

    if (CRM_Utils_Array::value('id', $params)) {
      CRM_Utils_Hook::post('edit', 'ContributionRecur', $recurring->id, $recurring);
    }
    else {
      CRM_Utils_Hook::post('create', 'ContributionRecur', $recurring->id, $recurring);
    }

    return $result;
  }

  /**
   * Check if there is a contribution with the same trxn_id or invoice_id
   *
   * @param array  $params (reference ) an assoc array of name/value pairs
   * @param array  $duplicates (reference ) store ids of duplicate contribs
   *
   * @return boolean true if duplicate, false otherwise
   * @access public
   * static
   */
  static
  function checkDuplicate($params, &$duplicates) {
    $id         = CRM_Utils_Array::value('id', $params);
    $trxn_id    = CRM_Utils_Array::value('trxn_id', $params);
    $invoice_id = CRM_Utils_Array::value('invoice_id', $params);

    $clause = array();
    $params = array();

    if ($trxn_id) {
      $clause[] = "trxn_id = %1";
      $params[1] = array($trxn_id, 'String');
    }

    if ($invoice_id) {
      $clause[] = "invoice_id = %2";
      $params[2] = array($invoice_id, 'String');
    }

    if (empty($clause)) {
      return FALSE;
    }

    $clause = implode(' OR ', $clause);
    if ($id) {
      $clause = "( $clause ) AND id != %3";
      $params[3] = array($id, 'Integer');
    }

    $query  = "SELECT id FROM civicrm_contribution_recur WHERE $clause";
    $dao    = CRM_Core_DAO::executeQuery($query, $params);
    $result = FALSE;
    while ($dao->fetch()) {
      $duplicates[] = $dao->id;
      $result = TRUE;
    }
    return $result;
  }

  static
  function getPaymentProcessor($id, $mode) {
    $sql = "
SELECT p.payment_processor_id
  FROM civicrm_contribution c,
       civicrm_contribution_recur r,
       civicrm_contribution_page  p
 WHERE c.contribution_recur_id = %1
   AND c.contribution_page_id  = p.id
   AND p.payment_processor_id is not null
 LIMIT 1";
    $params = array(1 => array($id, 'Integer'));
    $paymentProcessorID = &CRM_Core_DAO::singleValueQuery($sql,
      $params
    );
    if (!$paymentProcessorID) {
      return NULL;
    }

    require_once 'CRM/Core/BAO/PaymentProcessor.php';
    return CRM_Core_BAO_PaymentProcessor::getPayment($paymentProcessorID, $mode);
  }

  /**
   * Function to get the number of installment done/completed for each recurring contribution
   *
   * @param array  $ids (reference ) an array of recurring contribution ids
   *
   * @return array $totalCount an array of recurring ids count
   * @access public
   * static
   */
  static
  function getCount(&$ids) {
    $recurID = implode(',', $ids);
    $totalCount = array();

    $query = " 
         SELECT contribution_recur_id, count( contribution_recur_id ) as commpleted
         FROM civicrm_contribution
         WHERE contribution_recur_id IN ( {$recurID}) AND is_test = 0
         GROUP BY contribution_recur_id";

    $res = CRM_Core_DAO::executeQuery($query, CRM_Core_DAO::$_nullArray);

    while ($res->fetch()) {
      $totalCount[$res->contribution_recur_id] = $res->commpleted;
    }
    return $totalCount;
  }

  /**
   * Delete Recurring contribution.
   *
   * @return true / false.
   * @access public
   * @static
   */
  static
  function deleteRecurContribution($recurId) {
    $result = FALSE;
    if (!$recurId) {
      return $result;
    }

    $recur     = new CRM_Contribute_DAO_ContributionRecur();
    $recur->id = $recurId;
    $result    = $recur->delete();

    return $result;
  }

  /**
   * Cancel Recurring contribution.
   *
   * @param integer  $recurId recur contribution id.
   * @param array    $objects an array of objects that is to be cancelled like
   *                          contribution, membership, event. At least contribution object is a must.
   *
   * @return true / false.
   * @access public
   * @static
   */
  static
  function cancelRecurContribution($recurId, $objects) {
    if (!$recurId) {
      return FALSE;
    }

    require_once 'CRM/Contribute/PseudoConstant.php';
    $contributionStatus = CRM_Contribute_PseudoConstant::contributionStatus(NULL, 'name');
    $canceledId         = array_search('Cancelled', $contributionStatus);
    $recur              = new CRM_Contribute_DAO_ContributionRecur();
    $recur->id          = $recurId;
    $recur->whereAdd("contribution_status_id != $canceledId");

    if ($recur->find(TRUE)) {
      require_once 'CRM/Core/Transaction.php';
      $transaction = new CRM_Core_Transaction();
      $recur->contribution_status_id = $canceledId;
      $recur->start_date = CRM_Utils_Date::isoToMysql($recur->start_date);
      $recur->create_date = CRM_Utils_Date::isoToMysql($recur->create_date);
      $recur->modified_date = CRM_Utils_Date::isoToMysql($recur->modified_date);
      $recur->cancel_date = date('YmdHis');
      $recur->save();

      if ($objects == CRM_Core_DAO::$_nullObject) {
        $transaction->commit();
        return TRUE;
      }
      else {
        require_once 'CRM/Core/Payment/BaseIPN.php';
        $baseIPN = new CRM_Core_Payment_BaseIPN();
        return $baseIPN->cancelled($objects, $transaction);
      }
    }

    return FALSE;
  }

  /**
   * Function to get list of recurring contribution of contact Ids
   *
   * @param int $contactId Contact ID
   *
   * @return return the list of recurring contribution fields
   *
   * @access public
   * @static
   */
  static
  function getRecurContributions($contactId) {
    $params = array();
    require_once 'CRM/Contribute/DAO/ContributionRecur.php';
    $recurDAO = new CRM_Contribute_DAO_ContributionRecur();
    $recurDAO->contact_id = $contactId;
    $recurDAO->find();
    require_once 'CRM/Contribute/PseudoConstant.php';
    $contributionStatus = CRM_Contribute_PseudoConstant::contributionStatus();

    while ($recurDAO->fetch()) {
      $params[$recurDAO->id]['id'] = $recurDAO->id;
      $params[$recurDAO->id]['contactId'] = $recurDAO->contact_id;
      $params[$recurDAO->id]['start_date'] = $recurDAO->start_date;
      $params[$recurDAO->id]['end_date'] = $recurDAO->end_date;
      $params[$recurDAO->id]['next_sched_contribution'] = $recurDAO->next_sched_contribution;
      $params[$recurDAO->id]['amount'] = $recurDAO->amount;
      $params[$recurDAO->id]['frequency_unit'] = $recurDAO->frequency_unit;
      $params[$recurDAO->id]['frequency_interval'] = $recurDAO->frequency_interval;
      $params[$recurDAO->id]['installments'] = $recurDAO->installments;
      $params[$recurDAO->id]['contribution_status_id'] = $recurDAO->contribution_status_id;
      $params[$recurDAO->id]['contribution_status'] = CRM_Utils_Array::value($recurDAO->contribution_status_id, $contributionStatus);
      $params[$recurDAO->id]['is_test'] = $recurDAO->is_test;
      $params[$recurDAO->id]['payment_processor_id'] = $recurDAO->payment_processor_id;
    }

    return $params;
  }

  /**
   * update the is_active flag in the db
   *
   * @param int      $id        id of the database record
   * @param boolean  $is_active value we want to set the is_active field
   *
   * @return Object             DAO object on sucess, null otherwise
   * @static
   */
  static
  function setIsActive($id, $is_active) {
    if (!$is_active) {
      return self::cancelRecurContribution($id, CRM_Core_DAO::$_nullObject);
    }
    return FALSE;
  }
}

