<?php // no direct access
/*
 +--------------------------------------------------------------------+
 | CiviCRM version 4.1                                                |
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
/*
 * Copyright (C) 2009 Elin Waring
 * Licensed to CiviCRM under the Academic Free License version 3.0.
 */
defined('_JEXEC') or die('Restricted access'); ?>
<?php if ( $this->params->def( 'show_page_title', 1 ) ) : ?>
	<div class="componentheading <?php echo $this->params->get( 'pageclass_sfx' ); ?>">
		<?php echo $this->escape($this->params->get('page_title')); ?>
	</div>
<?php endif; ?>

<?php if ( ($this->params->def('image', -1) != -1) || $this->params->def('show_comp_description', 1) ) : ?>
<table width="100%" cellpadding="4" cellspacing="0" border="0" align="center" class="contentpane<?php echo $this->params->get( 'pageclass_sfx' ); ?>">
<tr>
	<td valign="top" class="contentdescription <?php echo $this->params->get( 'pageclass_sfx' ); ?>">
	<?php
		if ( isset($this->image) ) :  echo $this->image; endif;
		echo $this->params->get('comp_description');
	?>
	</td>
</tr>
</table>
<?php endif; ?>
/* wrap in crm-container div so crm styles are used */
<div id="crm-container" lang="{$config->lcMessages|truncate:2:"":true}" xml:lang="{$config->lcMessages|truncate:2:"":true}">

  /*create is mode 1*/   
    <?php strip ?>
    <?php if $help_pre && $action neq 4 ?>
    <div class="messages help"><?php echo $this->$help_pre ?></div>
    <?php endif; ?>

    <?php include file="CRM/common/CMSUser.tpl" ?>

    {assign var=zeroField value="Initial Non Existent Fieldset"}
    {assign var=fieldset  value=$zeroField}
    {foreach from=$fields item=field key=fieldName}

		<?php if $field.groupTitle != $fieldset ?>
			<?php if $fieldset != $zeroField ?>
			   </table>
				<?php if $groupHelpPost ?>
				  <div class="messages help"><?php echo $this->$groupHelpPost ?></div>
			   <?php endif; ?>

	  
				  </fieldset>
				  </div>
			<?php endif; ?>

   
           {assign var="groupId" value="id_"|cat:$field.group_id}
			<?php if $context neq 'dialog' ?>
				  <div id="<?php echo $this->$groupId ?>_show" class="section-hidden section-hidden-border">
				  <a href="#" onclick="hide('<?php echo $this->$groupId ?>_show'); show('<?php echo $this->$groupId ?>'); return false;"><img src="<?php echo $this->$config->resourceBase ?>i/TreePlus.gif" class="action-icon" alt="{ts}open section{/ts}"/></a><label>{ts}{$field.groupTitle}{/ts}</label><br />
				   </div>

				  <div id="<?php echo $this->$groupId} ?>">
				  <fieldset><legend><a href="#" onclick="hide('<?php echo $this->$groupId ?>'); show('<?php echo $this->$groupId ?>_show'); return false;"><img src="<?php echo $this->$config->resourceBase ?>i/TreeMinus.gif" class="action-icon" alt="{ts}close section{/ts}"/></a>{ts}{$field.groupTitle}{/ts}</legend>
			   {else}
				  <div>
			  <fieldset><legend>{ts}{$field.groupTitle}{/ts}</legend>
			<?php endif; ?>	
		<?php endif; ?>
			{assign var=fieldset  value=`$field.groupTitle`}
			{assign var=groupHelpPost  value=`$field.groupHelpPost`}
		<?php if  $field.groupHelpPre ?>
				<div class="messages help">{$field.groupHelpPre}</div>
			<?php endif; ?>
			<table class="form-layout-compressed">
		<?php endif; ?>

		{assign var=n value=$field.name}
	   <?php if $field.options_per_line ?>
		<tr>
        <td class="option-label">{$form.$n.label}</td>
        <td>
	    {assign var="count" value="1"}
        {strip}
        <table class="form-layout-compressed">
        <tr>
          /* sort by fails for option per line. Added a variable to iterate through the element array*/
		  {assign var="index" value="1"}
          {foreach name=outer key=key item=item from=$form.$n}
          <?php if $index < 10 ?>
              {assign var="index" value=`$index+1`}
          {else}
              <td class="labels font-light">{$form.$n.$key.html}</td>
              <?php if $count == $field.options_per_line ?>
                  </tr>
                   <tr>
                   {assign var="count" value="1"}
              {else}
          	       {assign var="count" value=`$count+1`}
              <?php endif; ?>
          <?php endif; ?>  
	{/foreach}
        </tr>
        </table>
        {/strip}
        </td>
    </tr>
	{else}
        <tr>
           <td class="label">{$form.$n.label}</td>
           <td>
           <?php if $n|substr:0:3 eq 'im-' ?>
             {assign var="provider" value=$n|cat:"-provider_id"}
             {$form.$provider.html}&nbsp;
           <?php endif; ?>
           {$form.$n.html}
           </td>
        </tr>
	  {if $form.$n.type eq 'file'}
	      <tr><td class="label"></td><td>{$customFiles.$n.displayURL}</td></tr>
	      <tr><td class="label"></td><td>{$customFiles.$n.deleteURL}</td></tr>
	  <?php endif; ?> 
	<?php endif; ?>
        /* Show explanatory text for field if not in 'view' mode */
        <?php if $field.help_post && $action neq 4 && $form.$n.html ?><tr><td>&nbsp;</td><td class="description">{$field.help_post}</td></tr>
        <?php endif; ?>

    {/foreach}

        <?php if $addToGroupId ?>
	        <tr><td class="label">{$form.group[$addToGroupId].label}</td><td>{$form.group[$addToGroupId].html}</td></tr>
        <?php endif; ?>

    <?php if $isCaptcha   ?>
        {include file='CRM/common/ReCAPTCHA.tpl'}
     <?php endif; ?>

    </table>

    <?php if $field.groupHelpPost ?>
        <div class="messages help">{$field.groupHelpPost}</div>
    <?php endif; ?>

 
        </fieldset>
        </div>


     <?php if $help_post && $action neq 4 ?>
	 <br /><div class="messages help">{$help_post}</div>
	 <?php endif; ?>
    {/strip}
</div> /* end crm-container div */

<script type="text/javascript">
  <?php if $mode ne 8 and $context ne 'dialog' ?>

    var showBlocks = new Array({$showBlocks});
    var hideBlocks = new Array({$hideBlocks});

    /* hide and display the appropriate blocks as directed by the php code */
    on_load_init_blocks( showBlocks, hideBlocks );
    
  <?php endif; ?>
