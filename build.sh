#!/bin/bash

SRC="main.cpp gui.cpp core.cpp"
BUILD_FOLDER="bin"
WXFLAGS=`wx-config --cxxflags --libs core`

BUILD_TYPE=${1:-release}
CXXFLAGS="-std=c++20 -Wall -Wextra"

if [ "$BUILD_TYPE" = "debug" ]; then
    echo "Building in Debug mode..."
    CXXFLAGS+=" -g"
elif [ "$BUILD_TYPE" = "release" ]; then
    echo "Building in Release mode..."
    CXXFLAGS+=" -O2"
else
    echo "Invalid build type. Please use 'debug' or 'release'."
    exit 1
fi

if [ ! -d "$BUILD_FOLDER" ]; then
    mkdir -p "$BUILD_FOLDER"
fi

set -xe

g++ -o "$BUILD_FOLDER/TickList" $SRC $CXXFLAGS $WXFLAGS
