
--------------------------------------------------------------------------------
                               Rules Links
--------------------------------------------------------------------------------

This module allows you to create links, which trigger rules.

Dependencies
------------

Rules Links dependes on following modules:
 * Rules - http://drupal.org/project/rules
 * Entity API - http://drupal.org/project/entity
 * (Optionally) Views 3 - http://drupal.org/project/views

Usage
-----

To create a new link go to admin/config/workflow/rules_links and click on
"Add rules link". Enter the title of your link and set all the settings. After
clicking on the "Save Rules Link" button, the module will generate a new And-
and Rules-Set and their forms will be embedded into the rules link editing form.
The And-Sets are called Visibility Conditions and will define when to render
the links. The Rules set is the rule, that will be triggered on clicking the
rule. Additionally Rules Links also generates access permission for each link,
which you'll have to set in admin/people/permissions.

The best way to display the links is currently using views. Rules Links link
will create a Views field for each link, using the title of the link as title
for the field. So if you have link for nodes with the delete 'Delete node',
you'll find the Views field under name 'Content: Delete node'.
Alternatively you can use render the links directly in the entities that can be
displayed, using the option 'Show link in entity' in the Rules Links settings.
Note that this will not work for links were the parameters have multiple.
paramaters.
If you would like to render a link yourself in a theme or in a module, than use
the function rules_link_render_link().