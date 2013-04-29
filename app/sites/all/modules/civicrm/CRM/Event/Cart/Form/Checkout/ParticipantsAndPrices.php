<?php
require_once 'CRM/Core/Form.php';
require_once 'CRM/Core/OptionGroup.php';
require_once 'CRM/Event/Cart/BAO/Cart.php';
require_once 'CRM/Event/Cart/Form/Cart.php';
require_once 'CRM/Price/BAO/Set.php';
class CRM_Event_Cart_Form_Checkout_ParticipantsAndPrices extends CRM_Event_Cart_Form_Cart {
  public $price_fields_for_event; function preProcess() {
    parent::preProcess();

    $this->cid = CRM_Utils_Request::retrieve('cid', 'Positive', $this);
    if (!isset($this->cid) || $this->cid > 0) {
      //TODO users with permission can default to another contact
      $this->cid = self::getContactID();
    }
  }

  function buildQuickForm() {
    $this->price_fields_for_event = array();
    require_once ('CRM/Event/Cart/Form/MerParticipant.php');
    foreach ($this->cart->get_main_event_participants() as $participant) {
      $form = new CRM_Event_Cart_Form_MerParticipant($participant);
      $form->buildQuickForm($this);
    }
    foreach ($this->cart->get_main_events_in_carts() as $event_in_cart) {
      $this->price_fields_for_event[$event_in_cart->event_id] = $this->build_price_options($event_in_cart->event);
    }
    // XXX
    $this->addElement('text', 'discountcode', ts('If you have a discount code, enter it here'));
    $this->assign('events_in_carts', $this->cart->get_main_events_in_carts());
    $this->assign('price_fields_for_event', $this->price_fields_for_event);
    $this->addButtons(
      array(
        array(
          'type' => 'upload',
          'name' => ts('Continue >>'),
          'spacing' => '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;',
          'isDefault' => TRUE,
        ),
      )
    );

    if ($this->cid) {
      $params         = array('id' => $this->cid);
      $contact        = CRM_Contact_BAO_Contact::retrieve($params, $defaults);
      $contact_values = array();
      CRM_Core_DAO::storeValues($contact, $contact_values);
      $this->assign('contact', $contact_values);
    }
  }

  static
  function primary_email_from_contact($contact) {
    foreach ($contact->email as $email) {
      if ($email['is_primary']) {
        return $email['email'];
      }
    }

    return NULL;
  }

  function build_price_options($event) {
    $price_fields_for_event = array();
    $base_field_name = "event_{$event->id}_amount";
    $price_set_id = CRM_Price_BAO_Set::getFor('civicrm_event', $event->id);
    if ($price_set_id === FALSE && $event->is_monetary) {
      require_once 'CRM/Utils/Money.php';
      //$fee_data = array();
      CRM_Core_OptionGroup::getAssoc("civicrm_event.amount.{$event->id}", $fee_data, TRUE);
      $choices = array();
      foreach ($fee_data as $fee) {
        if (is_array($fee)) {
          $choices[] = $this->createElement('radio', NULL, '', CRM_Utils_Money::format($fee['value']) . ' ' . $fee['label'], $fee['amount_id']);
        }
      }
      $this->addGroup($choices, $base_field_name, $event->fee_label);
      $this->addRule($base_field_name, ts('Select at least one option from Price Levels'), 'required');
      $price_fields_for_event[] = $base_field_name;
    }
    elseif ($price_set_id) {
      $price_sets = CRM_Price_BAO_Set::getSetDetail($price_set_id, TRUE, TRUE);
      $price_set  = $price_sets[$price_set_id];
      $index      = -1;
      foreach ($price_set['fields'] as $field) {
        $index++;
        $field_name = "event_{$event->id}_price_{$field['id']}";
        CRM_Price_BAO_Field::addQuickFormElement($this, $field_name, $field['id'], FALSE);
        $price_fields_for_event[] = $field_name;
      }
    }
    return $price_fields_for_event;
  }

  function validate() {
    parent::validate();
    if ($this->_errors) {
      return FALSE;
    }
    $this->cart->load_associations();
    $fields = $this->_submitValues;

    foreach ($this->cart->get_main_events_in_carts() as $event_in_cart) {
      $price_set_id = CRM_Event_BAO_Event::usesPriceSet($event_in_cart->event_id);
      if ($price_set_id) {
        $priceField = new CRM_Price_DAO_Field();
        $priceField->price_set_id = $price_set_id;
        $priceField->find();

        $check = array();

        while ($priceField->fetch()) {
          if (!empty($fields["event_{$event_in_cart->event_id}_price_{$priceField->id}"])) {
            $check[] = $priceField->id;
          }
        }

        //XXX
        if (empty($check)) {
          $this->_errors['_qf_default'] = ts("Select at least one option from Price Levels.");
        }

        $lineItem = array();
        if (is_array($this->_values['fee']['fields'])) {
          CRM_Price_BAO_Set::processAmount($this->_values['fee']['fields'], $fields, $lineItem);
          //XXX total...
          if ($fields['amount'] < 0) {
            $this->_errors['_qf_default'] = ts("Price Levels can not be less than zero. Please select the options accordingly");
          }
        }
      }

      foreach ($event_in_cart->participants as $mer_participant) {
        $participant_fields = $fields['event'][$event_in_cart->event_id]['participant'][$mer_participant->id];
        $contact_id = self::find_or_create_contact($participant_fields);

        require_once ('CRM/Event/BAO/Participant.php');
        $participant = new CRM_Event_BAO_Participant();
        $participant->event_id = $event_in_cart->event_id;
        $participant->contact_id = $contact_id;
        $statusTypes = CRM_Event_PseudoConstant::participantStatus(NULL, 'is_counted = 1');
        $participant->find();
        while ($participant->fetch()) {
          if (array_key_exists($participant->status_id, $statusTypes)) {
            $form = $mer_participant->get_form();
            $this->_errors[$form->html_field_name('email')] = ts("The participant %1 is already registered for %2 (%3).", array(1 => $participant_fields['email'], 2 => $event_in_cart->event->title, 3 => $event_in_cart->event->start_date));
          }
        }
      }
    }
    return empty($this->_errors);
  }

  public function setDefaultValues() {
    $this->loadCart();

    $defaults = array();
    require_once 'CRM/Event/Cart/Form/MerParticipant.php';
    foreach ($this->cart->get_main_event_participants() as $participant) {
      $form = $participant->get_form();
      if (empty($participant->email)
        && !CRM_Event_Cart_Form_Cart::is_administrator()
        && ($participant->get_participant_index() == 1)
        && ($this->cid != 0)
      ) {
        require_once 'CRM/Contact/BAO/Contact.php';
        $defaults = array();
        $params = array('id' => $this->cid);
        $contact = CRM_Contact_BAO_Contact::retrieve($params, $defaults);
        $participant->contact_id = $this->cid;
        $participant->save();
        $participant->email = self::primary_email_from_contact($contact);
      }
      elseif ($this->cid == 0
        && $participant->contact_id == self::getContactID()
      ) {
        $participant->email = NULL;
        $participant->contact_id = self::find_or_create_contact(array());
      }
      $defaults += $form->setDefaultValues();
    }
    return $defaults;
  }

  function postProcess() {
    if (!array_key_exists('event', $this->_submitValues)) {
      return;
    }
    foreach ($this->_submitValues['event'] as $event_id => $participants) {
      foreach ($participants['participant'] as $participant_id => $fields) {
        require_once 'CRM/Contact/BAO/Contact.php';
        $contact_id = self::find_or_create_contact($fields);

        $participant = $this->cart->get_event_in_cart_by_event_id($event_id)->get_participant_by_id($participant_id);
        if ($participant->contact_id && $contact_id != $participant->contact_id) {
          foreach ($this->cart->get_subparticipants($participant) as $subparticipant) {
            $subparticipant->contact_id = $contact_id;
            $subparticipant->save();
          }
        }

        //TODO security check that participant ids are already in this cart
        $participant_params = array(
          'id' => $participant_id,
          'cart_id' => $this->cart->id,
          'event_id' => $event_id,
          'contact_id' => $contact_id,
          //'registered_by_id' => $this->cart->user_id,
          'email' => $fields['email'],
        );
        $participant = new CRM_Event_Cart_BAO_MerParticipant($participant_params);
        $participant->save();
        $this->cart->add_participant_to_cart($participant);

        if (array_key_exists('field', $this->_submitValues) && array_key_exists($participant_id, $this->_submitValues['field'])) {
          $custom_fields = array_merge($participant->get_form()->get_participant_custom_data_fields());

          CRM_Contact_BAO_Contact::createProfileContact($this->_submitValues['field'][$participant_id], $custom_fields, $contact_id);
        }
      }
    }
    $this->cart->save();
  }
}

