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
class CRM_Utils_PDF_Utils {

  static
  function html2pdf(&$text, $fileName = 'civicrm.pdf', $output = FALSE, $pdfFormat = NULL) {
    if (is_array($text)) {
      $pages = &$text;
    }
    else {
      $pages = array($text);
    }
    // Get PDF Page Format
    require_once "CRM/Core/BAO/PdfFormat.php";
    $format = CRM_Core_BAO_PdfFormat::getDefaultValues();
    if (is_array($pdfFormat)) {
      // PDF Page Format parameters passed in
      $format = array_merge($format, $pdfFormat);
    }
    else {
      // PDF Page Format ID passed in
      $format = CRM_Core_BAO_PdfFormat::getById($pdfFormat);
    }
    require_once 'CRM/Core/BAO/PaperSize.php';
    $paperSize    = CRM_Core_BAO_PaperSize::getByName($format['paper_size']);
    $paper_width  = self::convertMetric($paperSize['width'], $paperSize['metric'], 'pt');
    $paper_height = self::convertMetric($paperSize['height'], $paperSize['metric'], 'pt');
    // dompdf requires dimensions in points
    $paper_size  = array(0, 0, $paper_width, $paper_height);
    $orientation = CRM_Core_BAO_PdfFormat::getValue('orientation', $format);
    $metric      = CRM_Core_BAO_PdfFormat::getValue('metric', $format);
    $t           = CRM_Core_BAO_PdfFormat::getValue('margin_top', $format);
    $r           = CRM_Core_BAO_PdfFormat::getValue('margin_right', $format);
    $b           = CRM_Core_BAO_PdfFormat::getValue('margin_bottom', $format);
    $l           = CRM_Core_BAO_PdfFormat::getValue('margin_left', $format);

    $config = CRM_Core_Config::singleton();
    $html = "
<html>
  <head>
    <style>@page { margin: {$t}{$metric} {$r}{$metric} {$b}{$metric} {$l}{$metric}; }</style>
    <style type=\"text/css\">@import url({$config->userFrameworkResourceURL}css/print.css);</style>
  </head>
  <body>
    <div id=\"crm-container\">\n";

    // Strip <html>, <header>, and <body> tags from each page
    $htmlElementstoStrip = array(
      '@<head[^>]*?>.*?</head>@siu',
      '@<body>@siu',
      '@</body>@siu',
      '@<html[^>]*?>@siu',
      '@</html>@siu',
      '@<!DOCTYPE[^>]*?>@siu',
    );
    $htmlElementsInstead = array('', '', '', '', '', '');
    foreach ($pages as & $page) {
      $page = preg_replace($htmlElementstoStrip,
        $htmlElementsInstead,
        $page
      );
    }
    // Glue the pages together
    $html .= implode("\n<div style=\"page-break-after: always\"></div>\n", $pages);
    $html .= "
    </div>
  </body>
</html>";

    require_once 'packages/dompdf/dompdf_config.inc.php';
    spl_autoload_register('DOMPDF_autoload');
    $dompdf = new DOMPDF();
    $dompdf->set_paper($paper_size, $orientation);
    $dompdf->load_html(utf8_decode($html));
    $dompdf->render();

    if ($output) {
      return $dompdf->output();
    }
    else {
      $dompdf->stream($fileName);
    }
  }

  /*
     * function to convert value from one metric to another
     */

  static
  function convertMetric($value, $from, $to, $precision = NULL) {
    switch ($from . $to) {
      case 'incm':
        $value *= 2.54;
        break;

      case 'inmm':
        $value *= 25.4;
        break;

      case 'inpt':
        $value *= 72;
        break;

      case 'cmin':
        $value /= 2.54;
        break;

      case 'cmmm':
        $value *= 10;
        break;

      case 'cmpt':
        $value *= 72 / 2.54;
        break;

      case 'mmin':
        $value /= 25.4;
        break;

      case 'mmcm':
        $value /= 10;
        break;

      case 'mmpt':
        $value *= 72 / 25.4;
        break;

      case 'ptin':
        $value /= 72;
        break;

      case 'ptcm':
        $value *= 2.54 / 72;
        break;

      case 'ptmm':
        $value *= 25.4 / 72;
        break;
    }
    if (!is_null($precision)) {
      $value = round($value, $precision);
    }
    return $value;
  }

  static
  function &pdflib($fileName,
    $searchPath,
    &$values,
    $numPages = 1,
    $echo     = TRUE,
    $output   = 'College_Match_App',
    $creator  = 'CiviCRM',
    $author   = 'http://www.civicrm.org/',
    $title    = '2006 College Match Scholarship Application'
  ) {
    try {
      $pdf = new PDFlib();
      $pdf->set_parameter("compatibility", "1.6");
      $pdf->set_parameter("licensefile", "/home/paras/bin/license/pdflib.txt");

      if ($pdf->begin_document('', '') == 0) {
        CRM_Core_Error::statusBounce("PDFlib Error: " . $pdf->get_errmsg());
      }

      $config = CRM_Core_Config::singleton();
      $pdf->set_parameter('resourcefile', $config->templateDir . '/Quest/pdf/pdflib.upr');
      $pdf->set_parameter('textformat', 'utf8');

      /* Set the search path for fonts and PDF files */

      $pdf->set_parameter('SearchPath', $searchPath);

      /* This line is required to avoid problems on Japanese systems */

      $pdf->set_parameter('hypertextencoding', 'winansi');

      $pdf->set_info('Creator', $creator);
      $pdf->set_info('Author', $author);
      $pdf->set_info('Title', $title);

      $blockContainer = $pdf->open_pdi($fileName, '', 0);
      if ($blockContainer == 0) {
        CRM_Core_Error::statusBounce('PDFlib Error: ' . $pdf->get_errmsg());
      }

      for ($i = 1; $i <= $numPages; $i++) {
        $page = $pdf->open_pdi_page($blockContainer, $i, '');
        if ($page == 0) {
          CRM_Core_Error::statusBounce('PDFlib Error: ' . $pdf->get_errmsg());
        }

        /* dummy page size */
        $pdf->begin_page_ext(20, 20, '');

        /* This will adjust the page size to the block container's size. */

        $pdf->fit_pdi_page($page, 0, 0, 'adjustpage');


        $status = array();
        /* Fill all text blocks with dynamic data */

        foreach ($values as $key => $value) {
          if (is_array($value)) {
            continue;
          }

          // pdflib does like the forward slash character, hence convert
          $value = str_replace('/', '_', $value);

          $res = $pdf->fill_textblock($page,
            $key,
            $value,
            'embedding encoding=winansi'
          );

          /**
           if ( $res == 0 ) {
           CRM_Core_Error::debug( "$key, $value: $res", $pdf->get_errmsg( ) );
           } else {
           CRM_Core_Error::debug( "SUCCESS: $key, $value", null );
           }
           **/
        }

        $pdf->end_page_ext('');
        $pdf->close_pdi_page($page);
      }

      $pdf->end_document('');
      $pdf->close_pdi($blockContainer);

      $buf = $pdf->get_buffer();
      $len = strlen($buf);

      if ($echo) {
        header('Content-type: application/pdf');
        header("Content-Length: $len");
        header("Content-Disposition: inline; filename={$output}.pdf");
        echo $buf;
        CRM_Utils_System::civiExit();
      }
      else {
        return $buf;
      }
    }
    catch(PDFlibException$excp) {
      CRM_Core_Error::statusBounce('PDFlib Error: Exception' .
        "[" . $excp->get_errnum() . "] " . $excp->get_apiname() . ": " .
        $excp->get_errmsg()
      );
    }
    catch(Exception$excp) {
      CRM_Core_Error::statusBounce("PDFlib Error: " . $excp->get_errmsg());
    }
  }
}

