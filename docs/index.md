# Welcome to FoodCoopSystem
![FoodCoopSystem](http://www.foodcoop.pl/images/logo.png)

[![Build Status](https://travis-ci.org/FoodCoopSystem/foodcoopsystem.svg)](https://travis-ci.org/FoodCoopSystem/foodcoopsystem)

FoodCoop System is an open source group ordering and billing system based on Drupal Commerce (Drupal 7) and PHP language.

Main goal of a project is build easy to install platform for (not only Polish) food cooperatives, to help manage orders, stock, tasks and members.

First version of the system was founded in 2013 by UNDP for Warsaw Food Cooperative and was developed by Obin.org and RatioWeb.pl members. 

If you want to help us, say hello by mail: contact(at)foodcoop.pl. Instruction of system and documentation will be avalible here: http://foodcoop.pl/  

Our versioning is following Semantic [Versioning 2.0.0](http://semver.org/).


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
