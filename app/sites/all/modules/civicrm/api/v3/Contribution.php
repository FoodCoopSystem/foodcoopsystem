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
 * File for the CiviCRM APIv3 Contribution functions
 *
 * @package CiviCRM_APIv3
 * @subpackage API_Contribute
 *
 * @copyright CiviCRM LLC (c) 2004-2011
 * @version $Id: Contribution.php 30486 2010-11-02 16:12:09Z shot $
 *
 */

/**
 * Include utility functions
 */
require_once 'CRM/Contribute/BAO/Contribution.php';
require_once 'CRM/Utils/Rule.php';
require_once 'CRM/Contribute/PseudoConstant.php';

/**
 * Add or update a contribution
 *
 * @param  array   $params           (reference ) input parameters
 *
 * @return array  Api result array
 * @static void
 * @access public
 * @example ContributionCreate.php
 * {@getfields Contribution_create}
 */
function civicrm_api3_contribution_create($params) {
  civicrm_api3_verify_one_mandatory($params, NULL, array('contribution_type_id', 'contribution_type'));


  $values = array();

  $error = _civicrm_api3_contribute_format_params($params, $values);
  if (civicrm_error($error)) {
    return $error;
  }
  _civicrm_api3_custom_format_params($params, $values, 'Contribution');
  $values["contact_id"] = $params["contact_id"];
  $values["source"] = CRM_Utils_Array::value('source', $params);

  $ids = array();
  if (CRM_Utils_Array::value('id', $params)) {
    $ids['contribution'] = $params['id'];
  }
  $contribution = CRM_Contribute_BAO_Contribution::create($values, $ids);
  if (is_a($contribution, 'CRM_Core_Error')) {
    return civicrm_api3_create_error($contribution->_errors[0]['message']);
  }
  _civicrm_api3_object_to_array($contribution, $contributeArray[$contribution->id]);

  return civicrm_api3_create_success($contributeArray, $params, 'contribution', 'create', $contribution);
}
/*
 * Adjust Metadata for Create action
 * 
 * The metadata is used for setting defaults, documentation & validation
 * @param array $params array or parameters determined by getfields
 */
function _civicrm_api3_contribution_create_spec(&$params) {
  $params['contact_id']['api.required'] = 1;
  $params['total_amount']['api.required'] = 1;
  $params['note'] = array(
    'name' => 'note',
    'title' => 'note',
    'type' => 2,
    'description' => 'Associated Note in the notes table',
  );
  $params['soft_credit_to'] = array(
    'name' => 'soft_credit_to',
    'title' => 'Soft Credit contact ID',
    'type' => 1,
    'description' => 'ID of Contact to be Soft credited to',
    'FKClassName' => 'CRM_Contact_DAO_Contact',
  );
}

/**
 * Delete a contribution
 *
 * @param  array   $params           (reference ) input parameters
 *
 * @return boolean        true if success, else false
 * @static void
 * @access public
 * {@getfields Contribution_delete}
 * @example ContributionDelete.php
 */
function civicrm_api3_contribution_delete($params) {

  $contributionID = CRM_Utils_Array::value('contribution_id', $params) ? $params['contribution_id'] : $params['id'];
  if (CRM_Contribute_BAO_Contribution::deleteContribution($contributionID)) {
    return civicrm_api3_create_success(array($contributionID => 1));
  }
  else {
    return civicrm_api3_create_error('Could not delete contribution');
  }
}
/*
 * modify metadata. Legacy support for contribution_id
 */
function _civicrm_api3_contribution_delete_spec(&$params) {
  $params['id']['api.aliases'] = array('contribution_id');
}

/**
 * Retrieve a set of contributions, given a set of input params
 *
 * @param  array   $params           (reference ) input parameters
 * @param array    $returnProperties Which properties should be included in the
 * returned Contribution object. If NULL, the default
 * set of properties will be included.
 *
 * @return array (reference )        array of contributions, if error an array with an error id and error message
 * @static void
 * @access public
 * {@getfields Contribution_get}
 * @example ContributionGet.php
 */
function civicrm_api3_contribution_get($params) {

  if (CRM_Utils_Array::value('id', $params)) {
    //api supports 'id' but BAO supports 'contribution_id. Change it here
    $params['contribution_id'] = CRM_Utils_Array::value('contribution_id', $params, $params['id']);
    unset($params['id']);
  }

  $inputParams      = array();
  $returnProperties = array();
  $otherVars        = array('sort', 'offset', 'rowCount');

  $sort     = NULL;
  $offset   = 0;
  $rowCount = 25;
  foreach ($params as $n => $v) {
    if (substr($n, 0, 7) == 'return.') {
      $returnProperties[substr($n, 7)] = $v;
    }
    elseif (in_array($n, $otherVars)) {
      $$n = $v;
    }
    else {
      $inputParams[$n] = $v;
    }
  }

  require_once 'CRM/Contribute/BAO/Query.php';
  require_once 'CRM/Contact/BAO/Query.php';
  if (empty($returnProperties)) {
    $returnProperties = CRM_Contribute_BAO_Query::defaultReturnProperties(CRM_Contact_BAO_Query::MODE_CONTRIBUTE);
  }

  $newParams = &CRM_Contact_BAO_Query::convertFormValues($inputParams);
  $query = new CRM_Contact_BAO_Query($newParams, $returnProperties, NULL,
    FALSE, FALSE, CRM_Contact_BAO_Query::MODE_CONTRIBUTE
  );
  list($select, $from, $where, $having) = $query->query();

  $sql = "$select $from $where $having";

  if (!empty($sort)) {
    $sql .= " ORDER BY $sort ";
  }
  $sql .= " LIMIT $offset, $rowCount ";
  $dao = &CRM_Core_DAO::executeQuery($sql);

  $contribution = array();
  while ($dao->fetch()) {
    $contribution[$dao->contribution_id] = $query->store($dao);
  }

  return civicrm_api3_create_success($contribution, $params, 'contribution', 'get', $dao);
}
/*
 * Adjust Metadata for Get action
 * 
 * The metadata is used for setting defaults, documentation & validation
 * @param array $params array or parameters determined by getfields
 */
function _civicrm_api3_contribution_get_spec(&$params) {
  $params['contribution_test']['api.default'] = 0;
  $params['contact_id'] = $params['contribution_contact_id'];
  $params['contact_id']['api.aliases'] = array('contribution_contact_id');
  unset($params['contribution_contact_id']);
}

/**
 * take the input parameter list as specified in the data model and
 * convert it into the same format that we use in QF and BAO object
 *
 * @param array  $params       Associative array of property name/value
 * pairs to insert in new contact.
 * @param array  $values       The reformatted properties that we can use internally
 * '
 *
 * @return array|CRM_Error
 * @access public
 */
function _civicrm_api3_contribute_format_params($params, &$values, $create = FALSE) {
  // copy all the contribution fields as is


  require_once 'CRM/Contribute/DAO/Contribution.php';
  $fields = &CRM_Contribute_DAO_Contribution::fields();

  _civicrm_api3_store_values($fields, $params, $values);

  foreach ($params as $key => $value) {
    // ignore empty values or empty arrays etc
    if (CRM_Utils_System::isNull($value)) {
      continue;
    }

    switch ($key) {
      case 'non_deductible_amount':
      case 'total_amount':
      case 'fee_amount':
      case 'net_amount':
        if (!CRM_Utils_Rule::money($value)) {
          return civicrm_api3_create_error("$key not a valid amount: $value");
        }
        break;

      case 'currency':
        if (!CRM_Utils_Rule::currencyCode($value)) {
          return civicrm_api3_create_error("currency not a valid code: $value");
        }
        break;

      case 'contribution_type_id':
        if (!CRM_Utils_Array::value($value, CRM_Contribute_PseudoConstant::contributionType())) {
          return civicrm_api3_create_error("Invalid Contribution Type Id");
        }
        break;

      case 'contribution_type':
        $contributionTypeId = CRM_Utils_Array::key(ucfirst($value), CRM_Contribute_PseudoConstant::contributionType());
        if ($contributionTypeId) {
          if (CRM_Utils_Array::value('contribution_type_id', $values) && $contributionTypeId != $values['contribution_type_id']) {
            return civicrm_api3_create_error("Mismatched Contribution Type and Contribution Type Id");
          }
          $values['contribution_type_id'] = $contributionTypeId;
        }
        else {
          return civicrm_api3_create_error("Invalid Contribution Type");
        }
        break;

      case 'payment_instrument':
        require_once 'CRM/Core/OptionGroup.php';
        $values['payment_instrument_id'] = CRM_Core_OptionGroup::getValue('payment_instrument', $value);
        break;

      case 'soft_credit_to':
        if (!CRM_Utils_Rule::integer($value)) {
          return civicrm_api3_create_error("$key not a valid Id: $value");
        }
        $values['soft_credit_to'] = $value;
        break;

      default:
        break;
    }
  }

  if (array_key_exists('note', $params)) {
    $values['note'] = $params['note'];
  }




  return array();
}

/**
 * Process a transaction and record it against the contact.
 *
 * @param  array   $params           (reference ) input parameters
 *
 * @return array (reference )        contribution of created or updated record (or a civicrm error)
 * @static void
 * @access public
 *
 */
function civicrm_api3_contribution_transact($params) {
  $required = array('amount');
  foreach ($required as $key) {
    if (!isset($params[$key])) {
      return civicrm_api3_create_error("Missing parameter $key: civicrm_contribute_transact() requires a parameter '$key'.");
    }
  }

  // allow people to omit some values for convenience
    // 'payment_processor_id' => NULL /* we could retrieve the default processor here, but only if it's missing to avoid an extra lookup */
  $defaults = array(
    'payment_processor_mode' => 'live');
  $params = array_merge($defaults, $params);

  // clean up / adjust some values which
  if (!isset($params['total_amount'])) {
    $params['total_amount'] = $params['amount'];
  }
  if (!isset($params['net_amount'])) {
    $params['net_amount'] = $params['amount'];
  }
  if (!isset($params['receive_date'])) {
    $params['receive_date'] = date('Y-m-d');
  }
  if (!isset($params['invoiceID']) && isset($params['invoice_id'])) {
    $params['invoiceID'] = $params['invoice_id'];
  }

  require_once 'CRM/Core/BAO/PaymentProcessor.php';
  $paymentProcessor = CRM_Core_BAO_PaymentProcessor::getPayment($params['payment_processor_id'], $params['payment_processor_mode']);
  if (civicrm_error($paymentProcessor)) {
    return $paymentProcessor;
  }

  require_once 'CRM/Core/Payment.php';
  $payment = &CRM_Core_Payment::singleton($params['payment_processor_mode'], $paymentProcessor);
  if (civicrm_error($payment)) {
    return $payment;
  }

  $transaction = $payment->doDirectPayment($params);
  if (civicrm_error($transaction)) {
    return $transaction;
  }

  // but actually, $payment->doDirectPayment() doesn't return a
  // CRM_Core_Error by itself
  if (get_class($transaction) == 'CRM_Core_Error') {
    $errs = $transaction->getErrors();
    if (!empty($errs)) {
      $last_error = array_shift($errs);
      return CRM_Core_Error::createApiError($last_error['message']);
    }
  }

  $contribution = civicrm_api('contribution', 'create', $params);
  return $contribution['values'];
}

