<?php

/**
 * @file
 * Page callbacks for invite module.
 */

/**
 * Menu callback; display an overview of sent invitations.
 *
 * @param $page
 *   Which invites to list: accepted, pending, or expired.
 */
function invite_user_overview($page, $account) {
  global $user;

  drupal_add_css(drupal_get_path('module', 'invite') . '/css/invite.css');
  drupal_add_js(drupal_get_path('module', 'invite') . '/js/invite.js');

  $rows = array();
  $profile_access = user_access('access user profiles');
  $allow_delete = user_access('withdraw own invitations');
  $allow_accepted_delete = user_access('withdraw own accepted invitations');

  $query = db_select('invite', 'i')
    ->fields('i')
    ->extend('PagerDefault')
    ->condition('i.uid', $account->uid)
    ->condition('canceled', 0);

  switch ($page) {
    case 'accepted':
    // @TODO: I believe that if default is at the top it will always be
    // defaulted to and the other cases will never be hit.
    default:
      $query->leftJoin('users', 'u', 'u.uid = i.invitee AND u.uid <> 0');
      $query->fields('u', array('uid', 'status'));
      $query->condition('i.joined', 0, '<>');
      $query->orderBy('u.uid', 'DESC');
      //$sql = "SELECT i.*, u.uid AS account FROM {invite} i LEFT JOIN {users} u ON u.uid = i.invitee AND u.uid <> 0 WHERE i.uid = %d AND i.joined <> 0 AND canceled = 0 ORDER BY u.uid DESC";
      break;
    case 'pending':
      $query->condition('i.joined', 0);
      $query->condition('i.expiry', REQUEST_TIME, '>=');
      $query->orderBy('i.expiry', 'DESC');
      //$sql = "SELECT * FROM {invite} WHERE uid = %d AND joined = 0 AND canceled = 0 AND expiry >= %d ORDER BY expiry DESC";
      break;
    case 'expired':
      $query->condition('i.joined', 0);
      $query->condition('i.expiry', REQUEST_TIME, '<');
      $query->orderBy('i.expiry', 'DESC');
      //$sql = "SELECT * FROM {invite} WHERE uid = %d AND joined = 0 AND canceled = 0 AND expiry < %d ORDER BY expiry DESC";
      break;
  }

  //$result = pager_query($sql, 50, 0, NULL, $user->uid, REQUEST_TIME);
  $result = $query->execute()->fetchAll();

  foreach ($result as $invite) {
    $row = array();
    switch ($page) {
      case 'accepted':
      default:
        $account_exists = !is_null($invite->uid);

        if ($profile_access && $account_exists && $invite->status > 0) {
          $row[] = l($invite->email, 'user/' . $invite->invitee, array('title' => t('View user profile.')));
        }
        else {
          $row[] = check_plain($invite->email);
        }
        $row[] = $account_exists ? t('Accepted') : t('Deleted');
        $row[] = $allow_accepted_delete ? l(t('withdraw'), "invite/withdraw/$invite->reg_code", array('query' => drupal_get_destination())) : '';
        break;

      case 'pending':
      case 'expired':
        $expired = ($invite->expiry < REQUEST_TIME);

        $row[] = check_plain($invite->email);
        $row[] = $expired ? t('Expired') : t('Pending');

        $operations = array();
        if ($allow_delete) {
          $operations[] = l(t('withdraw'), "invite/withdraw/$invite->reg_code", array('query' => drupal_get_destination()));
        }
        if ($expired && $account->uid == $user->uid) {
          $operations[] = l(t('resend'), "invite/resend/$invite->reg_code");
        }
        if ($page ==  'pending') {
          $operations[] = l(t('show link'), 'invite/accept/' . $invite->reg_code, array('attributes' => array('class' => array('invite-reg-link'), 'title' => t('Registration link for @email', array('@email' => $invite->email))), 'absolute' => TRUE));
        }

        $row[] = implode(' | ', $operations);
        break;
    }
    $rows[] = $row;
  }

  return theme('invite_user_overview', array('items' => $rows, 'page' => $page));
}

/**
 * Menu callback; handle incoming requests for accepting an invite.
 *
 * @param $invite
 *   A (unvalidated) invite object.
 */
function invite_accept($invite) {
  global $user;

  $status = invite_validate($invite);

  if (!$user->uid && $status == INVITE_VALID) {
    $_SESSION[INVITE_SESSION] = $invite->reg_code;
    drupal_goto(variable_get('invite_registration_path', 'user/register'));
  }
  else {
    switch ($status) {
      case INVITE_WITHDRAWN:
        drupal_set_message(t('This invitation has been withdrawn.'));
        break;

      case INVITE_USED:
        drupal_set_message(t('This invitation has already been used.'));
        break;

      case INVITE_EXPIRED:
        drupal_set_message(t('This invitation has expired.'));
        break;
    }
  }

  drupal_goto();
}

/**
 * Menu callback; resend an expired invite.
 *
 * @param $invite
 *   An invitate object.
 */
function invite_resend($invite) {
  global $user;

  // Inviter must match current user and invitation must have expired.
  if ($invite->uid == $user->uid && $invite->expiry < REQUEST_TIME && $invite->joined == 0 && $invite->canceled == 0) {
    return drupal_get_form('invite_form', 'page', $invite);
  }

  return MENU_ACCESS_DENIED;
}

/**
 * Theme function; display the invite overview table.
 *
 * @param $items
 *   An array of table rows.
 *
 * @ingroup themeable
 */
function theme_invite_user_overview($variables) {
  $output = '';
  $items = $variables['items'];

  if ($variables['page'] == 'pending') {
    $output .= '
    <div id="invite-reg-link-container" class="messages">
      <a href="" id="invite-reg-link-close">' . t('Hide') . '</a>
      <div id="invite-reg-title"></div>
      <div id="invite-reg-link"></div>
    </div>';
  }

  if (count($items) > 0) {
    $headers = array(t('E-mail'), t('Status'), t('Operations'));
    $output  .= theme('table', array('header' => $headers, 'rows' => $items, 'attributes' => array('id' => 'invite-user-overview')));
    $output .= theme('pager');
  }
  else {
    $output .= t('No invitations available.');
  }

  return $output;
}
