<?php
/**
 * Smarty plugin
 * @package Smarty
 * @subpackage plugins
 */


/**
 * Smarty {eval} function plugin
 *
 * Type:     function<br>
 * Name:     eval<br>
 * Purpose:  evaluate a template variable as a template<br>
 * @link http://smarty.php.net/manual/en/language.function.eval.php {eval}
 *       (Smarty online manual)
 * @author Monte Ohrt <monte at ohrt dot com>
 * @param array
 * @param Smarty
 */
function smarty_function_eval($params, &$smarty)
{
    static $compiledCache = array( );

    if (!isset($params['var'])) {
        $smarty->trigger_error("eval: missing 'var' parameter");
        return;
    }

    if($params['var'] == '') {
        return;
    }

    $md5 = md5( $params['var'] );
    if ( isset( $compiledCache[$md5] ) ) {
        $_var_compiled = $compiledCache[$md5];
    } else {
        $smarty->_compile_source('evaluated template', $params['var'], $_var_compiled);
        $compiledCache[$md5] = $_var_compiled;
    }

    ob_start();
    $smarty->_eval('?>' . $_var_compiled);
    $_contents = ob_get_contents();
    ob_end_clean();

    if (!empty($params['assign'])) {
        $smarty->assign($params['assign'], $_contents);
    } else {
        return $_contents;
    }
}

/* vim: set expandtab: */

?>
