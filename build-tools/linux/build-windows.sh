#!/bin/sh

set -xe

WX_VERSION="3.2.4"
WX_SRC_DIR="wxWidgets-${WX_VERSION}-windows"
WX_PREFIX_DIR="${HOME}/wx/windows-wx-${WX_VERSION}"
WX_ZIP=wxWidgets-${WX_VERSION}.zip

# ==============================
# Extração do código-fonte
# ==============================
echo "[INFO] Extraindo ${WX_ZIP}..."
unzip ${WX_ZIP} -d ${WX_SRC_DIR}

# FIX:
#   $ cp /usr/x86_64-w64-mingw32/include/uxtheme.h{,.SAVED}
#   $ diff /usr/x86_64-w64-mingw32/include/uxtheme.h{,.SAVED}
#   196c196
#   <     WTA_NONCLIENT = 1
#   ---
#   >     WTA_NONCLIENT = 1
#   356c356
#   < THEMEAPI GetThemeSysFont(HTHEME hTheme,int iFontId,LOGFONTW *plf);
#   ---
#   > THEMEAPI GetThemeSysFont(HTHEME hTheme,int iFontId,LOGFONT *plf);
#   ^

cd ${WX_SRC_DIR} &&
    mkdir build_w64 &&
    cd build_w64 &&
    ../configure \
        --prefix="${WX_PREFIX_DIR}" \
        --host=x86_64-w64-mingw32 \
        --build=x86_64-linux \
        --disable-unicode \
        --enable-monolithic \
        --disable-shared \
        CFLAGS=-m64 CXXFLAGS=-m64 LDFLAGS=-m64 &&
    make

exit 0
