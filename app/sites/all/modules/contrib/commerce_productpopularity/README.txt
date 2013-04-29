Commerce Product Popularity
***************************

Description
***********

Commerce Product Popularity integrates with Radioactivity[1] to provide a
product popularity field. If you're not familiar with Radioactivity, then it's
probably worth familiarizing yourself[1]. This module allows you to configure
individual field instances to be updated on checkout. Every time a customer
pays for an order, any appropriately configured Radioactivity fields in any
products or product displays will have their energy increased (proportional to
the quantity of products sold).

It can be used for creating Views of most popular products and marking
individual products (or product displays) as 'hot sellers'.

Installation & Use
******************

The following are required:
* Drupal Commerce (commerce)[2]
* Entity API (entity)[3]
* Radioactivity (radioactivity)[1]
* Rules (rules)[4]

1.  Ensure you've downloaded the dependencies.
2.  Enable Commerce Product Popularity as normal[5].
3.  Create a Radioactivity profile (see example[6]).
4.  Add a Radioactivity field to a product or a product display, and on the
    field instance settings page, check the checkbox marked 'Update with
    Commerce Product Popularity'.
5.  If you only want purchases to affect the item's popularity (this is the
    anticipated use) then ensure the field's formatter's not configured to add
    energy on view.

As the rule's enabled by default, the fields should now increase with purchases.

Credits
*******

author: AndyF
http://drupal.org/user/220112

[1] http://drupal.org/project/radioactivity
[2] http://drupal.org/project/commerce
[3] http://drupal.org/project/entity
[4] http://drupal.org/project/rules
[5] http://drupal.org/documentation/install/modules-themes/modules-7
[6] http://mearra.com/blogs/teemu-merikoski/radioactivity-2-basics
