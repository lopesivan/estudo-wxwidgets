#!/usr/bin/env bash
set -e

echo "📦 Criando instalador para minimal"

APP_NAME="minimal"
BUILD_DIR="build/gccmsw"
INSTALLER_SCRIPT="minimal_installer.iss"
OUTPUT_DIR="installer_output"

# Caminho padrão do Inno Setup no prefixo Wine
WINE_INNO_DIR="$HOME/.wine/drive_c/Program Files (x86)/Inno Setup 6"
INNO_COMPILER="$WINE_INNO_DIR/ISCC.exe"

echo "🔎 Verificando Wine..."
if ! command -v wine >/dev/null 2>&1; then
  echo "ERRO: Wine não está instalado."
  echo "Ubuntu/Debian: sudo apt install wine64 wine32"
  echo "Fedora:        sudo dnf install wine"
  echo "Arch:          sudo pacman -S wine"
  exit 1
fi
echo "   OK - $(wine --version)"

echo "🔎 Verificando Inno Setup..."
if [ ! -f "$INNO_COMPILER" ]; then
  POSSIBLE_PATHS=(
    "$HOME/.wine/drive_c/InnoSetup/ISCC.exe"
    "$HOME/.wine/drive_c/Program Files/Inno Setup 6/ISCC.exe"
    "$HOME/.wine/drive_c/Program Files (x86)/Inno Setup 6/ISCC.exe"
  )
  FOUND=0
  for path in "${POSSIBLE_PATHS[@]}"; do
    if [ -f "$path" ]; then
      INNO_COMPILER="$path"
      FOUND=1
      break
    fi
  done
  if [ $FOUND -eq 0 ]; then
    echo "ERRO: Inno Setup (ISCC.exe) não encontrado no prefixo Wine."
    echo "Baixe/instale: https://jrsoftware.org/isdl.php (wine innosetup-6.x.x.exe)"
    exit 1
  fi
fi
echo "   OK - Inno Setup em: $INNO_COMPILER"

echo "🔎 Verificando binários..."
if [ ! -f "$BUILD_DIR/$APP_NAME.exe" ]; then
  echo "ERRO: Executável não encontrado: $BUILD_DIR/$APP_NAME.exe"
  echo "Dica: compile antes (ex.: ./build.sh)"
  exit 1
fi

echo "🔎 Verificando DLLs..."
MISSING_DLLS=0
NEEDED_DLLS=( "libgcc_s_seh-1.dll" "libstdc++-6.dll" "libwinpthread-1.dll" )
for dll in "${NEEDED_DLLS[@]}"; do
  if [ ! -f "$BUILD_DIR/$dll" ]; then
    echo "⚠️  Faltando: $dll"
    # Não marcamos como erro fatal; o .iss possui #ifexist para a libwinpthread.
    # Mas libgcc_s_seh-1.dll e libstdc++-6.dll são praticamente obrigatórias.
    if [ "$dll" != "libwinpthread-1.dll" ]; then
      MISSING_DLLS=1
    fi
  fi
done

if [ $MISSING_DLLS -eq 1 ]; then
  echo "ERRO: DLLs essenciais faltando. Copie-as para $BUILD_DIR e tente novamente."
  exit 1
fi

echo "📁 Criando pasta de saída..."
mkdir -p "$OUTPUT_DIR"

if [ ! -f "$INSTALLER_SCRIPT" ]; then
  echo "ERRO: Script do instalador não encontrado: $INSTALLER_SCRIPT"
  exit 1
fi

# Caminho do .iss em formato Windows para o Wine
SCRIPT_PATH="$(pwd)/$INSTALLER_SCRIPT"
if command -v winepath >/dev/null 2>&1; then
  WINE_SCRIPT_PATH="$(winepath -w "$SCRIPT_PATH")"
else
  WINE_SCRIPT_PATH="Z:$SCRIPT_PATH"
fi

echo "🚀 Compilando com Inno Setup via Wine..."
# Silencia ruídos do Wine sem perder exit code
WINEDEBUG=-all wine "$INNO_COMPILER" "$WINE_SCRIPT_PATH"
EXIT_CODE=$?

if [ $EXIT_CODE -ne 0 ]; then
  echo "❌ Falha ao criar o instalador (exit $EXIT_CODE)."
  echo "Tente rodar manualmente:"
  echo "  WINEDEBUG=-all wine \"$INNO_COMPILER\" \"$WINE_SCRIPT_PATH\""
  exit $EXIT_CODE
fi

echo "✅ Compilação concluída."
echo "🔎 Procurando executável gerado..."
FOUND=0
if ls "$OUTPUT_DIR"/*.exe >/dev/null 2>&1; then
  for f in "$OUTPUT_DIR"/*.exe; do
    [ -f "$f" ] || continue
    SIZE=$(du -h "$f" | cut -f1)
    echo "📦 Arquivo: $(basename "$f")  ($SIZE)"
    echo "    Caminho: $(realpath "$f")"
    FOUND=1
  done
fi

if [ $FOUND -eq 0 ]; then
  echo "⚠️  Nenhum .exe encontrado no diretório $OUTPUT_DIR."
  echo "Verifique mensagens do Inno Setup e caminhos de saída."
fi

echo "ℹ️  Teste rápido no Wine:"
echo "    wine \"$OUTPUT_DIR/minimal-setup-$MyAppVersion-x64.exe\"  # ajuste o nome"


