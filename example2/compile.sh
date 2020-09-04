#!/bin/bash

inc=`gmt-config --cflags`
lib=`gmt-config --libs`
g++ plot_grid.cpp $inc $lib -o plot_grid
./plot_grid