<?php
require_once 'CRM/Core/Form.php';
require_once 'CRM/Event/Cart/BAO/Cart.php';
class CRM_Event_Cart_Form_Checkout_Payment extends CRM_Event_Cart_Form_Cart {
  public $contribution_type_id;
  public $description;
  public $line_items;
  public $_fields = array();
  public $_paymentProcessor;
  public $total;
  public $sub_total;
  public $payment_required = TRUE;
  public $payer_contact_id;
  public $is_pay_later = FALSE;
  public $pay_later_receipt; function registerParticipant($params, &$participant, $event) {
    require_once 'CRM/Core/Transaction.php';
    $transaction = new CRM_Core_Transaction();

    // handle register date CRM-4320
    $registerDate = date('YmdHis');
    $participantParams = array(
      'id' => $participant->id,
      'event_id' => $event->id,
      'register_date' => $registerDate,
      'source' => CRM_Utils_Array::value('participant_source', $params, $this->description),
      //'fee_level'     => $participant->fee_level,
      'is_pay_later' => CRM_Utils_Array::value('is_pay_later', $params, 0),
      'fee_amount' => CRM_Utils_Array::value('amount', $params, 0),
      //XXX why is this a ref to participant and not contact?:
      //'registered_by_id' => $this->payer_contact_id,
      'fee_currency' => CRM_Utils_Array::value('currencyID', $params),
    );

    $participant_statuses = CRM_Event_PseudoConstant::participantStatus();
    if ($participant->must_wait) {
      $participantParams['status_id'] = array_search('On waitlist', $participant_statuses);
      //XXX magic:
      $this->assign('isOnWaitlist', TRUE);
    }
    elseif (CRM_Utils_Array::value('is_pay_later', $params)) {
      $participantParams['status_id'] = array_search('Pending from pay later', $participant_statuses);
    }
    else {
      $participantParams['status_id'] = array_search('Registered', $participant_statuses);
    }

    if ($this->_action & CRM_Core_Action::PREVIEW || CRM_Utils_Array::value('mode', $params) == 'test') {
      $participantParams['is_test'] = 1;
    }
    else {
      $participantParams['is_test'] = 0;
    }

    if (self::is_administrator()) {
      if (CRM_Utils_Array::value('note', $params)) {
        $note_params = array(
          'participant_id' => $participant->id,
          'contact_id' => self::getContactID(),
          'note' => $params['note'],
        );
        CRM_Event_BAO_Participant::update_note($note_params);
      }
    }

    $participant->copyValues($participantParams);
    $participant->save();

    if (CRM_Utils_Array::value('contributionID', $params)) {
      require_once 'CRM/Event/BAO/ParticipantPayment.php';
      $payment_params = array(
        'participant_id' => $participant->id,
        'contribution_id' => $params['contributionID'],
      );
      $ids = array();
      $paymentParticpant = CRM_Event_BAO_ParticipantPayment::create($payment_params, $ids);
    }

    $defaults            = array();
    $ids                 = array('contact_id' => $participant->contact_id);
    $contact             = CRM_Contact_BAO_Contact::retrieve($ids, $defaults);
    $contact->is_deleted = 0;
    $contact->save();

    $transaction->commit();

    $event_values = array();
    CRM_Core_DAO::storeValues($event, $event_values);
    require_once 'CRM/Event/Cart/Form/MerParticipant.php';
    list($pre_id, $post_id) = CRM_Event_Cart_Form_MerParticipant::get_profile_groups($participant->event_id);
    $payer_values = array(
      'email' => '',
      'name' => '',
    );
    if ($this->payer_contact_id) {
      $payer_contact_details = CRM_Contact_BAO_Contact::getContactDetails($this->payer_contact_id);
      $payer_values = array(
        'email' => $payer_contact_details[1],
        'name' => $payer_contact_details[0],
      );
    }
    $values = array(
      'params' => $participantParams,
      'event' => $event_values,
      'custom_pre_id' => $pre_id,
      'custom_post_id' => $post_id,
      'payer' => $payer_values,
    );
    CRM_Event_BAO_Event::sendMail($participant->contact_id, $values, $participant->id);

    return $participant;
  }

  function buildPaymentFields() {
    $payment_processor_id = NULL;
    $can_pay_later = TRUE;
    $pay_later_text = ts('Bill me');
    $this->pay_later_receipt = ts("This is an invoice. Payment due before registration is finalized.");
    foreach ($this->cart->get_main_events_in_carts() as $event_in_cart) {
      if ($payment_processor_id == NULL && $event_in_cart->event->payment_processor_id != NULL) {
        $payment_processor_id = $event_in_cart->event->payment_processor_id;
        $this->contribution_type_id = $event_in_cart->event->contribution_type_id;
      }
      else {
        if ($event_in_cart->event->payment_processor_id != NULL && $event_in_cart->event->payment_processor_id != $payment_processor_id) {
          CRM_Core_Error::statusBounce(ts('When registering for multiple events all events must use the same payment processor. '));
        }
      }
      if (!$event_in_cart->event->is_pay_later) {
        $can_pay_later = FALSE;
      }
      else {
        //XXX
        $pay_later_text = $event_in_cart->event->pay_later_text;
        $this->pay_later_receipt = $event_in_cart->event->pay_later_receipt;
      }
    }

    if ($payment_processor_id == NULL) {
      CRM_Core_Error::statusBounce(ts('A payment processor must be selected for this event registration page, or the event must be configured to give users the option to pay later (contact the site administrator for assistance).'));
    }

    require_once 'CRM/Core/BAO/PaymentProcessor.php';
    $this->_paymentProcessor = CRM_Core_BAO_PaymentProcessor::getPayment($payment_processor_id, $this->_mode);
    $this->assign('paymentProcessor', $this->_paymentProcessor);

    require_once 'CRM/Core/Payment/Form.php';
    CRM_Core_Payment_Form::setCreditCardFields($this);
    CRM_Core_Payment_Form::buildCreditCard($this);

    if ($can_pay_later) {
      $element = $this->addElement('checkbox', 'is_pay_later',
        $pay_later_text
      );
    }
  }

  function buildQuickForm() {
    require_once 'CRM/Core/BAO/CustomValueTable.php';

    $this->line_items    = array();
    $this->sub_total     = 0;
    $this->_price_values = $this->getValuesForPage('ParticipantsAndPrices');

    // iterate over each event in cart
    foreach ($this->cart->get_main_events_in_carts() as $event_in_cart) {
      $this->process_event_line_item($event_in_cart);
      foreach ($this->cart->get_events_in_carts_by_main_event_id($event_in_cart->event_id) as $subevent) {
        $this->process_event_line_item($subevent, 'subevent');
      }
    }

    $this->total = $this->sub_total;
    $this->payment_required = ($this->total > 0);
    $this->assign('payment_required', $this->payment_required);
    $this->assign('line_items', $this->line_items);
    $this->assign('sub_total', $this->sub_total);
    $this->assign('total', $this->total);
    $buttons = array();
    $buttons[] = array(
      'name' => ts('<< Go Back'),
      'spacing' => '&nbsp;&nbsp;&nbsp;&nbsp',
      'type' => 'back',
    );
    $buttons[] = array(
      'isDefault' => TRUE,
      'name' => ts('Complete Transaction >>'),
      'spacing' => '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;',
      'type' => 'next',
    );

    if ($this->total > 0) {
      $this->add('text', 'billing_contact_email', 'Billing Email', '', TRUE);
    }
    if (self::is_administrator()) {
      $this->add('textarea', 'note', 'Note');
      $this->add('text', 'source', 'Source', array('size' => 80));
      $instruments = array();
      CRM_Core_OptionGroup::getAssoc('payment_instrument', $instruments, TRUE);
      $options = array();
      foreach ($instruments as $type) {
        $options[] = $this->createElement('radio', NULL, '', $type['label'], $type['value']);
      }
      $this->addGroup($options, 'payment_type', ts("Alternative Payment Type"));
      $this->add('text', 'check_number', ts('Check No.'), array('size' => 20));
      $this->addElement('checkbox', 'is_pending', ts('Create a pending registration'));

      $this->assign('administrator', TRUE);
    }
    $this->addButtons($buttons);

    $this->addFormRule(array('CRM_Event_Cart_Form_Checkout_Payment', 'formRule'), $this);

    if ($this->payment_required) {
      $this->buildPaymentFields();
    }
  }

  function process_event_line_item(&$event_in_cart, $class = NULL) {
    $cost         = 0;
    $price_set_id = CRM_Price_BAO_Set::getFor("civicrm_event", $event_in_cart->event_id);
    $amount_level = NULL;
    if ($price_set_id === FALSE) {
      CRM_Core_OptionGroup::getAssoc("civicrm_event.amount.{$event_in_cart->event_id}", $fee_data, TRUE);
      $price_set_amount = CRM_Utils_Array::value("event_{$event_in_cart->event_id}_amount", $this->_price_values);
      $fee_level = CRM_Utils_Array::value($price_set_amount, $fee_data);
      if ($fee_level) {
        $cost = $fee_data[$price_set_amount]['value'];
      }
    }
    else {
      $event_price_values = array();
      foreach ($this->_price_values as $key => $value) {
        if (preg_match("/event_{$event_in_cart->event_id}_(price.*)/", $key, $matches)) {
          $event_price_values[$matches[1]] = $value;
        }
      }
      $price_sets       = CRM_Price_BAO_Set::getSetDetail($price_set_id, TRUE);
      $price_set        = $price_sets[$price_set_id];
      $price_set_amount = array();
      CRM_Price_BAO_Set::processAmount($price_set['fields'], $event_price_values, $price_set_amount);
      $price_set_index = end(array_keys($event_price_values['amount_priceset_level_radio']));
      $cost            = $event_price_values['amount'];
      $amount_level    = $event_price_values['amount_level'];
    }

    // iterate over each participant in event
    foreach ($event_in_cart->participants as & $participant) {
      $participant->cost = $cost;
      $participant->fee_level = $amount_level;
    }

    $this->add_line_item($event_in_cart, $class);
  }

  function add_line_item($event_in_cart, $class = NULL) {
    $amount = 0;
    $cost = 0;
    $not_waiting_participants = array();
    foreach ($event_in_cart->not_waiting_participants() as $participant) {
      $amount += $participant->cost;
      $cost = max($cost, $participant->cost);
      $not_waiting_participants[] = array(
        'display_name' => CRM_Contact_BAO_Contact::displayName($participant->contact_id),
      );
    }
    $waiting_participants = array();
    foreach ($event_in_cart->waiting_participants() as $participant) {
      $waiting_participants[] = array(
        'display_name' => CRM_Contact_BAO_Contact::displayName($participant->contact_id),
      );
    }
    $this->line_items[] = array(
      'amount' => $amount,
      'cost' => $cost,
      'event' => $event_in_cart->event,
      'participants' => $not_waiting_participants,
      'num_participants' => count($not_waiting_participants),
      'num_waiting_participants' => count($waiting_participants),
      'waiting_participants' => $waiting_participants,
      'class' => $class,
    );

    $this->sub_total += $amount;
  }

  function getDefaultFrom() {
    require_once 'CRM/Core/OptionGroup.php';
    $values = CRM_Core_OptionGroup::values('from_email_address');
    return $values[1];
  }

  function emailReceipt($events_in_cart, $params) {
    require_once 'CRM/Contact/BAO/Contact.php';
    $contact_details    = CRM_Contact_BAO_Contact::getContactDetails($this->payer_contact_id);
    $state_province     = new CRM_Core_DAO_StateProvince();
    $state_province->id = $params["billing_state_province_id-{$this->_bltID}"];
    $state_province->find();
    $state_province->fetch();
    $country = new CRM_Core_DAO_Country();
    $country->id = $params["billing_country_id-{$this->_bltID}"];
    $country->find();
    $country->fetch();
    foreach ($this->line_items as & $line_item) {
      $location_params = array('entity_id' => $line_item['event']->id, 'entity_table' => 'civicrm_event');
      $line_item['location'] = CRM_Core_BAO_Location::getValues($location_params, TRUE);
    }
    $send_template_params = array(
      'table' => 'civicrm_msg_template',
      'contactId' => $this->payer_contact_id,
      'from' => $this->getDefaultFrom(),
      'groupName' => 'msg_tpl_workflow_event',
      'isTest' => FALSE,
      'toEmail' => $contact_details[1],
      'toName' => $contact_details[0],
      'tplParams' => array
      (
        'billing_name' => "{$params['billing_first_name']} {$params['billing_last_name']}",
        'billing_city' => $params["billing_city-{$this->_bltID}"],
        'billing_country' => $country->name,
        'billing_postal_code' => $params["billing_postal_code-{$this->_bltID}"],
        'billing_state' => $state_province->abbreviation,
        'billing_street_address' => $params["billing_street_address-{$this->_bltID}"],
        'credit_card_exp_date' => $params['credit_card_exp_date'],
        'credit_card_type' => $params['credit_card_type'],
        'credit_card_number' => "************" . substr($params['credit_card_number'], -4, 4),
        // XXX cart->get_discounts
        'discounts' => $this->discounts,
        'email' => $contact_details[1],
        'events_in_cart' => $events_in_cart,
        'line_items' => $this->line_items,
        'name' => $contact_details[0],
        'transaction_id' => $params['trxn_id'],
        'transaction_date' => $params['trxn_date'],
        'is_pay_later' => $this->is_pay_later,
        'pay_later_receipt' => $this->pay_later_receipt,
      ),
      'valueName' => 'event_registration_receipt',
    );
    $template_params_to_copy = array(
      'billing_name',
      'billing_city',
      'billing_country',
      'billing_postal_code',
      'billing_state',
      'billing_street_address',
      'credit_card_exp_date',
      'credit_card_type',
      'credit_card_number',
    );
    foreach ($template_params_to_copy as $template_param_to_copy) {
      $this->set($template_param_to_copy, $send_template_params['tplParams'][$template_param_to_copy]);
    }

    require_once 'CRM/Core/BAO/MessageTemplates.php';
    CRM_Core_BAO_MessageTemplates::sendTemplate($send_template_params);
  }

  static
  function formRule($fields, $files, $self) {
    $errors = array();

    if ($self->payment_required) {
      require_once 'CRM/Core/BAO/PaymentProcessor.php';
      require_once 'CRM/Core/Payment/Form.php';
      require_once 'CRM/Core/Payment.php';
      $payment = &CRM_Core_Payment::singleton($self->_mode, $self->_paymentProcessor, $this);
      $error = $payment->checkConfig($self->_mode);
      if ($error) {
        $errors['_qf_default'] = $error;
      }

      foreach ($self->_fields as $name => $field) {
        if ($field['is_required'] && CRM_Utils_System::isNull(CRM_Utils_Array::value($name, $fields))) {
          $errors[$name] = ts('%1 is a required field.', array(1 => $field['title']));
        }
      }

      require_once 'CRM/Utils/Rule.php';

      if (CRM_Utils_Array::value('credit_card_type', $fields)) {
        if (CRM_Utils_Array::value('credit_card_number', $fields) &&
          !CRM_Utils_Rule::creditCardNumber($fields['credit_card_number'], $fields['credit_card_type'])
        ) {
          $errors['credit_card_number'] = ts("Please enter a valid Credit Card Number");
        }

        if (CRM_Utils_Array::value('cvv2', $fields) &&
          !CRM_Utils_Rule::cvv($fields['cvv2'], $fields['credit_card_type'])
        ) {
          $errors['cvv2'] = ts("Please enter a valid Credit Card Verification Number");
        }
      }
    }

    return empty($errors) ? TRUE : $errors;
  }

  function validate() {
    if ($this->is_pay_later) {
      $this->_fields['credit_card_number']['is_required'] = FALSE;
      $this->_fields['cvv2']['is_required'] = FALSE;
      $this->_fields['credit_card_exp_date']['is_required'] = FALSE;
      $this->_fields['credit_card_type']['is_required'] = FALSE;
    }
    return parent::validate();
  }

  function preProcess() {
    $params = $this->_submitValues;
    $this->is_pay_later = CRM_Utils_Array::value('is_pay_later', $params, FALSE);

    parent::preProcess();
  }

  function postProcess() {
    require_once 'CRM/Contact/BAO/Contact.php';
    require_once 'CRM/Contribute/BAO/Contribution.php';
    require_once 'CRM/Contribute/PseudoConstant.php';
    require_once 'CRM/Core/BAO/CustomValueTable.php';
    require_once 'CRM/Core/Config.php';
    require_once 'CRM/Core/Transaction.php';
    require_once 'CRM/Core/BAO/FinancialTrxn.php';
    require_once 'CRM/Event/PseudoConstant.php';
    require_once 'CRM/Utils/Rule.php';

    $transaction = new CRM_Core_Transaction();
    $trxn        = NULL;
    $params      = $this->_submitValues;

    if (array_key_exists('billing_contact_email', $params)) {
      $this->payer_contact_id = self::find_or_create_contact(array(
          'email' => $params['billing_contact_email'],
          'first_name' => $params['billing_first_name'],
          'last_name' => $params['billing_last_name'],
          'is_deleted' => FALSE,
        ));

      $ctype = CRM_Core_DAO::getFieldValue('CRM_Contact_DAO_Contact',
        $this->payer_contact_id,
        'contact_type'
      );
      $billing_fields = array(
        "billing_first_name" => 1,
        "billing_middle_name" => 1,
        "billing_last_name" => 1,
        "billing_street_address-{$this->_bltID}" => 1,
        "billing_city-{$this->_bltID}" => 1,
        "billing_state_province_id-{$this->_bltID}" => 1,
        "billing_postal_code-{$this->_bltID}" => 1,
        "billing_country_id-{$this->_bltID}" => 1,
        "address_name-{$this->_bltID}" => 1,
        "email-{$this->_bltID}" => 1,
      );

      $params["email-{$this->_bltID}"] = $params['billing_contact_email'];
      CRM_Contact_BAO_Contact::createProfileContact(
        $params,
        $billing_fields,
        $this->payer_contact_id,
        NULL,
        NULL,
        $ctype,
        TRUE
      );
    }

    $params['now'] = date('YmdHis');
    $params['invoiceID'] = md5(uniqid(rand(), TRUE));
    $params['amount'] = $this->total;
    $params['contribution_type_id'] = $this->contribution_type_id;
    if ($this->payment_required && !$this->is_pay_later) {
      $trxn = $this->make_payment($params);
      $params['trxn_id'] = $trxn->trxn_id;
      $params['trxn_date'] = $trxn->trxn_date;
      $params['currencyID'] = $trxn->currency;
      $params['financial_trxn_id'] = $trxn->id;
    }
    if ($trxn == NULL) {
      $params['trxn_id'] = strftime("VR%Y%m%d%H%M%S");
      $params['trxn_date'] = $params['now'];
    }
    $this->cart->completed = TRUE;
    $this->cart->save();
    $this->set('last_event_cart_id', $this->cart->id);

    if ($this->payment_required) {
      $this->emailReceipt($this->cart->events_in_carts, $params);
    }
    $contribution_statuses = CRM_Contribute_PseudoConstant::contributionStatus(NULL, 'name');
    if ($this->is_pay_later) {
      $params['contribution_status_id'] = array_search('Pending', $contribution_statuses);
      $params['payment_instrument_id'] = NULL;
    }
    else {
      $params['contribution_status_id'] = array_search('Completed', $contribution_statuses);
      $params['payment_instrument_id'] = CRM_Core_OptionGroup::getValue('payment_instrument', 'Credit Card', 'name');
    }
    // n.b. we need to process the subparticipants before main event
    // participants so that session attendance can be included in the email
    $main_participants = $this->cart->get_main_event_participants();
    $all_participants = array();
    foreach ($main_participants as $main_participant) {
      $all_participants = array_merge($all_participants, $this->cart->get_subparticipants($main_participant));
    }
    $all_participants = array_merge($all_participants, $main_participants);

    $this->sub_trxn_index = 0;
    foreach ($all_participants as $mer_participant) {
      $event_in_cart = $this->cart->get_event_in_cart_by_event_id($mer_participant->event_id);

      $this->sub_trxn_index += 1;

      if ($mer_participant->must_wait) {
        $this->registerParticipant($params, $mer_participant, $event_in_cart->event);
      }
      else {
        // XXX move
        $params['amount'] = $mer_participant->cost - $mer_participant->discount_amount;

        if ($event_in_cart->event->contribution_type_id) {
          $params['contribution_type_id'] = $event_in_cart->event->contribution_type_id;
          $this->record_contribution($params, $mer_participant);
        }
        $this->registerParticipant($params, $mer_participant, $event_in_cart->event);
      }
    }
    $this->saveDataToSession($params['trxn_id']);
    $transaction->commit();
  }

  function make_payment(&$params) {
    $payment = &CRM_Core_Payment::singleton($this->_mode, $this->_paymentProcessor, $this);
    CRM_Core_Payment_Form::mapParams("", $params, $params, TRUE);
    $params['month'] = $params['credit_card_exp_date']['M'];
    $params['year']  = $params['credit_card_exp_date']['Y'];
    $result          = &$payment->doDirectPayment($params);
    if (is_a($result, 'CRM_Core_Error')) {
      CRM_Core_Error::displaySessionError($result);
      CRM_Utils_System::redirect(CRM_Utils_System::url('civicrm/event/cart_checkout', "_qf_Payment_display=1&qfKey={$this->controller->_key}", TRUE, NULL, FALSE));
      return;
    }
    $trxnParams = array(
      'trxn_date' => $params['now'],
      'trxn_type' => 'Debit',
      'total_amount' => $params['amount'],
      'fee_amount' => CRM_Utils_Array::value('fee_amount', $result),
      'net_amount' => CRM_Utils_Array::value('net_amount', $result, $params['amount']),
      'currency' => CRM_Utils_Array::value('currencyID', $params),
      'payment_processor' => $this->_paymentProcessor['payment_processor_type'],
      'trxn_id' => $result['trxn_id'],
    );
    $trxn = new CRM_Core_DAO_FinancialTrxn();
    $trxn->copyValues($trxnParams);
    if (!CRM_Utils_Rule::currencyCode($trxn->currency)) {
      $config = CRM_Core_Config::singleton();
      $trxn->currency = $config->defaultCurrency;
    }
    $trxn->save();

    return $trxn;
  }

  function record_contribution(&$params, $mer_participant) {
    if (self::is_administrator() && CRM_Utils_Array::value('payment_type', $params)) {
      $params['payment_instrument_id'] = $params['payment_type'];
    }

    if ($this->payer_contact_id) {
      $payer = $this->payer_contact_id;
    }
    elseif (self::getContactID()) {
      $payer = self::getContactID();
    }
    else {
      $payer = $mer_participant->contact_id;
    }

    $contribParams = array(
      'contact_id' => $payer,
      'contribution_type_id' => $params['contribution_type_id'],
      'receive_date' => $params['now'],
      'total_amount' => $params['amount'],
      'amount_level' => $mer_participant->fee_level,
      'fee_amount' => $mer_participant->cost,
      'net_amount' => $params['amount'],
      'invoice_id' => "{$params['invoiceID']}-{$this->sub_trxn_index}",
      'trxn_id' => "{$params['trxn_id']}-{$this->sub_trxn_index}",
      'currency' => CRM_Utils_Array::value('currencyID', $params),
      'source' => $this->description,
      'is_pay_later' => CRM_Utils_Array::value('is_pay_later', $params, 0),
      'contribution_status_id' => $params['contribution_status_id'],
      'payment_instrument_id' => $params['payment_instrument_id'],
      'check_number' => CRM_Utils_Array::value('check_number', $params),
    );

    $contribution = &CRM_Contribute_BAO_Contribution::add($contribParams, $ids);
    if (is_a($contribution, 'CRM_Core_Error')) {
      CRM_Core_Error::fatal(ts("There was an error creating a contribution record for your event. Please report this error to the webmaster. Details: %1\n", array(1 => $contribution->getMessages($contribution))));
    }
    $params['contributionID'] = $contribution->id;
    $params['receive_date'] = $contribution->receive_date;
    if (CRM_Utils_Array::value('financial_trxn_id', $params)) {
      $entity_financial_trxn_params = array(
        'entity_table' => "civicrm_contribution",
        'entity_id' => $contribution->id,
        'financial_trxn_id' => $params['financial_trxn_id'],
        'amount' => $params['amount'],
        'currency' => CRM_Utils_Array::value('currencyID', $params),
      );
      $entity_trxn = &new CRM_Core_DAO_EntityFinancialTrxn();
      $entity_trxn->copyValues($entity_financial_trxn_params);
      $entity_trxn->save();
    }
  }

  function saveDataToSession($trxn_id) {
    $session_line_items = array();
    foreach ($this->line_items as $line_item) {
      $session_line_item = array();
      $session_line_item['amount'] = $line_item['amount'];
      $session_line_item['cost'] = $line_item['cost'];
      $session_line_item['event_id'] = $line_item['event']->id;
      $session_line_items[] = $session_line_item;
    }
    $this->set('line_items', $session_line_items);
    $this->set('payment_required', $this->payment_required);
    $this->set('is_pay_later', $this->is_pay_later);
    $this->set('pay_later_receipt', $this->pay_later_receipt);
    $this->set('trxn_id', $trxn_id);
    $this->set('total', $this->total);
  }

  function setDefaultValues() {
    require_once 'CRM/Core/Config.php';
    require_once 'CRM/Contact/BAO/Contact.php';
    require_once 'CRM/Event/Cart/BAO/MerParticipant.php';

    $defaults = parent::setDefaultValues();

    $config = CRM_Core_Config::singleton();
    $default_country = new CRM_Core_DAO_Country();
    $default_country->iso_code = $config->defaultContactCountry();
    $default_country->find(TRUE);
    $defaults["billing_country_id-{$this->_bltID}"] = $default_country->id;

    if (self::getContactID() && !self::is_administrator()) {
      $params = array('id' => self::getContactID());
      $contact = CRM_Contact_BAO_Contact::retrieve($params, $defaults);

      foreach ($contact->email as $email) {
        if ($email['is_billing']) {
          $defaults["billing_contact_email"] = $email['email'];
        }
      }
      if (!CRM_Utils_Array::value('billing_contact_email', $defaults)) {
        foreach ($contact->email as $email) {
          if ($email['is_primary']) {
            $defaults["billing_contact_email"] = $email['email'];
          }
        }
      }

      $defaults["billing_first_name"] = $contact->first_name;
      $defaults["billing_middle_name"] = $contact->middle_name;
      $defaults["billing_last_name"] = $contact->last_name;

      $billing_address = CRM_Event_Cart_BAO_MerParticipant::billing_address_from_contact($contact);

      if ($billing_address != NULL) {
        $defaults["billing_street_address-{$this->_bltID}"] = $billing_address['street_address'];
        $defaults["billing_city-{$this->_bltID}"] = $billing_address['city'];
        $defaults["billing_postal_code-{$this->_bltID}"] = $billing_address['postal_code'];
        $defaults["billing_state_province_id-{$this->_bltID}"] = $billing_address['state_province_id'];
        $defaults["billing_country_id-{$this->_bltID}"] = $billing_address['country_id'];
      }
    }

    $defaults["source"] = $this->description;

    return $defaults;
  }
}

