
(function ($) {
  Drupal.color = {
    logoChanged: false,
    bgChanged: false,
    callback: function(context, settings, form, farb, height, width) {

      // Move the color wheel downwards
      $('#placeholder', form).css('margin-top', '1000px'); // adjusted based on Seven theme

      // Apply layout style
      if (Drupal.settings.color.layout_style == 2) {
        // No page margin to header and footer
        $('#preview #preview-page', form).css('padding', '0px');
        $('#preview #preview-main', form).css('padding', '0px 10px');
      }
      // Apply sidebar layout style
      if (Drupal.settings.color.sb_layout_style == 3) { // right sidebar
        $('#preview .sidebar', form).css('float', 'right');
        if (Drupal.settings.color.layout_style == 2) {
          $('#preview .sidebar', form).css('margin-right', '20px');
        }
        else {
          $('#preview .sidebar', form).css('margin-right', '0px');
        }
        $('#preview #preview-content', form).css('margin-left', '0px');
        $('#preview #preview-content', form).css('margin-right', '10px');
      }
      else {
        $('#preview .sidebar', form).css('float', 'left');
        $('#preview #preview-content', form).css('margin-left', '10px');
        $('#preview #preview-content', form).css('margin-right', '0px');
      }

      // Apply base vertical margin
      $('#preview #preview-page-wrapper', form).css('padding-top', Drupal.settings.color.base_vmargin);
      $('#preview #preview-page-wrapper', form).css('padding-bottom', Drupal.settings.color.base_vmargin);

      // Change the logo to be the real one.
      if (!this.logoChanged) {
        if (Drupal.settings.color.logo) {
          $('#preview #preview-logo img').attr('src', Drupal.settings.color.logo);
        }
        else {
          $('#preview #preview-logo img').remove();
        }
        this.logoChanged = true;
      }

      // Base background.
      $('#preview-page-wrapper', form).css('background-color', $('#palette input[name="palette[wall]"]', form).val());

      // Page background.
      $('#preview-page', form).css('background-color', $('#palette input[name="palette[bg]"]', form).val());


      // Generic text and link
      $('#preview', form).css('color', $('#palette input[name="palette[text]"]', form).val());
      $('#preview table tr td', form).css('color', $('#palette input[name="palette[text]"]', form).val());
      $('#preview table tr th', form).css('color', $('#palette input[name="palette[text]"]', form).val());
      $('#preview a', form).css('color', $('#palette input[name="palette[link]"]', form).val());


      // Page title background.
      $('#preview-page-title', form).css('background-color', $('#palette input[name="palette[pagetitle]"]', form).val());

      // Page title text
      $('#preview-page-title', form).css('color', $('#palette input[name="palette[pagetitletext]"]', form).val());


      // Menu divider
      if (Drupal.settings.color.menubar_style == 1) {
        $('#preview #preview-navigation', form).css('border-bottom-color', $('#palette input[name="palette[menudivider]"]', form).val());
        $('#preview #preview-navigation ul li', form).css('border-right-color', $('#palette input[name="palette[menudivider]"]', form).val());
        $('#preview .highlight', form).css('background-color', $('#palette input[name="palette[highlight]"]', form).val());
      }
      else if (Drupal.settings.color.menubar_style == 2) {
        $('#preview #preview-navigation a', form).css('color', '#dddddd');
        $('#preview .highlight', form).css('background-color', '#444444');
      }

      // Node background.
      $('#preview .node', form).css('background-color', $('#palette input[name="palette[node]"]', form).val());

      // Node border
      $('#preview .node', form).css('border-color', $('#palette input[name="palette[nodeborders]"]', form).val());

      // Node divider
      $('#preview .node h2', form).css('border-bottom-color', $('#palette input[name="palette[nodedivider]"]', form).val());

      // Sticky node background.
      $('#preview .node-sticky', form).css('background-color', $('#palette input[name="palette[stickynode]"]', form).val());


      // Table background
      $('#preview table tr th', form).css('background-color', $('#palette input[name="palette[tableheader]"]', form).val());
      $('#preview table tr.even td', form).css('background-color', $('#palette input[name="palette[even]"]', form).val());
      $('#preview table tr.odd td', form).css('background-color', $('#palette input[name="palette[node]"]', form).val());
      $('#preview table tr th', form).css('border-color', $('#palette input[name="palette[node]"]', form).val());

      // Sidebar background.
      $('#preview .sidebar .block', form).css('background-color', $('#palette input[name="palette[sidebar]"]', form).val());

      // Sidebar border
      $('#preview .sidebar .block', form).css('border-color', $('#palette input[name="palette[sidebarborders]"]', form).val());

      // Sidebar divider
      $('#preview .sidebar h2', form).css('border-bottom-color', $('#palette input[name="palette[sidebardivider]"]', form).val());

      // Sidebar text and link
      $('#preview .sidebar .block', form).css('color', $('#palette input[name="palette[sidebartext]"]', form).val());
      $('#preview .sidebar a', form).css('color', $('#palette input[name="palette[sidebarlink]"]', form).val());


      // Footer background.
      $('#preview #preview-footer-wrapper', form).css('background-color', $('#palette input[name="palette[footer]"]', form).val());

      // Footer text and link
      $('#preview #preview-footer-wrapper', form).css('color', $('#palette input[name="palette[footertext]"]', form).val());
      $('#preview #preview-footer-wrapper a', form).css('color', $('#palette input[name="palette[footerlink]"]', form).val());

      if (Drupal.settings.color.header_bg_file) {
        if (!this.bgChanged) {
          // Change the header_bg_file to be the real one.
          this.bgChanged = true;
          // Header background image
          $('#preview #preview-header', form).attr('style', 'border: none; background-image: url(' + Drupal.settings.color.header_bg_file + '); background-position: ' + Drupal.settings.color.header_bg_alignment + ';');
        }
      }
      else {
        // CSS3 Gradients.
        var gradient_start = $('#palette input[name="palette[left]"]', form).val();
        var gradient_end = $('#palette input[name="palette[right]"]', form).val();

        // Header background
        $('#preview #preview-header', form).attr('style', "background-color: " + gradient_start + "; background-image: -webkit-gradient(linear, left top, right top, from(" + gradient_start + "), to(" + gradient_end + ")); background-image: -moz-linear-gradient(0deg, " + gradient_start + ", " + gradient_end + "); filter:progid:DXImageTransform.Microsoft.Gradient(StartColorStr=" + gradient_start + ", EndColorStr=" + gradient_end + ", GradientType=1); -ms-filter:\"progid:DXImageTransform.Microsoft.gradient(startColorstr=" + gradient_start + ", endColorstr=" + gradient_end + ", GradientType=1)\";");

        if (Drupal.settings.color.layout_style == 2) {
          $('#preview #preview-header', form).css('border', 'none');
        }
        else {
          // Header border
          $('#preview #preview-header', form).css('border-color', $('#palette input[name="palette[headerborders]"]', form).val());
          $('#preview #preview-header', form).css('border-width', Drupal.settings.color.header_border_width);
        }
      }
      if (Drupal.settings.color.header_watermark > 0) {
        var url = '/sites/all/themes/mayo/images/pat-' + Drupal.settings.color.header_watermark + '.png';
        $('#preview #preview-header-watermark', form).attr('style', 'background-image: url(' + url + ');');
      }

      // Title and slogan
      $('#preview #preview-name-and-slogan', form).css('color', $('#palette input[name="palette[titleslogan]"]', form).val());
      $('#preview #preview-name-and-slogan a', form).css('color', $('#palette input[name="palette[titleslogan]"]', form).val());
    }
  };
})(jQuery);
