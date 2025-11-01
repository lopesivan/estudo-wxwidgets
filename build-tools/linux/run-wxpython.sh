#!/usr/bin/env bash

PYENV_VERSION="NEOVIM"
PYTHON_VERSION_PATH="$(pyenv root)/versions/${PYENV_VERSION}"
PYTHON_LIB_PATH="${PYTHON_VERSION_PATH}/lib"
WX_PACKAGES_PATH="${PYTHON_LIB_PATH}/python3.10/site-packages/wx"

echo LD_LIBRARY_PATH=${WX_PACKAGES_PATH} python hello_world.py
LD_LIBRARY_PATH=${WX_PACKAGES_PATH} python hello_world.py

exit 0
