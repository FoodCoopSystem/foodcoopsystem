<?php

/**
 * @file
 * Token integration functions for invite module.
 */

/**
 * Implements hook_tokens().
 */
function invite_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  if ($type == 'invite' && !empty($data['invite'])) {
    $invite = $data['invite'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'inviter':
          $replacements[$original] = check_plain($invite->inviter->name);
          break;

        case 'inviter-raw':
          $replacements[$original] = $invite->inviter->name;
          break;

        case 'invite-mail':
          if (!empty($invite->email)) {
            $replacements[$original] = $invite->email;
          }
          break;

        case 'join-link':
          if (!empty($invite->reg_code)) {
            $replacements[$original] = url('invite/accept/' . $invite->reg_code, array('absolute' => TRUE));
          }
          break;

      }
    }
  }

  return $replacements;
}


/**
 * Implements hook_token_info().
 */
function invite_token_info() {
  $types['invite'] = array(
    'name' => t("Invite"),
    'description' => t("Tokens related to invites."),
    'needs-data' => 'invite',
  );

  $tokens['inviter'] = array(
    'name' => t('Inviter'),
    'description' => t('The user name of the inviter.'),
  );

  $tokens['inviter-raw'] = array(
    'name' => t('Inviter (raw)'),
    'description' => t('The user name of the inviter. WARNING - raw user input.'),
  );

  $tokens['invite-mail'] = array(
    'name' => t('Invite mail'),
    'description' => t('The e-mail address of the invited user.'),
  );

  $tokens['invite-message'] = array(
    'name' => t('Invite message'),
    'description' => t('The personal message for the invitee.'),
  );

  $tokens['invite-message-raw'] = array(
    'name' => t('Invite message raw'),
    'description' => t('The personal message for the invitee as unfiltered text. WARNING - raw user input.'),
  );

  $tokens['join-link'] = array(
    'name' => t('Join link'),
    'description' => t('The link to the registration page, including the invitation code.'),
  );

  return array(
    'types' => $types,
    'tokens' => array('invite' => $tokens),
  );
}
