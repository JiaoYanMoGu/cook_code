#!/bin/bash

filename=$1
g++ ${filename}  -I/usr/include/eigen3  -o ./exe/${filename%.*}
