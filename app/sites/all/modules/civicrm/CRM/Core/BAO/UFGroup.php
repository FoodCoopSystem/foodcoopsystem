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

require_once 'CRM/Core/DAO/UFGroup.php';
require_once 'CRM/Core/Permission.php';

/**
 *
 */
class CRM_Core_BAO_UFGroup extends CRM_Core_DAO_UFGroup {
  CONST PUBLIC_VISIBILITY = 1, ADMIN_VISIBILITY = 2, LISTINGS_VISIBILITY = 4;

  /**
   * cache the match clause used in this transaction
   *
   * @var string
   */
  static $_matchFields = NULL;

  /**
   * Takes a bunch of params that are needed to match certain criteria and
   * retrieves the relevant objects. Typically the valid params are only
   * contact_id. We'll tweak this function to be more full featured over a period
   * of time. This is the inverse function of create. It also stores all the retrieved
   * values in the default array
   *
   * @param array $params      (reference) an assoc array of name/value pairs
   * @param array $defaults    (reference) an assoc array to hold the flattened values
   *
   * @return object   CRM_Core_DAO_UFGroup object
   * @access public
   * @static
   */
  static
  function retrieve(&$params, &$defaults) {
    return CRM_Core_DAO::commonRetrieve('CRM_Core_DAO_UFGroup', $params, $defaults);
  }

  /**
   * Retrieve the first non-generic contact type
   *
   * @param int $id  id of uf_group
   *
   * @return string  contact type
   */
  static
  function getContactType($id) {
    require_once 'CRM/Contact/BAO/ContactType.php';

    $validTypes = array_filter(array_keys(CRM_Core_SelectValues::contactType()));
    $validSubTypes = CRM_Contact_BAO_ContactType::subTypeInfo();

    $typesParts = explode(CRM_Core_DAO::VALUE_SEPARATOR, CRM_Core_DAO::getFieldValue('CRM_Core_DAO_UFGroup', $id, 'group_type'));
    $types = explode(',', $typesParts[0]);

    $cType = NULL;
    foreach ($types as $type) {
      if (in_array($type, $validTypes)) {
        $cType = $type;
      }
      elseif (array_key_exists($type, $validSubTypes)) {
        $cType = CRM_Utils_Array::value('parent', $validSubTypes[$type]);
      }
      if ($cType)
      break;
    }

    return $cType;
  }

  /**
   * Get the form title.
   *
   * @param int $id id of uf_form
   *
   * @return string title
   *
   * @access public
   * @static
   *
   */
  public static function getTitle($id) {
    return CRM_Core_DAO::getFieldValue('CRM_Core_DAO_UFGroup', $id, 'title');
  }

  /**
   * update the is_active flag in the db
   *
   * @param int      $id           id of the database record
   * @param boolean  $is_active    value we want to set the is_active field
   *
   * @return Object             CRM_Core_DAO_UFGroup object on success, null otherwise
   * @access public
   * @static
   */
  static
  function setIsActive($id, $is_active) {
    return CRM_Core_DAO::setFieldValue('CRM_Core_DAO_UFGroup', $id, 'is_active', $is_active);
  }

  /**
   * get all the registration fields
   *
   * @param int $action   what action are we doing
   * @param int $mode     mode
   *
   * @return array the fields that are needed for registration
   * @static
   * @access public
   */

  static
  function getRegistrationFields($action, $mode, $ctype = NULL) {
    if ($mode & CRM_Profile_Form::MODE_REGISTER) {
      $ufGroups = CRM_Core_BAO_UFGroup::getModuleUFGroup('User Registration');
    }
    else {
      $ufGroups = CRM_Core_BAO_UFGroup::getModuleUFGroup('Profile');
    }

    if (!is_array($ufGroups)) {
      return FALSE;
    }

    $fields = array();

    require_once 'CRM/Core/BAO/UFField.php';
    foreach ($ufGroups as $id => $title) {
      if ($ctype) {
        $fieldType = CRM_Core_BAO_UFField::getProfileType($id);
        if (($fieldType != 'Contact') &&
          ($fieldType != $ctype) &&
          !CRM_Contact_BAO_ContactType::isExtendsContactType($fieldType, $ctype)
        ) {
          continue;
        }
        if (CRM_Contact_BAO_ContactType::isaSubType($fieldType)) {
          $profileSubType = $fieldType;
        }
      }

      $subset = self::getFields($id, TRUE, $action,
        NULL, NULL, FALSE, NULL, TRUE, $ctype
      );

      // we do not allow duplicates. the first field is the winner
      foreach ($subset as $name => $field) {
        if (!CRM_Utils_Array::value($name, $fields)) {
          $fields[$name] = $field;
        }
      }
    }

    return $fields;
  }

  /**
   * get all the listing fields
   *
   * @param int     $action            what action are we doing
   * @param int     $visibility        visibility of fields we are interested in
   * @param bool    $considerSelector  whether to consider the in_selector parameter
   * @param array   $ufGroupIds
   * @param boolean $searchable
   *
   * @return array   the fields that are listings related
   * @static
   * @access public
   */
  static
  function getListingFields($action,
    $visibility,
    $considerSelector = FALSE,
    $ufGroupIds       = NULL,
    $searchable       = NULL,
    $restrict         = NULL,
    $skipPermission   = FALSE,
    $permissionType   = CRM_Core_Permission::SEARCH
  ) {
    if ($ufGroupIds) {
      $subset = self::getFields($ufGroupIds, FALSE, $action,
        $visibility, $searchable,
        FALSE, $restrict,
        $skipPermission,
        NULL,
        $permissionType
      );
      if ($considerSelector) {
        // drop the fields not meant for the selector
        foreach ($subset as $name => $field) {
          if (!$field['in_selector']) {
            unset($subset[$name]);
          }
        }
      }
      $fields = $subset;
    }
    else {
      $ufGroups = CRM_Core_PseudoConstant::ufGroup();

      $fields = array();
      foreach ($ufGroups as $id => $title) {
        $subset = self::getFields($id, FALSE, $action,
          $visibility, $searchable,
          FALSE, $restrict,
          $skipPermission,
          NULL,
          $permissionType
        );
        if ($considerSelector) {
          // drop the fields not meant for the selector
          foreach ($subset as $name => $field) {
            if (!$field['in_selector'])unset($subset[$name]);
          }
        }
        $fields = array_merge($fields, $subset);
      }
    }
    return $fields;
  }

  /**
   * get all the fields that belong to the group with the name title
   *
   * @param mix      $id           the id of the UF group or ids of ufgroup
   * @param int      $register     are we interested in registration fields
   * @param int      $action       what action are we doing
   * @param int      $visibility   visibility of fields we are interested in
   * @param          $searchable
   * @param boolean  $showall
   * @param string   $restrict     should we restrict based on a specified profile type
   *
   * @return array   the fields that belong to this ufgroup(s)
   * @static
   * @access public
   */
  static
  function getFields($id, $register = FALSE, $action = NULL,
    $visibility     = NULL, $searchable = NULL,
    $showAll        = FALSE, $restrict = NULL,
    $skipPermission = FALSE,
    $ctype          = NULL,
    $permissionType = CRM_Core_Permission::CREATE,
    $orderBy        = 'field_name', $orderProfiles = NULL
  ) {
    if (!is_array($id)) {
      $id = CRM_Utils_Type::escape($id, 'Positive');
      $profileIds = array($id);
    }
    else {
      $profileIds = $id;
    }

    $gids = implode(',', $profileIds);
    $params = array();
    if ($restrict) {
      $query = "SELECT g.* from civicrm_uf_group g, civicrm_uf_join j 
                WHERE g.id IN ( {$gids} ) 
                AND j.uf_group_id IN ( {$gids} )
                AND j.module      = %1
                ";
      $params = array(1 => array($restrict, 'String'));
    }
    else {
      $query = "SELECT g.* from civicrm_uf_group g WHERE g.id IN ( {$gids} ) ";
    }

    if (!$showAll) {
      $query .= " AND g.is_active = 1";
    }

    // add permissioning for profiles only if not registration
    if (!$skipPermission) {
      require_once 'CRM/Core/Permission.php';
      $permissionClause = CRM_Core_Permission::ufGroupClause($permissionType, 'g.');
      $query .= " AND $permissionClause ";
    }

    if ($orderProfiles AND count($profileIds) > 1) {
      $query .= " ORDER BY FIELD(  g.id, {$gids} )";
    }
    $group      = CRM_Core_DAO::executeQuery($query, $params);
    $fields     = array();
    $validGroup = FALSE;

    while ($group->fetch()) {
      $validGroup = TRUE;
      $where = " WHERE uf_group_id = {$group->id}";

      if ($searchable) {
        $where .= " AND is_searchable = 1";
      }

      if (!$showAll) {
        $where .= " AND is_active = 1";
      }

      if ($visibility) {
        $clause = array();
        if ($visibility & self::PUBLIC_VISIBILITY) {
          $clause[] = 'visibility = "Public Pages"';
        }
        if ($visibility & self::ADMIN_VISIBILITY) {
          $clause[] = 'visibility = "User and User Admin Only"';
        }
        if ($visibility & self::LISTINGS_VISIBILITY) {
          $clause[] = 'visibility = "Public Pages and Listings"';
        }
        if (!empty($clause)) {
          $where .= ' AND ( ' . implode(' OR ', $clause) . ' ) ';
        }
      }

      $query = "SELECT * FROM civicrm_uf_field $where ORDER BY weight";
      if ($orderBy) {
        $query .= ", " . $orderBy;
      }

      $field = CRM_Core_DAO::executeQuery($query);
      require_once 'CRM/Contact/BAO/Contact.php';
      if (!$showAll) {
        $importableFields = CRM_Contact_BAO_Contact::importableFields('All');
      }
      else {
        $importableFields = CRM_Contact_BAO_Contact::importableFields('All', FALSE, TRUE);
      }

      require_once 'CRM/Core/Component.php';
      require_once 'CRM/Core/BAO/UFField.php';
      require_once 'CRM/Activity/BAO/Activity.php';
      $profileType = CRM_Core_BAO_UFField::getProfileType($group->id);
      $contactActivityProfile = CRM_Core_BAO_UFField::checkContactActivityProfileType($group->id);

      if ($profileType == 'Activity' || $contactActivityProfile) {
        $componentFields = CRM_Activity_BAO_Activity::getProfileFields();
      }
      else {
        $componentFields = CRM_Core_Component::getQueryFields();
      }

      $importableFields = array_merge($importableFields, $componentFields);

      $importableFields['group']['title'] = ts('Group(s)');
      $importableFields['group']['where'] = NULL;
      $importableFields['tag']['title'] = ts('Tag(s)');
      $importableFields['tag']['where'] = NULL;

      $locationFields = array(
        'street_address',
        'supplemental_address_1',
        'supplemental_address_2',
        'city',
        'postal_code',
        'postal_code_suffix',
        'geo_code_1',
        'geo_code_2',
        'state_province',
        'country',
        'county',
        'phone',
        'email',
        'im',
        'address_name',
      );

      //get location type
      $locationType = array();
      $locationType = CRM_Core_PseudoConstant::locationType();

      require_once 'CRM/Core/BAO/CustomField.php';
      $customFields = CRM_Core_BAO_CustomField::getFieldsForImport($ctype);

      // hack to add custom data for components
      $components = array('Contribution', 'Participant', 'Membership', 'Activity');
      foreach ($components as $value) {
        $customFields = array_merge($customFields, CRM_Core_BAO_CustomField::getFieldsForImport($value));
      }
      $addressCustomFields = CRM_Core_BAO_CustomField::getFieldsForImport('Address');
      $customFields = array_merge($customFields, $addressCustomFields);

      while ($field->fetch()) {
        $name  = $title = $locType = $phoneType = '';
        $name  = $field->field_name;
        $title = $field->label;

        $addressCustom = FALSE;
        if (in_array($permissionType, array(
          CRM_Core_Permission::CREATE,
              CRM_Core_Permission::EDIT,
            )) &&
          in_array($field->field_name, array_keys($addressCustomFields))
        ) {
          $addressCustom = TRUE;
          $name = "address_{$name}";
        }

        if ($field->location_type_id) {
          $name .= "-{$field->location_type_id}";
          $locType = " ( {$locationType[$field->location_type_id]} ) ";
        }
        else {
          if (in_array($field->field_name, $locationFields) || $addressCustom) {
            $name .= '-Primary';
            $locType = ' ( Primary ) ';
          }
        }

        if (isset($field->phone_type_id)) {
          $name .= "-{$field->phone_type_id}";
          // this hack is to prevent Phone Phone (work)
          if ($field->phone_type_id != '1') {
            $phoneType = "-{$field->phone_type_id}";
          }
        }

        $fields[$name] = array(
          'name' => $name,
          'groupTitle' => $group->title,
          'groupHelpPre' => $group->help_pre,
          'groupHelpPost' => $group->help_post,
          'title' => $title,
          'where' => CRM_Utils_Array::value('where', $importableFields[$field->field_name]),
          'attributes' => CRM_Core_DAO::makeAttribute(CRM_Utils_Array::value($field->field_name,
              $importableFields
            )),
          'is_required' => $field->is_required,
          'is_view' => $field->is_view,
          'help_pre' => $field->help_pre,
          'help_post' => $field->help_post,
          'visibility' => $field->visibility,
          'in_selector' => $field->in_selector,
          'rule' => CRM_Utils_Array::value('rule', $importableFields[$field->field_name]),
          'location_type_id' => $field->location_type_id,
          'phone_type_id' => isset($field->phone_type_id) ? $field->phone_type_id : NULL,
          'group_id' => $group->id,
          'add_to_group_id' => $group->add_to_group_id,
          'add_captcha' => $group->add_captcha,
          'field_type' => $field->field_type,
          'field_id' => $field->id,
        );

        //adding custom field property
        if (substr($field->field_name, 0, 6) == 'custom' ||
          substr($field->field_name, 0, 14) === 'address_custom'
        ) {
          // if field is not present in customFields, that means the user
          // DOES NOT HAVE permission to access that field
          if (array_key_exists($field->field_name, $customFields)) {
            $fields[$name]['is_search_range'] = $customFields[$field->field_name]['is_search_range'];
            // fix for CRM-1994
            $fields[$name]['options_per_line'] = $customFields[$field->field_name]['options_per_line'];
            $fields[$name]['data_type'] = $customFields[$field->field_name]['data_type'];
            $fields[$name]['html_type'] = $customFields[$field->field_name]['html_type'];

            if (CRM_Utils_Array::value('html_type', $fields[$name]) == 'Select Date') {
              $fields[$name]['date_format'] = $customFields[$field->field_name]['date_format'];
              $fields[$name]['time_format'] = $customFields[$field->field_name]['time_format'];
            }
          }
          else {
            unset($fields[$name]);
          }
        }
      }
      $field->free();
    }

    if (empty($fields) && !$validGroup) {
      CRM_Core_Error::fatal(ts('The requested Profile (gid=%1) is disabled OR it is not configured to be used for \'Profile\' listings in its Settings OR there is no Profile with that ID OR you do not have permission to access this profile. Please contact the site administrator if you need assistance.',
          array(1 => implode(',', $profileIds))
        ));
    }
    return $fields;
  }

  /**
   * check the data validity
   *
   * @param int    $userID    the user id that we are actually editing
   * @param string $title     the title of the group we are interested in
   * @pram  boolean $register is this the registrtion form
   * @param int    $action  the action of the form
   *
   * @return boolean   true if form is valid
   * @static
   * @access public
   */
  static
  function isValid($userID, $title, $register = FALSE, $action = NULL) {
    require_once 'CRM/Core/Controller/Simple.php';
    $session = CRM_Core_Session::singleton();

    if ($register) {
      $controller = new CRM_Core_Controller_Simple('CRM_Profile_Form_Dynamic',
        ts('Dynamic Form Creator'),
        $action
      );
      $controller->set('id', $userID);
      $controller->set('register', 1);
      $controller->process();
      return $controller->validate();
    }
    else {
      // make sure we have a valid group
      $group = new CRM_Core_DAO_UFGroup();

      $group->title = $title;

      if ($group->find(TRUE) && $userID) {
        $controller = new CRM_Core_Controller_Simple('CRM_Profile_Form_Dynamic', ts('Dynamic Form Creator'), $action);
        $controller->set('gid', $group->id);
        $controller->set('id', $userID);
        $controller->set('register', 0);
        $controller->process();
        return $controller->validate();
      }
      return TRUE;
    }
  }

  /**
   * get the html for the form that represents this particular group
   *
   * @param int     $userID    the user id that we are actually editing
   * @param string  $title     the title of the group we are interested in
   * @param int     $action    the action of the form
   * @param boolean $register  is this the registration form
   * @param boolean $reset     should we reset the form?
   * @param int     $profileID do we have the profile ID?
   *
   * @return string       the html for the form on success, otherwise empty string
   * @static
   * @access public
   */
  static
  function getEditHTML($userID,
    $title,
    $action       = NULL,
    $register     = FALSE,
    $reset        = FALSE,
    $profileID    = NULL,
    $doNotProcess = FALSE,
    $ctype        = NULL
  ) {
    require_once 'CRM/Core/Controller/Simple.php';

    $session = CRM_Core_Session::singleton();

    if ($register) {
      $controller = new CRM_Core_Controller_Simple('CRM_Profile_Form_Dynamic',
        ts('Dynamic Form Creator'),
        $action
      );
      if ($reset || $doNotProcess) {
        // hack to make sure we do not process this form
        $oldQFDefault = CRM_Utils_Array::value('_qf_default',
          $_POST
        );
        unset($_POST['_qf_default']);
        unset($_REQUEST['_qf_default']);
        if ($reset) {
          $controller->reset();
        }
      }

      $controller->set('id', $userID);
      $controller->set('register', 1);
      $controller->set('skipPermission', 1);
      $controller->set('ctype', $ctype);
      $controller->process();
      if ($doNotProcess) {
        $controller->validate();
      }
      $controller->setEmbedded(TRUE);

      //CRM-5839 - though we want to process form, get the control back.
      $controller->setSkipRedirection(($doNotProcess) ? FALSE : TRUE);

      $controller->run();

      // we are done processing so restore the POST/REQUEST vars
      if (($reset || $doNotProcess) && $oldQFDefault) {
        $_POST['_qf_default'] = $_REQUEST['_qf_default'] = $oldQFDefault;
      }

      $template = CRM_Core_Smarty::singleton();

      return trim($template->fetch('CRM/Profile/Form/Dynamic.tpl'));
    }
    else {
      if (!$profileID) {
        // make sure we have a valid group
        $group = new CRM_Core_DAO_UFGroup();

        $group->title = $title;

        if ($group->find(TRUE)) {
          $profileID = $group->id;
        }
      }

      if ($profileID) {
        // make sure profileID and ctype match if ctype exists
        if ($ctype) {
          require_once 'CRM/Core/BAO/UFField.php';
          $profileType = CRM_Core_BAO_UFField::getProfileType($profileID);
          if (CRM_Contact_BAO_ContactType::isaSubType($profileType)) {
            $profileType = CRM_Contact_BAO_ContactType::getBasicType($profileType);
          }

          if (($profileType != 'Contact') && ($profileType != $ctype)) {
            return NULL;
          }
        }

        $controller = new CRM_Core_Controller_Simple('CRM_Profile_Form_Dynamic',
          ts('Dynamic Form Creator'),
          $action
        );
        if ($reset) {
          $controller->reset();
        }
        $controller->set('gid', $profileID);
        $controller->set('id', $userID);
        $controller->set('register', 0);
        $controller->set('skipPermission', 1);
        if ($ctype) {
          $controller->set('ctype', $ctype);
        }
        $controller->process();
        $controller->setEmbedded(TRUE);

        //CRM-5846 - give the control back to drupal.
        $controller->setSkipRedirection(($doNotProcess) ? FALSE : TRUE);
        $controller->run();

        $template = CRM_Core_Smarty::singleton();

        $templateFile = "CRM/Profile/Form/{$profileID}/Dynamic.tpl";
        if (!$template->template_exists($templateFile)) {
          $templateFile = 'CRM/Profile/Form/Dynamic.tpl';
        }
        return trim($template->fetch($templateFile));
      }
      else {
        require_once 'CRM/Contact/BAO/Contact/Location.php';
        $userEmail = CRM_Contact_BAO_Contact_Location::getEmailDetails($userID);

        // if post not empty then only proceed
        if (!empty($_POST)) {
          // get the new email
          $config = CRM_Core_Config::singleton();
          $email = CRM_Utils_Array::value('mail', $_POST);

          if (CRM_Utils_Rule::email($email) && ($email != $userEmail[1])) {
            require_once 'CRM/Core/BAO/UFMatch.php';
            CRM_Core_BAO_UFMatch::updateContactEmail($userID, $email);
          }
        }
      }
    }
    return '';
  }

  /**
   * searches for a contact in the db with similar attributes
   *
   * @param array $params the list of values to be used in the where clause
   * @param int    $id          the current contact id (hence excluded from matching)
   * @param boolean $flatten should we flatten the input params
   *
   * @return contact_id if found, null otherwise
   * @access public
   * @static
   */
  public static function findContact(&$params, $id = NULL, $contactType = 'Individual') {
    require_once 'CRM/Dedupe/Finder.php';
    $dedupeParams = CRM_Dedupe_Finder::formatParams($params, $contactType);
    $dedupeParams['check_permission'] = CRM_Utils_Array::value('check_permission', $params, TRUE);
    $ids = CRM_Dedupe_Finder::dupesByParams($dedupeParams, $contactType, 'Fuzzy', array($id));
    if (!empty($ids)) {
      return implode(',', $ids);
    }
    else {
      return NULL;
    }
  }

  /**
   * Given a contact id and a field set, return the values from the db
   * for this contact
   *
   * @param int     $id             the contact id
   * @param array   $fields         the profile fields of interest
   * @param array   $values         the values for the above fields
   * @param boolean $searchable     searchable or not
   * @param array   $componentWhere component condition
   * @param boolean $absolute       return urls in absolute form (useful when sending an email)
   *
   * @return void
   * @access public
   * @static
   */
  public static function getValues($cid, &$fields, &$values,
    $searchable = TRUE, $componentWhere = NULL,
    $absolute = FALSE
  ) {
    if (empty($cid) && empty($componentWhere)) {
      return NULL;
    }

    $options = $studentFields = array();
    if (CRM_Core_Permission::access('Quest', FALSE)) {
      //student fields ( check box )
      require_once 'CRM/Quest/BAO/Student.php';
      $studentFields = CRM_Quest_BAO_Student::$multipleSelectFields;
    }

    // get the contact details (hier)
    $returnProperties = CRM_Contact_BAO_Contact::makeHierReturnProperties($fields);

    $params = $cid ? array(array('contact_id', '=', $cid, 0, 0)) : array();

    // add conditions specified by components. eg partcipant_id etc
    if (!empty($componentWhere)) {
      $params = array_merge($params, $componentWhere);
    }

    $query = new CRM_Contact_BAO_Query($params, $returnProperties, $fields);
    $options = &$query->_options;

    $details = $query->searchQuery();
    if (!$details->fetch()) {
      return;
    }

    $config = CRM_Core_Config::singleton();

    require_once 'CRM/Core/PseudoConstant.php';
    require_once 'CRM/Contact/BAO/Contact.php';
    $locationTypes = $imProviders = array();
    $locationTypes = CRM_Core_PseudoConstant::locationType();
    $imProviders   = CRM_Core_PseudoConstant::IMProvider();
    $websiteTypes  = CRM_Core_PseudoConstant::websiteType();

    $multipleFields = array('url');
    $nullIndex = $nullValueIndex = ' ';
    //start of code to set the default values
    foreach ($fields as $name => $field) {
      // fix for CRM-3962
      if ($name == 'id') {
        $name = 'contact_id';
      }

      $index = $field['title'];
      //handle for the label not set for the field
      if (empty($field['title'])) {
        $index = $nullIndex;
        $nullIndex .= $nullIndex;
      }

      //handle the case to avoid re-write where the profile field labels are the same
      if (CRM_Utils_Array::value($index, $values)) {
        $index .= $nullValueIndex;
        $nullValueIndex .= $nullValueIndex;
      }
      $params[$index] = $values[$index] = '';
      $customFieldName = NULL;
      // hack for CRM-665
      if (isset($details->$name) || $name == 'group' || $name == 'tag') {
        // to handle gender / suffix / prefix
        if (in_array($name, array(
          'gender', 'individual_prefix', 'individual_suffix'))) {
          $values[$index] = $details->$name;
          $name           = $name . '_id';
          $params[$index] = $details->$name;
        }
        elseif (in_array($name, CRM_Contact_BAO_Contact::$_greetingTypes)) {
          $dname          = $name . '_display';
          $values[$index] = $details->$dname;
          $name           = $name . '_id';
          $params[$index] = $details->$name;
        }
        elseif (in_array($name, array(
          'state_province', 'country', 'county'))) {
          $values[$index] = $details->$name;
          $idx            = $name . '_id';
          $params[$index] = $details->$idx;
        }
        elseif ($name === 'preferred_communication_method') {
          $communicationFields = CRM_Core_PseudoConstant::pcm();
          $pref                = $compref = array();
          $pref                = explode(CRM_Core_DAO::VALUE_SEPARATOR, $details->$name);

          foreach ($pref as $k) {
            if ($k) {
              $compref[] = $communicationFields[$k];
            }
          }
          $params[$index] = $details->$name;
          $values[$index] = implode(',', $compref);
        }
        elseif ($name === 'preferred_language') {
          $languages      = CRM_Core_PseudoConstant::languages();
          $params[$index] = $details->$name;
          $values[$index] = $languages[$details->$name];
        }
        elseif ($name == 'group') {
          require_once 'CRM/Contact/BAO/Group.php';
          require_once 'CRM/Core/Permission.php';
          require_once 'CRM/Utils/Array.php';
          $groups = CRM_Contact_BAO_GroupContact::getContactGroup($cid, 'Added', NULL, FALSE, TRUE);
          $title = $ids = array();

          foreach ($groups as $g) {
            // CRM-8362: User and User Admin visibility groups should be included in display if user has
            // VIEW permission on that group
            $groupPerm = CRM_Contact_BAO_Group::checkPermission($g['group_id'], $g['title']);

            if ($g['visibility'] != 'User and User Admin Only' ||
              CRM_Utils_Array::key(CRM_Core_Permission::VIEW, $groupPerm)
            ) {
              $title[] = $g['title'];
              if ($g['visibility'] == 'Public Pages') {
                $ids[] = $g['group_id'];
              }
            }
          }
          $values[$index] = implode(', ', $title);
          $params[$index] = implode(',', $ids);
        }
        elseif ($name == 'tag') {
          require_once 'CRM/Core/BAO/EntityTag.php';
          $entityTags = CRM_Core_BAO_EntityTag::getTag($cid);
          $allTags    = CRM_Core_PseudoConstant::tag();
          $title      = array();
          foreach ($entityTags as $tagId) {
            $title[] = $allTags[$tagId];
          }
          $values[$index] = implode(', ', $title);
          $params[$index] = implode(',', $entityTags);
        }
        elseif (array_key_exists($name, $studentFields)) {
          require_once 'CRM/Core/OptionGroup.php';
          $paramsNew = array($name => $details->$name);
          if ($name == 'test_tutoring') {
            $names = array($name => array('newName' => $index, 'groupName' => 'test'));
            // for  readers group
          }
          elseif (substr($name, 0, 4) == 'cmr_') {
            $names = array($name => array('newName' => $index, 'groupName' => substr($name, 0, -3)));
          }
          else {
            $names = array($name => array('newName' => $index, 'groupName' => $name));
          }
          CRM_Core_OptionGroup::lookupValues($paramsNew, $names, FALSE);
          $values[$index] = $paramsNew[$index];
          $params[$index] = $paramsNew[$name];
        }
        elseif ($name == 'activity_status_id') {
          $activityStatus = CRM_Core_PseudoConstant::activityStatus();
          $values[$index] = $activityStatus[$details->$name];
          $params[$index] = $details->$name;
        }
        elseif ($name == 'activity_date_time') {
          $values[$index] = CRM_Utils_Date::customFormat($details->$name);
          $params[$index] = $details->$name;
        }
        elseif ($name == 'contact_sub_type') {
          $values[$index] = str_replace(CRM_Core_DAO::VALUE_SEPARATOR, ', ',
            trim($details->$name, CRM_Core_DAO::VALUE_SEPARATOR)
          );
          $params[$index] = $details->$name;
        }
        else {
          $processed = FALSE;
          if (CRM_Core_Permission::access('Quest', FALSE)) {
            require_once 'CRM/Quest/BAO/Student.php';
            $processed = CRM_Quest_BAO_Student::buildStudentForm($this, $field);
          }
          if (!$processed) {
            if (substr($name, 0, 7) === 'do_not_' ||
              substr($name, 0, 3) === 'is_'
            ) {
              if ($details->$name) {
                $values[$index] = '[ x ]';
              }
            }
            else {
              require_once 'CRM/Core/BAO/CustomField.php';
              if ($cfID = CRM_Core_BAO_CustomField::getKeyID($name)) {
                $htmlType = $field['html_type'];
                $dataType = $field['data_type'];

                // field_type is only set when we are retrieving profile values
                // when sending email, we call the same function to get custom field
                // values etc, i.e. emulating a profile
                $fieldType = CRM_Utils_Array::value('field_type', $field);

                if ($htmlType == 'File') {
                  $entityId = $cid;
                  if (!$cid &&
                    $fieldType == 'Activity' &&
                    CRM_Utils_Array::value(2, $componentWhere[0])
                  ) {
                    $entityId = $componentWhere[0][2];
                  }

                  $fileURL = CRM_Core_BAO_CustomField::getFileURL($entityId,
                    $cfID,
                    NULL,
                    $absolute
                  );
                  $params[$index] = $values[$index] = $fileURL['file_url'];
                }
                else {
                  $customVal = NULL;
                  if (isset($dao) && property_exists($dao, 'data_type') &&
                    ($dao->data_type == 'Int' ||
                      $dao->data_type == 'Boolean'
                    )
                  ) {
                    $customVal = (int )($details->{$name});
                  }
                  elseif (isset($dao) && property_exists($dao, 'data_type')
                    && $dao->data_type == 'Float'
                  ) {
                    $customVal = (float )($details->{$name});
                  }
                  elseif (!CRM_Utils_System::isNull(explode(CRM_Core_DAO::VALUE_SEPARATOR,
                        $details->{$name}
                      ))) {
                    $customVal = $details->{$name};
                  }

                  //CRM-4582
                  if (CRM_Utils_System::isNull($customVal)) {
                    continue;
                  }

                  $params[$index] = $customVal;
                  $values[$index] = CRM_Core_BAO_CustomField::getDisplayValue($customVal,
                    $cfID,
                    $options
                  );
                  if ($htmlType == 'Autocomplete-Select') {
                    $params[$index] = $values[$index];
                  }
                  if (CRM_Core_DAO::getFieldValue('CRM_Core_DAO_CustomField',
                      $cfID, 'is_search_range'
                    )) {
                    $customFieldName = "{$name}_from";
                  }
                }
              }
              elseif ($name == 'image_URL') {
                list($width, $height) = getimagesize($details->$name);
                list($thumbWidth, $thumbHeight) = CRM_Contact_BAO_Contact::getThumbSize($width, $height);

                $image_URL = '<img src="' . $details->$name . '" height= ' . $thumbHeight . ' width= ' . $thumbWidth . '  />';
                $values[$index] = "<a href='#' onclick='contactImagePopUp(\"{$details->$name}\", {$width}, {$height});'>{$image_URL}</a>";
              }
              elseif (in_array($name, array(
                'birth_date', 'deceased_date', 'membership_start_date', 'membership_end_date', 'join_date'))) {
                require_once 'CRM/Utils/Date.php';
                $values[$index] = CRM_Utils_Date::customFormat($details->$name);
                $params[$index] = CRM_Utils_Date::isoToMysql($details->$name);
              }
              else {
                $dao = '';
                if ($index == 'Campaign') {
                  $dao = 'CRM_Campaign_DAO_Campaign';
                }
                elseif ($index == 'Contribution Page') {
                  $dao = 'CRM_Contribute_DAO_ContributionPage';
                }
                if ($dao) {
                  $value = CRM_Core_DAO::getFieldValue($dao, $details->$name, 'title');
                }
                else {
                  $value = $details->$name;
                }
                $values[$index] = $value;
              }
            }
          }
        }
      }
      elseif (strpos($name, '-') !== FALSE) {
        list($fieldName, $id, $type) = CRM_Utils_System::explode('-', $name, 3);

        if (!in_array($fieldName, $multipleFields)) {
          if ($id == 'Primary') {
            // fix for CRM-1543
            // not sure why we'd every use Primary location type id
            // we need to fix the source if we are using it
            // $locationTypeName = CRM_Contact_BAO_Contact::getPrimaryLocationType( $cid );
            $locationTypeName = 1;
          }
          else {
            $locationTypeName = CRM_Utils_Array::value($id, $locationTypes);
          }

          if (!$locationTypeName) {
            continue;
          }

          $detailName = "{$locationTypeName}-{$fieldName}";
          $detailName = str_replace(' ', '_', $detailName);

          if (in_array($fieldName, array(
            'phone', 'im', 'email', 'openid'))) {
            if ($type) {
              $detailName .= "-{$type}";
            }
          }

          if (in_array($fieldName, array(
            'state_province', 'country', 'county'))) {
            $values[$index] = $details->$detailName;
            $idx            = $detailName . '_id';
            $params[$index] = $details->$idx;
          }
          elseif ($fieldName == 'im') {
            $providerId = $detailName . '-provider_id';
            $providerName = $imProviders[$details->$providerId];
            if ($providerName) {
              $values[$index] = $details->$detailName . " (" . $providerName . ")";
            }
            else {
              $values[$index] = $details->$detailName;
            }
            $params[$index] = $details->$detailName;
          }
          else {
            $values[$index] = $params[$index] = $details->$detailName;
          }
        }
        else {
          $detailName = "website-{$id}-{$fieldName}";
          $url = CRM_Utils_System::fixURL($details->$detailName);
          if ($details->$detailName) {
            $websiteTypeId  = "website-{$id}-website_type_id";
            $websiteType    = $websiteTypes[$details->$websiteTypeId];
            $values[$index] = "<a href=\"$url\">{$details->$detailName} ( {$websiteType} )</a>";
          }
          else {
            $values[$index] = '';
          }
        }
      }

      if ((CRM_Utils_Array::value('visibility', $field) == 'Public Pages and Listings') &&
        CRM_Core_Permission::check('profile listings and forms')
      ) {

        if (CRM_Utils_System::isNull($params[$index])) {
          $params[$index] = $values[$index];
        }
        if (!isset($params[$index])) {
          continue;
        }
        $customFieldID = CRM_Core_BAO_CustomField::getKeyID($field['name']);
        if (!$customFieldName) {
          $fieldName = $field['name'];
        }
        else {
          $fieldName = $customFieldName;
        }

        $url = NULL;
        if (CRM_Core_BAO_CustomField::getKeyID($field['name'])) {
          $htmlType = $field['html_type'];
          if ($htmlType == 'Link') {
            $url = $params[$index];
          }
          elseif (in_array($htmlType, array(
            'CheckBox', 'Multi-Select', 'AdvMulti-Select',
                'Multi-Select State/Province', 'Multi-Select Country',
              ))) {
            $valSeperator = CRM_Core_DAO::VALUE_SEPARATOR;
            $selectedOptions = explode($valSeperator, $params[$index]);

            foreach ($selectedOptions as $key => $multiOption) {
              if ($multiOption) {
                $url[] = CRM_Utils_System::url('civicrm/profile',
                  'reset=1&force=1&gid=' . $field['group_id'] . '&' .
                  urlencode($fieldName) .
                  '=' .
                  urlencode($multiOption)
                );
              }
            }
          }
          else {
            $url = CRM_Utils_System::url('civicrm/profile',
              'reset=1&force=1&gid=' . $field['group_id'] . '&' .
              urlencode($fieldName) .
              '=' .
              urlencode($params[$index])
            );
          }
        }
        else {
          $url = CRM_Utils_System::url('civicrm/profile',
            'reset=1&force=1&gid=' . $field['group_id'] . '&' .
            urlencode($fieldName) .
            '=' .
            urlencode($params[$index])
          );
        }

        if ($url &&
          !empty($values[$index]) &&
          $searchable
        ) {

          if (is_array($url) && !empty($url)) {
            $links = array();
            $eachMultiValue = explode(', ', $values[$index]);
            foreach ($eachMultiValue as $key => $valueLabel) {
              $links[] = '<a href="' . $url[$key] . '">' . $valueLabel . '</a>';
            }
            $values[$index] = implode(', ', $links);
          }
          else {
            $values[$index] = '<a href="' . $url . '">' . $values[$index] . '</a>';
          }
        }
      }
    }
  }

  /**
   * Check if profile Group used by any module.
   *
   * @param int  $id    profile Id
   *
   * @return boolean
   *
   * @access public
   * @static
   *
   */
  public static function usedByModule($id) {
    //check whether this group is used by any module(check uf join records)
    $sql = "SELECT id
                 FROM civicrm_uf_join
                 WHERE civicrm_uf_join.uf_group_id=$id";

    $dao = new CRM_Core_DAO();
    $dao->query($sql);
    if ($dao->fetch()) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Delete the profile Group.
   *
   * @param int  $id    profile Id
   *
   * @return boolean
   *
   * @access public
   * @static
   *
   */
  public static function del($id) {
    //check whether this group contains  any profile fields
    require_once 'CRM/Core/BAO/UFField.php';
    $profileField = new CRM_Core_DAO_UFField();
    $profileField->uf_group_id = $id;
    $profileField->find();
    while ($profileField->fetch()) {
      CRM_Core_BAO_UFField::del($profileField->id);
    }

    //delete records from uf join table
    require_once 'CRM/Core/DAO/UFJoin.php';
    $ufJoin = new CRM_Core_DAO_UFJoin();
    $ufJoin->uf_group_id = $id;
    $ufJoin->delete();

    //delete profile group
    $group = new CRM_Core_DAO_UFGroup();
    $group->id = $id;
    $group->delete();
    return 1;
  }

  /**
   * function to add the UF Group
   *
   * @param array $params reference array contains the values submitted by the form
   * @param array $ids    reference array contains the id
   *
   * @access public
   * @static
   *
   * @return object
   */
  static
  function add(&$params, &$ids) {
    require_once 'CRM/Utils/Array.php';
    require_once 'CRM/Utils/String.php';
    $fields = array('is_active', 'add_captcha', 'is_map', 'is_update_dupe', 'is_edit_link', 'is_uf_link', 'is_cms_user');
    foreach ($fields as $field) {
      $params[$field] = CRM_Utils_Array::value($field, $params, FALSE);
    }

    $params['limit_listings_group_id'] = CRM_Utils_Array::value('group', $params);
    $params['add_to_group_id'] = CRM_Utils_Array::value('add_contact_to_group', $params);

    $ufGroup = new CRM_Core_DAO_UFGroup();
    $ufGroup->copyValues($params);

    $ufGroupID = CRM_Utils_Array::value('ufgroup', $ids);
    if (!$ufGroupID) {
      $ufGroup->name = CRM_Utils_String::munge($ufGroup->title, '_', 56);
    }
    $ufGroup->id = $ufGroupID;

    $ufGroup->save();

    if (!$ufGroupID) {
      $ufGroup->name = $ufGroup->name . "_{$ufGroup->id}";
      $ufGroup->save();
    }

    return $ufGroup;
  }

  /**
   * Function to make uf join entries for an uf group
   *
   * @param array $params       (reference) an assoc array of name/value pairs
   * @param int   $ufGroupId    ufgroup id
   *
   * @return void
   * @access public
   * @static
   */
  static
  function createUFJoin(&$params, $ufGroupId) {
    $groupTypes = CRM_Utils_Array::value('uf_group_type', $params);

    // get ufjoin records for uf group
    $ufGroupRecord = CRM_Core_BAO_UFGroup::getUFJoinRecord($ufGroupId);

    // get the list of all ufgroup types
    $allUFGroupType = CRM_Core_SelectValues::ufGroupTypes();

    // this fix is done to prevent warning generated by array_key_exits incase of empty array is given as input
    if (!is_array($groupTypes)) {
      $groupTypes = array();
    }

    // this fix is done to prevent warning generated by array_key_exits incase of empty array is given as input
    if (!is_array($ufGroupRecord)) {
      $ufGroupRecord = array();
    }

    // check which values has to be inserted/deleted for contact
    $menuRebuild = FALSE;
    foreach ($allUFGroupType as $key => $value) {
      $joinParams = array();
      $joinParams['uf_group_id'] = $ufGroupId;
      $joinParams['module'] = $key;
      if ($key == 'User Account') {
        $menuRebuild = TRUE;
      }
      if (array_key_exists($key, $groupTypes) && !in_array($key, $ufGroupRecord)) {
        // insert a new record
        CRM_Core_BAO_UFGroup::addUFJoin($joinParams);
      }
      elseif (!array_key_exists($key, $groupTypes) && in_array($key, $ufGroupRecord)) {
        // delete a record for existing ufgroup
        CRM_Core_BAO_UFGroup::delUFJoin($joinParams);
      }
    }

    //update the weight
    $query = "
UPDATE civicrm_uf_join 
SET    weight = %1
WHERE  uf_group_id = %2
AND    ( entity_id IS NULL OR entity_id <= 0 )
";
    $p = array(1 => array($params['weight'], 'Integer'),
      2 => array($ufGroupId, 'Integer'),
    );
    CRM_Core_DAO::executeQuery($query, $p);

    // do a menu rebuild if we are on drupal, so it gets all the new menu entries
    // for user account
    $config = CRM_Core_Config::singleton();
    if ($menuRebuild &&
      $config->userSystem->is_drupal
    ) {
      menu_rebuild();
    }
  }

  /**
   * Function to get the UF Join records for an ufgroup id
   *
   * @params int $ufGroupId uf group id
   * @params int $displayName if set return display name in array
   * @params int $status if set return module other than default modules (User Account/User registration/Profile)
   *
   * @return array $ufGroupJoinRecords
   *
   * @access public
   * @static
   */
  public static function getUFJoinRecord($ufGroupId = NULL, $displayName = NULL, $status = NULL) {
    if ($displayName) {
      $UFGroupType = array();
      require_once 'CRM/Core/SelectValues.php';
      $UFGroupType = CRM_Core_SelectValues::ufGroupTypes();
    }

    $ufJoin = array();
    require_once 'CRM/Core/DAO/UFJoin.php';
    $dao = new CRM_Core_DAO_UFJoin();

    if ($ufGroupId) {
      $dao->uf_group_id = $ufGroupId;
    }

    $dao->find();
    $ufJoin = array();

    while ($dao->fetch()) {
      if (!$displayName) {
        $ufJoin[$dao->id] = $dao->module;
      }
      else {
        if (isset($UFGroupType[$dao->module])) {
          // skip the default modules
          if (!$status) {
            $ufJoin[$dao->id] = $UFGroupType[$dao->module];
          }
          // added for CRM-1475
        }
        elseif (!CRM_Utils_Array::key($dao->module, $ufJoin)) {
          $ufJoin[$dao->id] = $dao->module;
        }
      }
    }
    return $ufJoin;
  }

  /**
   * Function takes an associative array and creates a ufjoin record for ufgroup
   *
   * @param array $params (reference) an assoc array of name/value pairs
   *
   * @return object CRM_Core_BAO_UFJoin object
   * @access public
   * @static
   */
  static
  function addUFJoin(&$params) {
    require_once 'CRM/Core/DAO/UFJoin.php';
    $ufJoin = new CRM_Core_DAO_UFJoin();
    $ufJoin->copyValues($params);
    $ufJoin->save();
    return $ufJoin;
  }

  /**
   * Function to delete the uf join record for an uf group
   *
   * @param array  $params    (reference) an assoc array of name/value pairs
   *
   * @return void
   * @access public
   * @static
   */
  static
  function delUFJoin(&$params) {
    require_once 'CRM/Core/DAO/UFJoin.php';
    $ufJoin = new CRM_Core_DAO_UFJoin();
    $ufJoin->copyValues($params);
    $ufJoin->delete();
  }

  /**
   * Function to get the weight for ufjoin record
   *
   * @param int $ufGroupId     if $ufGroupId get update weight or add weight
   *
   * @return int   weight of the UFGroup
   * @access public
   * @static
   */
  static
  function getWeight($ufGroupId = NULL) {
    //calculate the weight
    $p = array();
    if (!$ufGroupId) {
      $queryString = "SELECT ( MAX(civicrm_uf_join.weight)+1) as new_weight
                            FROM civicrm_uf_join 
                            WHERE module = 'User Registration' OR module = 'User Account' OR module = 'Profile'";
    }
    else {
      $queryString = "SELECT MAX(civicrm_uf_join.weight) as new_weight
                            FROM civicrm_uf_join
                            WHERE civicrm_uf_join.uf_group_id = %1
                            AND ( entity_id IS NULL OR entity_id <= 0 )";
      $p[1] = array($ufGroupId, 'Integer');
    }

    $dao = CRM_Core_DAO::executeQuery($queryString, $p);
    $dao->fetch();
    return ($dao->new_weight) ? $dao->new_weight : 1;
  }

  /**
   * Function to get the uf group for a module
   *
   * @param string $moduleName module name
   * $param int    $count no to increment the weight
   *
   * @return array $ufGroups array of ufgroups for a module
   * @access public
   * @static
   */
  public static function getModuleUFGroup($moduleName = NULL, $count = 0, $skipPermission = TRUE) {
    require_once 'CRM/Core/DAO.php';

    $dao = new CRM_Core_DAO();
    $queryString = 'SELECT civicrm_uf_group.id, title, civicrm_uf_group.is_active, is_reserved, group_type
                        FROM civicrm_uf_group
                        LEFT JOIN civicrm_uf_join ON (civicrm_uf_group.id = uf_group_id)';
    $p = array();
    if ($moduleName) {
      $queryString .= ' AND civicrm_uf_group.is_active = 1
                              WHERE civicrm_uf_join.module = %2';
      $p[2] = array($moduleName, 'String');
    }


    // add permissioning for profiles only if not registration
    if (!$skipPermission) {
      require_once 'CRM/Core/Permission.php';
      $permissionClause = CRM_Core_Permission::ufGroupClause(CRM_Core_Permission::VIEW, 'civicrm_uf_group.');
      if (strpos($queryString, 'WHERE') !== FALSE) {
        $queryString .= " AND $permissionClause ";
      }
      else {
        $queryString .= " $permissionClause ";
      }
    }

    $queryString .= ' ORDER BY civicrm_uf_join.weight, civicrm_uf_group.title';
    $dao = CRM_Core_DAO::executeQuery($queryString, $p);

    $ufGroups = array();
    require_once 'CRM/Core/BAO/UFField.php';
    while ($dao->fetch()) {
      //skip mix profiles in user Registration / User Account
      if (($moduleName == 'User Registration' || $moduleName == 'User Account') &&
        CRM_Core_BAO_UFField::checkProfileType($dao->id)
      ) {
        continue;
      }
      $ufGroups[$dao->id]['name'] = $dao->title;
      $ufGroups[$dao->id]['title'] = $dao->title;
      $ufGroups[$dao->id]['is_active'] = $dao->is_active;
      $ufGroups[$dao->id]['group_type'] = $dao->group_type;
      $ufGroups[$dao->id]['is_reserved'] = $dao->is_reserved;
    }

    // Allow other modules to alter/override the UFGroups.
    require_once 'CRM/Utils/Hook.php';
    CRM_Utils_Hook::buildUFGroupsForModule($moduleName, $ufGroups);

    return $ufGroups;
  }

  /**
   * Function to filter ufgroups based on logged in user contact type
   *
   * @params int $ufGroupId uf group id (profile id)
   *
   * @return boolean true or false
   * @static
   * @access public
   */
  static
  function filterUFGroups($ufGroupId, $contactID = NULL) {
    if (!$contactID) {
      $session = CRM_Core_Session::singleton();
      $contactID = $session->get('userID');
    }

    if ($contactID) {
      //get the contact type
      require_once 'CRM/Contact/BAO/Contact.php';
      $contactType = CRM_Contact_BAO_Contact::getContactType($contactID);

      //match if exixting contact type is same as profile contact type
      require_once 'CRM/Core/BAO/UFField.php';
      $profileType = CRM_Core_BAO_UFField::getProfileType($ufGroupId);

      require_once 'CRM/Contact/BAO/ContactType.php';
      if (CRM_Contact_BAO_ContactType::isaSubType($profileType)) {
        $profileType = CRM_Contact_BAO_ContactType::getBasicType($profileType);
      }

      //allow special mix profiles for Contribution and Participant
      $specialProfiles = array('Contribution', 'Participant', 'Membership');

      if (in_array($profileType, $specialProfiles)) {
        return TRUE;
      }

      if (($contactType == $profileType) || $profileType == 'Contact') {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Function to build profile form
   *
   * @params object  $form       form object
   * @params array   $field      array field properties
   * @params int     $mode       profile mode
   * @params int     $contactID  contact id
   *
   * @return null
   * @static
   * @access public
   */
  static
  function buildProfile(&$form, &$field, $mode, $contactId = NULL, $online = FALSE, $onBehalf = FALSE) {
    require_once 'CRM/Profile/Form.php';
    require_once 'CRM/Core/OptionGroup.php';
    require_once 'CRM/Core/BAO/UFField.php';
    require_once 'CRM/Contact/BAO/ContactType.php';
    require_once 'CRM/Contact/BAO/Contact.php';

    $defaultValues = array();
    $fieldName     = $field['name'];
    $title         = $field['title'];
    $attributes    = $field['attributes'];
    $rule          = $field['rule'];
    $view          = $field['is_view'];
    $required      = ($mode == CRM_Profile_Form::MODE_SEARCH) ? FALSE : $field['is_required'];
    $search        = ($mode == CRM_Profile_Form::MODE_SEARCH) ? TRUE : FALSE;
    $isShared      = CRM_Utils_Array::value('is_shared', $field, 0);

    // do not display view fields in drupal registration form
    // CRM-4632
    if ($view && $mode == CRM_Profile_Form::MODE_REGISTER) {
      return;
    }

    if ($onBehalf) {
      $name = "onbehalf[$fieldName]";
    }
    elseif ($contactId && !$online) {
      $name = "field[$contactId][$fieldName]";
    }
    else {
      $name = $fieldName;
    }

    if ($fieldName == 'image_URL' && $mode == CRM_Profile_Form::MODE_EDIT) {
      $deleteExtra = ts('Are you sure you want to delete contact image.');
      $deleteURL = array(
        CRM_Core_Action::DELETE =>
        array(
          'name' => ts('Delete Contact Image'),
          'url' => 'civicrm/contact/image',
          'qs' => 'reset=1&id=%%id%%&gid=%%gid%%&action=delete',
          'extra' =>
          'onclick = "if (confirm( \'' . $deleteExtra . '\' ) ) {  this.href+=\'&amp;confirmed=1\'; else return false;}"',
        ),
      );
      $deleteURL = CRM_Core_Action::formLink($deleteURL,
        CRM_Core_Action::DELETE,
        array('id' => $form->get('id'),
          'gid' => $form->get('gid'),
        )
      );
      $form->assign('deleteURL', $deleteURL);
    }
    require_once 'CRM/Core/BAO/Setting.php';
    $addressOptions = CRM_Core_BAO_Setting::valueOptions(CRM_Core_BAO_Setting::SYSTEM_PREFERENCES_NAME,
      'address_options', TRUE, NULL, TRUE
    );

    if (substr($fieldName, 0, 14) === 'state_province') {
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Core_PseudoConstant::stateProvince(), $required
      );
    }
    elseif (substr($fieldName, 0, 7) === 'country') {
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Core_PseudoConstant::country(), $required
      );
      $config = CRM_Core_Config::singleton();
      if (!in_array($mode, array(
        CRM_Profile_Form::MODE_EDIT, CRM_Profile_Form::MODE_SEARCH)) &&
        $config->defaultContactCountry
      ) {
        $defaultValues[$name] = $config->defaultContactCountry;
        $form->setDefaults($defaultValues);
      }
    }
    elseif (substr($fieldName, 0, 6) === 'county') {
      if ($addressOptions['county']) {
        $form->add('select', $name, $title,
          array(
            '' => ts('- select state -')), $required
        );
      }
    }
    elseif (substr($fieldName, 0, 9) === 'image_URL') {
      $form->add('file', $name, $title, $attributes, $required);
      $form->addUploadElement($name);
    }
    elseif (substr($fieldName, 0, 2) === 'im') {
      $form->add('text', $name, $title, $attributes, $required);
      if (!$contactId) {
        if ($onBehalf) {
          if (substr($name, -1) == ']') {
            $providerName = substr($name, 0, -1) . '-provider_id]';
          }
          $form->add('select', $providerName, NULL,
            array(
              '' => ts('- select -')) + CRM_Core_PseudoConstant::IMProvider(), $required
          );
        }
        else {
          $form->add('select', $name . '-provider_id', $title,
            array(
              '' => ts('- select -')) + CRM_Core_PseudoConstant::IMProvider(), $required
          );
        }

        if ($view && $mode != CRM_Profile_Form::MODE_SEARCH) {
          $form->freeze($name . '-provider_id');
        }
      }
    }
    elseif (($fieldName === 'birth_date') || ($fieldName === 'deceased_date')) {
      $form->addDate($name, $title, $required, array('formatType' => 'birth'));
    }
    elseif (in_array($fieldName, array(
      'membership_start_date', 'membership_end_date', 'join_date'))) {
      $form->addDate($name, $title, $required, array('formatType' => 'custom'));
    }
    elseif ($field['name'] == 'membership_type') {
      require_once 'CRM/Member/PseudoConstant.php';
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Member_PseudoConstant::membershipType(), $required
      );
    }
    elseif ($field['name'] == 'membership_status') {
      require_once 'CRM/Member/PseudoConstant.php';
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Member_PseudoConstant::membershipStatus(NULL, NULL, 'label'), $required
      );
    }
    elseif ($fieldName === 'gender') {
      $genderOptions = array();
      $gender = CRM_Core_PseudoConstant::gender();
      foreach ($gender as $key => $var) {
        $genderOptions[$key] = HTML_QuickForm::createElement('radio', NULL, ts('Gender'), $var, $key);
      }
      $form->addGroup($genderOptions, $name, $title);
      if ($required) {
        $form->addRule($name, ts('%1 is a required field.', array(1 => $title)), 'required');
      }
    }
    elseif ($fieldName === 'individual_prefix') {
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Core_PseudoConstant::individualPrefix(), $required
      );
    }
    elseif ($fieldName === 'individual_suffix') {
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Core_PseudoConstant::individualSuffix(), $required
      );
    }
    elseif ($fieldName === 'contact_sub_type') {
      $gId = $form->get('gid') ? $form->get('gid') : CRM_Utils_Array::value('group_id', $form->_fields[$fieldName]);
      $profileType = $gId ? CRM_Core_BAO_UFField::getProfileType($gId) : NULL;

      $setSubtype = FALSE;
      if (CRM_Contact_BAO_ContactType::isaSubType($profileType)) {
        $setSubtype = $profileType;
        $profileType = CRM_Contact_BAO_ContactType::getBasicType($profileType);
      }
      $subtypes = $profileType ? CRM_Contact_BAO_ContactType::subTypePairs($profileType) : array();
      if ($setSubtype) {
        $subtypeList = array();
        $subtypeList[$setSubtype] = $subtypes[$setSubtype];
      }
      else {
        $subtypeList = $subtypes;
      }

      $sel = $form->add('select', $name, $title, $subtypeList, $required);
      $sel->setMultiple(TRUE);
    }
    elseif (in_array($fieldName, CRM_Contact_BAO_Contact::$_greetingTypes)) {
      //add email greeting, postal greeting, addressee, CRM-4575
      $gId = $form->get('gid') ? $form->get('gid') : CRM_Utils_Array::value('group_id', $field);
      $profileType = CRM_Core_BAO_UFField::getProfileType($gId, TRUE, FALSE, TRUE);

      if (empty($profileType) || in_array($profileType, array(
        'Contact', 'Contribution', 'Participant', 'Membership'))) {
        $profileType = 'Individual';
      }
      if (CRM_Contact_BAO_ContactType::isaSubType($profileType)) {
        $profileType = CRM_Contact_BAO_ContactType::getBasicType($profileType);
      }
      $greeting = array(
        'contact_type' => $profileType,
        'greeting_type' => $fieldName,
      );
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Core_PseudoConstant::greeting($greeting), $required
      );
      // add custom greeting element
      $form->add('text', $fieldName . '_custom', ts('Custom ' . ucwords(str_replace('_', ' ', $fieldName))),
        NULL, FALSE
      );
    }
    elseif ($fieldName === 'preferred_communication_method') {
      $communicationFields = CRM_Core_PseudoConstant::pcm();
      foreach ($communicationFields as $key => $var) {
        if ($key == '') {
          continue;
        }
        $communicationOptions[] = HTML_QuickForm::createElement('checkbox', $key, NULL, $var);
      }
      $form->addGroup($communicationOptions, $name, $title, '<br/>');
    }
    elseif ($fieldName === 'preferred_mail_format') {
      $form->add('select', $name, $title, CRM_Core_SelectValues::pmf());
    }
    elseif ($fieldName === 'preferred_language') {
      $form->add('select', $name, $title, array('' => ts('- select -')) + CRM_Core_PseudoConstant::languages());
    }
    elseif ($fieldName == 'external_identifier') {
      $form->add('text', $name, $title, $attributes, $required);
      $contID = $contactId;
      if (!$contID) {
        $contID = $form->get('id');
      }
      $form->addRule($name,
        ts('External ID already exists in Database.'),
        'objectExists',
        array('CRM_Contact_DAO_Contact', $contID, 'external_identifier')
      );
    }
    elseif ($fieldName === 'group') {
      require_once 'CRM/Contact/Form/Edit/TagsAndGroups.php';
      CRM_Contact_Form_Edit_TagsAndGroups::buildQuickForm($form, $contactId,
        CRM_Contact_Form_Edit_TagsAndGroups::GROUP,
        TRUE, $required,
        $title, NULL, $name
      );
    }
    elseif ($fieldName === 'tag') {
      require_once 'CRM/Contact/Form/Edit/TagsAndGroups.php';
      CRM_Contact_Form_Edit_TagsAndGroups::buildQuickForm($form, $contactId,
        CRM_Contact_Form_Edit_TagsAndGroups::TAG,
        FALSE, $required,
        NULL, $title, $name
      );
    }
    elseif (substr($fieldName, 0, 4) === 'url-') {
      $form->addElement('text', $name, $title,
        array_merge(CRM_Core_DAO::getAttribute('CRM_Core_DAO_Website', 'url'),
          array(
            'onfocus' => "if (!this.value) {  this.value='http://';} else return false",
            'onblur' => "if ( this.value == 'http://') {  this.value='';} else return false",
          )
        )
      );

      $form->addRule($name, ts('Enter a valid Website.'), 'url');

      //Website type select
      if ($onBehalf) {
        if (substr($name, -1) == ']') {
          $websiteTypeName = substr($name, 0, -1) . '-website_type_id]';
        }
        $form->addElement('select', $websiteTypeName, NULL, CRM_Core_PseudoConstant::websiteType());
      }
      else {
        $form->addElement('select', $name . '-website_type_id', NULL, CRM_Core_PseudoConstant::websiteType());
      }
      // added because note appeared as a standard text input
    }
    elseif ($fieldName == 'note') {
      $form->add('textarea', $name, $title, $attributes, $required);
    }
    elseif (substr($fieldName, 0, 6) === 'custom') {
      $customFieldID = CRM_Core_BAO_CustomField::getKeyID($fieldName);
      if ($customFieldID) {
        CRM_Core_BAO_CustomField::addQuickFormElement($form, $name, $customFieldID, FALSE, $required, $search, $title);
      }
    }
    elseif (substr($fieldName, 0, 14) === 'address_custom') {
      list($fName, $locTypeId) = CRM_Utils_System::explode('-', $fieldName, 2);
      $customFieldID = CRM_Core_BAO_CustomField::getKeyID(substr($fName, 8));
      if ($customFieldID) {
        CRM_Core_BAO_CustomField::addQuickFormElement($form, $name, $customFieldID, FALSE, $required, $search, $title);
      }
    }
    elseif (in_array($fieldName, array(
      'receive_date', 'receipt_date', 'thankyou_date', 'cancel_date'))) {
      $form->addDate($name, $title, $required, array('formatType' => 'custom'));
    }
    elseif ($fieldName == 'payment_instrument') {
      require_once 'CRM/Contribute/PseudoConstant.php';
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Contribute_PseudoConstant::paymentInstrument(), $required
      );
    }
    elseif ($fieldName == 'contribution_type') {
      require_once 'CRM/Contribute/PseudoConstant.php';
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Contribute_PseudoConstant::contributionType(), $required
      );
    }
    elseif ($fieldName == 'contribution_page_id') {
      require_once 'CRM/Contribute/PseudoConstant.php';
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Contribute_PseudoConstant::contributionPage(), $required, 'class="big"'
      );
    }
    elseif ($fieldName == 'participant_register_date') {
      $form->addDateTime($name, $title, $required, array('formatType' => 'activityDateTime'));
    }
    elseif ($fieldName == 'activity_status_id') {
      require_once 'CRM/Core/PseudoConstant.php';
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Core_PseudoConstant::activityStatus(), $required
      );
    }
    elseif ($fieldName == 'activity_date_time') {
      $form->addDateTime($name, $title, $required, array('formatType' => 'activityDateTime'));
    }
    elseif ($fieldName == 'participant_status') {
      require_once 'CRM/Event/PseudoConstant.php';
      $cond = NULL;
      if ($online == TRUE) {
        $cond = 'visibility_id = 1';
      }
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Event_PseudoConstant::participantStatus(NULL, $cond, 'label'), $required
      );
    }
    elseif ($fieldName == 'participant_role') {
      require_once 'CRM/Event/PseudoConstant.php';
      if (CRM_Utils_Array::value('is_multiple', $field)) {
        require_once 'CRM/Event/PseudoConstant.php';
        $form->addCheckBox($name, $title, CRM_Event_PseudoConstant::participantRole(), NULL, NULL, NULL, NULL, '&nbsp', TRUE);
      }
      else {
        $form->add('select', $name, $title,
          array(
            '' => ts('- select -')) + CRM_Event_PseudoConstant::participantRole(), $required
        );
      }
    }
    elseif ($fieldName == 'scholarship_type_id') {
      $form->add('select', $name, $title, array('' => '-- Select -- ') + array_flip(CRM_Core_OptionGroup::values('scholarship_type', TRUE)));
    }
    elseif ($fieldName == 'applicant_status_id') {
      $form->add('select', $name, $title, array('' => '-- Select -- ') + array_flip(CRM_Core_OptionGroup::values('applicant_status', TRUE)));
    }
    elseif ($fieldName == 'highschool_gpa_id') {
      $form->add('select', $name, $title, array('' => '-- Select -- ') + CRM_Core_OptionGroup::values('highschool_gpa'));
    }
    elseif ($fieldName == 'world_region') {
      require_once 'CRM/Core/PseudoConstant.php';
      $form->add('select', $name, $title,
        array(
          '' => ts('- select -')) + CRM_Core_PseudoConstant::worldRegion(), $required
      );
    }
    elseif ($fieldName == 'signature_html') {
      $form->addWysiwyg($name, $title, CRM_Core_DAO::getAttribute('CRM_Core_DAO_Email', $fieldName));
    }
    elseif ($fieldName == 'signature_text') {
      $form->add('textarea', $name, $title, CRM_Core_DAO::getAttribute('CRM_Core_DAO_Email', $fieldName));
    }
    elseif (substr($fieldName, -11) == 'campaign_id') {
      require_once 'CRM/Campaign/BAO/Campaign.php';
      if (CRM_Campaign_BAO_Campaign::isCampaignEnable()) {
        $campaigns = CRM_Campaign_BAO_Campaign::getCampaigns(CRM_Utils_Array::value($contactId,
            $form->_componentCampaigns
          ));
        $campaign = &$form->add('select', $name, $title,
          array(
            '' => ts('- select -')) + $campaigns, $required, 'class="big"'
        );
      }
    }
    elseif ($fieldName == 'activity_details') {
      $form->addWysiwyg($fieldName, $title, array('rows' => 4, 'cols' => 60), $required);
    }
    elseif ($fieldName == 'activity_duration') {
      $form->add('text', $fieldName, $title, $attributes, $required);
      $form->addRule($name, ts('Please enter the duration as number of minutes (integers only).'), 'positiveInteger');
    }
    else {
      $processed = FALSE;
      if (CRM_Core_Permission::access('Quest', FALSE)) {
        require_once 'CRM/Quest/BAO/Student.php';
        $processed = CRM_Quest_BAO_Student::buildStudentForm($form, $fieldName, $title, $contactId);
      }
      if (!$processed) {
        if (substr($fieldName, 0, 3) === 'is_' or substr($fieldName, 0, 7) === 'do_not_') {
          $form->add('advcheckbox', $name, $title, $attributes, $required);
        }
        else {
          $form->add('text', $name, $title, $attributes, $required);
        }
      }
    }

    static $hiddenSubtype = FALSE;
    if (!$hiddenSubtype && CRM_Contact_BAO_ContactType::isaSubType($field['field_type'])) {
      // In registration mode params are submitted via POST and we don't have any clue
      // about profile-id or the profile-type (which could be a subtype)
      // To generalize the  behavior and simplify the process,
      // lets always add the hidden
      //subtype value if there is any, and we won't have to
      // compute it while processing.
      $form->addElement('hidden', 'contact_sub_type_hidden', $field['field_type']);
      $hiddenSubtype = TRUE;
    }

    if (($view && $mode != CRM_Profile_Form::MODE_SEARCH) || $isShared) {
      $form->freeze($name);
    }

    //add the rules
    if (in_array($fieldName, array(
      'non_deductible_amount', 'total_amount', 'fee_amount', 'net_amount'))) {
      $form->addRule($name, ts('Please enter a valid amount.'), 'money');
    }

    if ($rule) {
      if (!($rule == 'email' && $mode == CRM_Profile_Form::MODE_SEARCH)) {
        $form->addRule($name, ts('Please enter a valid %1', array(1 => $title)), $rule);
      }
    }
  }

  /**
   * Function to set profile defaults
   *
   * @params int     $contactId      contact id
   * @params array   $fields         associative array of fields
   * @params array   $defaults       defaults array
   * @params boolean $singleProfile  true for single profile else false(batch update)
   * @params int     $componentId    id for specific components like contribute, event etc
   *
   * @return null
   * @static
   * @access public
   */
  static
  function setProfileDefaults($contactId, &$fields, &$defaults,
    $singleProfile = TRUE, $componentId = NULL, $component = NULL
  ) {
    if (!$componentId) {
      //get the contact details
      require_once 'CRM/Contact/BAO/Contact.php';
      list($contactDetails, $options) = CRM_Contact_BAO_Contact::getHierContactDetails($contactId, $fields);
      $details = $contactDetails[$contactId];

      $multipleFields = array('website' => 'url');

      require_once 'CRM/Contact/Form/Edit/TagsAndGroups.php';
      //start of code to set the default values
      foreach ($fields as $name => $field) {
        //set the field name depending upon the profile mode(single/batch)
        if ($singleProfile) {
          $fldName = $name;
        }
        else {
          $fldName = "field[$contactId][$name]";
        }

        if ($name == 'group') {
          CRM_Contact_Form_Edit_TagsAndGroups::setDefaults($contactId, $defaults, CRM_Contact_Form_Edit_TagsAndGroups::GROUP, $fldName);
        }
        if ($name == 'tag') {
          CRM_Contact_Form_Edit_TagsAndGroups::setDefaults($contactId, $defaults, CRM_Contact_Form_Edit_TagsAndGroups::TAG, $fldName);
        }

        if (CRM_Utils_Array::value($name, $details) || isset($details[$name])) {
          //to handle custom data (checkbox) to be written
          // to handle gender / suffix / prefix / greeting_type
          if ($name == 'gender') {
            $defaults[$fldName] = $details['gender_id'];
          }
          elseif ($name == 'individual_prefix') {
            $defaults[$fldName] = $details['individual_prefix_id'];
          }
          elseif ($name == 'individual_suffix') {
            $defaults[$fldName] = $details['individual_suffix_id'];
          }
          elseif (($name == 'birth_date') || ($name == 'deceased_date')) {
            list($defaults[$fldName]) = CRM_Utils_Date::setDateDefaults($details[$name], 'birth');
          }
          elseif (in_array($name, CRM_Contact_BAO_Contact::$_greetingTypes)) {
            $defaults[$fldName] = $details[$name . '_id'];
            $defaults[$name . '_custom'] = $details[$name . '_custom'];
          }
          elseif ($name == 'preferred_communication_method') {
            $v = explode(CRM_Core_DAO::VALUE_SEPARATOR, $details[$name]);
            foreach ($v as $item) {
              if ($item) {
                $defaults[$fldName . "[$item]"] = 1;
              }
            }
          }
          elseif ($name == 'world_region') {
            $defaults[$fldName] = $details['worldregion_id'];
          }
          elseif ($customFieldId = CRM_Core_BAO_CustomField::getKeyID($name)) {
            //fix for custom fields
            $customFields = CRM_Core_BAO_CustomField::getFields(CRM_Utils_Array::value('Individual', $values));

            // hack to add custom data for components
            $components = array('Contribution', 'Participant', 'Membership', 'Activity');
            foreach ($components as $value) {
              $customFields = CRM_Utils_Array::crmArrayMerge($customFields,
                CRM_Core_BAO_CustomField::getFieldsForImport($value)
              );
            }

            switch ($customFields[$customFieldId]['html_type']) {
              case 'Multi-Select State/Province':
              case 'Multi-Select Country':
              case 'AdvMulti-Select':
              case 'Multi-Select':
                $v = explode(CRM_Core_DAO::VALUE_SEPARATOR, $details[$name]);
                foreach ($v as $item) {
                  if ($item) {
                    $defaults[$fldName][$item] = $item;
                  }
                }
                break;

              case 'CheckBox':
                $v = explode(CRM_Core_DAO::VALUE_SEPARATOR, $details[$name]);
                foreach ($v as $item) {
                  if ($item) {
                    $defaults[$fldName][$item] = 1;
                    // seems like we need this for QF style checkboxes in profile where its multiindexed
                    // CRM-2969
                    $defaults["{$fldName}[{$item}]"] = 1;
                  }
                }
                break;

              case 'Autocomplete-Select':
                if ($customFields[$customFieldId]['data_type'] == 'ContactReference') {
                  require_once 'CRM/Contact/BAO/Contact.php';
                  if (is_numeric($details[$name])) {
                    $defaults[$fldName . '_id'] = $details[$name];
                    $defaults[$fldName] = CRM_Core_DAO::getFieldValue('CRM_Contact_DAO_Contact', $details[$name], 'sort_name');
                  }
                }
                else {
                  $label = CRM_Core_BAO_CustomOption::getOptionLabel($customFieldId, $details[$name]);
                  $defaults[$fldName . '_id'] = $details[$name];
                  $defaults[$fldName] = $label;
                }
                break;

              case 'Select Date':
                // CRM-6681, set defult values according to date and time format (if any).
                $dateFormat = NULL;
                if (CRM_Utils_Array::value('date_format', $field)) {
                  $dateFormat = $field['date_format'];
                }

                if (!CRM_Utils_Array::value('time_format', $field)) {
                  list($defaults[$fldName]) = CRM_Utils_Date::setDateDefaults($details[$name], NULL,
                    $dateFormat
                  );
                }
                else {
                  $timeElement = $fldName . '_time';
                  if (substr($fldName, -1) == ']') {
                    $timeElement = substr($fldName, 0, -1) . '_time]';
                  }
                  list($defaults[$fldName], $defaults[$timeElement]) = CRM_Utils_Date::setDateDefaults($details[$name], NULL, $dateFormat, $field['time_format']);
                }
                break;

              default:
                $defaults[$fldName] = $details[$name];
                break;
            }
          }
          else {
            $defaults[$fldName] = $details[$name];
          }
        }
        else {
          list($fieldName, $locTypeId, $phoneTypeId) = CRM_Utils_System::explode('-', $name, 3);
          if (!in_array($fieldName, $multipleFields)) {
            if (is_array($details)) {
              foreach ($details as $key => $value) {
                // when we fixed CRM-5319 - get primary loc
                // type as per loc field and removed below code.
                if ($locTypeId == 'Primary') {
                  $locTypeId = CRM_Contact_BAO_Contact::getPrimaryLocationType($contactId);
                }

                // fixed for CRM-665
                if (is_numeric($locTypeId)) {
                  if ($locTypeId == CRM_Utils_Array::value('location_type_id', $value)) {
                    if (CRM_Utils_Array::value($fieldName, $value)) {
                      //to handle stateprovince and country
                      if ($fieldName == 'state_province') {
                        $defaults[$fldName] = $value['state_province_id'];
                      }
                      elseif ($fieldName == 'county') {
                        $defaults[$fldName] = $value['county_id'];
                      }
                      elseif ($fieldName == 'country') {
                        $defaults[$fldName] = $value['country_id'];
                        if (!isset($value['country_id']) || !$value['country_id']) {
                          $config = CRM_Core_Config::singleton();
                          if ($config->defaultContactCountry) {
                            $defaults[$fldName] = $config->defaultContactCountry;
                          }
                        }
                      }
                      elseif ($fieldName == 'phone') {
                        if ($phoneTypeId) {
                          if (isset($value['phone'][$phoneTypeId])) {
                            $defaults[$fldName] = $value['phone'][$phoneTypeId];
                          }
                        }
                        else {
                          $phoneDefault = CRM_Utils_Array::value('phone', $value);
                          // CRM-9216
                          if (!is_array($phoneDefault)) {
                            $defaults[$fldName] = $phoneDefault;
                          }
                        }
                      }
                      elseif ($fieldName == 'email') {
                        //adding the first email (currently we don't support multiple emails of same location type)
                        $defaults[$fldName] = $value['email'];
                      }
                      elseif ($fieldName == 'im') {
                        //adding the first im (currently we don't support multiple ims of same location type)
                        $defaults[$fldName] = $value['im'];
                        $defaults[$fldName . '-provider_id'] = $value['im_provider_id'];
                      }
                      else {
                        $defaults[$fldName] = $value[$fieldName];
                      }
                    }
                    elseif (substr($fieldName, 0, 14) === 'address_custom' &&
                      CRM_Utils_Array::value(substr($fieldName, 8), $value)
                    ) {
                      $defaults[$fldName] = $value[substr($fieldName, 8)];
                    }
                  }
                }
              }
            }
          }
          else {
            if (is_array($details)) {
              if ($fieldName === 'url') {
                if (!empty($details['website'])) {
                  foreach ($details['website'] as $val) {
                    $defaults[$fldName] = $val['url'];
                    $defaults[$fldName . '-website_type_id'] = $val['website_type_id'];
                  }
                }
              }
            }
          }
        }

        if (CRM_Core_Permission::access('Quest', FALSE)) {
          require_once 'CRM/Quest/BAO/Student.php';
          // Checking whether the database contains quest_student table.
          // Now there are two different schemas for core and quest.
          // So if only core schema in use then withought following check gets the DB error.
          $student = new CRM_Quest_BAO_Student();
          $tableStudent = $student->getTableName();

          if ($tableStudent) {
            //set student defaults
            CRM_Quest_BAO_Student::retrieve($details, $studentDefaults, $ids);
            $studentFields = array('educational_interest', 'college_type', 'college_interest', 'test_tutoring');
            foreach ($studentFields as $fld) {
              if ($studentDefaults[$fld]) {
                $values = explode(CRM_Core_DAO::VALUE_SEPARATOR, $studentDefaults[$fld]);
              }

              $studentDefaults[$fld] = array();
              if (is_array($values)) {
                foreach ($values as $v) {
                  $studentDefaults[$fld][$v] = 1;
                }
              }
            }

            foreach ($fields as $name => $field) {
              $fldName = "field[$contactId][$name]";
              if (array_key_exists($name, $studentDefaults)) {
                $defaults[$fldName] = $studentDefaults[$name];
              }
            }
          }
        }
      }
    }

    //Handling Contribution Part of the batch profile
    if (CRM_Core_Permission::access('CiviContribute') && $component == 'Contribute') {
      self::setComponentDefaults($fields, $componentId, $component, $defaults);
    }

    //Handling Event Participation Part of the batch profile
    if (CRM_Core_Permission::access('CiviEvent') && $component == 'Event') {
      self::setComponentDefaults($fields, $componentId, $component, $defaults);
    }

    //Handling membership Part of the batch profile
    if (CRM_Core_Permission::access('CiviMember') && $component == 'Membership') {
      self::setComponentDefaults($fields, $componentId, $component, $defaults);
    }

    //Handling Activity Part of the batch profile
    if ($component == 'Activity') {
      self::setComponentDefaults($fields, $componentId, $component, $defaults);
    }
  }

  /**
   * Function to get profiles by type  eg: pure Individual etc
   *
   * @param array   $types      associative array of types eg: types('Individual')
   * @param boolean $onlyPure   true if only pure profiles are required
   *
   * @return array  $profiles  associative array of profiles
   * @static
   * @access public
   */
  static
  function getProfiles($types, $onlyPure = FALSE) {
    require_once 'CRM/Core/BAO/UFField.php';
    $profiles = array();
    $ufGroups = CRM_Core_PseudoConstant::ufgroup();

    require_once 'CRM/Utils/Hook.php';
    CRM_Utils_Hook::aclGroup(CRM_Core_Permission::ADMIN, NULL, 'civicrm_uf_group', $ufGroups, $ufGroups);

    foreach ($ufGroups as $id => $title) {
      $ptype = CRM_Core_BAO_UFField::getProfileType($id, FALSE, $onlyPure);
      if (in_array($ptype, $types)) {
        $profiles[$id] = $title;
      }
    }

    return $profiles;
  }

  /**
   * Function to check whether a profile is valid combination of
   * required and/or optional profile types
   *
   * @param array   $required   array of types those are required
   * @param array   $optional   array of types those are optional
   *
   * @return array  $profiles  associative array of profiles
   * @static
   * @access public
   */
  static
  function getValidProfiles($required, $optional = NULL) {
    if (!is_array($required) || empty($required)) {
      return;
    }

    require_once 'CRM/Core/BAO/UFField.php';
    $profiles = array();
    $ufGroups = CRM_Core_PseudoConstant::ufgroup();

    require_once 'CRM/Utils/Hook.php';
    CRM_Utils_Hook::aclGroup(CRM_Core_Permission::ADMIN, NULL, 'civicrm_uf_group', $ufGroups, $ufGroups);

    foreach ($ufGroups as $id => $title) {
      $type = CRM_Core_BAO_UFField::checkValidProfileType($id, $required, $optional);
      if ($type) {
        $profiles[$id] = $title;
      }
    }

    return $profiles;
  }

  /**
   * Function to check whether a profile is valid combination of
   * required profile fields
   *
   * @param array   $ufId       integer id of the profile
   * @param array   $required   array of fields those are required in the profile
   *
   * @return array  $profiles  associative array of profiles
   * @static
   * @access public
   */
  static
  function checkValidProfile($ufId, $required = NULL) {
    $validProfile = FALSE;
    if (!$ufId) {
      return $validProfile;
    }

    if (!CRM_Core_DAO::getFieldValue('CRM_Core_DAO_UFGroup', $ufId, 'is_active')) {
      return $validProfile;
    }

    $profileFields = self::getFields($ufId, FALSE, CRM_Core_Action::VIEW, NULL,
      NULL, FALSE, NULL, FALSE, NULL,
      CRM_Core_Permission::CREATE, NULL
    );

    $validProfile = array();
    if (!empty($profileFields)) {
      $fields = array_keys($profileFields);
      foreach ($fields as $val) {
        foreach ($required as $key => $field) {
          if (strpos($val, $field) === 0) {
            unset($required[$key]);
          }
        }
      }

      $validProfile = (empty($required)) ? TRUE : FALSE;
    }

    return $validProfile;
  }

  /**
   * Function to get default value for Register.
   *
   * @return $defaults
   * @static
   * @access public
   */
  static
  function setRegisterDefaults(&$fields, &$defaults) {
    foreach ($fields as $name => $field) {
      if (substr($name, 0, 8) == 'country-') {
        $config = CRM_Core_Config::singleton();
        if ($config->defaultContactCountry) {
          $defaults[$name] = $config->defaultContactCountry;
        }
      }
    }
    return $defaults;
  }

  /**
   * This function is to make a copy of a profile, including
   * all the fields in the profile
   *
   * @param int $id the profile id to copy
   *
   * @return void
   * @access public
   */
  static
  function copy($id) {
    $fieldsFix = array('prefix' => array('title' => ts('Copy of ')));
    $copy = &CRM_Core_DAO::copyGeneric('CRM_Core_DAO_UFGroup',
      array('id' => $id),
      NULL,
      $fieldsFix
    );

    if ($pos = strrpos($copy->name, "_{$id}")) {
      $copy->name = substr_replace($copy->name, '', $pos);
    }
    $copy->name = CRM_Utils_String::munge($copy->name, '_', 56) . "_{$copy->id}";
    $copy->save();

    $copyUFJoin = &CRM_Core_DAO::copyGeneric('CRM_Core_DAO_UFJoin',
      array('uf_group_id' => $id),
      array('uf_group_id' => $copy->id),
      NULL,
      'entity_table'
    );

    $copyUFField = &CRM_Core_DAO::copyGeneric('CRM_Core_BAO_UFField',
      array('uf_group_id' => $id),
      array('uf_group_id' => $copy->id)
    );

    require_once 'CRM/Utils/Weight.php';
    $maxWeight = CRM_Utils_Weight::getMax('CRM_Core_DAO_UFJoin', NULL, 'weight');

    //update the weight
    $query = "
UPDATE civicrm_uf_join 
SET    weight = %1
WHERE  uf_group_id = %2
AND    ( entity_id IS NULL OR entity_id <= 0 )
";
    $p = array(1 => array($maxWeight + 1, 'Integer'),
      2 => array($copy->id, 'Integer'),
    );
    CRM_Core_DAO::executeQuery($query, $p);
    if ($copy->is_reserved) {
      $query = "UPDATE civicrm_uf_group SET is_reserved = 0 WHERE id = %1";
      $params = array(1 => array($copy->id, 'Integer'));
      CRM_Core_DAO::executeQuery($query, $params);
    }
    require_once 'CRM/Utils/Hook.php';
    CRM_Utils_Hook::copy('UFGroup', $copy);

    return $copy;
  }

  /**
   * Process that send notification e-mails
   *
   * @params int     $contactId      contact id
   * @params array   $values         associative array of name/value pair
   *
   * @return void
   * @access public
   */

  static
  function commonSendMail($contactID, &$values) {
    if (!$contactID || !$values) {
      return;
    }

    $template = CRM_Core_Smarty::singleton();

    $displayName = CRM_Core_DAO::getFieldValue('CRM_Contact_DAO_Contact',
      $contactID,
      'display_name'
    );

    self::profileDisplay($values['id'], $values['values'], $template);
    $emailList = explode(',', $values['email']);

    $contactLink = CRM_Utils_System::url('civicrm/contact/view',
      "reset=1&cid=$contactID",
      TRUE, NULL, FALSE
    );


    //get the default domain email address.
    require_once 'CRM/Core/BAO/Domain.php';
    list($domainEmailName, $domainEmailAddress) = CRM_Core_BAO_Domain::getNameAndEmail();

    if (!$domainEmailAddress || $domainEmailAddress == 'info@FIXME.ORG') {
      require_once 'CRM/Utils/System.php';
      $fixUrl = CRM_Utils_System::url('civicrm/admin/domain', 'action=update&reset=1');
      CRM_Core_Error::fatal(ts('The site administrator needs to enter a valid \'FROM Email Address\' in <a href="%1">Administer CiviCRM &raquo; Configure &raquo; Domain Information</a>. The email address used may need to be a valid mail account with your email service provider.', array(1 => $fixUrl)));
    }

    require_once 'CRM/Core/BAO/MessageTemplates.php';
    foreach ($emailList as $emailTo) {
      // FIXME: take the below out of the foreach loop
      CRM_Core_BAO_MessageTemplates::sendTemplate(
        array(
          'groupName' => 'msg_tpl_workflow_uf',
          'valueName' => 'uf_notify',
          'contactId' => $contactID,
          'tplParams' => array(
            'displayName' => $displayName,
            'currentDate' => date('r'),
            'contactLink' => $contactLink,
          ),
          'from' => "$domainEmailName <$domainEmailAddress>",
          'toEmail' => $emailTo,
        )
      );
    }
  }

  /**
   * Given a contact id and a group id, returns the field values from the db
   * for this group and notify email only if group's notify field is
   * set and field values are not empty
   *
   * @params $gid      group id
   * @params $cid      contact id
   * @params $params   associative array
   *
   * @return array
   * @access public
   */
  function checkFieldsEmptyValues($gid, $cid, $params) {
    if ($gid) {
      require_once 'CRM/Core/BAO/UFGroup.php';
      if (CRM_Core_BAO_UFGroup::filterUFGroups($gid, $cid)) {
        $values = array();
        $fields = CRM_Core_BAO_UFGroup::getFields($gid, FALSE, CRM_Core_Action::VIEW);
        CRM_Core_BAO_UFGroup::getValues($cid, $fields, $values, FALSE, $params, TRUE);

        $email = CRM_Core_DAO::getFieldValue('CRM_Core_DAO_UFGroup', $gid, 'notify');

        if (!empty($values) &&
          !empty($email)
        ) {
          $val = array(
            'id' => $gid,
            'values' => $values,
            'email' => $email,
          );
          return $val;
        }
      }
    }
    return NULL;
  }

  /**
   * Function to assign uf fields to template
   *
   * @params int     $gid      group id
   * @params array   $values   associative array of fields
   *
   * @return void
   * @access public
   */
  function profileDisplay($gid, $values, $template) {
    $groupTitle = CRM_Core_DAO::getFieldValue('CRM_Core_DAO_UFGroup', $gid, 'title');
    $template->assign('grouptitle', $groupTitle);
    if (count($values)) {
      $template->assign('values', $values);
    }
  }

  /**
   * Format fields for dupe Contact Matching
   *
   * @param array $params associated array
   *
   * @return array $data assoicated formatted array
   * @access public
   * @static
   */
  static
  function formatFields($params, $contactId = NULL) {
    if ($contactId) {
      // get the primary location type id and email
      require_once 'CRM/Contact/BAO/Contact/Location.php';
      list($name, $primaryEmail, $primaryLocationType) = CRM_Contact_BAO_Contact_Location::getEmailDetails($contactId);
    }
    else {
      require_once 'CRM/Core/BAO/LocationType.php';
      $defaultLocationType = CRM_Core_BAO_LocationType::getDefault();
      $primaryLocationType = $defaultLocationType->id;
    }

    $data            = array();
    $locationType    = array();
    $count           = 1;
    $primaryLocation = 0;
    foreach ($params as $key => $value) {
      list($fieldName, $locTypeId, $phoneTypeId) = explode('-', $key);

      if ($locTypeId == 'Primary') {
        $locTypeId = $primaryLocationType;
      }

      if (is_numeric($locTypeId)) {
        if (!in_array($locTypeId, $locationType)) {
          $locationType[$count] = $locTypeId;
          $count++;
        }
        require_once 'CRM/Utils/Array.php';
        $loc = CRM_Utils_Array::key($locTypeId, $locationType);

        $data['location'][$loc]['location_type_id'] = $locTypeId;

        // if we are getting in a new primary email, dont overwrite the new one
        if ($locTypeId == $primaryLocationType) {
          if (CRM_Utils_Array::value('email-' . $primaryLocationType, $params)) {
            $data['location'][$loc]['email'][$loc]['email'] = $fields['email-' . $primaryLocationType];
          }
          elseif (isset($primaryEmail)) {
            $data['location'][$loc]['email'][$loc]['email'] = $primaryEmail;
          }
          $primaryLocation++;
        }

        if ($loc == 1) {
          $data['location'][$loc]['is_primary'] = 1;
        }
        if ($fieldName == 'phone') {
          if ($phoneTypeId) {
            $data['location'][$loc]['phone'][$loc]['phone_type_id'] = $phoneTypeId;
          }
          else {
            $data['location'][$loc]['phone'][$loc]['phone_type_id'] = '';
          }
          $data['location'][$loc]['phone'][$loc]['phone'] = $value;
        }
        elseif ($fieldName == 'email') {
          $data['location'][$loc]['email'][$loc]['email'] = $value;
        }
        elseif ($fieldName == 'im') {
          $data['location'][$loc]['im'][$loc]['name'] = $value;
        }
        else {
          if ($fieldName === 'state_province') {
            $data['location'][$loc]['address']['state_province_id'] = $value;
          }
          elseif ($fieldName === 'country') {
            $data['location'][$loc]['address']['country_id'] = $value;
          }
          else {
            $data['location'][$loc]['address'][$fieldName] = $value;
          }
        }
      }
      else {
        if ($key === 'individual_suffix') {
          $data['suffix_id'] = $value;
        }
        elseif ($key === 'individual_prefix') {
          $data['prefix_id'] = $value;
        }
        elseif ($key === 'gender') {
          $data['gender_id'] = $value;
        }
        elseif (substr($key, 0, 6) === 'custom') {
          if ($customFieldID = CRM_Core_BAO_CustomField::getKeyID($key)) {
            //fix checkbox
            if ($customFields[$customFieldID]['html_type'] == 'CheckBox') {
              $value = implode(CRM_Core_DAO::VALUE_SEPARATOR, array_keys($value));
            }
            // fix the date field
            if ($customFields[$customFieldID]['data_type'] == 'Date') {
              $date = CRM_Utils_Date::format($value);
              if (!$date) {
                $date = '';
              }
              $value = $date;
            }

            $data['custom'][$customFieldID] = array(
              'id' => $id,
              'value' => $value,
              'extends' => $customFields[$customFieldID]['extends'],
              'type' => $customFields[$customFieldID]['data_type'],
              'custom_field_id' => $customFieldID,
            );
          }
        }
        elseif ($key == 'edit') {
          continue;
        }
        else {
          $data[$key] = $value;
        }
      }
    }

    if (!$primaryLocation) {
      $loc++;
      $data['location'][$loc]['email'][$loc]['email'] = $primaryEmail;
    }


    return $data;
  }

  /**
   * calculate the profile type 'group_type' as per profile fields.
   *
   * @param int $gid           profile id
   * @param int $ignoreFieldId ignore perticular profile field
   *
   * @return array list of calculated group type
   */
  function calculateGroupType($gId, $includeTypeValues = FALSE, $ignoreFieldId = NULL) {
    //get the profile fields.
    $ufFields = self::getFields($gId, FALSE, NULL, NULL, NULL, TRUE, NULL, TRUE);
    $groupType = $groupTypeValues = $customFieldIds = array();
    if (!empty($ufFields)) {
      foreach ($ufFields as $fieldName => $fieldValue) {
        //ignore field from group type when provided.
        //in case of update profile field.
        if ($ignoreFieldId && ($ignoreFieldId == $fieldValue['field_id'])) {
          continue;
        }
        if (!in_array($fieldValue['field_type'], $groupType)) {
          $groupType[$fieldValue['field_type']] = $fieldValue['field_type'];
        }

        if ($includeTypeValues && ($fldId = CRM_Core_BAO_CustomField::getKeyID($fieldName))) {
          $customFieldIds[$fldId] = $fldId;
        }
      }
    }

    if (!empty($customFieldIds)) {
      $query = 'SELECT DISTINCT(cg.id), cg.extends, cg.extends_entity_column_id, cg.extends_entity_column_value FROM civicrm_custom_group cg LEFT JOIN civicrm_custom_field cf ON cf.custom_group_id = cg.id WHERE cg.extends_entity_column_value IS NOT NULL AND cf.id IN (' . implode(',', $customFieldIds) . ')';

      $customGroups = CRM_Core_DAO::executeQuery($query);
      while ($customGroups->fetch()) {
        if (!$customGroups->extends_entity_column_value) {
          continue;
        }

        $groupTypeName = "{$customGroups->extends}Type";
        if ($customGroups->extends == 'Participant' && $customGroups->extends_entity_column_id) {
          require_once 'CRM/Core/OptionGroup.php';
          $groupTypeName = CRM_Core_OptionGroup::getValue('custom_data_type', $customGroups->extends_entity_column_id, 'value', 'String', 'name');
        }

        foreach (explode(CRM_Core_DAO::VALUE_SEPARATOR, $customGroups->extends_entity_column_value) as $val) {
          if ($val) {
            $groupTypeValues[$groupTypeName][$val] = $val;
          }
        }
      }

      if (!empty($groupTypeValues)) {
        $groupType = array_merge($groupType, $groupTypeValues);
      }
    }

    return $groupType;
  }

  /**
   * Update the profile type 'group_type' as per profile fields including group types and group subtype values.
   * Build and store string like: group_type1,group_type2[VALUE_SEPERATOR]group_type1Type:1:2:3,group_type2Type:1:2
   *
   * @param  Integer $gid         profile id
   * @param  Array   $groupTypes  With key having group type names
   *
   * @return Boolean
   */
  function updateGroupTypes($gId, $groupTypes = array(
    )) {
    if (!is_array($groupTypes) || !$gId) {
      return FALSE;
    }

    // If empty group types set group_type as 'null'
    if (empty($groupTypes)) {
      return CRM_Core_DAO::setFieldValue('CRM_Core_DAO_UFGroup', $gId, 'group_type', 'null');
    }

    $componentGroupTypes = array('Contribution', 'Participant', 'Membership', 'Activity');
    $validGroupTypes = array_merge(array('Contact', 'Individual', 'Organization', 'Household'), $componentGroupTypes, CRM_Contact_BAO_ContactType::subTypes());

    $gTypes = $gTypeValues = array();

    $participantExtends = array('ParticipantRole', 'ParticipantEventName', 'ParticipantEventType');
    // Get valid group type and group subtypes
    foreach ($groupTypes as $groupType => $value) {
      if (in_array($groupType, $validGroupTypes) && !in_array($groupType, $gTypes)) {
        $gTypes[] = $groupType;
      }

      $subTypesOf = NULL;

      if (in_array($groupType, $participantExtends)) {
        $subTypesOf = $groupType;
      }
      elseif (strpos($groupType, 'Type') > 0) {
        $subTypesOf = substr($groupType, 0, strpos($groupType, 'Type'));
      }
      else {
        continue;
      }

      if (!empty($value) &&
        (in_array($subTypesOf, $componentGroupTypes) ||
          in_array($subTypesOf, $participantExtends)
        )
      ) {
        $gTypeValues[$subTypesOf] = $groupType . ":" . implode(':', $value);
      }
    }

    if (empty($gTypes)) {
      return FALSE;
    }

    // Build String to store group types and group subtypes
    $groupTypeString = implode(',', $gTypes);
    if (!empty($gTypeValues)) {
      $groupTypeString .= CRM_Core_DAO::VALUE_SEPARATOR . implode(',', $gTypeValues);
    }

    return CRM_Core_DAO::setFieldValue('CRM_Core_DAO_UFGroup', $gId, 'group_type', $groupTypeString);
  }

  /**
   * This function is used to setDefault componet specific profile fields.
   *
   * @param array  $fields      profile fields.
   * @param int    $componentId componetID
   * @param string $component   component name
   * @param array  $defaults    an array of default values.
   *
   * @return void.
   */
  function setComponentDefaults(&$fields, $componentId, $component, &$defaults, $isStandalone = FALSE) {
    if (!$componentId ||
      !in_array($component, array('Contribute', 'Membership', 'Event', 'Activity'))
    ) {
      return;
    }

    $componentBAO = $componentSubType = NULL;
    switch ($component) {
      case 'Membership':
        $componentBAO     = 'CRM_Member_BAO_Membership';
        $componentBAOName = 'Membership';
        $componentSubType = array('membership_type_id');
        break;

      case 'Contribute':
        $componentBAO     = 'CRM_Contribute_BAO_Contribution';
        $componentBAOName = 'Contribution';
        $componentSubType = array('contribution_type_id');
        break;

      case 'Event':
        $componentBAO     = 'CRM_Event_BAO_Participant';
        $componentBAOName = 'Participant';
        $componentSubType = array('role_id', 'event_id');
        break;

      case 'Activity':
        $componentBAO     = 'CRM_Activity_BAO_Activity';
        $componentBAOName = 'Activity';
        $componentSubType = array('activity_type_id');
        break;
    }

    $values = array();
    $params = array('id' => $componentId);

    //get the component values.
    CRM_Core_DAO::commonRetrieve($componentBAO, $params, $values);

    $formattedGroupTree = array();
    foreach ($fields as $name => $field) {
      $fldName = $isStandalone ? $name : "field[$componentId][$name]";
      if ($name == 'participant_register_date' || $name == 'activity_date_time') {
        $timefldName = $isStandalone ? "{$name}_time" : "field[$componentId][{$name}_time]";
        list($defaults[$fldName], $defaults[$timefldName]) = CRM_Utils_Date::setDateDefaults($values[$name]);
      }
      elseif (array_key_exists($name, $values)) {
        $defaults[$fldName] = $values[$name];
      }
      elseif ($name == 'participant_note') {
        require_once 'CRM/Core/BAO/Note.php';
        $noteDetails        = array();
        $noteDetails        = CRM_Core_BAO_Note::getNote($componentId, 'civicrm_participant');
        $defaults[$fldName] = array_pop($noteDetails);
      }
      elseif (in_array($name, array(
        'contribution_type', 'payment_instrument', 'participant_status', 'participant_role', 'membership_type'))) {
        $defaults[$fldName] = $values["{$name}_id"];
      }
      elseif ($name == 'membership_status') {
        $defaults[$fldName] = $values['status_id'];
      }
      elseif ($customFieldInfo = CRM_Core_BAO_CustomField::getKeyID($name, TRUE)) {
        if (empty($formattedGroupTree)) {
          //get the groupTree as per subTypes.
          $groupTree = array();
          require_once 'CRM/Core/BAO/CustomGroup.php';
          foreach ($componentSubType as $subType) {
            $subTree = CRM_Core_BAO_CustomGroup::getTree($componentBAOName, CRM_Core_DAO::$_nullObject,
              $componentId, 0, $values[$subType]
            );
            $groupTree = CRM_Utils_Array::crmArrayMerge($groupTree, $subTree);
          }
          $formattedGroupTree = CRM_Core_BAO_CustomGroup::formatGroupTree($groupTree, 1, CRM_Core_DAO::$_nullObject);
          CRM_Core_BAO_CustomGroup::setDefaults($formattedGroupTree, $defaults);
        }

        //FIX ME: We need to loop defaults, but once we move to custom_1_x convention this code can be simplified.
        foreach ($defaults as $customKey => $customValue) {
          if ($customFieldDetails = CRM_Core_BAO_CustomField::getKeyID($customKey, TRUE)) {
            if ($name == 'custom_' . $customFieldDetails[0]) {

              //hack to set default for checkbox
              //basically this is for weired field name like field[33][custom_19]
              //we are converting this field name to array structure and assign value.
              $skipValue = FALSE;

              foreach ($formattedGroupTree as $tree) {
                if ('CheckBox' == CRM_Utils_Array::value('html_type', $tree['fields'][$customFieldDetails[0]])) {
                  $skipValue = TRUE;
                  $defaults['field'][$componentId][$name] = $customValue;
                  break;
                }
                elseif (CRM_Utils_Array::value('data_type', $tree['fields'][$customFieldDetails[0]]) == 'Date') {
                  $skipValue = TRUE;

                  // CRM-6681, $default contains formatted date, time values.
                  $defaults[$fldName] = $customValue;
                  if (CRM_Utils_Array::value($customKey . '_time', $defaults)) {
                    $defaults['field'][$componentId][$name . '_time'] = $defaults[$customKey . '_time'];
                  }
                }
              }

              if (!$skipValue || $isStandalone) {
                $defaults[$fldName] = $customValue;
              }
              unset($defaults[$customKey]);
              break;
            }
          }
        }
      }
    }
  }

  /**
   * Function to retrieve reserved profiles
   *
   * @param string $name name if the reserve profile
   * @param array $extraProfiles associated array of profile id's that needs to merge
   *
   * @return array $reservedProfiles returns associated array
   * @static
   */
  static
  function getReservedProfiles($type = 'Contact', $extraProfiles = NULL) {
    $reservedProfiles = array();
    $profileNames = array();
    if ($type == 'Contact') {
      require_once 'CRM/Contact/BAO/ContactType.php';
      $whereClause = 'name IN ( "new_individual", "new_organization", "new_household" )';
      if (CRM_Contact_BAO_ContactType::isActive('Individual')) {
        $profileNames[] = '"new_individual"';
      }
      if (CRM_Contact_BAO_ContactType::isActive('Household')) {
        $profileNames[] = '"new_household"';
      }
      if (CRM_Contact_BAO_ContactType::isActive('Organization')) {
        $profileNames[] = '"new_organization"';
      }
    }
    if (!empty($profileNames)) {
      $whereClause = 'name IN ( ' . implode(',', $profileNames) . ' ) AND is_reserved = 1';
    }
    else {
      $whereClause = 'is_reserved = 1';
    }

    $query = "SELECT id, title FROM civicrm_uf_group WHERE {$whereClause}";

    $dao = CRM_Core_DAO::executeQuery($query);
    while ($dao->fetch()) {
      $key = $dao->id;
      if ($extraProfiles) {
        $key .= ',' . implode(',', $extraProfiles);
      }
      $reservedProfiles[$key] = $dao->title;
    }
    return $reservedProfiles;
  }

  /**
   * Function to retrieve groups of  profiles
   *
   * @param integer $profileID id of the profile
   *
   * @return array  returns array
   * @static
   */

  static
  function profileGroups($profileID) {
    $groupTypes = array();
    $profileTypes = CRM_Core_DAO::getFieldValue('CRM_Core_DAO_UFGroup', $profileID, 'group_type');
    if ($profileTypes) {
      $groupTypeParts = explode(CRM_Core_DAO::VALUE_SEPARATOR, $profileTypes);
      $groupTypes = explode(',', $groupTypeParts[0]);
    }
    return $groupTypes;
  }

  /**
   * Function to alter contact params by filtering existing subscribed groups and returns
   * unsubscribed groups array for subscription.
   *
   * @param  array  $params             contact params
   * @param  int    $contactId          user contact id
   *
   * @return array  $subscribeGroupIds  This contains array of groups for subscription
   */
  static
  function getDoubleOptInGroupIds(&$params, $contactId = NULL) {

    require_once 'CRM/Core/Config.php';
    $config = CRM_Core_Config::singleton();
    $subscribeGroupIds = array();

    // process further only if profileDoubleOptIn enabled and if groups exist
    if (!array_key_exists('group', $params) ||
      !self::isProfileDoubleOptin() ||
      CRM_Utils_System::isNull($params['group'])
    ) {
      return $subscribeGroupIds;
    }

    //check if contact email exist.
    $hasEmails = FALSE;
    foreach ($params as $name => $value) {
      if (strpos($name, 'email-') !== FALSE) {
        $hasEmails = TRUE;
        break;
      }
    }

    //Proceed furthur only if email present
    if (!$hasEmails) {
      return $subscribeGroupIds;
    }

    //do check for already subscriptions.
    $contactGroups = array();
    if ($contactId) {
      $query = "
SELECT  group_id
  FROM  civicrm_group_contact
  WHERE status = 'Added'
    AND contact_id = %1";

      $dao = CRM_Core_DAO::executeQuery($query, array(1 => array($contactId, 'Integer')));
      while ($dao->fetch()) {
        $contactGroups[$dao->group_id] = $dao->group_id;
      }
    }

    //since we don't have names, compare w/ label.
    $mailingListGroupType = array_search('Mailing List', CRM_Core_OptionGroup::values('group_type'));

    //actual processing start.
    foreach ($params['group'] as $groupId => $isSelected) {
      //unset group those are not selected.
      if (!$isSelected) {
        unset($params['group'][$groupId]);
        continue;
      }

      $groupTypes = explode(CRM_Core_DAO::VALUE_SEPARATOR,
        CRM_Core_DAO::getFieldValue('CRM_Contact_DAO_Group', $groupId, 'group_type', 'id')
      );
      //get only mailing type group and unset it from params
      if (in_array($mailingListGroupType, $groupTypes) && !in_array($groupId, $contactGroups)) {
        $subscribeGroupIds[$groupId] = $groupId;
        unset($params['group'][$groupId]);
      }
    }

    return $subscribeGroupIds;
  }

  /**
   * Function to check if we are rendering mixed profiles
   *
   * @param array $profileIds associated array of profile ids
   *
   * @return boolean $mixProfile true if profile is mixed
   * @static
   * @access public
   */
  static
  function checkForMixProfiles($profileIds) {
    $mixProfile = FALSE;

    $contactTypes = array('Individual', 'Household', 'Organization');
    require_once 'CRM/Contact/BAO/ContactType.php';
    $subTypes = CRM_Contact_BAO_ContactType::subTypes();

    $components = array('Contribution', 'Participant', 'Membership', 'Activity');

    require_once 'CRM/Core/BAO/UFField.php';
    $typeCount = array('ctype' => array(), 'subtype' => array());
    foreach ($profileIds as $gid) {
      $profileType = CRM_Core_BAO_UFField::getProfileType($gid);
      // ignore profile of type Contact
      if ($profileType == 'Contact') {
        continue;
      }
      if (in_array($profileType, $contactTypes)) {
        if (!isset($typeCount['ctype'][$profileType])) {
          $typeCount['ctype'][$profileType] = 1;
        }

        // check if we are rendering profile of different contact types
        if (count($typeCount['ctype']) == 2) {
          $mixProfile = TRUE;
          break;
        }
      }
      elseif (in_array($profileType, $components)) {
        $mixProfile = TRUE;
        break;
      }
      else {
        if (!isset($typeCount['subtype'][$profileType])) {
          $typeCount['subtype'][$profileType] = 1;
        }
        // check if we are rendering profile of different contact sub types
        if (count($typeCount['subtype']) == 2) {
          $mixProfile = TRUE;
          break;
        }
      }
    }
    return $mixProfile;
  }

  /**
   * Funtion to determine of we show overlay profile or not
   *
   * @return boolean true if profile should be shown else false
   * @static
   * @access public
   */
  static
  function showOverlayProfile() {
    $showOverlay = TRUE;

    // get the id of overlay profile
    $overlayProfileId = CRM_Core_DAO::getFieldValue('CRM_Core_DAO_UFGroup', 'summary_overlay', 'id', 'name');
    $query = "SELECT count(id) FROM civicrm_uf_field WHERE uf_group_id = {$overlayProfileId} AND visibility IN ('Public Pages', 'Public Pages and Listings') ";

    $count = CRM_Core_DAO::singleValueQuery($query);

    //check if there are no public fields and use is anonymous
    $session = CRM_Core_Session::singleton();
    if (!$count && !$session->get('userID')) {
      $showOverlay = FALSE;
    }

    return $showOverlay;
  }

  /*
     * function to get group type values of the profile
     *
     * @params Integer $profileId       Profile Id
     * @params String  $groupType       Group Type
     *
     * @return Array   group type values
     * @static
     * @access public
     */

  static
  function groupTypeValues($profileId, $groupType = NULL) {
    $groupTypeValue = array();
    $groupTypes = CRM_Core_DAO::getFieldValue('CRM_Core_DAO_UFGroup', $profileId, 'group_type');

    $groupTypeParts = explode(CRM_Core_DAO::VALUE_SEPARATOR, $groupTypes);
    if (!CRM_Utils_Array::value(1, $groupTypeParts)) {
      return $groupTypeValue;
    }
    $participantExtends = array('ParticipantRole', 'ParticipantEventName', 'ParticipantEventType');

    foreach (explode(',', $groupTypeParts[1]) as $groupTypeValues) {
      $values = array();
      $valueParts = explode(':', $groupTypeValues);
      if ($groupType &&
        ($valueParts[0] != "{$groupType}Type" ||
          ($groupType == 'Participant' &&
            !in_array($valueParts[0], $participantExtends)
          )
        )
      ) {
        continue;
      }
      foreach ($valueParts as $val) {
        if (CRM_Utils_Rule::integer($val)) {
          $values[$val] = $val;
        }
      }
      if (!empty($values)) {
        $typeName = substr($valueParts[0], 0, -4);
        if (in_array($valueParts[0], $participantExtends)) {
          $typeName = $valueParts[0];
        }
        $groupTypeValue[$typeName] = $values;
      }
    }

    return $groupTypeValue;
  }

  static
  function isProfileDoubleOptin() {
    // check for double optin
    $config = CRM_Core_Config::singleton();
    if (in_array('CiviMail', $config->enableComponents)) {
      require_once 'CRM/Core/BAO/Setting.php';
      return CRM_Core_BAO_Setting::getItem(CRM_Core_BAO_Setting::MAILING_PREFERENCES_NAME,
        'profile_double_optin', NULL, FALSE
      );
    }
    return FALSE;
  }

  static
  function isProfileAddToGroupDoubleOptin() {
    // check for add to group double optin
    $config = CRM_Core_Config::singleton();
    if (in_array('CiviMail', $config->enableComponents)) {
      require_once 'CRM/Core/BAO/Setting.php';
      return CRM_Core_BAO_Setting::getItem(CRM_Core_BAO_Setting::MAILING_PREFERENCES_NAME,
        'profile_add_to_group_double_optin', NULL, FALSE
      );
    }
    return FALSE;
  }
}

