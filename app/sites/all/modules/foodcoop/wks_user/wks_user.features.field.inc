<?php
/**
 * @file
 * wks_user.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function wks_user_field_default_fields() {
  $fields = array();

  // Exported field: 'commerce_order-commerce_order-commerce_customer_billing'.
  $fields['commerce_order-commerce_order-commerce_customer_billing'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(
        0 => 'commerce_order',
      ),
      'field_name' => 'commerce_customer_billing',
      'foreign keys' => array(
        'profile_id' => array(
          'columns' => array(
            'profile_id' => 'profile_id',
          ),
          'table' => 'commerce_customer',
        ),
      ),
      'indexes' => array(
        'profile_id' => array(
          0 => 'profile_id',
        ),
      ),
      'locked' => '0',
      'module' => 'commerce_customer',
      'settings' => array(
        'options_list_limit' => 50,
        'profile_type' => 'billing',
      ),
      'translatable' => '0',
      'type' => 'commerce_customer_profile_reference',
    ),
    'field_instance' => array(
      'bundle' => 'commerce_order',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'administrator' => array(
          'label' => 'above',
          'module' => 'commerce_customer',
          'settings' => array(),
          'type' => 'commerce_customer_profile_reference_display',
          'weight' => -5,
        ),
        'customer' => array(
          'label' => 'above',
          'module' => 'commerce_customer',
          'settings' => array(),
          'type' => 'commerce_customer_profile_reference_display',
          'weight' => -5,
        ),
        'default' => array(
          'label' => 'hidden',
          'module' => 'commerce_customer',
          'settings' => array(),
          'type' => 'commerce_customer_profile_reference_display',
          'weight' => '3',
        ),
      ),
      'entity_type' => 'commerce_order',
      'field_name' => 'commerce_customer_billing',
      'label' => 'Dane do płatności',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'commerce_customer',
        'settings' => array(),
        'type' => 'commerce_customer_profile_manager',
        'weight' => '2',
      ),
    ),
  );

  // Exported field: 'user-user-field_adressfield'.
  $fields['user-user-field_adressfield'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_adressfield',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'addressfield',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'addressfield',
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'default_value' => array(
        0 => array(
          'element_key' => 'user|user|field_adressfield|und|0',
          'thoroughfare' => '',
          'premise' => '',
          'postal_code' => '',
          'locality' => '',
          'country' => 'PL',
          'first_name' => '',
          'last_name' => '',
        ),
      ),
      'deleted' => '0',
      'description' => 'Podaj imię i nazwisko',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'addressfield',
          'settings' => array(
            'format_handlers' => array(
              0 => 'address',
            ),
            'use_widget_handlers' => 1,
          ),
          'type' => 'addressfield_default',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'user',
      'field_name' => 'field_adressfield',
      'label' => 'Imię i nazwisko',
      'required' => 1,
      'settings' => array(
        'user_register_form' => 1,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'addressfield',
        'settings' => array(
          'available_countries' => array(
            'PL' => 'PL',
          ),
          'format_handlers' => array(
            'address-hide-country' => 'address-hide-country',
            'name-full' => 'name-full',
            'address' => 0,
            'organisation' => 0,
            'name-oneline' => 0,
            'copy_billing_address' => 0,
          ),
        ),
        'type' => 'addressfield_standard',
        'weight' => '6',
      ),
    ),
  );

  // Exported field: 'user-user-field_dzielnica'.
  $fields['user-user-field_dzielnica'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dzielnica',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '100',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Dzielnica w której mieszkasz.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '2',
        ),
      ),
      'entity_type' => 'user',
      'field_name' => 'field_dzielnica',
      'label' => 'Dzielnica',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => 1,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '8',
      ),
    ),
  );

  // Exported field: 'user-user-field_prawo_jazdy'.
  $fields['user-user-field_prawo_jazdy'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_prawo_jazdy',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => 'Nie mam prawa jazdy',
          1 => 'Mam prawo jazdy',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_boolean',
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'default_value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '4',
        ),
      ),
      'entity_type' => 'user',
      'field_name' => 'field_prawo_jazdy',
      'label' => 'Prawo jazdy',
      'required' => 0,
      'settings' => array(
        'user_register_form' => 1,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 0,
        ),
        'type' => 'options_onoff',
        'weight' => '10',
      ),
    ),
  );

  // Exported field: 'user-user-field_samoch_d'.
  $fields['user-user-field_samoch_d'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_samoch_d',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => 'Nie mam samochodu',
          1 => 'Mam samochód',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_boolean',
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'default_value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'deleted' => '0',
      'description' => 'Czy posiadasz lub możesz korzystać samochodu?',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '3',
        ),
      ),
      'entity_type' => 'user',
      'field_name' => 'field_samoch_d',
      'label' => 'Samochód',
      'required' => 0,
      'settings' => array(
        'user_register_form' => 1,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 0,
        ),
        'type' => 'options_onoff',
        'weight' => '9',
      ),
    ),
  );

  // Exported field: 'user-user-field_telefon'.
  $fields['user-user-field_telefon'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_telefon',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '12',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Podaj swój telefon.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '1',
        ),
      ),
      'entity_type' => 'user',
      'field_name' => 'field_telefon',
      'label' => 'Telefon',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => 1,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '7',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Czy posiadasz lub możesz korzystać samochodu?');
  t('Dane do płatności');
  t('Dzielnica');
  t('Dzielnica w której mieszkasz.');
  t('Imię i nazwisko');
  t('Podaj imię i nazwisko');
  t('Podaj swój telefon.');
  t('Prawo jazdy');
  t('Samochód');
  t('Telefon');

  return $fields;
}
