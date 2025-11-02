#!/usr/bin/env bash
set -euo pipefail

# ==============================
# Configurações
# ==============================
WX_VERSION="3.2.4"
WX_SRC_DIR="wxWidgets-${WX_VERSION}-linux"
WX_PREFIX_DIR="${HOME}/wx/linux-wx-${WX_VERSION}"
WX_ZIP=wxWidgets-${WX_VERSION}.zip

# ==============================
# Extração do código-fonte
# ==============================
echo "[INFO] Extraindo ${WX_ZIP}..."
unzip ${WX_ZIP} -d ${WX_SRC_DIR}

exit 0
