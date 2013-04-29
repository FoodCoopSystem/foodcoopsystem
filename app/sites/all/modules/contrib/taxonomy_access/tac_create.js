/**
 * Disable disallowed terms in taxonomy fields, and re-enable on submit.
 *
 * We do this in jQuery because FAPI does not yet support it:
 * @see
 *   http://drupal.org/node/284917
 * @see
 *   http://drupal.org/node/342316
 *
 * @todo 
 *   Use clearer coding standards.
 * @see
 *   http://jsdemystified.drupalgardens.com/
 */
Drupal.behaviors.tac_create = {};
Drupal.behaviors.tac_create.attach = function(context, settings) {
  var $ = jQuery;
  var $fields = $(Drupal.settings.taxonomy_access);

  // For each controlled field, disable disallowed terms.
  $.each($fields, function(i, field) {
    var fieldname = "." + field.field;

    // Disable disallowed term and its label, if any.
    $.each(field.disallowed_tids, function(j, tid) {

      // Children of the widget element with the specified tid as a value.
      // Can be either <option> or <input>.
      // .tac_fieldname [value='1']
      selector = fieldname + " [value='" + tid + "']";
      $(selector).attr('disabled','disabled');

      // Label sibling adjacent the child element.
      // .tac_fieldname [value='1'] + label
      label_selector = fieldname + " [value='" + tid + "']" + " + label";
      $(label_selector).attr('class','option disabled');
    });
  });

  // Re-enable and re-select disallowed defaults on submit.
  $("form").submit(function() {

    // For each controlled field, re-enable disallowed terms.
    $.each($fields, function(i, field) {
      var fieldname = "." + field.field;

      // Enable and select disallowed defaults.
      $.each(field.disallowed_defaults, function(j, tid) {

        // Children of the widget element with the specified tid as a value.
        // Can be either <option> or <input>.
        // .tac_fieldname [value='1']
        selector = fieldname + " [value='" + tid + "']";
        $(selector).attr('disabled','');
        $(selector).attr('selected','selected');
      });
    });
  });

}
