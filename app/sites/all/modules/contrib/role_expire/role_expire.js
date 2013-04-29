
/**
 * @file
 * Role Expire js
 *
 * Set of jQuery related routines.
 */

// See http://drupal.org/node/756722 for conversion to D7 and Drupal.behaviors.

(function ($) {
  
  Drupal.behaviors.role_expire = {
    attach: function (context, settings) {
      $('input.role-expire-role-expiry', context).parent().hide();

      $('#edit-roles input.form-checkbox', context).each(function() {
        var textfieldId = this.id.replace("roles", "role-expire");

        // Move all expiry date fields under corresponding checkboxes
        $(this).parent().after($('#'+textfieldId).parent());

        // Show all expiry date fields that have checkboxes checked
        if ($(this).attr("checked")) {
          $('#'+textfieldId).parent().show();
        }
      });

      $('#edit-roles input.form-checkbox', context).click(function() {
        var textfieldId = this.id.replace("roles", "role-expire");

        // Toggle expiry date fields
        if ($(this).attr("checked")) {
          $('#'+textfieldId).parent().show(200);
        }
        else {
          $('#'+textfieldId).parent().hide(200);
        }
      });
    }
  }

})(jQuery);