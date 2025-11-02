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

/c/mingw64/bin/mingw32-make.exe -C build-mingw64 run

exit 0
