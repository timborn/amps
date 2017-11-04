#!/bin/sh
INSTALLPATH=/opt/amps
RUNPATH=/var/run/amps
CONFIGPATH=/opt/etc/amps

PATH=/sbin:$PATH

exec 3>&2
exec 2>/dev/null

STYLE="systemd"
echo "Using systemd service definitions."

# Now find a good installpath
if [ -d /usr/lib/systemd/system ]; then
    SYSTEMD_PATH=/usr/lib/systemd/system
else
    SYSTEMD_PATH=/usr/lib/systemd/
fi

exec 2>&3

mkdir -p $INSTALLPATH
mkdir -p $RUNPATH
mkdir -p $CONFIGPATH

# Copy the bin and lib directories to the run directory.
cp -r bin $INSTALLPATH/.
cp -r lib $INSTALLPATH/.


echo "Installed AMPS binaries to $INSTALLPATH"

cp -f bin/amps.service ${SYSTEMD_PATH}/amps.service
ERROR=$?
if [ ! -e ${SYSTEMD_PATH}/amps.service ]; then
   echo "Installation failed: $ERROR"
   exit $ERROR
fi
echo "AMPS service definition installed to /usr/lib/systemd/system/amps.service"
systemctl daemon-reload
echo "Loaded AMPS service definition"

echo "Daemon is installed. You must provide a config file at /opt/etc/amps/config.xml"
