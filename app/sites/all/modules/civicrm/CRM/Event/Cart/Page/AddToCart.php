<?php
require_once 'CRM/Core/Page.php';
class CRM_Event_Cart_Page_AddToCart extends CRM_Core_Page {
  function run() {
    $this->_id = CRM_Utils_Request::retrieve('id', 'Positive', $this, TRUE);
    if (!CRM_Core_Permission::check('register for events')) {
      CRM_Core_Error::fatal(ts('You do not have permission to register for this event'));
    }
    require_once 'CRM/Event/BAO/Event.php';
    if (!CRM_Core_Permission::event(CRM_Core_Permission::VIEW, $this->_id)) {
      CRM_Core_Error::fatal(ts('You cannot register for an event you do not have permission to view'));
    }

    require_once 'CRM/Event/Cart/BAO/Cart.php';
    $cart = CRM_Event_Cart_BAO_Cart::find_or_create_for_current_session();
    $event_in_cart = $cart->add_event($this->_id);

    drupal_set_message(ts("<b>%1</b> has been added to your cart. <a href='/civicrm/event/view_cart'>View your cart.</a>", array(1 => $event_in_cart->event->title)));

    return CRM_Utils_System::redirect($_SERVER['HTTP_REFERER']);
  }
}



