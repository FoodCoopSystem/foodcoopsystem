#!/bin/bash

find . -iname "*.php" -o -iname "*.module" -o  -iname "*.install" -o -iname "*.inc" -print0|xargs -0 -P8 -n1 php -l
