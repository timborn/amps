FROM centos:7
ENV container docker
ENV NAME World
WORKDIR /app
ADD . /app
# NB needed to use explicit path even though this script is in this directory
RUN ./install-amps-daemon-chkconfig.sh

EXPOSE 8085
EXPOSE 9007

# CMD [ "python", "app.py"]
# is you need systemd check this: https://hub.docker.com/_/centos/

CMD [ "/opt/amps/bin/ampServer", "--sample-config", ">", "/Downloads/config-sample.xml" ]
