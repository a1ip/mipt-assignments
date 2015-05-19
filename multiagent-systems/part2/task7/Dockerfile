FROM ubuntu:14.04

RUN \
  sed -i 's/# \(.*multiverse$\)/\1/g' /etc/apt/sources.list && \
  apt-get update && \
  apt-get -y upgrade && \
  apt-get install -y g++ python python-pip && \
  apt-get install -y python python-pip && \
  apt-get install -y python3 python3-pip && \
  rm -rf /var/lib/apt/lists/*

ENV HOME /root
WORKDIR /root
CMD ["bash"]
