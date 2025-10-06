#!/usr/bin/env bash
set -euo pipefail

V=3.2.4

# Caminho base onde ficam as dependências e fontes do wxWidgets
WX_BASE_DIR="${HOME}/wx"
WX_SRC_DIR="${WX_BASE_DIR}/wxWidgets-${V}-linux"

# Prefixo de instalação do wxWidgets (binários e ferramentas)
WX_INSTALL_DIR="$WX_BASE_DIR/linux-wx-${V}"

# Versão do Python usada para executar bakefile
PYTHON_VERSION="2.7.18"

# === Gerar arquivos de build ===
PYENV_VERSION="$PYTHON_VERSION" \
    ${WX_INSTALL_DIR}/bin/bakefile_gen \
    -I ${WX_SRC_DIR}/build/bakefiles/wxpresets/presets

if test -e Makefile; then
    PATH=/usr/bin:bin \
        WX_CONFIG=$WX_INSTALL_DIR/bin/wx-config \
        make
fi

# /usr/local/bin
# /bin
# /usr/bin
exit 0
