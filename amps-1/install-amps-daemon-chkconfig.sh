#!/bin/sh
# barebones install for AMPS 5.2 on CentOS 7
## Copyright (c) 2012-2017 60East Technologies Inc., All Rights Reserved.

INSTALLPATH=/opt/amps
RUNPATH=/var/run/amps
CONFIGPATH=/opt/etc/amps

STYLE="chkconfig" 

PATH=/sbin:$PATH

mkdir -p $INSTALLPATH
mkdir -p $RUNPATH
mkdir -p $CONFIGPATH

# Copy the bin and lib directories to the run directory.
cp -r bin $INSTALLPATH/.
cp -r lib $INSTALLPATH/.

echo "Installed AMPS binaries to $INSTALLPATH"

echo "Setting up SYSV style service"
cp -f bin/amps-init-script-rhat /etc/init.d/amps
ERROR=$?
if [ ! -x /etc/init.d/amps ]; then
   echo "Installation failed: $ERROR."
   exit $ERROR
fi
echo "AMPS initialization script installed to /etc/init.d/amps"
chkconfig --add amps  || echo "Couldn't add AMPS as a service"
chkconfig --level 345 amps on || echo "Couldn't turn AMPS service on"
echo "Configured AMPS service"

echo "Daemon is installed. You must provide a config file at /opt/etc/amps/config.xml"
