#!/usr/bin/env bash

# mkdir build && cd build
# cmake .. -DCMAKE_BUILD_TYPE=Release
# cmake --build . --parallel
# ./wx_allegro_paint        # Linux/macOS
# wx_allegro_paint.exe      # Windows

mkdir build && cd build
PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig \
    cmake .. -DCMAKE_BUILD_TYPE=Release

exit 0
