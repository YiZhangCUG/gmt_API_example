#!/bin/bash

inc=`gmt-config --cflags`
lib=`gmt-config --libs`
g++ plot_memory.cpp $inc $lib -o plot_memory
./plot_memory