
User Relationship Blocks
------------------------
Provides a few basic blocks for use with User Relationships. Block explanations are below.


Send comments to Jeff Smick: http://drupal.org/user/107579/contact, or post an issue at
http://drupal.org/project/user_relationships.


Requirements
------------
Drupal 7
User Relationships API
User Relationships UI


Installation
------------
1. Enable User Relationship Blocks in the "Modules" administration screen.


Blocks
------
User Relationships: Actions
  Some basic actions that can be performed between the current user and the user being viewed*.
  This block is useful for those using nodeprofile

My Pending Relationships
  Shows all the pending relationship requests, both requested from and of the current user.

My Relationships: All Relationships
  Shows all of the current users relationships. The number of relationships and the order
  in which they're shown can be configured.

My Relationships: {Relationship Type}
  Where {Relationship Type} is a specific relationship type. This will show only the current
  user's relationships of the specified type.

User Relationships: All Relationships
  Like "My Relationships: All Relationships" only it shows the relationships of the user being
  viewed*

User Relationships: {Relationship Type}
  Like "My Relationships: {Relationship Type}" only it shows the relationships of the user being
  viewed*


* A note about the user being viewed:
  This user is found using _user_relationship_blocks_get_uid(). This can be
  overridden by implementing hook_user_relationship_blocks_get_uid().


Theme Developers
----------------
There is one theme function and three theme files.

The function is "theme_user_relationship_block_subject" and is used to generate the title of the block

The files are under the "templates" directory and listed below:
  user_relationships-block for "My Relationships: " and "User Relationships: " blocks
  user_relationships-pending_block for pending relationships
  user_relationships-actions_block for relationship actions


Credits
-------
Originally Written by JB Christy.
Rewritten by Jeff Smick.
Written originally for and financially supported by OurChart Inc. (http://www.ourchart.com)
