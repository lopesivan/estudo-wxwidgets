#!/usr/bin/env bash

MINGW64="/c/mingw64"

# Adiciona o binário do MinGW ao PATH
export PATH="$MINGW64/bin:$PATH"

echo ""
echo "✅ Ambiente MinGW-w64 ativado!"
echo "👉 GCC em: $MINGW64/bin"
echo "🛠 Versão:"
gcc --version
echo ""

cmake -S . -B build-mingw64 -G "MinGW Makefiles" \
    -D CMAKE_BUILD_TYPE=Release \
    -D wxUSE_WEBVIEW=OFF \
    -D wxUSE_WEBVIEW_EDGE=OFF \
    -D CMAKE_MAKE_PROGRAM="C:/mingw64/bin/mingw32-make.exe" \
    -D CMAKE_C_COMPILER="C:/mingw64/bin/gcc.exe" \
    -D CMAKE_CXX_COMPILER="C:/mingw64/bin/g++.exe" \
    -D CMAKE_PROJECT_TOP_LEVEL_INCLUDES:FILEPATH="${PWD}/run.cmake" \
    -D wxWidgets_DIR="C:/Users/ivan/source/repos/SDKs/MinGW64/wx-3.3.1-gcc-x64/lib/cmake/wxWidgets"

/c/mingw64/bin/mingw32-make.exe -C build-mingw64

exit 0
