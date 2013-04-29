<section id="wrapper" role="container" class="clearfix">
  <header id="header" role="banner" class="clearfix">
    <div id="header-container">
      <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php endif; ?>

      <?php print render($page['header']); ?>

      <?php if ($main_menu): ?>
        <div id="main-menu" class="navigation">
          <?php
          print theme('links__system_main_menu', array(
                'links' => $main_menu,
                'attributes' => array(
                  'id' => 'main-menu-links',
                  'class' => array('links', 'clearfix'),
                ),
                'heading' => array(
                  'text' => t('Main menu'),
                  'level' => 'h2',
                  'class' => array('element-invisible'),
                ),
              ));
          ?>
        </div> <!-- /#main-menu -->
      <?php endif; ?>
    </div>
  </header> <!-- /#header -->
  <section id="main-wrapper" role="main" class="clearfix">
    <section id="main" role="main" class="clearfix">
      <?php if ($breadcrumb): print $breadcrumb;
      endif; ?>
      <?php print $messages; ?>
      <a id="main-content"></a>
      <?php if ($tabs): ?>
        <div class="tabs">
          <?php print render($tabs); ?>
        </div>
      <?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      <div id="content"role="main" class="clearfix"><?php print render($page['content']); ?></div>
    </section> <!-- /#main -->

    <?php if ($page['sidebar_first']): ?>
      <aside id="sidebar-first" role="complementary" class="sidebar clearfix">
        <?php print render($page['sidebar_first']); ?>
      </aside>  <!-- /#sidebar-first -->
    <?php endif; ?>

  </section>

  <footer id="footer" role="contentinfo" class="clearfix">
    <?php print render($page['footer']) ?>
    <?php print $feed_icons ?>
  </footer> <!-- /#footer -->


</section>