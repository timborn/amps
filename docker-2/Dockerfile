FROM ansible/centos7-ansible:latest
RUN yum update -y && install -y java-1.8.0-openjdk-devel.x86_64 & yum clean all
ENV JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.91-b14.e16.x86_64
