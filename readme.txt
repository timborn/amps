Fri Oct 27 15:27:09 CDT 2017
----------------------------
Docker + AMPS
http://devnull.crankuptheamps.com/releases/amps/

Timex:Music timborn$ docker run -it -v ~/Downloads/:/Downloads ubuntu bash

root@65d1254dce33:/Downloads/AMPS-5.2.0.70-Release-Linux/bin# ./install-amps-daemon.sh 
Testing system install style
Using systemd service definitions.
Installed AMPS binaries to /opt/amps
AMPS service definition installed to /usr/lib/systemd/system/amps.service
Failed to connect to bus: No such file or directory
Loaded AMPS service definition
Daemon is installed. You must provide a config file at /opt/etc/amps/config.xml

