(function ($) {

  // Increase/decrease quantity
  Drupal.commerce_extra_quantity_quantity = function(selector, way) {

    // Find out current quantity and figure out new one
    var quantity = parseInt($(selector).val());
    if (way == 1) {
      // Increase
      var new_quantity = quantity+1;
    }
    else if (way == -1) {
      // Decrease
      var new_quantity = quantity-1;
    }
    else {
      var new_quantity = quantity;
    }

    // Set new quantity
    if (new_quantity >= 0) {
      $(selector).val(new_quantity);
    }

    // Set disabled class depending on new quantity
    if (new_quantity <= 0) {
      $(selector).prev('span').addClass('commerce-quantity-plusminus-link-disabled');
    }
    else {
      $(selector).prev('span').removeClass('commerce-quantity-plusminus-link-disabled');
    }

  }

}(jQuery));
