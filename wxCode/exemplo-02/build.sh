#!/usr/bin/env bash

# 1) Gere o Makefile com o bakefile (ok)
PYENV_VERSION=2.7.18 \
    LD_LIBRARY_PATH=/home/ivan/wx/xml2/lib \
    /home/ivan/wx/linux-wx-3.2.4/bin/bakefile -f gnu \
    minimal.bkl -o Makefile

mkdir -p build
mv Makefile build
cd build

make WX_CONFIG=/home/ivan/wx/linux-wx-3.2.4/bin/wx-config \
    CXXFLAGS="$(wx-config --toolkit=gtk3 --unicode=yes --debug=no --cxxflags)"

exit 0
