/**
 * @file
 * Adds javascript functions for font resizing
 */
jQuery(document).ready(function() {
  var originalFontSize = jQuery('body').css('font-size');

  // Reset font size
  jQuery(".resetFont").click(function() {
    mayoColumnsResetHeight();
    jQuery('body').css('font-size', originalFontSize);
    mayoColumnsAdjustHeight();
    return false;
  });

  // Increase font size
  jQuery(".increaseFont").click(function() {
    var currentFontSize = jQuery('body').css('font-size');
    var currentFontSizeNum = parseFloat(currentFontSize, 10);
    var newFontSizeNum = currentFontSizeNum + 1;
    if (20 >= newFontSizeNum) { /* max 20px */
      var newFontSize = newFontSizeNum + 'px';
      mayoColumnsResetHeight();
      jQuery('body').css('font-size', newFontSize);
      mayoColumnsAdjustHeight();
    }
    return false;
  });

  // Decrease font size
  jQuery(".decreaseFont").click(function() {
    var currentFontSize = jQuery('body').css('font-size');
    var currentFontSizeNum = parseFloat(currentFontSize, 10);
    var newFontSizeNum = currentFontSizeNum - 1;
    if (10 <= newFontSizeNum) { /* min 10px */
      var newFontSize = newFontSizeNum + 'px';
      mayoColumnsResetHeight();
      jQuery('body').css('font-size', newFontSize);
      mayoColumnsAdjustHeight();
    }
    return false;
  });
});

function mayoColumnsResetHeight() {
  // reset height of column blocks to 'auto' before chaning font size
  // so that the column blocks can change the size based on the new
  // font size
  if (mayoFunctionExists('mayoEqualHeight')) {
    jQuery("#top-columns .column-block").height('auto');
    jQuery("#bottom-columns .column-block").height('auto');
  }
}
function mayoColumnsAdjustHeight() {
  // equalize the height of the column blocks to the tallest height
  if (mayoFunctionExists('mayoEqualHeight')) {
    mayoEqualHeight(jQuery("#top-columns .column-block"));
    mayoEqualHeight(jQuery("#bottom-columns .column-block"));
  }
}
function mayoFunctionExists(function_name) {
  if (typeof function_name == 'string') {
    return (typeof this.window[function_name] == 'function');
  }
  else {
    return (function_name instanceof Function);
  }
}
