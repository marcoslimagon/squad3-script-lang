#!/bin/bash
autoreconf -vi
./configure --prefix=/usr
make
make check