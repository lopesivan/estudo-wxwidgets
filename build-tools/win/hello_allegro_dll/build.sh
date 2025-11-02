#!/usr/bin/env bash
set -euo pipefail

MINGW64="/c/mingw64"
export PATH="$MINGW64/bin:$PATH"

echo ""
echo "✅ Ambiente MinGW-w64 ativado!"
"$MINGW64/bin/gcc.exe" --version || true
echo ""

# ====== Configuração do CMake ======
# Método A (recomendado): CMAKE_PREFIX_PATH -> RAIZ do Allegro
cmake -S . -B build -G "MinGW Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_VERBOSE_MAKEFILE=ON \
    -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES:FILEPATH="${PWD}/run.cmake" \
    -DCMAKE_MAKE_PROGRAM="C:/mingw64/bin/mingw32-make.exe" \
    -DCMAKE_C_COMPILER="C:/mingw64/bin/gcc.exe" \
    -DCMAKE_CXX_COMPILER="C:/mingw64/bin/g++.exe" \
    -DCMAKE_PREFIX_PATH="C:/Users/ivan/source/repos/SDKs/MinGW64/allegro"

# Método B (alternativo): usar Allegro_DIR
# cmake -S . -B build -G "MinGW Makefiles" \
#   -DCMAKE_BUILD_TYPE=Release \
#   -DCMAKE_C_COMPILER="C:/mingw64/bin/gcc.exe" \
#   -DCMAKE_CXX_COMPILER="C:/mingw64/bin/g++.exe" \
#   -DAllegro_DIR="C:/Users/ivan/source/repos/SDKs/MinGW64/allegro/lib/cmake/allegro"

#/c/mingw64/bin/mingw32-make.exe -C build-mingw64
cmake --build build -j

echo ""
echo "➡️  Binário em: build/hello_allegro.exe"
echo "Se faltar DLL, adicione ao PATH: C:/Users/ivan/source/repos/SDKs/MinGW64/allegro/bin"
