{*
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
*}
<h3>{ts}Cancel Auto-renewal Option for {$membershipType} Membership{/ts}</h3>
<div class="crm-block crm-form-block crm-auto-renew-membership-cancellation">
<div class="crm-submit-buttons">{include file="CRM/common/formButtons.tpl" location="top"}</div>
<div class="messages status">
          <div class="icon inform-icon"></div>       
          {ts}Click the button below if you want to cancel the auto-renewal option for your {$membershipType} membership? This will not cancel your membership. However you will need to arrange payment for renewal when your membership expires.{/ts}  
</div>
<div class="crm-submit-buttons">{include file="CRM/common/formButtons.tpl" location="top"}</div>
</div>
