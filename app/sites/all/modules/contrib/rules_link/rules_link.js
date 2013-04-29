/**
 * @file
 * Converts a normal link into javascrup rules link.
 */

(function($) {
  Drupal.behaviors.rules_link = {
    attach: function(context) {
      $('a.rules-link-js').once( function() {
        var message = $('<span class="rules-link-message">Link executed</a>').hide();
        var wrapper = $(this).parents('.rules-link-wrapper').append(message);
      });
      $('a.rules-link-js').click(rules_link);
    }
  };

  function rules_link(context) {
    var element = this;
    var wrapper = $(element).parents('.rules-link-wrapper');
    var message = wrapper.children('.rules-link-message');
    if (wrapper.is('.rules-link-waiting')) {
      return false;
    }
    wrapper.addClass('rules-link-waiting');

    $.ajax({
      type: 'GET',
      url: element.href,
      dataType: 'json',
      success: function (data) {
        wrapper.removeClass('rules-link-waiting');
        if (data.message.status !== undefined) {
          $(element).replaceWith("<span>" + data.message.status +"</span>");
        }
        else {
          $(element).remove();
        }
      },
      error: function (xmlhttp) {
        alert(Drupal.t('An HTTP error '+ xmlhttp.status +' occurred.\n'+ element.href));
        wrapper.removeClass('rules-link-waiting');
        message.text("Error while executing the rules link.");
        message.show();
        setTimeout(function(){
          message.fadeOut();
        }, 3000);
      }
    });
    return false;
  };

})(jQuery);