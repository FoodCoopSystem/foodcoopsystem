#!/bin/bash

# This script removes all external (like contrib and libraries) source
# from current working directory, donwloads it (using webapp.make file)
# and applies patches (if needed) on downloaded codebase.

# First: remove (if needed) some junk data from local directory
cd app/
rm -rf sites/all/libraries
rm -rf sites/all/modules/contrib/*
rm -rf authorize.php
rm -rf CHANGELOG.txt
rm -rf COPYRIGHT.txt
rm -rf cron.php
rm -rf includes
rm -rf index.php
rm -rf INSTALL.mysql.txt
rm -rf INSTALL.pgsql.txt
rm -rf install.php
rm -rf INSTALL.sqlite.txt
rm -rf INSTALL.txt
rm -rf LICENSE.txt
rm -rf log
rm -rf MAINTAINERS.txt
rm -rf misc
rm -rf modules
rm -rf profiles
rm -rf README.txt
rm -rf robots.txt
rm -rf scripts
rm -rf themes
rm -rf update.php
rm -rf UPGRADE.txt
rm -rf web.config
rm -rf .htaccess
rm -rf xmlrpc.php

# Second: execute drush make and download all required libraries / modules etc.
drush -y make --contrib-destination=sites/all/ foodcoopsystem.make

#(First time only) Move files to proper dir.
mkdir -p sites/default/files
chmod 777 -R sites/default/files/
mkdir -p tmp
chmod 777 -R tmp

#cp -R conf/demo/files/* sites/default/files/


#(Move settings.php and sites.php to proper dir.
cp ../conf/settings/settings.php sites/default/

if [ ! -f sites/default/local.settings.php ]; then
  echo "You need to store local.settings.php file into sites/default/ subdirectory. This file has to include database access credentials.";
  exit 1;
fi

#Update script fo database changes.

gunzip ../database/foodcoop.sql.gz
drush sql-drop -y
drush sql-cli < ../database/foodcoop.sql
drush -y updatedb

drush -y vset preprocess_css 0
drush -y vset preprocess_js 0
drush fra -y
drush cc all
