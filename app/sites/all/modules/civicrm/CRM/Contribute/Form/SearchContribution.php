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

require_once 'CRM/Core/Form.php';
require_once 'CRM/Contribute/PseudoConstant.php';
class CRM_Contribute_Form_SearchContribution extends CRM_Core_Form {

  /**
   * Build the form
   *
   * @access public
   *
   * @return void
   */
  public function buildQuickForm() {
    $attributes = CRM_Core_DAO::getAttribute('CRM_Contribute_DAO_ContributionPage', 'title');
    $attributes['style'] = 'width: 90%';

    $this->add('text', 'title', ts('Find'), $attributes);

    $contribution_type = CRM_Contribute_PseudoConstant::contributionType();
    foreach ($contribution_type as $contributionId => $contributionName) {
      $this->addElement('checkbox', "contribution_type_id[$contributionId]", 'Contribution Type', $contributionName);
    }

    require_once 'CRM/Campaign/BAO/Campaign.php';
    CRM_Campaign_BAO_Campaign::addCampaignInComponentSearch($this);

    $this->addButtons(array(
        array(
          'type' => 'refresh',
          'name' => ts('Search'),
          'isDefault' => TRUE,
        ),
      ));
  }

  function postProcess() {
    $params = $this->controller->exportValues($this->_name);
    $parent = $this->controller->getParent();
    $parent->set('searchResult', 1);
    if (!empty($params)) {
      $fields = array('title', 'contribution_type_id', 'campaign_id');
      foreach ($fields as $field) {
        if (isset($params[$field]) &&
          !CRM_Utils_System::isNull($params[$field])
        ) {
          $parent->set($field, $params[$field]);
        }
        else {
          $parent->set($field, NULL);
        }
      }
    }
  }
}

