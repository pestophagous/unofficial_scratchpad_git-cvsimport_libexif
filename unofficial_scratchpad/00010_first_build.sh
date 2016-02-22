#!/bin/bash

cd ..

./configure CFLAGS='-g -O0' --disable-static --prefix=$PWD/install

cd unofficial_scratchpad
