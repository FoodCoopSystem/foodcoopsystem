/**
 * @file
 * Provides the administration JavaScript for the Mayo theme settings page
 */

(function ($) {
  Drupal.behaviors.mayo = {
    attach : function(context, settings) {

      var base = $("#edit-base-font-family option:selected").val();
      if (base == 2) $('#base-custom-font-family-wrapper').show();
      else $('#base-custom-font-family-wrapper').hide();

      var heading = $("#edit-heading-font-family option:selected").val();
      if (heading == 2) $('#heading-custom-font-family-wrapper').show();
      else $('#heading-custom-font-family-wrapper').hide();

      $("#edit-base-font-family").change(function() {
        var sel = $(this).val();
        if (sel == 2) $('#base-custom-font-family-wrapper').show();
        else $('#base-custom-font-family-wrapper').hide();
      });

      $("#edit-heading-font-family").change(function() {
        var sel = $(this).val();
        if (sel == 2) $('#heading-custom-font-family-wrapper').show();
        else $('#heading-custom-font-family-wrapper').hide();
      });
    }
  };
})(jQuery);
