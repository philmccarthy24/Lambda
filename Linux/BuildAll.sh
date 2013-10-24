#!/bin/bash

mkdir build
make -f StaticLib.make
make -f Lambda.make
make -f LambdaTests.make
rm *.o
build/lambdatests
