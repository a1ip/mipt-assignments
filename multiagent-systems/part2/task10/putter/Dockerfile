FROM ubuntu:14.04

RUN \
  apt-get update && \
  apt-get install -y python python-pip && \
  apt-get install -y libyaml-dev libpython-dev && \
  pip install -U nameko

ADD . /code
WORKDIR /code

CMD ./run.sh putter
