
-- SUMMARY --

Invitations are important to create network effects and exponential growth of a
community of interest. This module adds an 'Invite a friend' feature that
allows your users to send and track invitations to join your site.

For a full description visit the project page:
  http://drupal.org/project/invite
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/invite


-- REQUIREMENTS --

* Token module http://drupal.org/project/token


-- INSTALLATION --

1. Copy the invite module to your modules directory and enable it on the Modules
   page (admin/modules).

2. Give some roles permission to send invites on the Permissions page
   (admin/people/permissions). The following permissions can be controlled:

   administer invitations - Allows users to access the administrative overview
     and settings pages, and to see other people's invitations.

   send mass invitations - Allows users to send an invitation to multiple
     recipients (this was formerly a setting known as "limit per turn").

   track invitations - Gives users access to the user overview pages and
     associated actions (withdraw etc). Useful to hide overviews from anonymous
     users.

   withdraw own invitations - Allows users to withdraw invitations. If an
     invitation has been withdrawn, it cannot be used to join the site
     and it does not count against the sender's invitation limit.

   withdraw own accepted invitations - This will allow your users to delete
     accepted invitations. Disable it to prevent users from deleting
     their account to be re-invited. With the help of the Cancel User Accounts
     module it is possible to terminate user accounts by withdrawing an
     invitation.

   view invite statistics (invite_stats module) - Allows users to view invite
     statistics on their profile pages as well as view the Top inviters/User
     rank block.

   view own invite statistics (invite_stats module) - Same as above, but limits
     viewing statistics to the user's own profile.

3. Invite adds a new registration mode called 'New user registration by
   invitation only' to the User settings page (admin/config/people/accounts),
   which allows you to maintain a semi-private site. You can enable it if you
   need it.

4. Configure the module at Configuration > Invite
   (admin/config/people/invite). For an explanation of the configuration
   settings see below.


-- CONFIGURATION --

--- General settings ---

* Default target role
  Allows you to specify the role invited users will be added to when they
  register, regardless of who invited them.

* Invitation expiry
  Specify how long sent invitations are valid (in days). After an invitation
  expires the registration link becomes invalid.

* Path to registration page
  Specifies where the users will be redirected when they click the invitation
  link.

--- Role settings (separate sections for each role) ---

* Target role
  Allows you to specify an additional role invited users will be added to if
  their inviter has a specific role.

* Invitation limit
  Allows you to limit the total number of invitations each role can send.

--- E-mail settings ---

* Subject
  The default subject of the invitation e-mail.

* Editable subject
  Whether the user should be able to customize the subject.

* Mail template
  The e-mail body.

* From/Reply-To e-mail address
  Choose whether to send the e-mail on behalf of the user or in the name of the
  site.

* Manually override From/Reply-To e-mail address (Advanced settings)
  Allows to override the sender and reply-to addresses used in all e-mails.
  Make sure the domain matches that of your SMTP server, or your e-mails will
  likely be marked as spam.

--- Invite page customization ---

* Invite page title
  Allows you to change the title of the invite page and link.

-- USAGE --

Sent invitations show up in one of three states: accepted, pending, expired, or
deleted.

* Accepted: Shows that the person you have invited has accepted the invitation
  to join the site.
* Pending: The invitation has been sent, but the invitee has since not accepted
  the invitation.
* Expired: The invitation has not been used to register within the expiration
  period.
* Deleted: The user account has been blocked.

At any time, pending or expired invitations may be withdrawn. Accepted
invitations may only be withdrawn if the configuration allows you to.


-- INVITE API --

See invite.api.php.

-- CREDITS --

Original author:
  David Hill (tatonca)

Current maintainer:
  Stefan M. Kudwien (smk-ka) - http://drupal.org/user/48898
