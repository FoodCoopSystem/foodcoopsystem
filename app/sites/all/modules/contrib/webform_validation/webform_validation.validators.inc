<?php

/**
 * @file
 * Provides validation functionality and hooks
 */

/**
 * Implements hook_webform_validation_validators().
 *
 * This function returns an array of validators, in the validator key => options array form.
 * Possible options:
 * - name (required): name of the validator
 * - component types (required): defines which component types can be validated by this validator. Specify 'all' to allow all types
 * - custom_error (optional): define whether a user can specify a custom error message upon creating the validation rule.
 * - custom_data (optional): define whether custom data can be added to the validation rule
 * - min_components (optional): define the minimum number of components to be selected for creating a validation rule
 * - max_components (optional): define the maximum number of components to be selected for creating a validation rule
 * - description (optional): provide a descriptive explanation about the validator
 */
function webform_validation_webform_validation_validators() {
  return array(
    'numeric' => array(
      'name' => "Numeric values",
      'component_types' => array(
        'textfield',
        'hidden',
      ),
      'custom_data' => array(
        'label' => t('Specify numeric validation range'),
        'description' => t('Optionally specify the minimum-maximum range to validate the user-entered numeric value against.') . ' ' . t('Usage') . ':'
        . theme('item_list', array('items' => array(t('empty: no value validation'), t('"100": greater than or equal to 100'), t('"|100": less than or equal to 100 (including negative numbers)'), t('"0|100": greater than or equal to 0 &amp; less than or equal to 100'), t('"10|100": greater than or equal to 10 &amp; less than or equal to 100'), t('"-100|-10": greater than or equal to -100 &amp; less than or equal to -10')))),
        'required' => FALSE,
      ),
      'description' => t('Verifies that user-entered values are numeric, with the option to specify min and / or max values.'),
    ),
    'min_length' => array(
      'name' => "Minimum length",
      'component_types' => array(
        'textfield',
        'textarea',
        'email',
        'hidden',
      ),
      'custom_data' => array(
        'label' => t('Minimum number of characters'),
        'description' => t('Specify the minimum number of characters that have to be entered to pass validation.'),
      ),
      'description' => t('Verifies that a user-entered value contains at least the specified number of characters'),
    ),
    'max_length' => array(
      'name' => "Maximum length",
      'component_types' => array(
        'textfield',
        'textarea',
        'email',
        'hidden',
      ),
      'custom_data' => array(
        'label' => t('Maximum number of characters'),
        'description' => t('Specify the maximum number of characters that can be entered to pass validation.'),
      ),
      'description' => t('Verifies that a user-entered value contains at most the specified number of characters'),
    ),
    'min_words' => array(
      'name' => "Minimum number of words",
      'component_types' => array(
        'textfield',
        'textarea',
        'hidden',
      ),
      'custom_data' => array(
        'label' => t('Minimum number of words'),
        'description' => t('Specify the minimum number of words that have to be entered to pass validation. Words are defined as strings of letters separated by spaces.')
      ),
      'description' => t('Verifies that a user-entered value contains at least the specified number of words'),
    ),
    'max_words' => array(
      'name' => "Maximum number of words",
      'component_types' => array(
        'textfield',
        'textarea',
        'hidden',
      ),
      'custom_data' => array(
        'label' => t('Maximum number of words'),
        'description' => t('Specify the maximum number of words that have to be entered to pass validation. Words are defined as strings of letters separated by spaces.')
      ),
      'description' => t('Verifies that a user-entered value contains at most the specified number of words'),
    ),
    'equal' => array(
      'name' => "Equal values",
      'component_types' => array(
        'textfield',
        'email',
        'select',
        'hidden',
      ),
      'min_components' => 2,
      'description' => t('Verifies that all specified components contain equal values'),
    ),
    'unique' => array(
      'name' => "Unique values",
      'component_types' => array(
        'textfield',
        'email',
        'select',
        'hidden',
      ),
      'min_components' => 2,
      'description' => t('Verifies that all specified components contain unique values'),
    ),
    'specific_value' => array(
      'name' => "Specific value(s)",
      'component_types' => array(
        'select',
        'textfield',
        'textarea',
        'email',
        'hidden',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('(Key) value'),
        'description' => t('Specify the specific value(s) you want the component to contain. Separate multiple options by a comma. For components that have keys, use the key value instead.'),
      ),
      'max_components' => 1,
      'description' => t('Verifies that the specified component contains a defined value'),
    ),
    'oneoftwo' => array(
      'name' => "Require at least one of two fields",
      'component_types' => array(
        'textfield',
        'textarea',
        'email',
        'select',
      ),
      'min_components' => 2,
      'max_components' => 2,
      'description' => t('Forces the user to specify / select at least one of two selected webform components'),
    ),
    'oneofseveral' => array(
      'name' => "Require at least one of several fields",
      'component_types' => array(
        'textfield',
        'textarea',
        'email',
        'select',
      ),
      'min_components' => 2,
      'description' => t('Forces the user to specify / select at least one of several selected webform components'),
    ),
    'select_min' => array(
      'name' => "Minimum number of selections required",
      'component_types' => array(
        'select',
      ),
      'custom_data' => array(
        'label' => t('Minimum number of selections'),
        'description' => t('Specify the minimum number of options a user should select.'),
      ),
      'description' => t('Forces the user to select at least a defined number of options from the specified webform components'),
    ),
    'select_max' => array(
      'name' => "Maximum number of selections allowed",
      'component_types' => array(
        'select',
      ),
      'custom_data' => array(
        'label' => t('Maximum number of selections'),
        'description' => t('Specify the maximum number of options a user can select.'),
      ),
      'description' => t('Forces the user to select at most a defined number of options from the specified webform components'),
    ),
    'select_exact' => array(
      'name' => "Exact number of selections required",
      'component_types' => array(
        'select',
      ),
      'custom_data' => array(
        'label' => t('Number of selections'),
        'description' => t('Specify how many options a user can select.'),
      ),
      'description' => t('Forces the user to select exactly the defined number of options from the specified webform components'),
    ),
    'plain_text' => array(
      'name' => "Plain text (disallow tags)",
      'component_types' => array(
        'textfield',
        'textarea',
        'email',
        'hidden',
      ),
      'description' => t("Verifies that user-entered data doesn't contain HTML tags"),
    ),
    'regex' => array(
      'name' => "Regular expression",
      'component_types' => array(
        'textfield',
        'textarea',
        'email',
        'hidden',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Regex code'),
        'description' => t('Specify regex code to validate the user input against.'),
      ),
      'description' => t("Validates user-entered text against a specified regular expression. Note: don't include delimiters such as /."),
    ),
    'must_be_empty' => array(
      'name' => "Must be empty",
      'component_types' => array(
        'textfield',
        'hidden',
      ),
      'description' => t('Verifies that a specified textfield remains empty - Recommended use case: used as an anti-spam measure by hiding the element with CSS'),
    ),
    'blacklist' => array(
      'name' => "Words blacklist",
      'component_types' => array(
        'textfield',
        'textarea',
        'email',
        'hidden',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Blacklisted words'),
        'description' => t('Specify illegal words, seperated by commas. Make sure to escape reserved regex characters with an escape (\) character.'),
      ),
      'description' => t("Validates that user-entered data doesn't contain any of the specified illegal words"),
    ),
   'username' => array(
      'name' => "Must match a username",
      'component_types' => array(
        'textfield',
        'hidden',
      ),
      'description' => t("Validates that user-entered data matches a username"),
    ),
  );
}

/**
 * Implements hook_webform_validation_validate().
 */
function webform_validation_webform_validation_validate($validator_name, $items, $components, $rule) {
  if ($items) {
    $errors = array();
    switch ($validator_name) {
      case 'numeric':
        $num_range = _webform_numeric_check_data($rule['data']);
        foreach ($items as $key => $val) {
          if ($val != '') {
            // first check if the value is numeric
            if (!is_numeric($val)) {
              $errors[$key] = t('%item is not numeric', array('%item' => $components[$key]['name']));
            }

            // now validate the entered numeric value against the validator range settings, if appropriate
            // a. validate min & max
            if (isset($num_range['min']) && isset($num_range['max'])) {
              // validate the min - max range
              if ($val < $num_range['min'] || $val > $num_range['max']) {
                $errors[$key] = t('%item is not within the allowed range %range', array('%item' => $components[$key]['name'], '%range' => str_replace('|' , ' - ', $rule['data'])));
              }
            }
            else {
              // b. validate min
              if (isset($num_range['min'])) {
                if ($val < $num_range['min']) {
                  $errors[$key] = t('%item should be greater than or equal to %val', array('%item' => $components[$key]['name'], '%val' => $num_range['min']));
                }
              }
              // c. validate max
              if (isset($num_range['max'])) {
                if ($val > $num_range['max']) {
                  $errors[$key] = t('%item should be less than or equal to %val', array('%item' => $components[$key]['name'], '%val' => $num_range['max']));
                }
              }
            }
          }
        }
        return $errors;
        break;
      case 'min_length':
        $min_length = $rule['data'];
        foreach ($items as $key => $val) {
          if ($val != '' && (drupal_strlen($val) < $min_length)) {
            $errors[$key] = t('%item needs to be at least %num characters long', array('%item' => $components[$key]['name'], '%num' => $min_length));
          }
        }
        return $errors;
        break;
      case 'max_length':
        $max_length = $rule['data'];
        foreach ($items as $key => $val) {
          if ($val != '' && (drupal_strlen($val) > $max_length)) {
            $errors[$key] = t('%item can be maximum %num characters long', array('%item' => $components[$key]['name'], '%num' => $max_length));
          }
        }
        return $errors;
        break;
      case 'min_words':
 		    $min_words = $rule['data'];
 		    foreach ($items as $key => $val) {
 		      if ($val != '' && (count(preg_split("/[\s]+/", trim($val))) < $min_words)) {
            $error = format_plural($min_words, '%item needs to be at least 1 word long', '%item needs to be at least @count words long', array('%item' => $components[$key]['name']));
 			      $errors[$key] = $error;
 		      }
 		    }
 		    return $errors;
 		    break;
 	    case 'max_words':
 		    $max_words = $rule['data'];
 		    foreach ($items as $key => $val) {
 		      if ($val != '' && (count(preg_split("/[\s]+/", trim($val))) > $max_words)) {
            $error = format_plural($max_words, '%item can be maximum 1 word long', '%item can be maximum @count words long', array('%item' => $components[$key]['name']));
 			      $errors[$key] = $error;
 		      }
 		    }
 		    return $errors;
 		    break;
      case "equal":
        $first_entry_key = key($items);
        $first_entry = array_shift($items);
        $first_entry = _webform_validation_flatten_array($first_entry); // flatten in case of array
        // now check if following components equal the first one
        foreach ($items as $key => $val) {
          $val = _webform_validation_flatten_array($val); // flatten in case of array
          if ($val !== $first_entry) {
            $errors[$key] = t('%item_checked does not match %item_first', array('%item_checked' => $components[$key]['name'], '%item_first' => $components[$first_entry_key]['name']));
          }
        }
        return $errors;
        break;
      case "unique":
        foreach ($items as $key => $val) {
          if (is_array($val)) {
            // make sure to flatten arrays first
            $items[$key] = _webform_validation_flatten_array($val);
          }
          if (empty($items[$key])) {
            // items without a value selected shouldn't be validated
            unset($items[$key]);
          }
        }
        // now we count how many times each value appears, and find out which values appear more than once
        $items_count = array_count_values(array_map('strtolower', array_map('trim', $items)));
        $doubles = array_filter($items_count, create_function('$x', 'return $x > 1;'));
        foreach ($items as $key => $val) {
          if (in_array(strtolower($val), array_keys($doubles))) {
            $errors[$key] = t('The value of %item is not unique', array('%item' => $components[$key]['name']));
          }
        }
        return $errors;
        break;
      case "specific_value":
        $specific_values = explode(',', $rule['data']);
        $specific_values = array_map('trim', $specific_values);
        foreach ($items as $key => $val) {
          if (is_array($val)) {
            $val = _webform_validation_flatten_array($val);
          }
          if (!in_array($val, $specific_values)) {
            $errors[$key] = _webform_validation_i18n_error_message($rule);
          }
        }
        return $errors;
        break;
      case "oneoftwo":
        // $components should have 2 items
        $keys = array_keys($items);
        $item1 = array_shift($keys);
        $item2 = array_shift($keys);
        $entry1 = _webform_validation_flatten_array($items[$item1]);
        $entry2 = _webform_validation_flatten_array($items[$item2]);
        if (empty($entry1) && empty($entry2)) {
          return array($item1 => t('You have to specify %item1 or %item2 (or both)', array('%item1' => $components[$item1]['name'], '%item2' => $components[$item2]['name'])));
        }
        break;
      case "oneofseveral":
        foreach ($items as $key => $val) {
          if (is_array($val)) {
            // make sure to flatten arrays first
            $items[$key] = _webform_validation_flatten_array($val);
          }
        }
        // $components should have at least one of several items
        if (count(array_filter($items)) < 1) {
          $keys = array_keys($items);
          $names = array();
          foreach ($keys as $value) {
            $names[] = $components[$value]['name'];
          }
          return array($keys[0] => t('You have to specify at least one of these items:') . theme('item_list', array('items' => $names)));
        }
        break;
      case "select_min":
        $min_selections = $rule['data'];
        foreach ($items as $key => $val) {
          if (is_array($val) && (count(array_filter($val, '_webform_validation_check_false')) < $min_selections)) {
            $errors[$key] = t('Please select at least %num options for %item', array('%num' => $min_selections, '%item' => $components[$key]['name']));
          }
        }
        return $errors;
        break;
      case "select_max":
        $max_selections = $rule['data'];
        foreach ($items as $key => $val) {
          if (is_array($val) && (count(array_filter($val, '_webform_validation_check_false')) > $max_selections)) {
            $errors[$key] = t('Please select maximum %num options for %item', array('%num' => $max_selections, '%item' => $components[$key]['name']));
          }
        }
        return $errors;
        break;
      case "select_exact":
        $allowed_selections = $rule['data'];
        foreach ($items as $key => $val) {
          if (is_array($val) && (count(array_filter($val, '_webform_validation_check_false')) != $allowed_selections)) {
            $errors[$key] = t('Please select %num options for %item', array('%num' => $allowed_selections, '%item' => $components[$key]['name']));
          }
        }
        return $errors;
        break;
      case "plain_text":
        foreach ($items as $key => $val) {
          if ($val != '' && (strcmp($val, strip_tags($val)))) {
            $errors[$key] = t('%item only allows the use of plain text', array('%item' => $components[$key]['name']));
          }
        }
        return $errors;
        break;
      case "regex":
        mb_regex_encoding('UTF-8');
        $regex = $rule['data'];
        foreach ($items as $key => $val) {
          if ($val != '' && (!mb_ereg("$regex", $val))) {
            $errors[$key] = _webform_validation_i18n_error_message($rule);
          }
        }
        return $errors;
        break;
      case 'must_be_empty':
        foreach ($items as $key => $val) {
          if ($val) {
            $errors[$key] = t('%item does not contain the correct data', array('%item' => $components[$key]['name']));
          }
        }
        return $errors;
        break;
      case "blacklist":
        $blacklist = explode(',', $rule['data']);
        $blacklist = array_map('trim', $blacklist);
        $blacklist_regex = implode('|', $blacklist);
        foreach ($items as $key => $val) {
          if ($val != '' && (preg_match("/$blacklist_regex/i", $val))) {
            $errors[$key] = _webform_validation_i18n_error_message($rule);
          }
        }
        return $errors;
        break;
      case "username":
        foreach ($items as $key => $val) {
          // load user - if username does not match or status 0 throw error
          if ($val != '') {
            if (!db_query("SELECT COUNT(*) FROM {users} WHERE name = :username AND status = 1", array(':username' => $val))->fetchField()) {
              // Username doesn't exist
              $errors[$key] = t('The %item field does not match an active username.', array('%item' => $components[$key]['name']));
            }
          }
        }
        return $errors;
        break;
    }
  }
}

/**
 * Helper function to deal with submitted values that are arrays (e.g. multiple select component)
 * We flatten the array as a comma-separated list to do the comparison.
 */
function _webform_validation_flatten_array($val) {
  if (is_array($val)) {
    $arr = array_filter($val, '_webform_validation_check_false');
    return implode(',', $arr);
  }
  return $val;
}

/**
 * Get a list of validator definitions
 */
function webform_validation_get_validators() {
  $validators = module_invoke_all("webform_validation_validators");
  // let modules use hook_webform_validator_alter($validators) to change validator settings
  drupal_alter('webform_validator', $validators);
  return $validators;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function webform_validation_get_validators_selection() {
  $selection = array();
  $validators = webform_validation_get_validators();
  if ($validators) {
    foreach ($validators as $validator_key => $validator_info) {
      $selection[$validator_key] = $validator_info['name'];
    }
  }
  return $selection;
}

/**
 * Get a list of valid component types per validator, as defined via hook_webform_validation_validators().
 * If 'all' is specified, all available component types will be returned.
 */
function webform_validation_valid_component_types($validator) {
  $validators = webform_validation_get_validators();
  if ($info = $validators[$validator]) {
    $allowed_types = $info['component_types'];
    if (_webform_validation_all_allowed($allowed_types)) {
      return array_keys(webform_components());
    }
    return $info['component_types'];
  }
}

/**
 * Helper function to check whether all components are allowed to be used for a certain validator
 */
function _webform_validation_all_allowed($allowed) {
  if ($allowed) {
    foreach ($allowed as $type) {
      if ($type == "all") {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function webform_validation_get_validator_info($validator_key) {
  $validators = webform_validation_get_validators();
  return $validators[$validator_key];
}

/**
 * Handle translatable error messages, if available
 */
function _webform_validation_i18n_error_message($rule) {
  $rule['error_message'] = filter_xss($rule['error_message']);
  if (module_exists('i18nstrings')) {
    return i18nstrings('webform_validation:error_message:' . $rule['ruleid'] . ':message', $rule['error_message']);
  }
  return $rule['error_message'];
}

/**
 * Helper function used by array_filter to determine if a value was selected or not
 */
function _webform_validation_check_false($var) {
  return $var !== FALSE && $var !== 0;
}

/**
 * Process the numeric value validation range that was provided in the numeric validator options
 */
function _webform_numeric_check_data($data) {
  $range = array('min' => NULL, 'max' => NULL);
  // if no value was specified, don't validate
  if ($data == '') {
    return $range;
  }

  // If only one numeric value was specified, this is the min value
  if (is_numeric($data)) {
    $range['min'] = (int) $data;
  }

  if (strpos($data, '|') !== FALSE) {
    list($min, $max) = explode('|', $data);
    if ($min != '' && is_numeric($min)) {
      $range['min'] = (int) $min;
    }
    if ($max != '' && is_numeric($max)) {
      $range['max'] = (int) $max;
    }
  }
  return $range;
}
