#!/bin/bash

if [ -f /usr/bin/gnome-terminal ]
then
  cd /home/xairy/Documents/database/extra
	/usr/bin/gnome-terminal -t "SQL*Plus" -e "/usr/lib/oracle/xe/app/oracle/product/10.2.0/server/config/scripts/sqlplus.sh"
else
  cd /home/xairy/Documents/database/extra
	/usr/bin/xterm  -T "SQL*Plus" -n "SQL*Plus" -e "/usr/lib/oracle/xe/app/oracle/product/10.2.0/server/config/scripts/sqlplus.sh"
fi
