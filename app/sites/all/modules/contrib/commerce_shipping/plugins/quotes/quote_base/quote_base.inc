<?php

/**
 * A fairly typical ctools plugin array for an object oriented plugin.
 */
$plugin = array(
  // Title and description will be used through out the user interface and in
  // some menu items.
  'title' => t('Base implementation for quotes'),
  'description' => t('This plugin wont actually do anything but should instead be used as a parent.'),
  // The handler for our plugin, should be in the same directory as this
  // include file and in the format of name.class.php
  'handler' => array(
    'class' => 'CommerceShippingQuote',
    // Classes that extend another class can be created using the following
    // line. Reference the plugin file, not the class.
    // parent => 'base_plugin_name',
  ),
  // A special commerce_shipping function to avoid creating rules for plugins
  // that should be extended.
  'create_rule' => FALSE,
);
