-----------------------
GENERAL DESCRIPTION
-----------------------
This module allows you to set access permissions for various taxonomy 
categories based on user role.  

There are permissions to VIEW, UPDATE, and DELETE nodes in each category.
Additionally, the ADD TAG permission control whether the user can add a 
taxonomy term to a node, and the VIEW TAG permission controls whether the user
can see the taxonomy term listed on the node.


-----------------------
HELP PAGES
-----------------------
For more information about how to control access permissions with the Taxonomy
access control module (TAC), see the module's help page at:
"Administration >> Help >> Taxonomy access control"
(admin/help/taxonomy_access).

Also see the help pages at drupal.org: http://drupal.org/node/31601


-----------------------
DATABASE TABLES
-----------------------
Module creates two tables in database: 'taxonomy_access_term' and
'taxonomy_access_default'


-----------------------
TROUBLESHOOTING
-----------------------

If users can view or edit pages that they do not have permission for:

1. Make sure the user role does not have "administer nodes" permission.  This 
   permission will override any settings from Taxonomy Access.

2. Check whether the user role has "edit any [type] content" permissions 
   under "node module" on the page: 
   "Administration >> People >> Permissions"
   (http://www.example.com/admin/people/permissions).

   Granting this permission overrides TAC's "Update" permissions for the given 
   content type, so you will not be able to deny the role edit access to any 
   nodes in that type.  (The same is true of "delete any [type] content" 
   permissions.)

3. Check to see if the user has other roles that may be granting other 
   permissions. Remember: Deny overrides Allow within a role, but Allow from 
   one role can override Deny from another.

4. Review the configuration for the authenticated user role on page:
   "Administration >> People >> Permissions"
   (http://www.example.com/admin/people/permissions).

   Remember that users with custom roles also have the authenticated role, so 
   they gain any permissions granted that role.

5. Check whether you have ANY OTHER node access modules installed.
   Other modules can override TAC's grants.

6. Do a General Database Housekeeping
  (Tables: 'node_access','taxonomy_access_term' and 'taxonomy_access_default'):

   First DISABLE, then RE-ENABLE the Taxonomy access module on page:
   "Administration >> Modules"
   (http://www.example.com/admin/modules).
    
  This will force the complete rebuild of the 'node_access' table.
  
7. For debugging, install devel_node_access module (Devel project).
   This can show you some information about node_access values in 
   the database when viewing a node page.

8. Force rebuilding of the permissions cache (table 'node_access'):
   "Rebuild permissions" button on page:
   "Administration >> Reports >> Status report >> Node Access Permissions"
   (http://www.example.com/admin/reports/status/rebuild).

   If the site is experiencing problems with permissions to content, you may
   have to rebuild the permissions cache. Possible causes for permission
   problems are disabling modules or configuration changes to permissions.
   Rebuilding will remove all privileges to posts, and replace them with
   permissions based on the current modules and settings.

-----------------------
UNINSTALLING
-----------------------

1. First DISABLE the Taxonomy access module on page:
   "Administration >> Modules"
   (http://www.example.com/admin/modules).

2. After disabling, you can uninstall completely by choosing Taxonomy
   Access on page: 
   "Administration >> Modules >> Uninstall"
   (http://www.example.com/admin/modules/uninstall).

   This will remove all your settings of Taxonomy Access: variables and tables
   ('taxonomy_access_term' and 'taxonomy_access_default').

3. After uninstalling, if the site is experiencing problems with permissions to
   content, you can rebuild the permission cache.
   See "Troubleshooting" #8.
