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

require_once 'CRM/Report/Form.php';
require_once 'CRM/Contribute/PseudoConstant.php';
require_once 'CRM/Core/OptionGroup.php';
class CRM_Report_Form_Contribute_Detail extends CRM_Report_Form {
  protected $_addressField = FALSE;

  protected $_emailField = FALSE;
  protected $_emailFieldHonor = FALSE;

  protected $_nameFieldHonor = FALSE;

  protected $_summary = NULL;

  protected $_customGroupExtends = array(
    'Contribution'); function __construct() {
    $this->_columns = array(
      'civicrm_contact' =>
      array(
        'dao' => 'CRM_Contact_DAO_Contact',
        'fields' =>
        array(
          'sort_name' =>
          array('title' => ts('Donor Name'),
            'required' => TRUE,
            'no_repeat' => TRUE,
          ),
          'id' =>
          array(
            'no_display' => TRUE,
            'required' => TRUE,
          ),
        ),
        'filters' =>
        array(
          'sort_name' =>
          array('title' => ts('Donor Name'),
            'operator' => 'like',
          ),
          'id' =>
          array('title' => ts('Contact ID'),
            'no_display' => TRUE,
          ),
        ),
        'grouping' => 'contact-fields',
      ),
      'civicrm_email' =>
      array(
        'dao' => 'CRM_Core_DAO_Email',
        'fields' =>
        array(
          'email' =>
          array('title' => ts('Donor Email'),
            'default' => TRUE,
            'no_repeat' => TRUE,
          ),
        ),
        'grouping' => 'contact-fields',
      ),
      'civicrm_phone' =>
      array(
        'dao' => 'CRM_Core_DAO_Phone',
        'fields' =>
        array(
          'phone' =>
          array('title' => ts('Donor Phone'),
            'default' => TRUE,
            'no_repeat' => TRUE,
          ),
        ),
        'grouping' => 'contact-fields',
      ),
      'civicrm_contact_honor' =>
      array(
        'dao' => 'CRM_Contact_DAO_Contact',
        'fields' =>
        array(
          'sort_name_honor' =>
          array('title' => ts('Honoree Name'),
            'name' => 'sort_name',
            'alias' => 'contacthonor',
            'default' => FALSE,
            'no_repeat' => TRUE,
          ),
          'id_honor' =>
          array(
            'no_display' => TRUE,
            'title' => ts('Honoree ID'),
            'name' => 'id',
            'alias' => 'contacthonor',
            'required' => TRUE,
          ),
        ),
      ),
      'civicrm_email_honor' =>
      array(
        'dao' => 'CRM_Core_DAO_Email',
        'fields' =>
        array(
          'email_honor' =>
          array('title' => ts('Honoree Email'),
            'name' => 'email',
            'alias' => 'emailhonor',
            'default' => FALSE,
            'no_repeat' => TRUE,
          ),
        ),
        'grouping' => 'contact-fields',
      ),
      'civicrm_contribution' =>
      array(
        'dao' => 'CRM_Contribute_DAO_Contribution',
        'fields' =>
        array(
          'contribution_id' => array(
            'name' => 'id',
            'no_display' => TRUE,
            'required' => TRUE,
          ),
          'contribution_type_id' => array('title' => ts('Contribution Type'),
            'default' => TRUE,
          ),
          'contribution_status_id' => array('title' => ts('Contribution Status'),
          ),
          'payment_instrument_id' => array('title' => ts('Payment Type'),
          ),
          'trxn_id' => NULL,
          'receive_date' => array('default' => TRUE),
          'receipt_date' => NULL,
          'honor_type_id' => array('title' => ts('Honor Type'),
            'default' => FALSE,
          ),
          'fee_amount' => NULL,
          'net_amount' => NULL,
          'total_amount' => array('title' => ts('Amount'),
            'required' => TRUE,
            'statistics' =>
            array('sum' => ts('Amount')),
          ),
        ),
        'filters' =>
        array(
          'receive_date' =>
          array('operatorType' => CRM_Report_Form::OP_DATE),
          'contribution_type_id' =>
          array('title' => ts('Contribution Type'),
            'operatorType' => CRM_Report_Form::OP_MULTISELECT,
            'options' => CRM_Contribute_PseudoConstant::contributionType(),
          ),
          'payment_instrument_id' =>
          array('title' => ts('Payment Type'),
            'operatorType' => CRM_Report_Form::OP_MULTISELECT,
            'options' => CRM_Contribute_PseudoConstant::paymentInstrument(),
          ),
          'contribution_status_id' =>
          array('title' => ts('Contribution Status'),
            'operatorType' => CRM_Report_Form::OP_MULTISELECT,
            'options' => CRM_Contribute_PseudoConstant::contributionStatus(),
            'default' => array(1),
          ),
          'total_amount' =>
          array('title' => ts('Contribution Amount')),
        ),
        'grouping' => 'contri-fields',
      ),
      'civicrm_group' =>
      array(
        'dao' => 'CRM_Contact_DAO_GroupContact',
        'alias' => 'cgroup',
        'filters' =>
        array(
          'gid' =>
          array(
            'name' => 'group_id',
            'title' => ts('Group'),
            'operatorType' => CRM_Report_Form::OP_MULTISELECT,
            'group' => TRUE,
            'options' => CRM_Core_PseudoConstant::group(),
          ),
        ),
      ),
      'civicrm_contribution_ordinality' =>
      array(
        'dao' => 'CRM_Contribute_DAO_Contribution',
        'alias' => 'cordinality',
        'filters' =>
        array(
          'ordinality' =>
          array('title' => ts('Contribution Ordinality'),
            'operatorType' => CRM_Report_Form::OP_MULTISELECT,
            'options' => array(
              0 => 'First by Contributor',
              1 => 'Second or Later by Contributor',
            ),
          ),
        ),
      ),
    ) + $this->addAddressFields(FALSE);

    $this->_tagFilter = TRUE;
    parent::__construct();
  }

  function preProcess() {
    parent::preProcess();
  }

  function select() {
    $select = array();

    $this->_columnHeaders = array();
    foreach ($this->_columns as $tableName => $table) {
      if (array_key_exists('fields', $table)) {
        foreach ($table['fields'] as $fieldName => $field) {
          if (CRM_Utils_Array::value('required', $field) ||
            CRM_Utils_Array::value($fieldName, $this->_params['fields'])
          ) {
            if ($tableName == 'civicrm_address') {
              $this->_addressField = TRUE;
            }
            if ($tableName == 'civicrm_email') {
              $this->_emailField = TRUE;
            }
            elseif ($tableName == 'civicrm_email_honor') {
              $this->_emailFieldHonor = TRUE;
            }

            if ($tableName == 'civicrm_contact_honor') {
              $this->_nameFieldHonor = TRUE;
            }

            // only include statistics columns if set
            if (CRM_Utils_Array::value('statistics', $field)) {
              foreach ($field['statistics'] as $stat => $label) {
                switch (strtolower($stat)) {
                  case 'sum':
                    $select[] = "SUM({$field['dbAlias']}) as {$tableName}_{$fieldName}_{$stat}";
                    $this->_columnHeaders["{$tableName}_{$fieldName}_{$stat}"]['title'] = $label;
                    $this->_columnHeaders["{$tableName}_{$fieldName}_{$stat}"]['type'] = $field['type'];
                    $this->_statFields[] = "{$tableName}_{$fieldName}_{$stat}";
                    break;

                  case 'count':
                    $select[] = "COUNT({$field['dbAlias']}) as {$tableName}_{$fieldName}_{$stat}";
                    $this->_columnHeaders["{$tableName}_{$fieldName}_{$stat}"]['title'] = $label;
                    $this->_statFields[] = "{$tableName}_{$fieldName}_{$stat}";
                    break;

                  case 'avg':
                    $select[] = "ROUND(AVG({$field['dbAlias']}),2) as {$tableName}_{$fieldName}_{$stat}";
                    $this->_columnHeaders["{$tableName}_{$fieldName}_{$stat}"]['type'] = $field['type'];
                    $this->_columnHeaders["{$tableName}_{$fieldName}_{$stat}"]['title'] = $label;
                    $this->_statFields[] = "{$tableName}_{$fieldName}_{$stat}";
                    break;
                }
              }
            }
            else {
              $select[] = "{$field['dbAlias']} as {$tableName}_{$fieldName}";
              $this->_columnHeaders["{$tableName}_{$fieldName}"]['title'] = CRM_Utils_Array::value('title', $field);
              $this->_columnHeaders["{$tableName}_{$fieldName}"]['type'] = CRM_Utils_Array::value('type', $field);
            }
          }
        }
      }
    }

    $this->_select = "SELECT " . implode(', ', $select) . " ";
  }

  function from() {

    $this->_from = NULL;
    $this->_from = "
        FROM  civicrm_contact      {$this->_aliases['civicrm_contact']} {$this->_aclFrom}
              INNER JOIN civicrm_contribution {$this->_aliases['civicrm_contribution']} 
                      ON {$this->_aliases['civicrm_contact']}.id = {$this->_aliases['civicrm_contribution']}.contact_id AND {$this->_aliases['civicrm_contribution']}.is_test = 0";

    if (!empty($this->_params['ordinality_value'])) {
      $this->_from .= "
              INNER JOIN (SELECT c.id, IF(COUNT(oc.id) = 0, 0, 1) AS ordinality FROM civicrm_contribution c LEFT JOIN civicrm_contribution oc ON c.contact_id = oc.contact_id AND oc.receive_date < c.receive_date GROUP BY c.id) {$this->_aliases['civicrm_contribution_ordinality']} 
                      ON {$this->_aliases['civicrm_contribution_ordinality']}.id = {$this->_aliases['civicrm_contribution']}.id";
    }

    $this->_from .= "
               LEFT JOIN  civicrm_phone {$this->_aliases['civicrm_phone']} 
                      ON ({$this->_aliases['civicrm_contact']}.id = {$this->_aliases['civicrm_phone']}.contact_id AND 
                         {$this->_aliases['civicrm_phone']}.is_primary = 1)";

    if ($this->_addressField OR (!empty($this->_params['state_province_id_value']) OR !empty($this->_params['country_id_value']))) {
      $this->_from .= "
            LEFT JOIN civicrm_address {$this->_aliases['civicrm_address']} 
                   ON {$this->_aliases['civicrm_contact']}.id = {$this->_aliases['civicrm_address']}.contact_id AND 
                      {$this->_aliases['civicrm_address']}.is_primary = 1\n";
    }

    if ($this->_emailField) {
      $this->_from .= " 
            LEFT JOIN civicrm_email {$this->_aliases['civicrm_email']} 
                   ON {$this->_aliases['civicrm_contact']}.id = {$this->_aliases['civicrm_email']}.contact_id AND 
                      {$this->_aliases['civicrm_email']}.is_primary = 1\n";
    }

    // include Honor name field
    if ($this->_nameFieldHonor) {
      $this->_from .= "
            LEFT JOIN civicrm_contact contacthonor 
                      ON contacthonor.id = {$this->_aliases['civicrm_contribution']}.honor_contact_id";
    }
    // include Honor email field
    if ($this->_emailFieldHonor) {
      $this->_from .= "
            LEFT JOIN civicrm_email emailhonor 
                      ON emailhonor.contact_id = {$this->_aliases['civicrm_contribution']}.honor_contact_id
                      AND emailhonor.is_primary = 1\n";
    }
  }

  function groupBy() {
    $this->_groupBy = " GROUP BY {$this->_aliases['civicrm_contact']}.id, {$this->_aliases['civicrm_contribution']}.id ";
  }

  function orderBy() {
    $this->_orderBy = " ORDER BY {$this->_aliases['civicrm_contact']}.sort_name, {$this->_aliases['civicrm_contact']}.id ";
  }

  function statistics(&$rows) {
    $statistics = parent::statistics($rows);

    $select = "
        SELECT COUNT({$this->_aliases['civicrm_contribution']}.total_amount ) as count,
               SUM( {$this->_aliases['civicrm_contribution']}.total_amount ) as amount,
               ROUND(AVG({$this->_aliases['civicrm_contribution']}.total_amount), 2) as avg
        ";

    $sql = "{$select} {$this->_from} {$this->_where}";
    $dao = CRM_Core_DAO::executeQuery($sql);

    if ($dao->fetch()) {
      $statistics['counts']['amount'] = array(
        'value' => $dao->amount,
        'title' => 'Total Amount',
        'type' => CRM_Utils_Type::T_MONEY,
      );
      $statistics['counts']['avg'] = array(
        'value' => $dao->avg,
        'title' => 'Average',
        'type' => CRM_Utils_Type::T_MONEY,
      );
    }

    return $statistics;
  }

  function postProcess() {
    // get the acl clauses built before we assemble the query
    $this->buildACLClause($this->_aliases['civicrm_contact']);
    parent::postProcess();
  }

  function alterDisplay(&$rows) {
    // custom code to alter rows
    $checkList          = array();
    $entryFound         = FALSE;
    $display_flag       = $prev_cid = $cid = 0;
    $contributionTypes  = CRM_Contribute_PseudoConstant::contributionType();
    $contributionStatus = CRM_Contribute_PseudoConstant::contributionStatus();
    $paymentInstruments = CRM_Contribute_PseudoConstant::paymentInstrument();
    $honorTypes         = CRM_Core_OptionGroup::values('honor_type', FALSE, FALSE, FALSE, NULL, 'label');


    foreach ($rows as $rowNum => $row) {
      if (!empty($this->_noRepeats) && $this->_outputMode != 'csv') {
        // don't repeat contact details if its same as the previous row
        if (array_key_exists('civicrm_contact_id', $row)) {
          if ($cid = $row['civicrm_contact_id']) {
            if ($rowNum == 0) {
              $prev_cid = $cid;
            }
            else {
              if ($prev_cid == $cid) {
                $display_flag = 1;
                $prev_cid = $cid;
              }
              else {
                $display_flag = 0;
                $prev_cid = $cid;
              }
            }

            if ($display_flag) {
              foreach ($row as $colName => $colVal) {
                if (in_array($colName, $this->_noRepeats)) {
                  unset($rows[$rowNum][$colName]);
                }
              }
            }
            $entryFound = TRUE;
          }
        }
      }



      // convert donor sort name to link
      if (array_key_exists('civicrm_contact_sort_name', $row) &&
        CRM_Utils_Array::value('civicrm_contact_sort_name', $rows[$rowNum]) &&
        array_key_exists('civicrm_contact_id', $row)
      ) {
        $url = CRM_Utils_System::url("civicrm/contact/view",
          'reset=1&cid=' . $row['civicrm_contact_id'],
          $this->_absoluteUrl
        );
        $rows[$rowNum]['civicrm_contact_sort_name_link'] = $url;
        $rows[$rowNum]['civicrm_contact_sort_name_hover'] = ts("View Contact Summary for this Contact.");
      }

      // convert honoree sort name to link
      if (array_key_exists('civicrm_contact_honor_sort_name_honor', $row) &&
        CRM_Utils_Array::value('civicrm_contact_honor_sort_name_honor', $rows[$rowNum]) &&
        array_key_exists('civicrm_contact_honor_id_honor', $row)
      ) {

        $url = CRM_Utils_System::url("civicrm/contact/view",
          'reset=1&cid=' . $row['civicrm_contact_honor_id_honor'],
          $this->_absoluteUrl
        );
        $rows[$rowNum]['civicrm_contact_honor_sort_name_honor_link'] = $url;
        $rows[$rowNum]['civicrm_contact_honor_sort_name_honor_hover'] = ts("View Contact Summary for Honoree.");
      }

      if ($value = CRM_Utils_Array::value('civicrm_contribution_contribution_type_id', $row)) {
        $rows[$rowNum]['civicrm_contribution_contribution_type_id'] = $contributionTypes[$value];
        $entryFound = TRUE;
      }
      if ($value = CRM_Utils_Array::value('civicrm_contribution_contribution_status_id', $row)) {
        $rows[$rowNum]['civicrm_contribution_contribution_status_id'] = $contributionStatus[$value];
        $entryFound = TRUE;
      }
      if ($value = CRM_Utils_Array::value('civicrm_contribution_payment_instrument_id', $row)) {
        $rows[$rowNum]['civicrm_contribution_payment_instrument_id'] = $paymentInstruments[$value];
        $entryFound = TRUE;
      }
      if ($value = CRM_Utils_Array::value('civicrm_contribution_honor_type_id', $row)) {
        $rows[$rowNum]['civicrm_contribution_honor_type_id'] = $honorTypes[$value];
        $entryFound = TRUE;
      }
      if (($value = CRM_Utils_Array::value('civicrm_contribution_total_amount_sum', $row)) &&
        CRM_Core_Permission::check('access CiviContribute')
      ) {
        $url = CRM_Utils_System::url("civicrm/contact/view/contribution",
          "reset=1&id=" . $row['civicrm_contribution_contribution_id'] . "&cid=" . $row['civicrm_contact_id'] . "&action=view&context=contribution&selectedChild=contribute",
          $this->_absoluteUrl
        );
        $rows[$rowNum]['civicrm_contribution_total_amount_sum_link'] = $url;
        $rows[$rowNum]['civicrm_contribution_total_amount_sum_hover'] = ts("View Details of this Contribution.");
        $entryFound = TRUE;
      }
      $entryFound = $this->alterDisplayAddressFields($row, $rows, $rowNum, 'contribute/detail', 'List all contribution(s) for this ') ? TRUE : $entryFound;

      // skip looking further in rows, if first row itself doesn't
      // have the column we need
      if (!$entryFound) {
        break;
      }
      $lastKey = $rowNum;
    }
  }
}

