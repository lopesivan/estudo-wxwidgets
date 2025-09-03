#!/usr/bin/env bash
set -euo pipefail

# Caminho base onde ficam as dependências e fontes do wxWidgets
WX_BASE_DIR="$HOME/wx"

# Prefixo de instalação do wxWidgets (binários e ferramentas)
WX_INSTALL_DIR="$WX_BASE_DIR/linux-wx-3.2.4"

# Versão do Python usada para executar bakefile
PYTHON_VERSION="2.7.18"

# === Gerar arquivos de build ===
PYENV_VERSION="$PYTHON_VERSION" \
    "$WX_INSTALL_DIR/bin/bakefile" -f gnu hello.bkl -o Makefile

if test -e Makefile; then
    WX_CONFIG=$WX_INSTALL_DIR/bin/wx-config make
fi

exit 0
