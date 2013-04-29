<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 */
?>

<?php
  $page_wrapper_style = '';
  $page_width = theme_get_setting('page_width');
  if (empty($page_width)) $page_width = '90%';
  if (arg(0) == "admin") $page_width = '100%'; // admin page
  $page_wrapper_style = 'width: ' . $page_width . ';';
  $base_vmargin = theme_get_setting('base_vmargin');
  if (arg(0) == "admin") $base_vmargin = '0px'; // admin page
  if (empty($base_vmargin)) $base_vmargin = '0px';
  $page_wrapper_style .= ' margin-top: ' . $base_vmargin . '; margin-bottom: ' . $base_vmargin . ';';

  $page_style = '';
  $main_style = '';
  $layout_style = theme_get_setting('layout_style');
  $page_margin = theme_get_setting('page_margin');
  if (empty($page_margin)) $page_margin = '0px';
  if (arg(0) == "admin") $page_margin = '20px'; // admin page
  if ($layout_style == 1) {
    $page_style = 'padding: ' . $page_margin . ';';
  }
  else {
    $main_style = 'padding: 0px ' . $page_margin . ';';
  }

  $header_style = '';
  $header_height = theme_get_setting('header_height');
  if (!empty($header_height)) $header_style .= 'height: ' . $header_height . ';';
  $header_bg_file = theme_get_setting('header_bg_file');
  if ($header_bg_file) {
    $header_style .= 'filter:;background: url(' . $header_bg_file . ') repeat ';
    $header_style .= theme_get_setting('header_bg_alignment') . ';';
  }
  if ($layout_style == 2 || $header_bg_file) {
    // no header margin, so skip header borders to make it nicer
    $header_style .= 'border: none;';
  }
  else {
    $header_border_width = theme_get_setting('header_border_width');
    $header_style .= 'border-width: ' . $header_border_width . ';';
  }

  $header_watermark_style = '';
  $header_watermark = theme_get_setting('header_watermark');
  if($header_watermark) {
    $header_watermark_style = 'background-image: url(/' . drupal_get_path('theme', 'mayo') . '/images/pat-' . $header_watermark . '.png);';
  }

  $logo_style = '';
  $logo_left_margin = theme_get_setting('logo_left_margin');
  if (empty($logo_left_margin)) $logo_left_margin = '0px';
  $logo_top_margin = theme_get_setting('logo_top_margin');
  if (empty($logo_top_margin)) $logo_top_margin = '0px';
  $logo_style = 'padding-left: ' . $logo_left_margin . '; padding-top: ' . $logo_top_margin . ';';

  $sitename_style = '';
  $sitename_left_margin = theme_get_setting('sitename_left_margin');
  if (empty($sitename_left_margin)) $sitename_left_margin = '0px';
  $sitename_top_margin = theme_get_setting('sitename_top_margin');
  if (empty($sitename_top_margin)) $sitename_top_margin = '0px';
  $sitename_style = 'padding-left: ' . $sitename_left_margin . '; padding-top: ' . $sitename_top_margin . ';';

  $searchbox_style = '';
  $searchbox_right_margin = theme_get_setting('searchbox_right_margin');
  if (empty($searchbox_right_margin)) $searchbox_right_margin = '0px';
  $searchbox_top_margin = theme_get_setting('searchbox_top_margin');
  if (empty($searchbox_top_margin)) $searchbox_top_margin = '0px';
  $searchbox_style = 'padding-right: ' . $searchbox_right_margin . '; padding-top: ' . $searchbox_top_margin . ';';

  $fontsizer_top_margin = (intval($searchbox_top_margin) + 3) . 'px';
  $fontsizer_style = 'margin-top: ' . $fontsizer_top_margin . ';';

  $sb_layout_style = theme_get_setting('sidebar_layout_style');
  $sb_first_width = theme_get_setting('sidebar_first_width');
  if (empty($sb_first_width)) $sb_first_width = '25%';
  $sb_first_style = 'width: ' . $sb_first_width . ';';
  $sb_second_width = theme_get_setting('sidebar_second_width');
  if (empty($sb_second_width)) $sb_second_width = '25%';
  $sb_second_style = 'width: ' . $sb_second_width . ';';

  $content_width = 100;
  if ($page['sidebar_first']) {
    $content_width -= intval(preg_replace('/%/', '', $sb_first_width));
  }
  if ($page['sidebar_second']) {
    $content_width -= intval(preg_replace('/%/', '', $sb_second_width));
  }
  $content_style = 'width: ' . $content_width . '%;';

  $margins = mayo_get_margins($page['content'], $page['sidebar_first'], $page['sidebar_second']);
  $content_section_style = $margins['content'];
  $sb_first_section_style = $margins['sb_first'];
  $sb_second_section_style = $margins['sb_second'];

  if (theme_get_setting('header_fontsizer')) {
    drupal_add_js(drupal_get_path('theme', 'mayo') . '/js/mayo-fontsize.js');
  }
  if ($page['top_column_first'] ||
      $page['top_column_second'] ||
      $page['top_column_third'] ||
      $page['top_column_fourth'] ||
      $page['bottom_column_first'] ||
      $page['bottom_column_second'] ||
      $page['bottom_column_third'] ||
      $page['bottom_column_fourth']) {
    drupal_add_js(drupal_get_path('theme', 'mayo') . '/js/mayo-columns.js');
  }
?>

<div id="page-wrapper" style="<?php echo $page_wrapper_style; ?>">
  <div id="page" style="<?php echo $page_style; ?>">

    <div id="header" style="<?php echo $header_style; ?>">
    <div id="header-watermark" style="<?php echo $header_watermark_style; ?>">
    <div class="section clearfix">

      <?php if ($logo): ?>
        <div id="logo" style="<?php echo $logo_style; ?>">
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
        </div> <!-- /#logo -->
      <?php endif; ?>

      <?php if ($site_name || $site_slogan): ?>
        <div id="name-and-slogan" style="<?php echo $sitename_style; ?>">
          <?php if ($site_name): ?>
            <?php if ($title): ?>
              <div id="site-name"><strong>
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </strong></div>
            <?php else: /* Use h1 when the content title is empty */ ?>
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </h1>
            <?php endif; ?>
          <?php endif; ?>

          <?php if ($site_slogan): ?>
            <div id="site-slogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
        </div> <!-- /#name-and-slogan -->
      <?php endif; ?>

      <?php if ((theme_get_setting('header_searchbox')) && function_exists('search_box')) { ?>
        <div id="header-searchbox" style="<?php echo $searchbox_style; ?>">
      <?php  $output_form = drupal_get_form('search_block_form'); print render($output_form); ?>
        </div>
      <?php } ?>

      <?php if (theme_get_setting('header_fontsizer')) { ?>
        <div id="header-fontsizer" style="<?php echo $fontsizer_style; ?>">
        <a href="#" class="decreaseFont" title="Decrease text size"></a>
        <a href="#" class="resetFont"    title="Restore default text size"></a>
        <a href="#" class="increaseFont" title="Increase text size"></a>
        </div>
      <?php } ?>

      <div class="clearfix cfie"></div>

      <?php print render($page['header']); ?>

    </div> <!-- /.section -->
    </div> <!-- /#header-watermark -->
    </div> <!-- /#header -->

    <?php if ($main_menu || $secondary_menu) { ?>
      <div id="navigation"><div class="section">
        <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'inline', 'clearfix')))); ?>
        <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'inline', 'clearfix')))); ?>
      </div></div> <!-- /.section, /#navigation -->
    <?php } ?>

    <div class="clearfix cfie"></div>

    <!-- for nice_menus, superfish -->
    <?php if ($page['menubar']) { ?>
    <div id="menubar" class="menubar clearfix">
      <?php print render($page['menubar']); ?>
    </div>
    <?php } ?>
    <?php if ($page['submenubar']) { ?>
    <div id="submenubar" class="menubar clearfix">
      <?php print render($page['submenubar']); ?>
    </div>
    <?php } ?>

    <!-- space between menus and contents -->
    <div class="spacer clearfix cfie"></div>


    <div id="main-wrapper">
    <div id="main" class="clearfix" style="<?php echo $main_style; ?>">

      <?php print $messages; ?>

      <?php if ($page['banner_top']) { ?>
      <div id="banner-top" class="banner clearfix"><?php print render($page['banner_top']); ?></div>
      <div class="spacer clearfix cfie"></div>
      <?php } ?>

      <?php if ($page['top_column_first'] | $page['top_column_second'] |
              $page['top_column_third'] | $page['top_column_fourth']) { ?>
      <div id="top-wrapper">
        <div id="top-columns" class="clearfix">
        <?php print mayo_build_columns( array(
            $page['top_column_first'],
            $page['top_column_second'],
            $page['top_column_third'],
            $page['top_column_fourth'],
          ));
        ?>
        </div> <!--/#top-columns -->
      </div> <!-- /#top-wrapper -->
      <?php } ?>

      <div class="clearfix cfie"></div>


      <!-- sidebars (left) -->
      <?php if (($page['sidebar_first']) && ($sb_layout_style != 3)){ ?>
        <div id="sidebar-first" class="column sidebar" style="<?php echo $sb_first_style; ?>"><div class="section" style="<?php echo $sb_first_section_style; ?>">
          <?php print render($page['sidebar_first']); ?>
        </div></div> <!-- /.section, /#sidebar-first -->
      <?php } ?>
      <?php if (($page['sidebar_second']) && ($sb_layout_style == 2)) { ?>
        <div id="sidebar-second" class="column sidebar" style="<?php echo $sb_second_style; ?>"><div class="section" style="<?php echo $sb_second_section_style; ?>">
          <?php print render($page['sidebar_second']); ?>
        </div></div> <!-- /.section, /#sidebar-second -->
      <?php } ?>


      <!-- main content -->
      <div id="content" class="column" style="<?php echo $content_style; ?>"><div class="section" style="<?php echo $content_section_style; ?>">

        <?php if ($page['highlighted']) { ?>
          <div id="highlighted"><?php print render($page['highlighted']); ?></div>
        <?php } ?>

        <?php if ($breadcrumb && theme_get_setting('display_breadcrumb')) { ?>
          <div id="breadcrumb"><?php print $breadcrumb; ?></div>
        <?php } ?>

        <a id="main-content"></a>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
        <?php print render($page['content']); ?>
        <?php print $feed_icons; ?>

      </div></div> <!-- /.section, /#content -->


      <!-- sidebars (right) -->
      <?php if (($page['sidebar_first']) && ($sb_layout_style == 3)) { ?>
        <div id="sidebar-first-r" class="column sidebar" style="<?php echo $sb_first_style; ?>"><div class="section" style="<?php echo $sb_first_section_style; ?>">
          <?php print render($page['sidebar_first']); ?>
        </div></div> <!-- /.section, /#sidebar-first -->
      <?php } ?>
      <?php if (($page['sidebar_second']) && ($sb_layout_style != 2)) { ?>
        <div id="sidebar-second-r" class="column sidebar" style="<?php echo $sb_second_style; ?>"><div class="section" style="<?php echo $sb_second_section_style; ?>">
          <?php print render($page['sidebar_second']); ?>
        </div></div> <!-- /.section, /#sidebar-second -->
      <?php } ?>


      <div class="clearfix cfie"></div>

      <?php if ($page['bottom_column_first'] | $page['bottom_column_second'] |
              $page['bottom_column_third'] | $page['bottom_column_fourth']) { ?>
      <div id="bottom-wrapper">
        <div id="bottom-columns" class="clearfix">
        <?php print mayo_build_columns( array(
            $page['bottom_column_first'],
            $page['bottom_column_second'],
            $page['bottom_column_third'],
            $page['bottom_column_fourth'],
          ));
        ?>
        </div> <!--/#bottom-columns -->
      </div> <!-- /#bottom-wrapper -->
      <?php } ?>

      <div class="clearfix cfie"></div>


      <?php if ($page['banner_bottom']) { ?>
      <div id="spacer" class="clearfix cfie"></div>
      <div id="banner-bottom" class="banner clearfix"><?php print render($page['banner_bottom']); ?></div>
      <?php } ?>

    </div> <!-- /#main -->
    </div> <!-- /#main-wrapper -->

    <!-- space between contents and footer -->
    <div id="spacer" class="clearfix cfie"></div>

    <div id="footer-wrapper">
      <?php if ($page['footer_column_first'] | $page['footer_column_second'] |
              $page['footer_column_third'] | $page['footer_column_fourth']) { ?>
      <div id="footer-columns" class="clearfix">
      <?php print mayo_build_columns( array(
          $page['footer_column_first'],
          $page['footer_column_second'],
          $page['footer_column_third'],
          $page['footer_column_fourth'],
        ));
      ?>
      </div> <!--/#footer-columns -->
      <?php } ?>

      <?php if ($page['footer']) { ?>
      <div id="footer"><div class="section">
        <?php print render($page['footer']); ?>
      </div></div> <!-- /.section, /#footer -->
      <?php } ?>

    </div> <!-- /#footer-wrapper -->


  </div> <!-- /#page -->
</div> <!-- /#page-wrapper -->
