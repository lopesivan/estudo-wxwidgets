#!/usr/bin/env bash
set -euo pipefail

MINGW64="/c/mingw64"
export PATH="$MINGW64/bin:$PATH"

echo ""
echo "✅ Ambiente MinGW-w64 ativado!"
"$MINGW64/bin/gcc.exe" --version || true
echo ""

#/c/mingw64/bin/mingw32-make.exe -C build-mingw64
cmake --build build  --target run

