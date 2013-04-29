Commerce Stock Module
=====================

This module provides stock management for Drupal Commerce stores.
Commerce Stock 7.x-1.0-alpha3 is now compatible with Drupal Commerce 7.x-1.0-rc1+

To install and configure

   1. Install and enable the module.
   2. Visit admin/commerce/config/stock to enable stock tracking on your product type(s).
   3. Set the starting value of stock on each product.


he stock module does two main things

   1. Maintain stock levels
   2. Implement validation of stock to prevent users from ordering out of stock items

The module does the following stock validation checks

   1. Disable the add to cart button for out of stock products.
   2. Validates the add to cart quantity widget.
   3. Checks that all products and quantities in the shopping cart (/cart) are in stock
   4. On Checkout if you attempt to continue with out of stock items you get redirected to the shopping cart.

If you are using multiple products per display and are not using attributes:

   1. Marks items as out of stock in the dropdown

Management of stock

Each product type that is to be stock controlled is to be enabled using the admin interface.
Enabled product types will have a stock field added to them; this field will hold the current stock count.
You can disable stock checking for specific products this is useful if an organisation has a “flagship” product that’s always in stock.

A rule is created by the module to decrease the stock level when an order is complete.

The modules also provides some rule conditions and actions (increase / decrease stock) for you to create your own custom rules
