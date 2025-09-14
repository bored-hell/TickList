#!/bin/bash

SRC="main.cpp gui.cpp core.cpp"
CXXFLAGS="-std=c++20 -Wall -Wextra"
BUILD_FOLDER="bin"
WXFLAGS=`wx-config --cxxflags --libs core`

if [ ! -d $BUILD_FOLDER ]; then
  mkdir $BUILD_FOLDER
fi 

set -xe

g++ -o $BUILD_FOLDER/TaskList $SRC $CXXFLAGS $WXFLAGS
