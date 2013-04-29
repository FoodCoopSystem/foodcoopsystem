This module enables you to give decimal quantities for a product. 

The Drupal Commerce module allows only integer number quantities to be bought. This can be a great pain if you are selling liquids, curtains, wires or time related services.

This module adds a checkbox to the Product type edit form to enable decimal quantities for that product type. 

The module modifies the following forms to add decimal quantities for the products that have the above checkbox enabled:
* Add to cart form
* Admin order edit form
* Shopping cart block and page
* Any view that exposed the line item quantity field


This module works with commerce-stock (however commerce stock only validates the add to cart and cart forms)

Usage: 
1. Enable the module.
2. Go to the edit form of the product type you wish to enable decimal quantities for.
3. Check the checkbox 'Allow decimal quantities'.

Note: you can find the list of product types at: Administration » Store » Products » Product types

Please report the bugs you encounter in the issue queue. 