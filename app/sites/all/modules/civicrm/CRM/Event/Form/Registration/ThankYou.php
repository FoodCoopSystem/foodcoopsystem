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
 *
 * @package CRM
 * @copyright CiviCRM LLC (c) 2004-2011
 * $Id$
 *
 */

require_once 'CRM/Event/Form/Registration.php';

/**
 * This class generates form components for processing Event
 *
 */
class CRM_Event_Form_Registration_ThankYou extends CRM_Event_Form_Registration {

  /**
   * Function to set variables up before form is built
   *
   * @return void
   * @access public
   */
  function preProcess() {
    parent::preProcess();
    $this->_params      = $this->get('params');
    $this->_lineItem    = $this->get('lineItem');
    $this->_part        = $this->get('part');
    $this->_totalAmount = $this->get('totalAmount');
    $this->_receiveDate = $this->get('receiveDate');
    $this->_trxnId      = $this->get('trxnId');
    $finalAmount        = $this->get('finalAmount');
    $this->assign('finalAmount', $finalAmount);
    $participantInfo = $this->get('participantInfo');
    $this->assign('part', $this->_part);
    $this->assign('participantInfo', $participantInfo);
    $customGroup = $this->get('customProfile');
    $this->assign('customProfile', $customGroup);

    $this->assign('addParticipantProfile', $this->get('addParticipantProfile'));
    CRM_Utils_System::setTitle(CRM_Utils_Array::value('thankyou_title', $this->_values['event']));
  }

  /**
   * overwrite action, since we are only showing elements in frozen mode
   * no help display needed
   *
   * @return int
   * @access public
   */
  function getAction() {
    if ($this->_action & CRM_Core_Action::PREVIEW) {
      return CRM_Core_Action::VIEW | CRM_Core_Action::PREVIEW;
    }
    else {
      return CRM_Core_Action::VIEW;
    }
  }

  /**
   * Function to build the form
   *
   * @return None
   * @access public
   */
  public function buildQuickForm() {
    $this->assignToTemplate();

    $this->buildCustom($this->_values['custom_pre_id'], 'customPre', TRUE);
    $this->buildCustom($this->_values['custom_post_id'], 'customPost', TRUE);

    $this->assign('lineItem', $this->_lineItem);
    $this->assign('totalAmount', $this->_totalAmount);
    $hookDiscount = $this->get('hookDiscount');
    if ($hookDiscount) {
      $this->assign('hookDiscount', $hookDiscount);
    }

    $this->assign('receive_date', $this->_receiveDate);
    $this->assign('trxn_id', $this->_trxnId);

    //cosider total amount.
    $this->assign('isAmountzero', ($this->_totalAmount <= 0) ? TRUE : FALSE);

    $this->assign('defaultRole', FALSE);
    if (CRM_Utils_Array::value('defaultRole', $this->_params[0]) == 1) {
      $this->assign('defaultRole', TRUE);
    }
    $defaults = array();
    $fields = array();
    if (!empty($this->_fields)) {
      foreach ($this->_fields as $name => $dontCare) {
        $fields[$name] = 1;
      }
    }
    $fields['state_province'] = $fields['country'] = $fields['email'] = 1;
    require_once 'CRM/Contact/BAO/Contact.php';
    foreach ($fields as $name => $dontCare) {
      if (isset($this->_params[0][$name])) {
        $defaults[$name] = $this->_params[0][$name];
        if (substr($name, 0, 7) == 'custom_') {
          $timeField = "{$name}_time";
          if (isset($this->_params[0][$timeField])) {
            $defaults[$timeField] = $this->_params[0][$timeField];
          }
        }
        elseif (in_array($name, CRM_Contact_BAO_Contact::$_greetingTypes)
          && !empty($this->_params[0][$name . '_custom'])
        ) {
          $defaults[$name . '_custom'] = $this->_params[0][$name . '_custom'];
        }
      }
    }

    $this->_submitValues = array_merge($this->_submitValues, $defaults);

    $this->setDefaults($defaults);

    require_once 'CRM/Friend/BAO/Friend.php';

    $params['entity_id'] = $this->_eventId;
    $params['entity_table'] = 'civicrm_event';

    CRM_Friend_BAO_Friend::retrieve($params, $data);
    if (CRM_Utils_Array::value('is_active', $data)) {
      $friendText = $data['title'];
      $this->assign('friendText', $friendText);
      if ($this->_action & CRM_Core_Action::PREVIEW) {
        $url = CRM_Utils_System::url('civicrm/friend',
          "eid={$this->_eventId}&reset=1&action=preview&pcomponent=event"
        );
      }
      else {
        $url = CRM_Utils_System::url('civicrm/friend',
          "eid={$this->_eventId}&reset=1&pcomponent=event"
        );
      }
      $this->assign('friendURL', $url);
    }

    $this->freeze();

    //lets give meaningful status message, CRM-4320.
    $isOnWaitlist = $isRequireApproval = FALSE;
    if ($this->_allowWaitlist && !$this->_allowConfirmation) {
      $isOnWaitlist = TRUE;
    }
    if ($this->_requireApproval && !$this->_allowConfirmation) {
      $isRequireApproval = TRUE;
    }
    $this->assign('isOnWaitlist', $isOnWaitlist);
    $this->assign('isRequireApproval', $isRequireApproval);

    // find pcp info
    require_once "CRM/PCP/DAO/PCPBlock.php";
    $eventId           = $this->_eventId;
    $dao               = new CRM_PCP_DAO_PCPBlock();
    $dao->entity_table = 'civicrm_event';
    $dao->entity_id    = $eventId;
    $dao->is_active    = 1;
    $dao->find(TRUE);

    if ($dao->id) {
      $this->assign('pcpLink', CRM_Utils_System::url('civicrm/contribute/campaign', 'action=add&reset=1&pageId=' . $eventId . '&component=event'));
      $this->assign('pcpLinkText', $dao->link_text);
    }

    // Assign Participant Count to Lineitem Table
    require_once 'CRM/Price/BAO/Set.php';
    $this->assign('pricesetFieldsCount', CRM_Price_BAO_Set::getPricesetCount($this->_priceSetId));

    // can we blow away the session now to prevent hackery
    $this->controller->reset();
  }

  /**
   * Function to process the form
   *
   * @access public
   *
   * @return None
   */
  public function postProcess() {}
  //end of function

  /**
   * Return a descriptive name for the page, used in wizard header
   *
   * @return string
   * @access public
   */
  public function getTitle() {
    return ts('Thank You Page');
  }
}

