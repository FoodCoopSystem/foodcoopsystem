
Description
===========
Commerce Extra module allows site builders to enable extra features or
improvements which are missing from Drupal Commerce core. The module is
a collection of minimal submodules which some of developer might not consider
worth of a full project and therefore are added into this module.


Feature/improvement overview
============================
- Improve checkout by pre-populating customer profile information's
  addressfield. Needs an address field for user entity. See also Commerce
  Addressbook for similiar functionality.
- Add extra step where user may login, but doesn't require it.
- Improve quantity field by wrapping it with decrease/increase button links.


Installation & configuration
============================
The module itself obviously requires Commerce and submodules determine their
dependencies. See more information below.

Just enable the module and enable features from
admin/commerce/config/commerce_extra.


Commerce Extra Address Populate
===============================
Requires: Addressfield, Commerce Customer (comes with Commerce module)

Pre-populates customer profile information so user doesn't have to reenter
his/her information again. This is very similar with Commerce Addressbook
except this clones address information from user's account instead previously
made orders as Commerce Addressbook does.


Commerce Extra Login Page
=========================
Requires: Commerce Checkout (comes with Commerce module)

Creates extra step to checkout so that users may log in optionally. Many times
shop keeper want to promote user to log in for example to apply some discounts
or some other rules.

Related project: Commerce Checkout Login, by rszrama


Commerce Extra Quantity
=======================
Requires: Commerce Cart (comes with Commerce module)

Improves UX by creating button links for decreasing or increasing quantity
level of a product. Currently works in add-to-cart form and shopping-cart
view.
