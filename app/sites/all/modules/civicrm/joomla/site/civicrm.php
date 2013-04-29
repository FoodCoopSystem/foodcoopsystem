<?php
  //////////////////////////////////////////////////
  // CiviCRM Front-end Profile - Logic Layer
  //////////////////////////////////////////////////

defined('_JEXEC') or die('No direct access allowed'); 

// check for php version and ensure its greater than 5.
// do a fatal exit if
if ( (int ) substr( PHP_VERSION, 0, 1 ) < 5 ) {
    echo "CiviCRM requires PHP Version 5.2 or greater. You are running PHP Version " . PHP_VERSION . "<p>";
    exit( );
}

include_once 'civicrm.settings.php';

require_once 'PEAR.php';

require_once 'CRM/Core/Config.php';
require_once 'CRM/Core/DAO.php';
require_once 'CRM/Core/Error.php';
require_once 'CRM/Core/Invoke.php';

civicrm_invoke( );

function civicrm_init( ) {
    $config = CRM_Core_Config::singleton();

    // this is the front end, so let others know
    $config->userFrameworkFrontend = 1;
}


function civicrm_invoke( ) {
    civicrm_init( );

    // check and ensure that we have a valid session
    if ( ! empty( $_POST ) ) {
        // the session should not be empty
        // however for standalone forms, it will not have any CiviCRM variables in the
        // session either, so dont check for it
        if ( count( $_SESSION ) <= 1 ) {
            require_once 'CRM/Utils/System.php';

            $config = CRM_Core_Config::singleton( );
            CRM_Utils_System::redirect( $config->userFrameworkBaseURL );
        }
    }

    // add all the values from the itemId param
    // overrride the GET values if conflict
    if ( CRM_Utils_Array::value( 'Itemid', $_GET ) ) {
        $component  = JComponentHelper::getComponent('com_civicrm');
        $menu       = JSite::getMenu( );
        $params     =  $menu->getParams( $_GET['Itemid'] );
        $args = array( 'task', 'id', 'gid', 'pageId', 'action', 'csid' ); 
		$view =  CRM_Utils_Array::value( 'view', $_GET );
		if ( $view ) {
		    $args[] = 'reset';
 		}
        foreach ( $args as $a ) {
			$val = $params->get( $a, null ); 
            if ( $val !== null && $view ) { 
                $_GET[$a] = $val; 
            } 
        }
    }
	
    $task = CRM_Utils_Array::value( 'task', $_GET, '' );
    $args = explode( '/', trim( $task ) );

    require_once 'CRM/Utils/System/Joomla.php';
    CRM_Utils_System_Joomla::addHTMLHead( null, true );

    $user = JFactory::getUser( );
    require_once 'CRM/Core/BAO/UFMatch.php';
    CRM_Core_BAO_UFMatch::synchronize( $user, false, 'Joomla', 'Individual', true );

    CRM_Core_Invoke::invoke( $args );
}
