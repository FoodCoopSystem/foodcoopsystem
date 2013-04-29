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

// we should consider moving these to the settings table
// before the 4.1 release
define('EMAIL_ACTIVITY_TYPE_ID', NULL);
define('MAIL_BATCH_SIZE', 50);
class CRM_Utils_Mail_EmailProcessor {

  /**
   * Process the default mailbox (ie. that is used by civiMail for the bounce)
   *
   * @return boolean always returns true (for the api). at a later stage we should
   *                 fix this to return true on success / false on failure etc
   */
  static
  function processBounces() {
    require_once 'CRM/Core/DAO/MailSettings.php';
    $dao             = new CRM_Core_DAO_MailSettings;
    $dao->domain_id  = CRM_Core_Config::domainID();
    $dao->is_default = TRUE;
    $dao->find();

    while ($dao->fetch()) {
      self::_process(TRUE, $dao);
    }

    // always returns true, i.e. never fails :)
    return TRUE;
  }

  /**
   * Delete old files from a given directory (recursively)
   *
   * @param string $dir  directory to cleanup
   * @param int    $age  files older than this many seconds will be deleted (default: 60 days)
   *
   * @return void
   */
  static
  function cleanupDir($dir, $age = 5184000) {
    // return early if we can’t read/write the dir
    if (!is_writable($dir) or !is_readable($dir) or !is_dir($dir)) {
      return;
    }

    foreach (scandir($dir) as $file) {

      // don’t go up the directory stack and skip new files/dirs
      if ($file == '.' or $file == '..') {
        continue;
      }
      if (filemtime("$dir/$file") > time() - $age) {
        continue;
      }

      // it’s an old file/dir, so delete/recurse
      is_dir("$dir/$file") ? self::cleanupDir("$dir/$file", $age) : unlink("$dir/$file");
    }
  }

  /**
   * Process the mailboxes that aren't default (ie. that aren't used by civiMail for the bounce)
   *
   * @return void
   */
  static
  function processActivities() {
    require_once 'CRM/Core/DAO/MailSettings.php';
    $dao             = new CRM_Core_DAO_MailSettings;
    $dao->domain_id  = CRM_Core_Config::domainID();
    $dao->is_default = FALSE;
    $dao->find();
    $found = FALSE;
    while ($dao->fetch()) {
      $found = TRUE;
      self::_process(FALSE, $dao);
    }
    if (!$found) {
      CRM_Core_Error::fatal(ts('No mailboxes have been configured for Email to Activity Processing'));
    }
    return $found;
  }

  /**
   * Process the mailbox for all the settings from civicrm_mail_settings
   *
   * @param string $civiMail  if true, processing is done in CiviMail context, or Activities otherwise.
   *
   * @return void
   */
  static
  function process($civiMail = TRUE) {
    require_once 'CRM/Core/DAO/MailSettings.php';
    $dao = new CRM_Core_DAO_MailSettings;
    $dao->domain_id = CRM_Core_Config::domainID();
    $dao->find();

    while ($dao->fetch()) {
      self::_process($civiMail, $dao);
    }
  }

  static
  function _process($civiMail, $dao) {
    // 0 = activities; 1 = bounce;
    $usedfor = $dao->is_default;

    require_once 'CRM/Core/OptionGroup.php';
    $emailActivityTypeId = (defined('EMAIL_ACTIVITY_TYPE_ID') && EMAIL_ACTIVITY_TYPE_ID) ? EMAIL_ACTIVITY_TYPE_ID : CRM_Core_OptionGroup::getValue('activity_type',
      'Inbound Email',
      'name'
    );
    if (!$emailActivityTypeId) {
      CRM_Core_Error::fatal(ts('Could not find a valid Activity Type ID for Inbound Email'));
    }

    $config            = CRM_Core_Config::singleton();
    $verpSeperator     = preg_quote($config->verpSeparator);
    $twoDigitStringMin = $verpSeperator . '(\d+)' . $verpSeperator . '(\d+)';
    $twoDigitString    = $twoDigitStringMin . $verpSeperator;
    $threeDigitString  = $twoDigitString . '(\d+)' . $verpSeperator;

    // FIXME: legacy regexen to handle CiviCRM 2.1 address patterns, with domain id and possible VERP part
    $commonRegex = '/^' . preg_quote($dao->localpart) . '(b|bounce|c|confirm|o|optOut|r|reply|re|e|resubscribe|u|unsubscribe)' . $threeDigitString . '([0-9a-f]{16})(-.*)?@' . preg_quote($dao->domain) . '$/';
    $subscrRegex = '/^' . preg_quote($dao->localpart) . '(s|subscribe)' . $twoDigitStringMin . '@' . preg_quote($dao->domain) . '$/';

    // a common-for-all-actions regex to handle CiviCRM 2.2 address patterns
    $regex = '/^' . preg_quote($dao->localpart) . '(b|c|e|o|r|u)' . $twoDigitString . '([0-9a-f]{16})@' . preg_quote($dao->domain) . '$/';

    // a tighter regex for finding bounce info in soft bounces’ mail bodies
    $rpRegex = '/Return-Path: ' . preg_quote($dao->localpart) . '(b)' . $twoDigitString . '([0-9a-f]{16})@' . preg_quote($dao->domain) . '/';

    // retrieve the emails
    require_once 'CRM/Mailing/MailStore.php';
    try {
      $store = CRM_Mailing_MailStore::getStore($dao->name);
    }
    catch(Exception$e) {
      $message = ts('Could not connect to MailStore') . '<p>';
      $message .= ts('Error message: ');
      $message .= '<pre>' . $e->getMessage() . '</pre><p>';
      CRM_Core_Error::fatal($message);
    }

    require_once 'CRM/Utils/Hook.php';

    // process fifty at a time, CRM-4002
    while ($mails = $store->fetchNext(MAIL_BATCH_SIZE)) {
      foreach ($mails as $key => $mail) {

        // for every addressee: match address elements if it's to CiviMail
        $matches = array();
        $action = NULL;

        if ($usedfor == 1) {
          foreach ($mail->to as $address) {
            if (preg_match($regex, $address->email, $matches)) {
              list($match, $action, $job, $queue, $hash) = $matches;
              break;
              // FIXME: the below elseifs should be dropped when we drop legacy support
            }
            elseif (preg_match($commonRegex, $address->email, $matches)) {
              list($match, $action, $_, $job, $queue, $hash) = $matches;
              break;
            }
            elseif (preg_match($subscrRegex, $address->email, $matches)) {
              list($match, $action, $_, $job) = $matches;
              break;
            }
          }

          // CRM-5471: if $matches is empty, it still might be a soft bounce sent
          // to another address, so scan the body for ‘Return-Path: …bounce-pattern…’
          if (!$matches and preg_match($rpRegex, $mail->generateBody(), $matches)) {
            list($match, $action, $job, $queue, $hash) = $matches;
          }

          // if all else fails, check Delivered-To for possible pattern
          if (!$matches and preg_match($regex, $mail->getHeader('Delivered-To'), $matches)) {
            list($match, $action, $job, $queue, $hash) = $matches;
          }
        }

        // preseve backward compatibility
        if ($usedfor == 0 || !$civiMail) {
          // if its the activities that needs to be processed ..
          require_once 'CRM/Utils/Mail/Incoming.php';
          $mailParams = CRM_Utils_Mail_Incoming::parseMailingObject($mail);

          civicrm_api_include('activity', FALSE, 2);
          $params            = _civicrm_activity_buildmailparams($mailParams, $emailActivityTypeId);
          $params['version'] = 2;
          $result            = civicrm_api('activity', 'create', $params);

          if ($result['is_error']) {
            $matches = FALSE;
            echo "Failed Processing: {$mail->subject}. Reason: {$result['error_message']}\n";
          }
          else {
            $matches = TRUE;
            echo "Processed as Activity: {$mail->subject}\n";
          }

          CRM_Utils_Hook::emailProcessor('activity', $params, $mail, $result);
        }

        // if $matches is empty, this email is not CiviMail-bound
        if (!$matches) {
          $store->markIgnored($key);
          continue;
        }

        // get $replyTo from either the Reply-To header or from From
        // FIXME: make sure it works with Reply-Tos containing non-email stuff
        $replyTo = $mail->getHeader('Reply-To') ? $mail->getHeader('Reply-To') : $mail->from->email;

        // handle the action by passing it to the proper API call
        // FIXME: leave only one-letter cases when dropping legacy support
        if (!empty($action)) {
          $result = NULL;

          switch ($action) {
            case 'b':
            case 'bounce':
              $text = '';
              if ($mail->body instanceof ezcMailText) {
                $text = $mail->body->text;
              }
              elseif ($mail->body instanceof ezcMailMultipart) {
                if ($mail->body instanceof ezcMailMultipartRelated) {
                  foreach ($mail->body->getRelatedParts() as $part) {
                    if (isset($part->subType) and $part->subType == 'plain') {
                      $text = $part->text;
                      break;
                    }
                  }
                }
                else {
                  foreach ($mail->body->getParts() as $part) {
                    if (isset($part->subType) and $part->subType == 'plain') {
                      $text = $part->text;
                      break;
                    }
                  }
                }
              }
              $params = array(
                'job_id' => $job,
                'event_queue_id' => $queue,
                'hash' => $hash,
                'body' => $text,
                'version' => 3,
              );
              $result = civicrm_api('Mailing', 'event_bounce', $params);
              break;

            case 'c':
            case 'confirm':
              // CRM-7921
              $params = array(
                'contact_id' => $job,
                'subscribe_id' => $queue,
                'hash' => $hash,
                'version' => 3,
              );
              civicrm_api('Mailing', 'event_confirm', $params);
              break;

            case 'o':
            case 'optOut':
              $params = array(
                'job_id' => $job,
                'event_queue_id' => $queue,
                'hash' => $hash,
                'version' => 3,
              );
              $result = civicrm_api('MailingGroup', 'event_domain_unsubscribe', $params);
              break;

            case 'r':
            case 'reply':
              // instead of text and HTML parts (4th and 6th params) send the whole email as the last param
              $params = array(
                'job_id' => $job,
                'event_queue_id' => $queue,
                'hash' => $hash,
                'bodyTxt' => NULL,
                'replyTo' => $replyTo,
                'bodyHTML' => NULL,
                'fullEmail' => $mail->generate(),
                'version' => 3,
              );
              $result = civicrm_api('Mailing', 'event_reply', $params);
              break;

            case 'e':
            case 're':
            case 'resubscribe':
              $params = array(
                'job_id' => $job,
                'event_queue_id' => $queue,
                'hash' => $hash,
                'version' => 3,
              );
              $result = civicrm_api('MailingGroup', 'event_resubscribe', $params);
              break;

            case 's':
            case 'subscribe':
              $params = array(
                'email' => $mail->from->email,
                'group_id' => $job,
                'version' => 3,
              );
              $result = civicrm_api('MailingGroup', 'event_subscribe', $params);
              break;

            case 'u':
            case 'unsubscribe':
              $params = array(
                'job_id' => $job,
                'event_queue_id' => $queue,
                'hash' => $hash,
                'version' => 3,
              );
              $result = civicrm_api('MailingGroup', 'event_unsubscribe', $params);
              break;
          }

          CRM_Utils_Hook::emailProcessor('mailing', $params, $mail, $result, $action);
        }

        $store->markProcessed($key);
      }
      // CRM-7356 – used by IMAP only
      $store->expunge();
    }
  }
}

