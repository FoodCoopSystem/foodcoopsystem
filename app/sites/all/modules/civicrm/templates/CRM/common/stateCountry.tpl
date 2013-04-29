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
{if $config->stateCountryMap}
<script language="JavaScript" type="text/javascript">
{foreach from=$config->stateCountryMap item=stateCountryMap}
{if $stateCountryMap.country && $stateCountryMap.state_province}
{literal}
cj(function()
{
{/literal}
        countryID       = "#{$stateCountryMap.country}"
	    stateProvinceID = "#{$stateCountryMap.state_province}"
        callbackURL     = "{crmURL p='civicrm/ajax/jqState' h=0}"
{literal}
	cj(countryID).chainSelect(stateProvinceID, callbackURL, null );
});
{/literal}
{/if}
{if $stateCountryMap.state_province && $stateCountryMap.county}
{literal}
cj(function()
{
{/literal}
	    stateProvinceID = "#{$stateCountryMap.state_province}"
        countyID       = "#{$stateCountryMap.county}"
        callbackURL     = "{crmURL p='civicrm/ajax/jqCounty' h=0}"
{literal}
	cj(stateProvinceID).chainSelect(countyID, callbackURL, null );
});
{/literal}
{/if}
{/foreach}
</script>
{/if}
