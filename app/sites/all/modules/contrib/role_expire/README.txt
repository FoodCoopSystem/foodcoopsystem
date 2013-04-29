/* $Id$ */

Role expire provides expiration to roles attached to users. When you enable role_expire,

1. On the screen where admins create users and assign roles (admin/people/create),
   those roles can have expiry dates.
2. A user with sufficient privilege ('administer role expire' or 'administer users')
   can view and edit roles and expiry dates. (Role expire just adds the bit to
   /user that provides the expiry dates) [standard stuff on /user/#/edit]
3. On the role administration screen (admin/people/permissions/roles/edit), you
   can set a default duration for each role.
4. Selecting a role on user_profile_form triggers a textfield (or textfields)
   where admins can enter expiration date for the selected role.
5. Defined expiry dates are displayed on the user's profile page, and is visible
   only to owners of the profile or users with proper permissions).
6. Actual role expiration occurs at cron time. Cron automatically removes
   expired roles from affected users.

TODO: Views and rules integration.
