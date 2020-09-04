#!/bin/bash

inc=`gmt-config --cflags`
lib=`gmt-config --libs`
g++ gridding.cpp $inc $lib -o gridding
./gridding