FROM centos:7
RUN yum -y install initscripts
WORKDIR /app
ADD . /app
# NB needed to use explicit path even though this script is in this directory
RUN ./install-amps-daemon-chkconfig.sh
RUN cp AMPS-configs/config.xml /opt/etc/amps/config.xml

EXPOSE 8085
EXPOSE 9007

CMD [ "service", "amps", "start" ]
