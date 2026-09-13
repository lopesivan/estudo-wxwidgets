#!/usr/bin/env bash
set -euo pipefail

V=3.3.1
WX_BASE_DIR="${HOME}/.local/wxwidgets"
WX_SRC_DIR="${WX_BASE_DIR}/wxWidgets-${V}-linux-cmake"
WX_INSTALL_DIR="${WX_BASE_DIR}/linux-cmake-wx-${V}"
PYTHON_VERSION="2.7.18"

# === Gerar Makefile via bakefile ===
PYENV_VERSION="$PYTHON_VERSION" \
    LD_LIBRARY_PATH=/home/ivan/wx/xml2/lib \
    "${WX_INSTALL_DIR}"/bin/bakefile_gen \
    -I "${WX_SRC_DIR}"/build/bakefiles/wxpresets/presets \
    -DOUT_OF_TREE_MAKEFILES=1

if test -e Makefile; then
    make WX_CONFIG="$WX_INSTALL_DIR/bin/wx-config" \
        LDFLAGS="-L$WX_INSTALL_DIR/lib -Wl,-rpath,$WX_INSTALL_DIR/lib"
fi

exit 0
