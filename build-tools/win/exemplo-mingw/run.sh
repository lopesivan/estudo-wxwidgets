#!/usr/bin/env bash

# if exist path `build' then remove.
_d=build-mingw64

MINGW64="/c/mingw64"

# Adiciona o binário do MinGW ao PATH
export PATH="$MINGW64/bin:$PATH"

echo ""
echo "✅ Ambiente MinGW-w64 ativado!"
echo "👉 GCC em: $MINGW64/bin"
echo "🛠 Versão:"
gcc --version
echo ""

/c/mingw64/bin/mingw32-make.exe -C $_d run

exit 0
