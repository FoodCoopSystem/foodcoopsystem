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
 * Definition of the Tag of the CRM API.
 * More detailed documentation can be found
 * {@link http://objectledge.org/confluence/display/CRM/CRM+v1.0+Public+APIs
 * here}
 *
 * @package CiviCRM_APIv3
 * @subpackage API_File
 * @copyright CiviCRM LLC (c) 2004-2011
 * $Id: $
 *
 */

/**
 * Files required for this package
 */
require_once 'CRM/Core/DAO/File.php';
require_once 'CRM/Core/BAO/File.php';

/**
 * Create a file
 *
 * This API is used for creating a file
 *
 * @param   array  $params  an associative array of name/value property values of civicrm_file
 *
 * @return array of newly created file property values.
 * @access public
 */
function civicrm_api3_file_create($params) {

  civicrm_api3_verify_mandatory($params, 'CRM_Core_DAO_File', array('file_type_id'));

  if (!isset($params['upload_date'])) {
    $params['upload_date'] = date("Ymd");
  }

  require_once 'CRM/Core/DAO/File.php';

  $fileDAO = new CRM_Core_DAO_File();
  $properties = array('id', 'file_type_id', 'mime_type', 'uri', 'document', 'description', 'upload_date');

  foreach ($properties as $name) {
    if (array_key_exists($name, $params)) {
      $fileDAO->$name = $params[$name];
    }
  }

  $fileDAO->save();

  $file = array();
  _civicrm_api3_object_to_array($fileDAO, $file);

  return civicrm_api3_create_success($file, $params, 'file', 'create', $fileDAO);
}

/**
 * Get a file.
 *
 * This api is used for finding an existing file.
 * Required parameters : id OR file_type_id of a file
 *
 * @param  array $params  an associative array of name/value property values of civicrm_file
 *
 * @return  Array of all found file object property values.
 * @access public
 */
function civicrm_api3_file_get($params) {
  civicrm_api3_verify_one_mandatory($params);
  return _civicrm_api3_basic_get(_civicrm_api3_get_BAO(__FUNCTION__), $params);
}

/**
 * Update an existing file
 *
 * This api is used for updating an existing file.
 * Required parrmeters : id of a file
 *
 * @param  Array   $params  an associative array of name/value property values of civicrm_file
 *
 * @return array of updated file object property values
 * @access public
 */
function &civicrm_api3_file_update($params) {

  if (!isset($params['id'])) {
    return civicrm_api3_create_error('Required parameter missing');
  }

  require_once 'CRM/Core/DAO/File.php';
  $fileDAO = new CRM_Core_DAO_File();
  $fileDAO->id = $params['id'];
  if ($fileDAO->find(TRUE)) {
    $fileDAO->copyValues($params);
    if (!$params['upload_date'] && !$fileDAO->upload_date) {
      $fileDAO->upload_date = date("Ymd");
    }
    $fileDAO->save();
  }
  $file = array();
  _civicrm_api3_object_to_array(clone($fileDAO), $file);
  return $file;
}

/**
 * Deletes an existing file
 *
 * This API is used for deleting a file
 * Required parameters : id of a file
 *
 * @param  Int  $fileId  Id of the file to be deleted
 *
 * @return null if successfull, object of CRM_Core_Error otherwise
 * @access public

 */
function civicrm_api3_file_delete($params) {

  civicrm_api3_verify_mandatory($params, NULL, array('id'));

  $check = FALSE;

  require_once 'CRM/Core/DAO/EntityFile.php';
  $entityFileDAO = new CRM_Core_DAO_EntityFile();
  $entityFileDAO->file_id = $params['id'];
  if ($entityFileDAO->find()) {
    $check = $entityFileDAO->delete();
  }

  require_once 'CRM/Core/DAO/File.php';
  $fileDAO = new CRM_Core_DAO_File();
  $fileDAO->id = $params['id'];
  if ($fileDAO->find(TRUE)) {
    $check = $fileDAO->delete();
  }

  return $check ? NULL : civicrm_api3_create_error('Error while deleting a file.');
}

/**
 * Assigns an entity to a file
 *
 * @param object  $file            id of a file
 * @param object  $entity          id of a entity
 * @param string  $entity_table
 *
 * @return array of newly created entity-file object properties
 * @access public
 */
function civicrm_api3_entity_file_create($params) {

  require_once 'CRM/Core/DAO/EntityFile.php';
  civicrm_api3_verify_one_mandatory($params, NULL, array('file_id', 'entity_id'));

  if (empty($params['entity_table'])) {
    $params['entity_table'] = 'civicrm_contact';
  }

  $entityFileDAO = new CRM_Core_DAO_EntityFile();
  $entityFileDAO->copyValues($params);
  $entityFileDAO->save();

  $entityFile = array();
  _civicrm_api3_object_to_array($entityFileDAO, $entityFile);

  return civicrm_api3_create_success($entityFile, $params, 'entity_file', 'create', $entityFileDAO);
}

/**
 * Returns all files assigned to a single entity instance.
 *
 * @param object $entityID         id of the supported entity.
 * @param string $entity_table
 *
 * @return array   nested array of entity-file property values.
 * @access public
 */
function civicrm_api3_files_by_entity_get($params) {

  civicrm_api3_verify_mandatory($params, NULL, array('entity_id'));
  if (empty($entityTable)) {
    $entityTable = 'civicrm_contact';
  }

  require_once 'CRM/Core/DAO/EntityFile.php';
  require_once 'CRM/Core/DAO/File.php';

  $entityFileDAO = new CRM_Core_DAO_EntityFile();
  $entityFileDAO->entity_table = $entityTable;
  $entityFileDAO->entity_id = $params['entity_id'];
  if ($fileID) {
    $entityFileDAO->file_id = $params['file_id'];
  }
  if ($entityFileDAO->find()) {
    $entityFile = array();
    while ($entityFileDAO->fetch()) {
      _civicrm_api3_object_to_array($entityFileDAO, $entityFile);
      $files[$entityFileDAO->file_id] = $entityFile;

      if (array_key_exists('file_id', $files[$entityFileDAO->file_id])) {
        $fileDAO = new CRM_Core_DAO_File();
        $fileDAO->id = $entityFile['file_id'];
        $fileDAO->find(TRUE);
        _civicrm_api3_object_to_array($fileDAO, $files[$entityFileDAO->file_id]);
      }

      if (CRM_Utils_Array::value('file_type_id', $files[$entityFileDAO->file_id])) {
        $files[$entityFileDAO->file_id]['file_type'] = CRM_Core_OptionGroup::getLabel('file_type', $files[$entityFileDAO->file_id]['file_type_id']);
      }
    }
  }
  else {
    return civicrm_api3_create_error('Exact match not found');
  }

  return civicrm_api3_create_success($files, $params, 'file', 'get', $entityFileDAO);
}

/**
 * Deletes an existing entity file assignment.
 * Required parameters : 1.  id of an entity-file
 *                       2.  entity_id and entity_table of an entity-file
 *
 * @param   array $params   an associative array of name/value property values of civicrm_entity_file.
 *
 * @return  null if successfull, object of CRM_Core_Error otherwise
 * @access public
 */
function civicrm_api3_entity_file_delete($params) {

  civicrm_api3_verify_mandatory($params);
  require_once 'CRM/Core/DAO/EntityFile.php';

  //if ( ! isset($params['id']) && ( !isset($params['entity_id']) || !isset($params['entity_file']) ) ) {
  if (!isset($params['id']) && (!isset($params['entity_id']) || !isset($params['entity_table']))) {
    return civicrm_api3_create_error('Required parameters missing');
  }

  $entityFileDAO = new CRM_Core_DAO_EntityFile();

  $properties = array('id', 'entity_id', 'entity_table', 'file_id');
  foreach ($properties as $name) {
    if (array_key_exists($name, $params)) {
      $entityFileDAO->$name = $params[$name];
    }
  }

  return $entityFileDAO->delete() ? NULL : civicrm_api3_create_error('Error while deleting');
}

