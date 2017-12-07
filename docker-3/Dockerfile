# recipe for Kafka 3.2.1 Docker image
FROM bluedata/centos7:latest

# install Java 8
RUN yum -y install java-1.8.0-openjdk-devel

# dld and extract Kafka installation
RUN mkdir /usr/lib/kafka; curl -s http://packages.confluent.io/archive/3.2/confluent-3.2.1-2.11.tar.gz | tar xz -C /usr/lib/kafka

# create necessary directories for Kafka and Zookeeper to run
RUN mkdir /var/lib/zookeeper
# etc
