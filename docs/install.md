# Installation of FoodCoopSystem

*Here goes all information about how to install FCS*


## Installation

Application use [Drush Make](https://www.drupal.org/project/drush_make) to download all needed components.


1. Go to `app/` directory: `cd app/`
2. Run drush make script: `drush make foodcoopsystem.make`
3. Install default Drupal version (follow installer).
4. Replace installed new Drupal version with the Database from /database folder.
5. Run `drush updb`
6. Run `drush vdel preprocess_css`
7. Run `drush cc all`
8. [Optionally] You can set your admin user new password by running `drush upwd admin --password=admin`
