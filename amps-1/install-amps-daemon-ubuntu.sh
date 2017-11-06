#!/bin/sh
INSTALLPATH=/opt/amps
RUNPATH=/var/run/amps
CONFIGPATH=/opt/etc/amps
AMPS=AMPS-5.2.0.70-Release-Linux

# Ensure we have sbin
PATH=/sbin:$PATH

echo "Using Debian-style daemon installation."

mkdir -p $INSTALLPATH
mkdir -p $RUNPATH
mkdir -p $CONFIGPATH

# Copy the bin and lib directories to the run directory.
cp -r $AMPS/bin $INSTALLPATH/.
cp -r $AMPS/lib $INSTALLPATH/.

echo "Installed AMPS binaries to $INSTALLPATH"

echo "Copying init script to /etc/init.d/amps"
cp -f $AMPS/bin/amps-init-script-debian /etc/init.d/amps
ERROR=$?
if [ ! -x /etc/init.d/amps ]; then
   echo "Installation failed: $ERROR."
   exit $ERROR
fi
echo "AMPS initialization script installed to /etc/init.d/amps"
update-rc.d amps defaults || echo "Couldn't update!"
echo "Loaded AMPS service definition"

echo "Daemon is installed. You must provide a config file at /opt/etc/amps/config.xml"
