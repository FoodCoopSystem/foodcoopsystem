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
<div class="crm-block crm-form-block crm-map-form-block">
<div id="help">
    {ts}CiviCRM includes plugins for several mapping and geocoding web services. These services allow your users to display contact and event location addresses on a map.{/ts} {help id='map-intro-id'}
    <div class="status-removed"><div class="icon alert-icon"></div> &nbsp; {ts}Yahoo recently discontinued their geocoding and mapping API service. If you previously used Yahoo, you will need to select and configure an alternate service in order to continue using geocoding/mapping tools.{/ts}</div>
</div>
    <div class="crm-submit-buttons">{include file="CRM/common/formButtons.tpl" location="top"}</div>
    <table class="form-layout-compressed">
         <tr class="crm-map-form-block-mapProvider">
             <td>{$form.mapProvider.label}</td>
             <td>{$form.mapProvider.html}<br />
             <span class="description">{ts}Choose the mapping provider that has the best coverage for the majority of your contact addresses.{/ts}</span></td>
         </tr>
         <tr class="crm-map-form-block-mapAPIKey">
             <td>{$form.mapAPIKey.label}</td>
             <td>{$form.mapAPIKey.html|crmReplace:class:huge}<br />
             <span class="description">{ts}Enter your API Key or Application ID.{/ts}</span></td>
         </tr>
         <tr class="crm-map-form-block-geoProvider">
             <td>{$form.geoProvider.label}</td>
             <td>{$form.geoProvider.html}<br />
             <span class="description">{ts}You may choose a different webservice for geocoding. This is required if there is no geo-coding plugin for your selected mapping provider. You can leave the Geocoding fields blank if you are using Google as your mapping provider.{/ts}</span></td>
         </tr>
         <tr class="crm-map-form-block-geoAPIKey">
             <td>{$form.geoAPIKey.label}</td>
             <td>{$form.geoAPIKey.html|crmReplace:class:huge}<br />
             <span class="description">{ts}Enter the API key or Application ID associated with your geocoding provider.{/ts}</span></td>
         </tr>
    </table>
    <div class="crm-submit-buttons">{include file="CRM/common/formButtons.tpl" location="bottom"}</div>
</div>
{literal}
<script type="text/javascript">
showHideMapAPIkey( cj('#mapProvider').val( ) );
showHideGeoAPIkey( cj('#geoProvider').val( ) );

function showHideMapAPIkey( mapProvider ) {
  if ( mapProvider && ( mapProvider == 'Google' ||  mapProvider == 'OpenStreetMaps' ) ) {
    cj('#Mapping tr.crm-map-form-block-mapAPIKey').hide( );
  } else {
    cj('#Mapping tr.crm-map-form-block-mapAPIKey').show( );
  }
}

function showHideGeoAPIkey( geoProvider ) {
  if ( geoProvider && geoProvider == 'Google' ) {
    cj('#Mapping tr.crm-map-form-block-geoAPIKey').hide( );
  } else {
    cj('#Mapping tr.crm-map-form-block-geoAPIKey').show( );
  }
}
</script>
{/literal}