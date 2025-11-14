#!/usr/bin/env bash

mkdir build && cd build
PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig \
    cmake .. -DCMAKE_BUILD_TYPE=Release

exit 0
