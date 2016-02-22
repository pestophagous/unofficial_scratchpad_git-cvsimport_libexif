#!/bin/bash

check_return_val()
{
    if [ "$1" -ne 0 ]
    then
        exit $1
    fi
}

cd ..

./configure CFLAGS='-g -O0' --disable-static --prefix=$PWD/install

check_return_val $?

make

cd unofficial_scratchpad
