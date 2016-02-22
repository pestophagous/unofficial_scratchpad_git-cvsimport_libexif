#!/bin/bash

cd tis

make TIS_ARGS='-64 -val ' analyze/test/.libs/test-parse

cd ..
