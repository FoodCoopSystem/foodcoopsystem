<?php
/*
 +--------------------------------------------------------------------+
 | CiviCRM version 4.1                                                |
 +--------------------------------------------------------------------+
 | Copyright (C) 2011 Marty Wright                                    |
 | Licensed to CiviCRM under the Academic Free License version 3.0.   |
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

require_once 'CRM/Core/DAO/ActionSchedule.php';
require_once 'CRM/Core/DAO/ActionMapping.php';

/**
 * This class contains functions for managing Scheduled Reminders
 */
class CRM_Core_BAO_ActionSchedule extends CRM_Core_DAO_ActionSchedule {

  static
  function getMapping($id = NULL) {
    static $_action_mapping;

    if ($id && !is_null($_action_mapping) && isset($_action_mapping[$id])) {
      return $_action_mapping[$id];
    }

    $dao = new CRM_Core_DAO_ActionMapping();
    if ($id) {
      $dao->id = $id;
    }
    $dao->find();

    $mapping = $defaults = array();
    while ($dao->fetch()) {
      CRM_Core_DAO::storeValues($dao, $defaults);
      $mapping[$dao->id] = $defaults;
    }
    $_action_mapping = $mapping;

    return $mapping;
  }


  static
  function getSelection($id = NULL) {
    $mapping = self::getMapping($id);

    require_once 'CRM/Core/PseudoConstant.php';
    require_once 'CRM/Event/PseudoConstant.php';
    $participantStatus = CRM_Event_PseudoConstant::participantStatus(NULL, NULL, 'label');
    $activityStatus    = CRM_Core_PseudoConstant::activityStatus();
    $event             = CRM_Event_PseudoConstant::event(NULL, FALSE, "( is_template IS NULL OR is_template != 1 )");
    $activityType      = CRM_Core_PseudoConstant::activityType(FALSE) + CRM_Core_PseudoConstant::activityType(FALSE, TRUE);
    asort($activityType);
    $eventType = CRM_Event_PseudoConstant::eventType();

    $sel1 = $sel2 = $sel3 = $sel4 = $sel5 = array();
    $options = array('manual' => ts('Choose Recipient(s)'),
      'group' => ts('Select a Group'),
    );

    $entityMapping = array();
    $recipientMapping = array_combine(array_keys($options), array_keys($options));

    if (!$id) {

      $id = 1;

    }

    foreach ($mapping as $value) {
      $entityValue         = CRM_Utils_Array::value('entity_value', $value);
      $entityStatus        = CRM_Utils_Array::value('entity_status', $value);
      $entityDateStart     = CRM_Utils_Array::value('entity_date_start', $value);
      $entityDateEnd       = CRM_Utils_Array::value('entity_date_end', $value);
      $entityRecipient     = CRM_Utils_Array::value('entity_recipient', $value);
      $valueLabel          = array('- ' . strtolower(CRM_Utils_Array::value('entity_value_label', $value)) . ' -');
      $key                 = $value['id'];
      $entityMapping[$key] = $value['entity'];

      if ($entityValue == 'activity_type' &&
        $value['entity'] == 'civicrm_activity'
      ) {
        $val = ts('Activity');
      }
      elseif ($entityValue == 'event_type' &&
        $value['entity'] == 'civicrm_participant'
      ) {
        $val = ts('Event Type');
      }
      elseif ($entityValue == 'civicrm_event' &&
        $value['entity'] == 'civicrm_participant'
      ) {
        $val = ts('Event Name');
      }
      $sel1[$key] = $val;

      switch ($entityValue) {
        case 'activity_type':
          $sel2[$key] = $valueLabel + $activityType;
          break;

        case 'event_type':
          $sel2[$key] = $valueLabel + $eventType;
          break;

        case 'civicrm_event':
          $sel2[$key] = $valueLabel + $event;
          break;
      }

      if ($key == $id) {
        switch ($entityDateStart) {
          case 'activity_date_time':
            $sel4[$entityDateStart] = ts('Activity Date Time');
            break;

          case 'event_start_date':
            $sel4[$entityDateStart] = ts('Event Start Date');
            break;
        }

        switch ($entityDateEnd) {
          case 'event_end_date':
            $sel4[$entityDateEnd] = ts('Event End Date');
            break;
        }

        switch ($entityRecipient) {
          case 'activity_contacts':
            $activityContacts = CRM_Core_PseudoConstant::activityContacts();
            $sel5[$entityRecipient] = $activityContacts + $options;
            $recipientMapping += CRM_Core_PseudoConstant::activityContacts('name');
            break;

          case 'event_contacts':
            $eventContacts = CRM_Core_PseudoConstant::eventContacts();
            $sel5[$entityRecipient] = $eventContacts + $options;
            $recipientMapping += CRM_Core_PseudoConstant::eventContacts('name');
            break;
        }
      }
    }
    $sel3 = $sel2;

    foreach ($mapping as $value) {
      $entityStatus = $value['entity_status'];
      $statusLabel  = array('- ' . strtolower(CRM_Utils_Array::value('entity_status_label', $value)) . ' -');
      $id           = $value['id'];

      switch ($entityStatus) {
        case 'activity_status':
          foreach ($sel3[$id] as $kkey => & $vval) {
            $vval = $statusLabel + $activityStatus;
          }
          break;

        case 'civicrm_participant_status_type':
          foreach ($sel3[$id] as $kkey => & $vval) {
            $vval = $statusLabel + $participantStatus;
          }
          break;
      }
    }

    return array(
      'sel1' => $sel1,
      'sel2' => $sel2,
      'sel3' => $sel3,
      'sel4' => $sel4,
      'sel5' => $sel5,
      'entityMapping' => $entityMapping,
      'recipientMapping' => $recipientMapping,
    );
  }



  static
  function getSelection1($id = NULL) {
    $mapping = self::getMapping($id);

    require_once 'CRM/Core/PseudoConstant.php';
    require_once 'CRM/Event/PseudoConstant.php';
    $participantStatus = CRM_Event_PseudoConstant::participantStatus(NULL, NULL, 'label');
    $sel4              = $sel5 = array();
    $options           = array('manual' => ts('Choose Recipient(s)'),
      'group' => ts('Select a Group'),
    );

    $recipientMapping = array_combine(array_keys($options), array_keys($options));

    foreach ($mapping as $value) {

      $entityDateStart = $value['entity_date_start'];
      $entityDateEnd   = $value['entity_date_end'];
      $entityRecipient = $value['entity_recipient'];
      $key             = $value['id'];

      switch ($entityDateStart) {
        case 'activity_date_time':
          $sel4[$entityDateStart] = ts('Activity Date Time');
          break;

        case 'event_start_date':
          $sel4[$entityDateStart] = ts('Event Start Date');
          break;
      }

      switch ($entityDateEnd) {
        case 'event_end_date':
          $sel4[$entityDateEnd] = ts('Event End Date');
          break;
      }

      switch ($entityRecipient) {
        case 'activity_contacts':
          $activityContacts = CRM_Core_PseudoConstant::activityContacts();
          $sel5[$id] = $activityContacts + $options;
          $recipientMapping += CRM_Core_PseudoConstant::activityContacts('name');
          break;

        case 'event_contacts':
          $eventContacts = CRM_Core_PseudoConstant::eventContacts();
          $sel5[$id] = $eventContacts + $options;
          $recipientMapping += CRM_Core_PseudoConstant::eventContacts('name');
          break;
      }
    }

    return array(
      'sel4' => $sel4,
      'sel5' => $sel5[$id],
      'recipientMapping' => $recipientMapping,
    );
  }

  /**
   * Retrieve list of Scheduled Reminders
   *
   * @param bool    $namesOnly    return simple list of names
   *
   * @return array  (reference)   reminder list
   * @static
   * @access public
   */
  static
  function &getList($namesOnly = FALSE, $entityValue = NULL, $id = NULL) {
    require_once 'CRM/Core/PseudoConstant.php';
    require_once 'CRM/Event/PseudoConstant.php';

    $activity_type = CRM_Core_PseudoConstant::activityType(FALSE) + CRM_Core_PseudoConstant::activityType(FALSE, TRUE);
    asort($activity_type);
    $activity_status = CRM_Core_PseudoConstant::activityStatus();
    $event_type = CRM_Event_PseudoConstant::eventType();
    $civicrm_event = CRM_Event_PseudoConstant::event(NULL, FALSE, "( is_template IS NULL OR is_template != 1 )");
    $civicrm_participant_status_type = CRM_Event_PseudoConstant::participantStatus(NULL, NULL, 'label');
    krsort($activity_type);
    krsort($activity_status);
    krsort($event_type);
    krsort($civicrm_event);
    krsort($civicrm_participant_status_type);

    $entity = array(
      'civicrm_activity' => 'Activity',
      'civicrm_participant' => 'Event',
    );

    $query = "
SELECT 
       title,
       cam.entity,
       cas.id as id,
       cam.entity_value as entityValue, 
       cas.entity_value as entityValueIds,
       cam.entity_status as entityStatus,
       cas.entity_status as entityStatusIds,
       cas.start_action_date as entityDate,
       cas.start_action_offset,
       cas.start_action_unit,
       cas.start_action_condition,
       cas.absolute_date,
       is_repeat,
       is_active

FROM civicrm_action_schedule cas
LEFT JOIN civicrm_action_mapping cam ON (cam.id = cas.mapping_id)
";
    $params = CRM_Core_DAO::$_nullArray;

    if ($entityValue and $id) {
      $where = "
WHERE   cas.entity_value = $id AND
        cam.entity_value = '$entityValue'";

      $query .= $where;

      $params = array(1 => array($id, 'Integer'),
        2 => array($entityValue, 'String'),
      );
    }

    $dao = CRM_Core_DAO::executeQuery($query);
    while ($dao->fetch()) {
      $list[$dao->id]['id'] = $dao->id;
      $list[$dao->id]['title'] = $dao->title;
      $list[$dao->id]['start_action_offset'] = $dao->start_action_offset;
      $list[$dao->id]['start_action_unit'] = $dao->start_action_unit;
      $list[$dao->id]['start_action_condition'] = $dao->start_action_condition;
      $list[$dao->id]['entityDate'] = ucwords(str_replace('_', ' ', $dao->entityDate));
      $list[$dao->id]['absolute_date'] = $dao->absolute_date;

      $status = $dao->entityStatus;
      $statusArray = explode(CRM_Core_DAO::VALUE_SEPARATOR, $dao->entityStatusIds);
      foreach ($statusArray as & $s) {
        $s = CRM_Utils_Array::value($s, $$status);
      }
      $statusIds = implode(', ', $statusArray);

      $value = $dao->entityValue;
      $valueArray = explode(CRM_Core_DAO::VALUE_SEPARATOR, $dao->entityValueIds);
      foreach ($valueArray as & $v) {
        $v = CRM_Utils_Array::value($v, $$value);
      }
      $valueIds = implode(', ', $valueArray);

      $list[$dao->id]['entity'] = $entity[$dao->entity];
      $list[$dao->id]['value'] = $valueIds;
      $list[$dao->id]['status'] = $statusIds;
      $list[$dao->id]['is_repeat'] = $dao->is_repeat;
      $list[$dao->id]['is_active'] = $dao->is_active;
    }

    return $list;
  }

  static
  function sendReminder($contactId, $email, $scheduleID, $from, $tokenParams) {
    require_once 'CRM/Core/BAO/Domain.php';
    require_once 'CRM/Utils/String.php';
    require_once 'CRM/Utils/Token.php';

    $schedule = new CRM_Core_DAO_ActionSchedule();
    $schedule->id = $scheduleID;

    $domain     = CRM_Core_BAO_Domain::getDomain();
    $result     = NULL;
    $hookTokens = array();

    if ($schedule->find(TRUE)) {
      $body_text    = $schedule->body_text;
      $body_html    = $schedule->body_html;
      $body_subject = $schedule->subject;
      if (!$body_text) {
        $body_text = CRM_Utils_String::htmlToText($body_html);
      }

      $params = array(array('contact_id', '=', $contactId, 0, 0));
      list($contact, $_) = CRM_Contact_BAO_Query::apiQuery($params);

      //CRM-4524
      $contact = reset($contact);

      if (!$contact || is_a($contact, 'CRM_Core_Error')) {
        return NULL;
      }

      // merge activity tokens with contact array
      $contact = array_merge($contact, $tokenParams);

      //CRM-5734
      require_once 'CRM/Utils/Hook.php';
      CRM_Utils_Hook::tokenValues($contact, $contactId);

      CRM_Utils_Hook::tokens($hookTokens);
      $categories = array_keys($hookTokens);

      $type = array('html', 'text');

      foreach ($type as $key => $value) {
        require_once 'CRM/Mailing/BAO/Mailing.php';
        $dummy_mail = new CRM_Mailing_BAO_Mailing();
        $bodyType = "body_{$value}";
        $dummy_mail->$bodyType = $$bodyType;
        $tokens = $dummy_mail->getTokens();

        if ($$bodyType) {
          CRM_Utils_Token::replaceGreetingTokens($$bodyType, NULL, $contact['contact_id']);
          $$bodyType = CRM_Utils_Token::replaceDomainTokens($$bodyType, $domain, TRUE, $tokens[$value], TRUE);
          $$bodyType = CRM_Utils_Token::replaceContactTokens($$bodyType, $contact, FALSE, $tokens[$value], FALSE, TRUE);
          $$bodyType = CRM_Utils_Token::replaceComponentTokens($$bodyType, $contact, $tokens[$value], TRUE, FALSE);
          $$bodyType = CRM_Utils_Token::replaceHookTokens($$bodyType, $contact, $categories, TRUE);
        }
      }
      $html = $body_html;
      $text = $body_text;

      require_once 'CRM/Core/Smarty/resources/String.php';
      civicrm_smarty_register_string_resource();
      $smarty = CRM_Core_Smarty::singleton();
      foreach (array(
        'text', 'html') as $elem) {
        $$elem = $smarty->fetch("string:{$$elem}");
      }

      $message = new Mail_mime("\n");

      /* Do contact-specific token replacement in text mode, and add to the
             * message if necessary */

      if (!$html || $contact['preferred_mail_format'] == 'Text' ||
        $contact['preferred_mail_format'] == 'Both'
      ) {
        // render the &amp; entities in text mode, so that the links work
        $text = str_replace('&amp;', '&', $text);
        $message->setTxtBody($text);

        unset($text);
      }

      if ($html && ($contact['preferred_mail_format'] == 'HTML' ||
          $contact['preferred_mail_format'] == 'Both'
        )) {
        $message->setHTMLBody($html);
        unset($html);
      }
      $recipient = "\"{$contact['display_name']}\" <$email>";

      $matches = array();
      preg_match_all('/(?<!\{|\\\\)\{(\w+\.\w+)\}(?!\})/',
        $body_subject,
        $matches,
        PREG_PATTERN_ORDER
      );

      $subjectToken = NULL;
      if ($matches[1]) {
        foreach ($matches[1] as $token) {
          list($type, $name) = preg_split('/\./', $token, 2);
          if ($name) {
            if (!isset($subjectToken['contact'])) {
              $subjectToken['contact'] = array();
            }
            $subjectToken['contact'][] = $name;
          }
        }
      }

      $messageSubject = CRM_Utils_Token::replaceContactTokens($body_subject, $contact, FALSE, $subjectToken);
      $messageSubject = CRM_Utils_Token::replaceDomainTokens($messageSubject, $domain, TRUE, $tokens[$value]);
      $messageSubject = CRM_Utils_Token::replaceComponentTokens($messageSubject, $contact, $tokens[$value], TRUE);
      $messageSubject = CRM_Utils_Token::replaceHookTokens($messageSubject, $contact, $categories, TRUE);

      $messageSubject = $smarty->fetch("string:{$messageSubject}");

      $headers = array(
        'From' => $from,
        'Subject' => $messageSubject,
      );
      $headers['To'] = $recipient;

      $mailMimeParams = array(
        'text_encoding' => '8bit',
        'html_encoding' => '8bit',
        'head_charset' => 'utf-8',
        'text_charset' => 'utf-8',
        'html_charset' => 'utf-8',
      );
      $message->get($mailMimeParams);
      $message->headers($headers);

      $config = CRM_Core_Config::singleton();
      $mailer = &$config->getMailer();

      $body = $message->get();
      $headers = $message->headers();

      CRM_Core_Error::ignoreException();
      $result = $mailer->send($recipient, $headers, $body);
      CRM_Core_Error::setCallback();
    }
    $schedule->free();

    return $result;
  }

  /**
   * Function to add the schedules reminders in the db
   *
   * @param array $params (reference ) an assoc array of name/value pairs
   * @param array $ids    the array that holds all the db ids
   *
   * @return object CRM_Core_DAO_ActionSchedule
   * @access public
   * @static
   *
   */
  static
  function add(&$params, &$ids) {
    $actionSchedule = new CRM_Core_DAO_ActionSchedule();
    $actionSchedule->copyValues($params);

    return $actionSchedule->save();
  }

  static
  function retrieve(&$params, &$values) {
    if (empty($params)) {
      return NULL;
    }
    $actionSchedule = new CRM_Core_DAO_ActionSchedule();

    $actionSchedule->copyValues($params);

    if ($actionSchedule->find(TRUE)) {
      $ids['actionSchedule'] = $actionSchedule->id;

      CRM_Core_DAO::storeValues($actionSchedule, $values);

      return $actionSchedule;
    }
    return NULL;
  }

  /**
   * Function to delete a Reminder
   *
   * @param  int  $id     ID of the Reminder to be deleted.
   *
   * @access public
   * @static
   */
  static
  function del($id) {
    if ($id) {
      $dao = new CRM_Core_DAO_ActionSchedule();
      $dao->id = $id;
      if ($dao->find(TRUE)) {
        $dao->delete();
        return;
      }
    }
    CRM_Core_Error::fatal(ts('Invalid value passed to delete function.'));
  }

  static
  function setIsActive($id, $is_active) {
    return CRM_Core_DAO::setFieldValue('CRM_Core_DAO_ActionSchedule', $id, 'is_active', $is_active);
  }

  static
  function sendMailings($mappingID, $now) {
    require_once 'CRM/Activity/BAO/Activity.php';
    require_once 'CRM/Contact/BAO/Contact.php';
    require_once 'CRM/Core/BAO/ActionLog.php';
    require_once 'CRM/Core/BAO/Domain.php';
    $domainValues = CRM_Core_BAO_Domain::getNameAndEmail();
    $fromEmailAddress = "$domainValues[0] <$domainValues[1]>";

    require_once 'CRM/Core/DAO/ActionMapping.php';
    $mapping = new CRM_Core_DAO_ActionMapping();
    $mapping->id = $mappingID;
    $mapping->find(TRUE);

    $actionSchedule = new CRM_Core_DAO_ActionSchedule();
    $actionSchedule->mapping_id = $mappingID;
    $actionSchedule->is_active = 1;
    $actionSchedule->find(FALSE);

    $tokenFields = array();
    $session = CRM_Core_Session::singleton();

    while ($actionSchedule->fetch()) {
      $extraSelect = $extraJoin = $extraWhere = '';

      if ($actionSchedule->record_activity) {
        $activityTypeID = CRM_Core_OptionGroup::getValue('activity_type',
          'Reminder Sent', 'name'
        );
        $activityStatusID = CRM_Core_OptionGroup::getValue('activity_status',
          'Completed', 'name'
        );
      }

      if ($mapping->entity == 'civicrm_activity') {
        $tokenEntity = 'activity';
        $tokenFields = array('activity_id', 'activity_type', 'subject', 'details', 'activity_date_time');
        $extraSelect = ", ov.label as activity_type, e.id as activity_id";
        $extraJoin   = "INNER JOIN civicrm_option_group og ON og.name = 'activity_type'
INNER JOIN civicrm_option_value ov ON e.activity_type_id = ov.value AND ov.option_group_id = og.id";
        $extraWhere = "AND e.is_current_revision = 1 AND e.is_deleted = 0";
      }

      if ($mapping->entity == 'civicrm_participant') {
        $tokenEntity = 'event';
        $tokenFields = array('event_type', 'title', 'event_id', 'start_date', 'end_date', 'summary', 'description');
        $extraSelect = ", ov.label as event_type, ev.title, ev.id as event_id, ev.start_date, ev.end_date, ev.summary, ev.description";
        $extraJoin   = "
INNER JOIN civicrm_event ev ON e.event_id = ev.id
INNER JOIN civicrm_option_group og ON og.name = 'event_type'
INNER JOIN civicrm_option_value ov ON ev.event_type_id = ov.value AND ov.option_group_id = og.id";
      }

      $query = "
SELECT reminder.id as reminderID, reminder.*, e.id as entityID, e.* {$extraSelect} 
FROM  civicrm_action_log reminder
INNER JOIN {$mapping->entity} e ON e.id = reminder.entity_id
{$extraJoin}
WHERE reminder.action_schedule_id = %1 AND reminder.action_date_time IS NULL
{$extraWhere}";
      $dao = CRM_Core_DAO::executeQuery($query,
        array(1 => array($actionSchedule->id, 'Integer'))
      );

      while ($dao->fetch()) {
        $entityTokenParams = array();
        foreach ($tokenFields as $field) {
          $entityTokenParams["{$tokenEntity}." . $field] = $dao->$field;
        }

        $isError  = 0;
        $errorMsg = '';
        $toEmail  = CRM_Contact_BAO_Contact::getPrimaryEmail($dao->contact_id);
        if ($toEmail) {
          $result = CRM_Core_BAO_ActionSchedule::sendReminder($dao->contact_id,
            $toEmail,
            $actionSchedule->id,
            $fromEmailAddress,
            $entityTokenParams
          );
          if (!$result || is_a($result, 'PEAR_Error')) {
            // we could not send an email, for now we ignore, CRM-3406
            $isError = 1;
          }
        }
        else {
          $isError = 1;
          $errorMsg = "Couldn\'t find recipient\'s email address.";
        }

        // update action log record
        $logParams = array(
          'id' => $dao->reminderID,
          'is_error' => $isError,
          'message' => $errorMsg ? $errorMsg : "null",
          'action_date_time' => $now,
        );
        CRM_Core_BAO_ActionLog::create($logParams);

        // insert activity log record if needed
        if ($actionSchedule->record_activity) {
          $activityParams = array(
            'subject' => $actionSchedule->title,
            'details' => $actionSchedule->body_html,
            'source_contact_id' => $session->get('userID') ?
            $session->get('userID') : $dao->contact_id,
            'target_contact_id' => $dao->contact_id,
            'activity_date_time' => date('YmdHis'),
            'status_id' => $activityStatusID,
            'activity_type_id' => $activityTypeID,
            'source_record_id' => $dao->entityID,
          );
          $activity = CRM_Activity_BAO_Activity::create($activityParams);
        }
      }
      $dao->free();
    }
  }

  static
  function buildRecipientContacts($mappingID, $now) {
    $actionSchedule = new CRM_Core_DAO_ActionSchedule();
    $actionSchedule->mapping_id = $mappingID;
    $actionSchedule->is_active = 1;
    $actionSchedule->find();

    while ($actionSchedule->fetch()) {
      require_once 'CRM/Core/DAO/ActionMapping.php';
      $mapping = new CRM_Core_DAO_ActionMapping();
      $mapping->id = $mappingID;
      $mapping->find(TRUE);

      $select = $join = $where = array();

      $value = explode(CRM_Core_DAO::VALUE_SEPARATOR,
        trim($actionSchedule->entity_value, CRM_Core_DAO::VALUE_SEPARATOR)
      );
      $value = implode(',', $value);

      $status = explode(CRM_Core_DAO::VALUE_SEPARATOR,
        trim($actionSchedule->entity_status, CRM_Core_DAO::VALUE_SEPARATOR)
      );
      $status = implode(',', $status);

      require_once 'CRM/Core/OptionGroup.php';
      $recipientOptions = CRM_Core_OptionGroup::values($mapping->entity_recipient);

      $from = "{$mapping->entity} e";

      if ($mapping->entity == 'civicrm_activity') {
        switch ($recipientOptions[$actionSchedule->recipient]) {
          case 'Activity Assignees':
            $contactField = "r.assignee_contact_id";
            $join[] = "INNER JOIN civicrm_activity_assignment r ON  r.activity_id = e.id";
            break;

          case 'Activity Source':
            $contactField = "e.source_contact_id";
            break;

          case 'Activity Targets':
            $contactField = "r.target_contact_id";
            $join[] = "INNER JOIN civicrm_activity_target r ON  r.activity_id = e.id";
            break;

          default:
            break;
        }
        // build where clause
        if (!empty($value)) {
          $where[] = "e.activity_type_id IN ({$value})";
        }
        if (!empty($status)) {
          $where[] = "e.status_id IN ({$status})";
        }
        $where[] = " e.is_current_revision = 1 ";
        $where[] = " e.is_deleted = 0 ";

        $dateField = 'e.activity_date_time';
      }

      if ($mapping->entity == 'civicrm_participant') {
        $contactField = "e.contact_id";
        $join[] = "INNER JOIN civicrm_event r ON e.event_id = r.id";

        if ($actionSchedule->recipient_listing) {
          $rList = explode(CRM_Core_DAO::VALUE_SEPARATOR,
            trim($actionSchedule->recipient_listing, CRM_Core_DAO::VALUE_SEPARATOR)
          );
          $rList = implode(',', $rList);

          switch ($recipientOptions[$actionSchedule->recipient]) {
            case 'Participant Status':
              $where[] = "e.status_id IN ({$rList})";
              break;

            case 'Participant Role':
              $where[] = "e.role_id IN ({$rList})";
              break;

            default:
              break;
          }
        }
        // build where clause
        if (!empty($value)) {
          $where[] = ($mapping->entity_value == 'event_type') ? "r.event_type_id IN ({$value})" : "r.id IN ({$value})";
        }
        if (!empty($status)) {
          $where[] = "e.status_id IN ({$status})";
        }

        $where[] = "r.is_active = 1";
        $dateField = str_replace('event_', 'r.', $actionSchedule->start_action_date);
      }

      if ($actionSchedule->group_id) {
        $join[] = "INNER JOIN civicrm_group_contact grp ON {$contactField} = grp.contact_id AND grp.status = 'Added'";
        $where[] = "grp.group_id IN ({$actionSchedule->group_id})";
      }
      elseif (!empty($actionSchedule->recipient_manual)) {
        $rList = CRM_Utils_Type::escape($actionSchedule->recipient_manual, 'String');
        $where[] = "{$contactField} IN ({$rList})";
      }

      $select[]           = "{$contactField} as contact_id";
      $select[]           = "e.id as entity_id";
      $select[]           = "'{$mapping->entity}' as entity_table";
      $select[]           = "{$actionSchedule->id} as action_schedule_id";
      $reminderJoinClause = "civicrm_action_log reminder ON reminder.contact_id = {$contactField} AND 
reminder.entity_id          = e.id AND 
reminder.entity_table       = '{$mapping->entity}' AND
reminder.action_schedule_id = %1";

      $join[] = "INNER JOIN civicrm_contact c ON c.id = {$contactField}";
      $where[] = "c.is_deleted = 0";

      if ($actionSchedule->start_action_date) {
        $operator          = ($actionSchedule->start_action_condition == 'before' ? "DATE_SUB" : "DATE_ADD");
        $op                = ($actionSchedule->start_action_condition == 'before' ? "<=" : ">=");
        $date              = $operator . "({$dateField}, INTERVAL {$actionSchedule->start_action_offset} {$actionSchedule->start_action_unit})";
        $startDateClause[] = "'{$now}' >= {$date}";

        $startDateClause[] = $operator . "({$now}, INTERVAL 1 DAY ) {$op} " . $actionSchedule->start_action_date;
        $startDate = implode(' AND ', $startDateClause);
      }
      elseif ($actionSchedule->absolute_date) {
        $startDate = "DATEDIFF(DATE('{$now}'),'{$actionSchedule->absolute_date}') = 0";
      }

      // ( now >= date_built_from_start_time ) OR ( now = absolute_date )
      $dateClause = "reminder.id IS NULL AND {$startDate}";

      // start composing query
      $selectClause = "SELECT " . implode(', ', $select);
      $fromClause   = "FROM $from";
      $joinClause   = !empty($join) ? implode(' ', $join) : '';
      $whereClause  = "WHERE " . implode(' AND ', $where);

      $query = "
INSERT INTO civicrm_action_log (contact_id, entity_id, entity_table, action_schedule_id)
{$selectClause} 
{$fromClause} 
{$joinClause}
LEFT JOIN {$reminderJoinClause}
{$whereClause} AND {$dateClause}";

      CRM_Core_DAO::executeQuery($query, array(1 => array($actionSchedule->id, 'Integer')));
      // if repeat is turned ON:
      if ($actionSchedule->is_repeat) {
        $repeatEvent = ($actionSchedule->end_action == 'before' ? "DATE_SUB" : "DATE_ADD") . "({$dateField}, INTERVAL {$actionSchedule->end_frequency_interval} {$actionSchedule->end_frequency_unit})";

        if ($actionSchedule->repetition_frequency_unit == 'day') {
          $hrs = 24 * $actionSchedule->repetition_frequency_interval;
        }
        elseif ($actionSchedule->repetition_frequency_unit == 'week') {
          $hrs = 24 * $actionSchedule->repetition_frequency_interval * 7;
        }
        else {
          $hrs = $actionSchedule->repetition_frequency_interval;
        }

        // (now <= repeat_end_time )
        $repeatEventClause = "'{$now}' <= {$repeatEvent}";
        // diff(now && logged_date_time) >= repeat_interval
        $havingClause = "HAVING TIMEDIFF({$now}, latest_log_time) >= TIME('{$hrs}:00:00')";
        $groupByClause = "GROUP BY reminder.contact_id, reminder.entity_id, reminder.entity_table";
        $selectClause .= ", MAX(reminder.action_date_time) as latest_log_time";

        $sqlInsertValues = "{$selectClause} 
{$fromClause} 
{$joinClause}
INNER JOIN {$reminderJoinClause}
{$whereClause} AND {$repeatEventClause}
{$groupByClause}
{$havingClause}";

        $valsqlInsertValues = CRM_Core_DAO::executeQuery($sqlInsertValues, array(1 => array($actionSchedule->id, 'Integer')));

        $arrValues = array();
        while ($valsqlInsertValues->fetch()) {
          $arrValues[] = "( {$valsqlInsertValues->contact_id}, {$valsqlInsertValues->entity_id}, '{$valsqlInsertValues->entity_table}',{$valsqlInsertValues->action_schedule_id} )";
        }

        $valString = implode(',', $arrValues);

        if ($valString) {
          $query = "
              INSERT INTO civicrm_action_log (contact_id, entity_id, entity_table, action_schedule_id) VALUES " . $valString;
          CRM_Core_DAO::executeQuery($query, array(1 => array($actionSchedule->id, 'Integer')));
        }
      }
    }
  }

  static
  function processQueue($now = NULL) {
    require_once 'CRM/Utils/Time.php';
    $now = $now ? CRM_Utils_Time::setTime($now) : CRM_Utils_Time::getTime();

    $mappings = self::getMapping();

    foreach ($mappings as $mappingID => $mapping) {
      self::buildRecipientContacts($mappingID, $now);

      self::sendMailings($mappingID, $now);
    }

    $result = array(
      'is_error' => 0,
      'messages' => ts('Sent all scheduled reminders successfully'),
    );
    return $result;
  }

  static
  function isConfigured($id, $mappingID) {
    $queryString = "SELECT count(id) FROM civicrm_action_schedule
                        WHERE  mapping_id = %1 AND 
                               entity_value = %2";

    $params = array(1 => array($mappingID, 'Integer'),
      2 => array($id, 'Integer'),
    );
    return CRM_Core_DAO::singleValueQuery($queryString, $params);
  }

  static
  function getRecipientListing($mappingID, $recipientType) {
    $options = array();
    if (!$mappingID || !$recipientType) {
      return $options;
    }

    $mapping = self::getMapping($mappingID);

    switch ($mapping['entity']) {
      case 'civicrm_participant':
        $eventContacts = CRM_Core_PseudoConstant::eventContacts('name');
        if (!CRM_Utils_Array::value($recipientType, $eventContacts)) {
          return $options;
        }
        if ($eventContacts[$recipientType] == 'Participant Status') {
          $options = CRM_Event_PseudoConstant::participantStatus();
        }
        elseif ($eventContacts[$recipientType] == 'Participant Role') {
          $options = CRM_Event_PseudoConstant::participantRole();
        }
        break;
    }

    return $options;
  }
}

