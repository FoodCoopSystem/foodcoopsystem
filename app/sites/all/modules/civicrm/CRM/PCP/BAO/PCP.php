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

require_once 'CRM/PCP/DAO/PCP.php';
require_once 'CRM/PCP/DAO/PCPBlock.php';
require_once 'CRM/Contribute/DAO/Contribution.php';
class CRM_PCP_BAO_PCP extends CRM_PCP_DAO_PCP {

  /**
   * The action links that we need to display for the browse screen
   *
   * @var array
   * @static
   */
  static $_pcpLinks = NULL;
  function __construct() {
    parent::__construct();
  }

  /**
   * function to add or update either a Personal Campaign Page OR a PCP Block
   *
   * @param array $params reference array contains the values submitted by the form
   * @param bool  $pcpBlock if true, create or update PCPBlock, else PCP
   * @access public
   * @static
   *
   * @return object
   */
  static
  function add(&$params, $pcpBlock = TRUE) {
    if ($pcpBlock) {
      // action is taken depending upon the mode
      require_once 'CRM/PCP/DAO/PCPBlock.php';
      $dao = new CRM_PCP_DAO_PCPBlock();
      $dao->copyValues($params);
      $dao->save();
      return $dao;
    }
    else {
      require_once 'CRM/PCP/DAO/PCP.php';
      $dao = new CRM_PCP_DAO_PCP();
      $dao->copyValues($params);

      // ensure we set status_id since it is a not null field
      // we should change the schema and allow this to be null
      if (!$dao->id &&
        !isset($dao->status_id)
      ) {
        $dao->status_id = 0;
      }

      // set currency for CRM-1496
      if (!isset($dao->currency)) {
        $config = &CRM_Core_Config::singleton();
        $dao->currency = $config->defaultCurrency;
      }

      $dao->save();
      return $dao;
    }
  }

  /**
   * function to get the Display  name of a contact for a PCP
   *
   * @param  int    $id      id for the PCP
   *
   * @return null|string     Dispaly name of the contact if found
   * @static
   * @access public
   */
  static
  function displayName($id) {
    $id = CRM_Utils_Type::escape($id, 'Integer');

    $query = "
SELECT civicrm_contact.display_name
FROM   civicrm_pcp, civicrm_contact
WHERE  civicrm_pcp.contact_id = civicrm_contact.id
  AND  civicrm_pcp.id = {$id}
";
    return CRM_Core_DAO::singleValueQuery($query, CRM_Core_DAO::$_nullArray);
  }

  /**
   * Function to return PCP  Block info for dashboard
   *
   * @return array     array of Pcp if found
   * @access public
   * @static
   */
  static
  function getPcpDashboardInfo($contactId) {
    $links = self::pcpLinks();

    $query = "
        SELECT pg.start_date, pg.end_date, pg.title as pageTitle, pcp.id as pcpId, 
               pcp.title as pcpTitle, pcp.status_id as pcpStatusId, cov_status.label as pcpStatus,
               pcpblock.is_tellfriend_enabled as tellfriend, 
               pcpblock.id as blockId, pcp.is_active as pcpActive, pg.id as pageId
        FROM civicrm_contribution_page pg 
        LEFT JOIN civicrm_pcp pcp ON  (pg.id= pcp.page_id)
        LEFT JOIN civicrm_pcp_block as pcpblock ON ( pg.id = pcpblock.entity_id )
        
        LEFT JOIN civicrm_option_group cog_status ON cog_status.name = 'pcp_status'
        LEFT JOIN civicrm_option_value cov_status
               ON (pcp.status_id = cov_status.value
               AND cog_status.id = cov_status.option_group_id )
        
        INNER JOIN civicrm_contact as ct ON (ct.id = pcp.contact_id  AND pcp.contact_id = %1 )
        WHERE pcpblock.is_active = 1
        ORDER BY pcpStatus, pageTitle";

    $params          = array(1 => array($contactId, 'Integer'));
    $pcpInfoDao      = CRM_Core_DAO::executeQuery($query, $params);
    $pcpInfo         = array();
    $hide            = $mask = array_sum(array_keys($links['all']));
    $contactPCPPages = array();

    $approvedId = CRM_Core_OptionGroup::getValue('pcp_status', 'Approved', 'name');
    while ($pcpInfoDao->fetch()) {
      $mask = $hide;
      if ($links) {
        $replace = array(
          'pcpId' => $pcpInfoDao->pcpId,
          'pcpBlock' => $pcpInfoDao->blockId,
        );
      }
      $pcpLink = $links['all'];
      $class = '';

      if ($pcpInfoDao->pcpStatusId != $approvedId || $pcpInfoDao->pcpActive != 1) {
        $class = "disabled";
      }
      if (!$pcpInfoDao->tellfriend || $pcpInfoDao->pcpStatusId != $approvedId || $pcpInfoDao->pcpActive != 1) {
        $mask -= CRM_Core_Action::DETACH;
      }
      if ($pcpInfoDao->pcpActive == 1) {
        $mask -= CRM_Core_Action::ENABLE;
      }
      else {
        $mask -= CRM_Core_Action::DISABLE;
      }
      $action = CRM_Core_Action::formLink($pcpLink, $mask, $replace);
      $pcpInfo[] = array(
        'start_date' => $pcpInfoDao->start_date,
        'end_date' => $pcpInfoDao->end_date,
        'pageTitle' => $pcpInfoDao->pageTitle,
        'pcpId' => $pcpInfoDao->pcpId,
        'pcpTitle' => $pcpInfoDao->pcpTitle,
        'pcpStatus' => $pcpInfoDao->pcpStatus,
        'action' => $action,
        'class' => $class,
      );
      $contactPCPPages[] = $pcpInfoDao->pageId;
    }

    $excludePageClause = NULL;
    if (!empty($contactPCPPages)) {
      $excludePageClause = " AND pg.id NOT IN ( " . implode(',', $contactPCPPages) . ") ";
    }

    $query = "
        SELECT pg.id as pageId, pg.title as pageTitle, pg.start_date , 
                  pg.end_date 
        FROM civicrm_contribution_page pg 
        LEFT JOIN civicrm_pcp_block as pcpblock ON ( pg.id = pcpblock.entity_id )
        WHERE pcpblock.is_active = 1 {$excludePageClause}
        ORDER BY pageTitle ASC";

    $pcpBlockDao = CRM_Core_DAO::executeQuery($query);
    $pcpBlock    = array();
    $mask        = 0;

    while ($pcpBlockDao->fetch()) {
      if ($links) {
        $replace = array('pageId' => $pcpBlockDao->pageId);
      }
      $pcpLink    = $links['add'];
      $action     = CRM_Core_Action::formLink($pcpLink, $mask, $replace);
      $pcpBlock[] = array(
        'pageId' => $pcpBlockDao->pageId,
        'pageTitle' => $pcpBlockDao->pageTitle,
        'start_date' => $pcpBlockDao->start_date,
        'end_date' => $pcpBlockDao->end_date,
        'action' => $action,
      );
    }

    return array($pcpBlock, $pcpInfo);
  }

  /**
   * function to show the total amount for Personal Campaign Page on thermometer
   *
   * @param array $pcpId  contains the pcp ID
   *
   * @access public
   * @static
   *
   * @return total amount
   */
  static
  function thermoMeter($pcpId) {
    $query = "
SELECT SUM(cc.total_amount) as total
FROM civicrm_pcp pcp 
LEFT JOIN civicrm_contribution_soft cs ON ( pcp.id = cs.pcp_id ) 
LEFT JOIN civicrm_contribution cc ON ( cs.contribution_id = cc.id)
WHERE pcp.id = %1 AND cc.contribution_status_id =1 AND cc.is_test = 0";

    $params = array(1 => array($pcpId, 'Integer'));
    return CRM_Core_DAO::singleValueQuery($query, $params);
  }

  /**
   * function to show the amount, nickname on honor roll
   *
   * @param array $pcpId contains the pcp ID
   *
   * @access public
   * @static
   *
   * @return array $honor
   */
  static
  function honorRoll($pcpId) {
    $query = "
            SELECT cc.id, cs.pcp_roll_nickname, cs.pcp_personal_note,
                   cc.total_amount, cc.currency
            FROM civicrm_contribution cc 
                 LEFT JOIN civicrm_contribution_soft cs ON cc.id = cs.contribution_id
            WHERE cs.pcp_id = {$pcpId}
                  AND cs.pcp_display_in_roll = 1 
                  AND contribution_status_id = 1 
                  AND is_test = 0";
    $dao = CRM_Core_DAO::executeQuery($query, CRM_Core_DAO::$_nullArray);
    $honor = array();
    require_once 'CRM/Utils/Money.php';
    while ($dao->fetch()) {
      $honor[$dao->id]['nickname'] = ucwords($dao->pcp_roll_nickname);
      $honor[$dao->id]['total_amount'] = CRM_Utils_Money::format($dao->total_amount, $dao->currency);
      $honor[$dao->id]['personal_note'] = $dao->pcp_personal_note;
    }
    return $honor;
  }

  /**
   * Get action links
   *
   * @return array (reference) of action links
   * @static
   */
  static
  function &pcpLinks() {
    if (!(self::$_pcpLinks)) {
      $deleteExtra = ts('Are you sure you want to delete this Personal Campaign Page?') . '\n' . ts('This action cannot be undone.');

      self::$_pcpLinks['add'] = array(
        CRM_Core_Action::ADD => array('name' => ts('Create a Personal Campaign Page'),
          'url' => 'civicrm/contribute/campaign',
          'qs' => 'action=add&reset=1&pageId=%%pageId%%&component=contribute',
          'title' => ts('Configure'),
        ),
      );

      self::$_pcpLinks['all'] = array(
        CRM_Core_Action::UPDATE => array('name' => ts('Edit Your Page'),
          'url' => 'civicrm/pcp/info',
          'qs' => 'action=update&reset=1&id=%%pcpId%%&component=%%pageComponent%%',
          'title' => ts('Configure'),
        ),
        CRM_Core_Action::DETACH => array('name' => ts('Tell Friends'),
          'url' => 'civicrm/friend',
          'qs' => 'eid=%%pcpId%%&blockId=%%pcpBlock%%&reset=1&pcomponent=pcp&component=%%pageComponent%%',
          'title' => ts('Tell Friends'),
        ),
        CRM_Core_Action::BROWSE => array('name' => ts('Update Contact Information'),
          'url' => 'civicrm/pcp/info',
          'qs' => 'action=browse&reset=1&id=%%pcpId%%&component=%%pageComponent%%',
          'title' => ts('Update Contact Information'),
        ),
        CRM_Core_Action::ENABLE => array('name' => ts('Enable'),
          'url' => 'civicrm/pcp',
          'qs' => 'action=enable&reset=1&id=%%pcpId%%&component=%%pageComponent%%',
          'title' => ts('Enable'),
        ),
        CRM_Core_Action::DISABLE => array('name' => ts('Disable'),
          'url' => 'civicrm/pcp',
          'qs' => 'action=disable&reset=1&id=%%pcpId%%&component=%%pageComponent%%',
          'title' => ts('Disable'),
        ),
        CRM_Core_Action::DELETE => array('name' => ts('Delete'),
          'url' => 'civicrm/pcp',
          'qs' => 'action=delete&reset=1&id=%%pcpId%%&component=%%pageComponent%%',
          'extra' => 'onclick = "return confirm(\'' . $deleteExtra . '\');"',
          'title' => ts('Delete'),
        ),
      );
    }
    return self::$_pcpLinks;
  }

  /**
   * Function to Delete the campaign page
   *
   * @param int $id campaign page id
   *
   * @return null
   * @access public
   * @static
   *
   */
  function delete($id) {
    require_once 'CRM/Utils/Hook.php';
    CRM_Utils_Hook::pre('delete', 'Campaign', $id, CRM_Core_DAO::$_nullArray);

    require_once 'CRM/Core/Transaction.php';
    $transaction = new CRM_Core_Transaction();

    // delete from pcp table
    $pcp = new CRM_PCP_DAO_PCP();
    $pcp->id = $id;
    $pcp->delete();

    $transaction->commit();

    CRM_Utils_Hook::post('delete', 'Campaign', $id, $pcp);
  }

  /**
   * Function to build the form
   *
   * @param object $form form object
   *
   * @return None
   * @access public
   */
  function buildPCPForm($form) {
    $form->addElement('checkbox', 'pcp_active', ts('Enable Personal Campaign Pages?'), NULL, array('onclick' => "return showHideByValue('pcp_active',true,'pcpFields','block','radio',false);"));

    $form->addElement('checkbox', 'is_approval_needed', ts('Approval required'));

    $profile        = array();
    $isUserRequired = NULL;
    $config         = CRM_Core_Config::singleton();
    if ($config->userFramework != 'Standalone') {
      $isUserRequired = 2;
    }
    CRM_Core_DAO::commonRetrieveAll('CRM_Core_DAO_UFGroup', 'is_cms_user', $isUserRequired, $profiles, array('title', 'is_active'));
    if (!empty($profiles)) {
      foreach ($profiles as $key => $value) {
        if ($value['is_active']) {
          $profile[$key] = $value['title'];
        }
      }
      $form->assign('profile', $profile);
    }

    $form->add('select', 'supporter_profile_id', ts('Supporter profile'), array('' => ts('- select -')) + $profile);

    $form->addElement('checkbox', 'is_tellfriend_enabled', ts("Allow 'Tell a friend' functionality"), NULL, array('onclick' => "return showHideByValue('is_tellfriend_enabled',true,'tflimit','table-row','radio',false);"));

    $form->add('text',
      'tellfriend_limit',
      ts("'Tell a friend' maximum recipients limit"),
      CRM_Core_DAO::getAttribute('CRM_PCP_DAO_PCPBlock', 'tellfriend_limit')
    );
    $form->addRule('tellfriend_limit', ts('Please enter a valid limit.'), 'integer');

    $form->add('text',
      'link_text',
      ts("'Create Personal Campaign Page' link text"),
      CRM_Core_DAO::getAttribute('CRM_PCP_DAO_PCPBlock', 'link_text')
    );

    $form->add('text', 'notify_email', ts('Notify Email'), CRM_Core_DAO::getAttribute('CRM_PCP_DAO_PCPBlock', 'notify_email'));
  }


  /*
     * Add PCP form elements to a form
     */
  function buildPcp($pcpId, &$page, &$elements = NULL) {

    $prms = array('id' => $pcpId);
    CRM_Core_DAO::commonRetrieve('CRM_PCP_DAO_PCP', $prms, $pcpInfo);

    if ($pcpSupporter = CRM_PCP_BAO_PCP::displayName($pcpId)) {
      if ($pcpInfo['page_type'] == 'event') {
        $page->assign('pcpSupporterText', ts('This event registration is being made thanks to effort of <strong>%1</strong>, who supports our campaign. You can support it as well - once you complete the registration, you will be able to create your own Personal Campaign Page!', array(1 => $pcpSupporter)));
      }
      else {
        $page->assign('pcpSupporterText', ts('This contribution is being made thanks to effort of <strong>%1</strong>, who supports our campaign. You can support it as well - once you complete the donation, you will be able to create your own Personal Campaign Page!', array(1 => $pcpSupporter)));
      }
    }
    $page->assign('pcp', TRUE);

    // build honor roll fields for registration form if supporter has honor roll enabled for their PCP
    if ($pcpInfo['is_honor_roll']) {
      $page->assign('is_honor_roll', TRUE);
      $page->add('checkbox', 'pcp_display_in_roll', ts('Show my support in the public honor roll'), NULL, NULL,
        array('onclick' => "showHideByValue('pcp_display_in_roll','','nameID|nickID|personalNoteID','block','radio',false); pcpAnonymous( );")
      );
      $extraOption = array('onclick' => "return pcpAnonymous( );");
      $elements    = array();
      $elements[]  = &$page->createElement('radio', NULL, '', ts('Include my name and message'), 0, $extraOption);
      $elements[]  = &$page->createElement('radio', NULL, '', ts('List my support anonymously'), 1, $extraOption);
      $page->addGroup($elements, 'pcp_is_anonymous', NULL, '&nbsp;&nbsp;&nbsp;');
      $page->_defaults['pcp_is_anonymous'] = 0;

      $page->add('text', 'pcp_roll_nickname', ts('Name'), array('maxlength' => 30));
      $page->add('textarea', "pcp_personal_note", ts('Personal Note'), array('style' => 'height: 3em; width: 40em;'));
    }
    else {
      $page->assign('is_honor_roll', FALSE);
    }
  }

  /*
     * Process a PCP contribution/
     */
  function handlePcp($pcpId, $component, $entity) {

    $entity_table = self::getPcpEntityTable($component);

    if (!$pcpId) {
      return FALSE;
    }

    require_once 'CRM/Core/OptionGroup.php';
    $approvedId = CRM_Core_OptionGroup::getValue('pcp_status', 'Approved', 'name');

    $prms = array(
      'entity_id' => $entity['id'],
      'entity_table' => $entity_table,
    );
    require_once 'CRM/PCP/PseudoConstant.php';
    $pcpStatus = CRM_PCP_PseudoConstant::pcpStatus();
    CRM_Core_DAO::commonRetrieve('CRM_PCP_DAO_PCPBlock',
      $prms,
      $pcpBlock
    );
    $prms = array('id' => $pcpId);
    CRM_Core_DAO::commonRetrieve('CRM_PCP_DAO_PCP', $prms, $pcpInfo);

    //start and end date of the contribution page
    $startDate = CRM_Utils_Date::unixTime(CRM_Utils_Array::value('start_date', $page->_values));
    $endDate   = CRM_Utils_Date::unixTime(CRM_Utils_Array::value('end_date', $page->_values));
    $now       = time();


    if ($component == 'event') {
      $urlBase = 'civicrm/event/register';
    }
    elseif ($component == 'contribute') {
      $urlBase = 'civicrm/contribute/transact';
    }
    $url = CRM_Utils_System::url($urlBase,
      "reset=1&id={$entity['id']}",
      FALSE, NULL, FALSE, TRUE
    );

    $params = array('id' => $pcpInfo['pcp_block_id']);
    CRM_Core_DAO::commonRetrieve('CRM_PCP_DAO_PCPBlock', $params, $pcpBlock);

    if ($pcpBlock['target_entity_id'] != $entity['id']) {
      $statusMessage = ts('This page is not related to the Personal Campaign Page you have just visited. However you can still make a contribution here.');
      CRM_Core_Error::statusBounce($statusMessage, $url);
    }
    elseif ($pcpInfo['status_id'] != $approvedId) {
      $statusMessage = ts('The Personal Campaign Page you have just visited is currently %1. However you can still support the campaign by making a contribution here.', array(1 => $pcpStatus[$pcpInfo['status_id']]));
      CRM_Core_Error::statusBounce($statusMessage, $url);
    }
    elseif (!CRM_Utils_Array::value('is_active', $pcpBlock)) {
      $statusMessage = ts('Personal Campaign Pages are currently not enabled for this contribution page. However you can still support the campaign by making a contribution here.');
      CRM_Core_Error::statusBounce($statusMessage, $url);
    }
    elseif (!CRM_Utils_Array::value('is_active', $pcpInfo)) {
      $statusMessage = ts('The Personal Campaign Page you have just visited is current inactive. However you can still make a contribution here.');
      CRM_Core_Error::statusBounce($statusMessage, $url);
    }
    elseif (($startDate && $startDate > $now) || ($endDate && $endDate < $now)) {
      $customStartDate = CRM_Utils_Date::customFormat(CRM_Utils_Array::value('start_date', $entity['start_date']));
      $customEndDate = CRM_Utils_Date::customFormat(CRM_Utils_Array::value('end_date', $entity['end_date']));
      if ($startDate && $endDate) {
        $statusMessage = ts('The Personal Campaign Page you have just visited is only active between %1 to %2. However you can still support the campaign by making a contribution here.',
          array(1 => $customStartDate, 2 => $customEndDate)
        );
        CRM_Core_Error::statusBounce($statusMessage, $url);
      }
      elseif ($startDate) {
        $statusMessage = ts('The Personal Campaign Page you have just visited will be active beginning on %1. However you can still support the campaign by making a contribution here.', array(1 => $customStartDate));
        CRM_Core_Error::statusBounce($statusMessage, $url);
      }
      elseif ($endDate) {
        $statusMessage = ts('The Personal Campaign Page you have just visited is not longer active (as of %1). However you can still support the campaign by making a contribution here.', array(1 => $customEndDate));
        CRM_Core_Error::statusBounce($statusMessage, $url);
      }
    }

    return array(
      'pcpId' => $pcpId,
      'pcpBlock' => $pcpBlock,
      'pcpInfo' => $pcpInfo,
    );
  }

  /**
   * Function to Approve / Reject the campaign page
   *
   * @param int $id campaign page id
   *
   * @return null
   * @access public
   * @static
   *
   */
  static
  function setIsActive($id, $is_active) {
    switch ($is_active) {
      case 0:
        $is_active = 3;
        break;

      case 1:
        $is_active = 2;
        break;
    }

    CRM_Core_DAO::setFieldValue('CRM_PCP_DAO_PCP', $id, 'status_id', $is_active);

    require_once 'CRM/PCP/PseudoConstant.php';
    $pcpTitle  = CRM_Core_DAO::getFieldValue('CRM_PCP_DAO_PCP', $id, 'title');
    $pcpStatus = CRM_PCP_PseudoConstant::pcpStatus();
    $pcpStatus = $pcpStatus[$is_active];

    CRM_Core_Session::setStatus("$pcpTitle status has been updated to $pcpStatus.");

    // send status change mail
    $result = self::sendStatusUpdate($id, $is_active);

    if ($result) {
      CRM_Core_Session::setStatus("A notification email has been sent to the supporter.");
    }
  }

  /**
   * Function to send notfication email to supporter
   * 1. when their PCP status is changed by site admin.
   * 2. when supporter initially creates a Personal Campaign Page ($isInitial set to true).
   *
   * @param int $pcpId      campaign page id
   * @param int $newStatus  pcp status id
   * @param int $isInitial  is it the first time, campaign page has been created by the user
   *
   * @return null
   * @access public
   * @static
   *
   */
  static
  function sendStatusUpdate($pcpId, $newStatus, $isInitial = FALSE, $component = 'contribute') {
    require_once 'CRM/PCP/PseudoConstant.php';
    $pcpStatus = CRM_PCP_PseudoConstant::pcpStatus();
    $config = CRM_Core_Config::singleton();

    if (!isset($pcpStatus[$newStatus])) {
      return FALSE;
    }

    require_once 'CRM/Utils/Mail.php';
    require_once 'Mail/mime.php';
    require_once 'CRM/Contact/BAO/Contact/Location.php';

    //set loginUrl
    $loginUrl = $config->userFrameworkBaseURL;
    switch (ucfirst($config->userFramework)) {
      case 'Joomla':
        $loginUrl = str_replace('administrator/', '', $loginUrl);
        $loginUrl .= 'index.php?option=com_user&view=login';
        break;

      case 'Drupal':
        $loginUrl .= 'user';
        break;
    }

    // used in subject templates
    $contribPageTitle = self::getPcpPageTitle($pcpId, $component);

    $tplParams = array(
      'loginUrl' => $loginUrl,
      'contribPageTitle' => $contribPageTitle,
    );

    //get the default domain email address.
    require_once 'CRM/Core/BAO/Domain.php';
    list($domainEmailName, $domainEmailAddress) = CRM_Core_BAO_Domain::getNameAndEmail();

    if (!$domainEmailAddress || $domainEmailAddress == 'info@FIXME.ORG') {
      require_once 'CRM/Utils/System.php';
      $fixUrl = CRM_Utils_System::url("civicrm/admin/domain", 'action=update&reset=1');
      CRM_Core_Error::fatal(ts('The site administrator needs to enter a valid \'FROM Email Address\' in <a href="%1">Administer CiviCRM &raquo; Configure &raquo; Domain Information</a>. The email address used may need to be a valid mail account with your email service provider.', array(1 => $fixUrl)));
    }

    $receiptFrom = '"' . $domainEmailName . '" <' . $domainEmailAddress . '>';

    // get recipient (supporter) name and email
    $params = array('id' => $pcpId);
    CRM_Core_DAO::commonRetrieve('CRM_PCP_DAO_PCP', $params, $pcpInfo);
    list($name, $address) = CRM_Contact_BAO_Contact_Location::getEmailDetails($pcpInfo['contact_id']);

    // get pcp block info
    list($blockId, $eid) = self::getPcpBlockEntityId($pcpId, $component);
    $params = array('id' => $blockId);
    CRM_Core_DAO::commonRetrieve('CRM_PCP_DAO_PCPBlock', $params, $pcpBlockInfo);

    // assign urls required in email template
    if ($pcpStatus[$newStatus] == 'Approved') {
      $tplParams['isTellFriendEnabled'] = $pcpBlockInfo['is_tellfriend_enabled'];
      if ($pcpBlockInfo['is_tellfriend_enabled']) {
        $pcpTellFriendURL = CRM_Utils_System::url('civicrm/friend',
          "reset=1&eid=$pcpId&blockId=$blockId&pcomponent=pcp",
          TRUE, NULL, FALSE, TRUE
        );
        $tplParams['pcpTellFriendURL'] = $pcpTellFriendURL;
      }
    }
    $pcpInfoURL = CRM_Utils_System::url('civicrm/pcp/info',
      "reset=1&id=$pcpId",
      TRUE, NULL, FALSE, TRUE
    );
    $tplParams['pcpInfoURL'] = $pcpInfoURL;
    $tplParams['contribPageTitle'] = $contribPageTitle;
    if ($emails = CRM_Utils_Array::value('notify_email', $pcpBlockInfo)) {
      $emailArray = explode(',', $emails);
      $tplParams['pcpNotifyEmailAddress'] = $emailArray[0];
    }
    // get appropriate message based on status
    $tplParams['pcpStatus'] = $pcpStatus[$newStatus];

    $tplName = $isInitial ? 'pcp_supporter_notify' : 'pcp_status_change';

    require_once 'CRM/Core/BAO/MessageTemplates.php';
    list($sent, $subject, $message, $html) = CRM_Core_BAO_MessageTemplates::sendTemplate(
      array(
        'groupName' => 'msg_tpl_workflow_contribution',
        'valueName' => $tplName,
        'contactId' => $pcpInfo['contact_id'],
        'tplParams' => $tplParams,
        'from' => $receiptFrom,
        'toName' => $name,
        'toEmail' => $address,
      )
    );
    return $sent;
  }

  /**
   * Function to Enable / Disable the campaign page
   *
   * @param int $id campaign page id
   *
   * @return null
   * @access public
   * @static
   *
   */
  static
  function setDisable($id, $is_active) {
    return CRM_Core_DAO::setFieldValue('CRM_PCP_DAO_PCP', $id, 'is_active', $is_active);
  }

  /**
   * Function to get pcp block is active
   *
   * @param int $id campaign page id
   *
   * @return int
   * @access public
   * @static
   *
   */
  static
  function getStatus($pcpId, $component) {
    $query = "
         SELECT pb.is_active
         FROM civicrm_pcp pcp
         LEFT JOIN civicrm_pcp_block pb ON ( pcp.page_id = pb.entity_id )
         WHERE pcp.id = %1
         AND pb.entity_table = %2";

    $entity_table = self::getPcpEntityTable($component);

    $params = array(1 => array($pcpId, 'Integer'), 2 => array($entity_table, 'String'));
    return CRM_Core_DAO::singleValueQuery($query, $params);
  }

  /**
   * Function to get pcp block is enabled for component page
   *
   * @param int $id contribution page id
   *
   * @return String
   * @access public
   * @static
   *
   */
  static
  function getPcpBlockStatus($pageId, $component) {
    $query = "
     SELECT pb.link_text as linkText
     FROM civicrm_contribution_page cp 
          LEFT JOIN civicrm_pcp_block pb ON ( cp.id = pb.entity_id AND pb.entity_table = %2 )
     WHERE pb.is_active = 1 AND cp.id = %1";

    $entity_table = self::getPcpEntityTable($component);

    $params = array(1 => array($pageId, 'Integer'), 2 => array($entity_table, 'String'));
    return CRM_Core_DAO::singleValueQuery($query, $params);
  }

  /**
   * Function to find out if the PCP block is in use by one or more PCP page
   *
   * @param int $id pcp block id
   *
   * @return Boolean
   * @access public
   * @static
   *
   */
  static
  function getPcpBlockInUse($id) {
    $query = "
     SELECT count(*)
     FROM civicrm_pcp pcp
     WHERE pcp.pcp_block_id = %1";

    $params = array(1 => array($id, 'Integer'));
    $result = CRM_Core_DAO::singleValueQuery($query, $params);
    return $result > 0;
  }

  /**
   * Function to get email is enabled for supporter's profile
   *
   * @param int $id supporter's profile id
   *
   * @return boolean
   * @access public
   * @static
   *
   */
  static
  function checkEmailProfile($profileId) {
    $query = "
SELECT field_name
FROM civicrm_uf_field
WHERE field_name like 'email%' And is_active = 1 And uf_group_id = %1";

    $params = array(1 => array($profileId, 'Integer'));
    $dao = CRM_Core_DAO::executeQuery($query, $params);
    if (!$dao->fetch()) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Function to obtain the title of page associated with a pcp
   *
   * @param int $id campaign page id
   *
   * @return int
   * @access public
   * @static
   *
   */
  static
  function getPcpPageTitle($pcpId, $component) {
    if ($component == 'contribute') {
      $query = "
  SELECT cp.title
  FROM civicrm_pcp pcp
  LEFT JOIN civicrm_contribution_page as cp ON ( cp.id =  pcp.page_id )
  WHERE pcp.id = %1";
    }
    elseif ($component == 'event') {
      $query = "
  SELECT ce.title
  FROM civicrm_pcp pcp
  LEFT JOIN civicrm_event as ce ON ( ce.id =  pcp.page_id )
  WHERE pcp.id = %1";
    }

    $params = array(1 => array($pcpId, 'Integer'));
    return CRM_Core_DAO::singleValueQuery($query, $params);
  }

  /**
   * Function to get pcp block & entity id given pcp id
   *
   * @param int $id campaign page id
   *
   * @return String
   * @access public
   * @static
   *
   */
  static
  function getPcpBlockEntityId($pcpId, $component) {
    $entity_table = self::getPcpEntityTable($component);

    $query = "
SELECT pb.id as pcpBlockId, pb.entity_id
FROM civicrm_pcp pcp 
LEFT JOIN civicrm_pcp_block pb ON ( pb.entity_id = pcp.page_id AND pb.entity_table = %2 )
WHERE pcp.id = %1";

    $params = array(1 => array($pcpId, 'Integer'), 2 => array($entity_table, 'String'));
    $dao = CRM_Core_DAO::executeQuery($query, $params);
    if ($dao->fetch()) {
      return array($dao->pcpBlockId, $dao->entity_id);
    }

    return array();
  }

  /**
   * Function to get pcp entity table given a component.
   *
   * @param int $id campaign page id
   *
   * @return String
   * @access public
   * @static
   *
   */
  static
  function getPcpEntityTable($component) {
    $entity_table_map = array(
      'event' => 'civicrm_event',
      'civicrm_event' => 'civicrm_event',
      'contribute' => 'civicrm_contribution_page',
      'civicrm_contribution_page' => 'civicrm_contribution_page',
    );
    return isset($entity_table_map[$component]) ? $entity_table_map[$component] : FALSE;
  }

  /**
   * Function to get supporter profile id
   *
   * @param int $contributionPageId contribution page id
   *
   * @return int
   * @access public
   *
   */
  public function getSupporterProfileId($component_id, $component = 'contribute') {
    $entity_table = self::getPcpEntityTable($component);

    $query = "
SELECT pcp.supporter_profile_id
FROM civicrm_pcp_block pcp 
INNER JOIN civicrm_uf_group ufgroup 
      ON pcp.supporter_profile_id = ufgroup.id 
      WHERE pcp.entity_id = %1
      AND pcp.entity_table = %2
      AND ufgroup.is_active = 1";

    $params = array(1 => array($component_id, 'Integer'), 2 => array($entity_table, 'String'));
    if (!$supporterProfileId = CRM_Core_DAO::singleValueQuery($query, $params)) {
      CRM_Core_Error::fatal(ts('Supporter profile is not set for this Personal Campaign Page or the profile is disabled. Please contact the site administrator if you need assistance.'));
    }
    else {
      return $supporterProfileId;
    }
  }
}

