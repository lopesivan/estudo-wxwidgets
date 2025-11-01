#!/usr/bin/env bash

# if exist path `build' then remove.
_d=build-mingw64
test -d $_d && rm -rf $_d
mkdir $_d

MINGW64="/c/mingw64"

# Adiciona o binário do MinGW ao PATH
export PATH="$MINGW64/bin:$PATH"

echo ""
echo "✅ Ambiente MinGW-w64 ativado!"
echo "👉 GCC em: $MINGW64/bin"
echo "🛠 Versão:"
gcc --version
echo ""

cmake -S . -B $_d -G "MinGW Makefiles" \
    -D CMAKE_BUILD_TYPE=Release \
    -D wxUSE_WEBVIEW=OFF \
    -D wxUSE_WEBVIEW_EDGE=OFF \
    -D CMAKE_MAKE_PROGRAM="C:/mingw64/bin/mingw32-make.exe" \
    -D CMAKE_C_COMPILER="C:/mingw64/bin/gcc.exe" \
    -D CMAKE_CXX_COMPILER="C:/mingw64/bin/g++.exe" \
    -D wxWidgets_DIR="C:/Users/ivan/source/repos/SDKs/MinGW64/wx-3.3.1-gcc-x64/lib/cmake/wxWidgets"

/c/mingw64/bin/mingw32-make.exe -C $_d

exit 0
