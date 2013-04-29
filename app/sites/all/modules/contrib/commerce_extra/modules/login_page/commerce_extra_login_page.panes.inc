<?php
/**
 * @file
 *   Contains all pane method callbacks.
 */

/**
 * Commerce checkout method callback for CALLBACK_checkout_form().
 */
function commerce_extra_login_page_continue_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $form = array();

  // Add description
  $description = variable_get('commerce_extra_login_page_continue_pane_description', '');
  if ($description) {
    $form['description'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="commerce-extra-login-page-description">',
      '#markup' => check_plain($description),
      '#suffix' => '</div>',
      '#weight' => -10,
    );
  }

  return $form;
}

/**
 * Commerce checkout method callback for CALLBACK_settings_form().
 */
function commerce_extra_login_page_continue_pane_settings_form($checkout_pane) {
  return array(
    'commerce_extra_login_page_continue_pane_description' => array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => variable_get('commerce_extra_login_page_continue_pane_description', ''),
    ),
  );
}

/**
 * Commerce checkout method callback for CALLBACK_checkout_form().
 */
function commerce_extra_login_page_login_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {

  // Get default login form
  $form = array();
  $form = user_login_block($form);

  // Modify elements to suit our case
  $form['name']['#title'] = t('E-mail address');
  $form['name']['#required'] = FALSE;
  $form['pass']['#required'] = FALSE;

  // Remove un-needed stuff
  unset($form['#action']);
  unset($form['#validate']);
  unset($form['#submit']);
  unset($form['actions']);

  // Add description
  $description = variable_get('commerce_extra_login_page_login_pane_description', '');
  if ($description) {
    $form['description'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="commerce-extra-login-page-description">',
      '#markup' => check_plain($description),
      '#suffix' => '</div>',
      '#weight' => -10,
    );
  }

  return $form;
}

/**
 * Commerce checkout method callback for CALLBACK_checkout_validate().
 *
 * This validation replicates core's login validation by combining all three
 * validation functions and remapping values to follow commerce form structure
 *
 * @see user_login_name_validate()
 * @see user_login_authenticate_validate()
 * @see user_login_final_validate()
 */
function commerce_extra_login_page_login_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {

  $username = $form_state['values']['login_pane']['name'];
  $password = trim($form_state['values']['login_pane']['pass']);
  if (!empty($username) || !empty($password)) {

    // Make sure user isn't blocked
    if (user_is_blocked($username)) {
      form_set_error('login_pane][name', t('The username %name has not been activated or is blocked.', array('%name' => $username)));
      return FALSE;
    }

    // Do not allow any login from the current user's IP if the limit has been
    // reached. Default is 50 failed attempts allowed in one hour. This is
    // independent of the per-user limit to catch attempts from one IP to log
    // in to many different user accounts.  We have a reasonably high limit
    // since there may be only one apparent IP for all users at an institution
    if (!flood_is_allowed('failed_login_attempt_ip', variable_get('user_failed_login_ip_limit', 50), variable_get('user_failed_login_ip_window', 3600))) {
      $form_state['flood_control_triggered'] = 'ip';
      return FALSE;
    }
    $account = db_query("SELECT * FROM {users} WHERE name = :name AND status = 1", array(':name' => $username))->fetchObject();
    if ($account) {
      if (variable_get('user_failed_login_identifier_uid_only', FALSE)) {
        // Register flood events based on the uid only, so they apply for any
        // IP address. This is the most secure option.
        $identifier = $account->uid;
      }
      else {
        // The default identifier is a combination of uid and IP address. This
        // is less secure but more resistant to denial-of-service attacks that
        // could lock out all users with public user names.
        $identifier = $account->uid . '-' . ip_address();
      }
      $form_state['flood_control_user_identifier'] = $identifier;

      // Don't allow login if the limit for this user has been reached.
      // Default is to allow 5 failed attempts every 6 hours.
      if (!flood_is_allowed('failed_login_attempt_user', variable_get('user_failed_login_user_limit', 5), variable_get('user_failed_login_user_window', 21600), $identifier)) {
        $form_state['flood_control_triggered'] = 'user';
        return FALSE;
      }
    }
    // We are not limited by flood control, so try to authenticate.
    // Set $form_state['uid'] as a flag for user_login_final_validate().
    $form_state['uid'] = user_authenticate($username, $password);

    if (empty($form_state['uid'])) {
      // Always register an IP-based failed login event.
      flood_register_event('failed_login_attempt_ip', variable_get('user_failed_login_ip_window', 3600));
      // Register a per-user failed login event.
      if (isset($form_state['flood_control_user_identifier'])) {
        flood_register_event('failed_login_attempt_user', variable_get('user_failed_login_user_window', 21600), $form_state['flood_control_user_identifier']);
      }

      if (isset($form_state['flood_control_triggered'])) {
        if ($form_state['flood_control_triggered'] == 'user') {
          form_set_error('login_pane][name', format_plural(variable_get('user_failed_login_user_limit', 5), 'Sorry, there has been more than one failed login attempt for this account. It is temporarily blocked. Try again later or <a href="@url">request a new password</a>.', 'Sorry, there have been more than @count failed login attempts for this account. It is temporarily blocked. Try again later or <a href="@url">request a new password</a>.', array('@url' => url('user/password'))));
          return FALSE;
        }
        else {
          // We did not find a uid, so the limit is IP-based.
          form_set_error('login_pane][name', t('Sorry, too many failed login attempts from your IP address. This IP address is temporarily blocked. Try again later or <a href="@url">request a new password</a>.', array('@url' => url('user/password'))));
          return FALSE;
        }
      }
      else {
        form_set_error('login_pane][name', t('Sorry, unrecognized username or password. <a href="@password">Have you forgotten your password?</a>', array('@password' => url('user/password'))));
        watchdog('commerce_extra_login_page', 'Login attempt failed for %user.', array('%user' => $username));
        return FALSE;
      }
    }
    elseif (isset($form_state['flood_control_user_identifier'])) {
      // Clear past failures for this user so as not to block a user who might
      // log in and out more than once in an hour.
      flood_clear_event('failed_login_attempt_user', $form_state['flood_control_user_identifier']);
    }

  }
  return TRUE;
}

/**
 * Commerce checkout method callback for CALLBACK_checkout_form_submit().
 *
 * Replicates core's login submission because $form_state isn't same as it was
 * in core's forms.
 *
 * @see user_login_submit()
 * @see user_login_finalize()
 */
function commerce_extra_login_page_login_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  if (!empty($form_state['uid'])) {

    global $user;
    $user = user_load($form_state['uid']);

    watchdog('commerce_extra_login_page', 'Session opened for %name during checkout.', array('%name' => $user->name));
    // Update the user table timestamp noting user has logged in.
    // This is also used to invalidate one-time login links.
    $user->login = REQUEST_TIME;
    db_update('users')
      ->fields(array('login' => $user->login))
      ->condition('uid', $user->uid)
      ->execute();

    // Regenerate the session ID to prevent against session fixation attacks.
    // This is called before hook_user in case one of those functions fails
    // or incorrectly does a redirect which would leave the old session in
    // place.
    drupal_session_regenerate();

    // We need fake form state, because values are mapped differently in
    // commerce checkout process compared to core's forms.
    $fake_form_state = array('values' => $form_state['values']['login_pane']);
    user_module_invoke('login', $fake_form_state, $user);
  }
}

/**
 * Commerce checkout method callback for CALLBACK_settings_form().
 */
function commerce_extra_login_page_login_pane_settings_form($checkout_pane) {
  return array(
    'commerce_extra_login_page_login_pane_description' => array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => variable_get('commerce_extra_login_page_login_pane_description', ''),
    ),
  );
}
