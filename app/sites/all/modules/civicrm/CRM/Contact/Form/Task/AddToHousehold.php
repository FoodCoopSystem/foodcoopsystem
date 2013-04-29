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

require_once 'CRM/Contact/Form/Task.php';

/**
 * This class provides the functionality to add contact(s) to Household
 */
class CRM_Contact_Form_Task_AddToHousehold extends CRM_Contact_Form_Task {

  /**
   * Build the form
   *
   * @access public
   *
   * @return void
   */
  function preProcess() {
    /*
         * initialize the task and row fields
         */

    parent::preProcess();
  }

  /**
   * Function to build the form
   *
   * @access public
   *
   * @return None
   */
  function buildQuickForm() {

    CRM_Utils_System::setTitle(ts('Add Members to Household'));
    $this->addElement('text', 'name', ts('Find Target Household'));

    $this->add('select', 'relationship_type_id', ts('Relationship Type'),
      array(
        '' => ts('- select -')) +
      CRM_Contact_BAO_Relationship::getRelationType("Household"), TRUE
    );

    $searchRows = $this->get('searchRows');
    $searchCount = $this->get('searchCount');
    if ($searchRows) {
      $checkBoxes = array();
      $chekFlag = 0;
      foreach ($searchRows as $id => $row) {
        if (!$chekFlag) {
          $chekFlag = $id;
        }
        $checkBoxes[$id] = $this->createElement('radio', NULL, NULL, NULL, $id);
      }
      $this->addGroup($checkBoxes, 'contact_check');
      if ($chekFlag) {
        $checkBoxes[$chekFlag]->setChecked(TRUE);
      }
      $this->assign('searchRows', $searchRows);
    }

    $this->assign('searchCount', $searchCount);
    $this->assign('searchDone', $this->get('searchDone'));
    $this->assign('contact_type_display', ts('Household'));
    $this->addElement('submit', $this->getButtonName('refresh'), ts('Search'), array('class' => 'form-submit'));
    $this->addElement('submit', $this->getButtonName('cancel'), ts('Cancel'), array('class' => 'form-submit'));

    $this->addButtons(array(
        array(
          'type' => 'next',
          'name' => ts('Add to Household'),
          'isDefault' => TRUE,
        ),
        array(
          'type' => 'cancel',
          'name' => ts('Cancel'),
        ),
      )
    );
  }

  /**
   * process the form after the input has been submitted and validated
   *
   * @access public
   *
   * @return None
   */
  public function postProcess() {

    // store the submitted values in an array
    $params = $this->controller->exportValues($this->_name);

    $this->set('searchDone', 0);
    if (CRM_Utils_Array::value('_qf_AddToHousehold_refresh', $_POST)) {
      $searchParams['contact_type'] = array('Household' => 'Household');
      $searchParams['rel_contact'] = $params['name'];
      self::search($this, $searchParams);
      $this->set('searchDone', 1);
      return;
    }

    $data = array();
    //$params['relationship_type_id']='4_a_b';
    $data['relationship_type_id'] = $params['relationship_type_id'];
    $data['is_active'] = 1;
    $invalid = 0;
    $valid = 0;
    $duplicate = 0;
    if (is_array($this->_contactIds)) {
      foreach ($this->_contactIds as $value) {
        $ids = array();
        $ids['contact'] = $value;
        //contact b --> household
        // contact a  -> individual
        $errors = CRM_Contact_BAO_Relationship::checkValidRelationship($params, $ids, $params['contact_check']);
        if ($errors) {
          $invalid = $invalid + 1;
          continue;
        }

        if (CRM_Contact_BAO_Relationship::checkDuplicateRelationship($params,
            CRM_Utils_Array::value('contact', $ids),
            // step 2
            $params['contact_check']
          )) {
          $duplicate++;
          continue;
        }
        CRM_Contact_BAO_Relationship::add($data, $ids, $params['contact_check']);
        $valid++;
      }

      $status = array(
        ts('Added Contact(s) to Household'),
        ts('Total Selected Contact(s): %1', array(1 => $valid + $invalid + $duplicate)),
      );
      if ($valid) {
        $status[] = ts('New relationship record(s) created: %1.', array(
          1 => $valid)) . '<br/>';
      }
      if ($invalid) {
        $status[] = ts('Relationship record(s) not created due to invalid target contact type: %1.', array(
          1 => $invalid)) . '<br/>';
      }
      if ($duplicate) {
        $status[] = ts('Relationship record(s) not created - duplicate of existing relationship: %1.', array(
          1 => $duplicate)) . '<br/>';
      }
      CRM_Core_Session::setStatus($status);
    }
  }
  //end of function

  /**
   * This function is to get the result of the search for Add to * forms
   *
   * @param  array $params  This contains elements for search criteria
   *
   * @access public
   *
   * @return None
   *
   */
  function search(&$form, &$params) {
    //max records that will be listed
    $searchValues = array();
    if (CRM_Utils_Array::value('rel_contact', $params)) {
      if (isset($params['rel_contact_id']) &&
        is_numeric($params['rel_contact_id'])
      ) {
        $searchValues[] = array('contact_id', '=', $params['rel_contact_id'], 0, 1);
      }
      else {
        $searchValues[] = array('sort_name', 'LIKE', $params['rel_contact'], 0, 1);
      }
    }
    $contactTypeAdded = FALSE;

    $excludedContactIds = array();
    if (isset($form->_contactId)) {
      $excludedContactIds[] = $form->_contactId;
    }

    if (CRM_Utils_Array::value('relationship_type_id', $params)) {
      $relationshipType = new CRM_Contact_DAO_RelationshipType();
      list($rid, $direction) = explode('_', $params['relationship_type_id'], 2);

      $relationshipType->id = $rid;
      if ($relationshipType->find(TRUE)) {
        if ($direction == 'a_b') {
          $type = $relationshipType->contact_type_b;
          $subType = $relationshipType->contact_sub_type_b;
        }
        else {
          $type = $relationshipType->contact_type_a;
          $subType = $relationshipType->contact_sub_type_a;
        }

        $form->set('contact_type', $type);
        $form->set('contact_sub_type', $subType);
        if ($type == 'Individual' || $type == 'Organization' || $type == 'Household') {
          $searchValues[] = array('contact_type', '=', $type, 0, 0);
          $contactTypeAdded = TRUE;
        }

        if ($subType) {
          $searchValues[] = array('contact_sub_type', '=', $subType, 0, 0);
        }
      }
    }

    if (!$contactTypeAdded && CRM_Utils_Array::value('contact_type', $params)) {
      $searchValues[] = array('contact_type', '=', $params['contact_type'], 0, 0);
    }

    // get the count of contact
    $contactBAO  = new CRM_Contact_BAO_Contact();
    $query       = new CRM_Contact_BAO_Query($searchValues);
    $searchCount = $query->searchQuery(0, 0, NULL, TRUE);
    $form->set('searchCount', $searchCount);
    if ($searchCount <= 50) {
      // get the result of the search
      $result = $query->searchQuery(0, 50, NULL);

      $config = CRM_Core_Config::singleton();
      $searchRows = array();

      //variable is set if only one record is foun and that record already has relationship with the contact
      $duplicateRelationship = 0;

      while ($result->fetch()) {
        $contactID = $result->contact_id;
        if (in_array($contactID, $excludedContactIds)) {
          $duplicateRelationship++;
          continue;
        }

        $duplicateRelationship = 0;

        $searchRows[$contactID]['id'] = $contactID;
        $searchRows[$contactID]['name'] = $result->sort_name;
        $searchRows[$contactID]['city'] = $result->city;
        $searchRows[$contactID]['state'] = $result->state_province;
        $searchRows[$contactID]['email'] = $result->email;
        $searchRows[$contactID]['phone'] = $result->phone;

        $contact_type = '<img src="' . $config->resourceBase . 'i/contact_';

        require_once ('CRM/Contact/BAO/Contact/Utils.php');
        $searchRows[$contactID]['type'] = CRM_Contact_BAO_Contact_Utils::getImage($result->contact_sub_type ?
          $result->contact_sub_type : $result->contact_type
        );
      }

      $form->set('searchRows', $searchRows);
      $form->set('duplicateRelationship', $duplicateRelationship);
    }
    else {
      // resetting the session variables if many records are found
      $form->set('searchRows', NULL);
      $form->set('duplicateRelationship', NULL);
    }
  }
}

