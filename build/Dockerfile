FROM ubuntu:19.04

RUN apt-get update && apt-get install -y build-essential \
  autoconf \
  wget \
  libsubunit-dev \
  pkg-config

WORKDIR /opt

RUN wget https://github.com/libcheck/check/releases/download/0.12.0/check-0.12.0.tar.gz
RUN tar -zxvf check-0.12.0.tar.gz
WORKDIR /opt/check-0.12.0
RUN ./configure --prefix=/usr
RUN make
RUN make install

RUN mkdir /workdir
WORKDIR /workdir

ENTRYPOINT ./build/run.test.sh
