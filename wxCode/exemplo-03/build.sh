#!/usr/bin/env bash

LD_LIBRARY_PATH=/home/ivan/wx/xml2/lib \
    /home/ivan/wx/linux-wx-3.2.4/bin/bakefile -f gnu \
    minimal.bkl -o Makefile

mkdir -p build
mv Makefile build
cd build

make WX_CONFIG=/home/ivan/wx/windows-wx-3.3.1/bin/wx-config \
    CXXFLAGS="$(wx-config --debug=no --cxxflags)"

exit 0
