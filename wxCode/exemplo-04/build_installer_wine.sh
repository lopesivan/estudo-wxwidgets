#!/usr/bin/env bash

# ==========================================
# Script para criar instalador Windows usando
# Inno Setup via Wine no Linux
# ==========================================

set -e # Sair se houver erro

echo "=========================================="
echo "  Build Instalador - Minimal App (Wine)"
echo "=========================================="
echo ""

# ==========================================
# CONFIGURAÇÕES
# ==========================================
APP_NAME="minimal"
BUILD_DIR="build/gccmsw"
INSTALLER_SCRIPT="minimal_installer.iss"
OUTPUT_DIR="installer_output"

# Caminho correto do Inno Setup no seu Wine
WINE_INNO_DIR="$HOME/.wine/drive_c/Program Files (x86)/Inno Setup 6"
INNO_COMPILER="$WINE_INNO_DIR/ISCC.exe"

# ==========================================
# VERIFICAR WINE
# ==========================================
echo "[1/5] Verificando Wine..."
if ! command -v wine &>/dev/null; then
    echo ""
    echo "ERRO: Wine não está instalado!"
    echo ""
    echo "Instale com:"
    echo "  Ubuntu/Debian: sudo apt install wine64 wine32"
    echo "  Fedora:        sudo dnf install wine"
    echo "  Arch:          sudo pacman -S wine"
    echo ""
    exit 1
fi

echo "   OK - Wine versão: $(wine --version)"

# ==========================================
# VERIFICAR INNO SETUP
# ==========================================
echo "[2/5] Verificando Inno Setup..."

if [ ! -f "$INNO_COMPILER" ]; then
    echo ""
    echo "ERRO: Inno Setup não encontrado em:"
    echo "  $INNO_COMPILER"
    echo ""
    echo "Procurando em outros locais..."

    # Tentar encontrar em outros caminhos comuns
    POSSIBLE_PATHS=(
        "$HOME/.wine/drive_c/InnoSetup/ISCC.exe"
        "$HOME/.wine/drive_c/Program Files/Inno Setup 6/ISCC.exe"
        "$HOME/.wine/drive_c/Program Files (x86)/Inno Setup 6/ISCC.exe"
    )

    FOUND=0
    for path in "${POSSIBLE_PATHS[@]}"; do
        if [ -f "$path" ]; then
            INNO_COMPILER="$path"
            echo "   Encontrado: $path"
            FOUND=1
            break
        fi
    done

    if [ $FOUND -eq 0 ]; then
        echo ""
        echo "Inno Setup não encontrado!"
        echo "Baixe e instale de: https://jrsoftware.org/isdl.php"
        echo "Execute: wine innosetup-6.x.x.exe"
        exit 1
    fi
fi

echo "   OK - Inno Setup encontrado"
echo "   Caminho: $INNO_COMPILER"

# ==========================================
# VERIFICAR ARQUIVOS NECESSÁRIOS
# ==========================================
echo "[3/5] Verificando arquivos necessários..."

if [ ! -f "$BUILD_DIR/$APP_NAME.exe" ]; then
    echo ""
    echo "ERRO: Executável não encontrado: $BUILD_DIR/$APP_NAME.exe"
    echo ""
    echo "Execute primeiro: ./build.sh"
    exit 1
fi
echo "   OK - $APP_NAME.exe encontrado"

# Verificar DLLs
MISSING_DLLS=0
for dll in "libgcc_s_seh-1.dll" "libstdc++-6.dll"; do
    if [ ! -f "$BUILD_DIR/$dll" ]; then
        echo "   AVISO: $dll não encontrada!"
        MISSING_DLLS=1
    fi
done

if [ $MISSING_DLLS -eq 1 ]; then
    echo ""
    read -p "Continuar mesmo assim? (s/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Ss]$ ]]; then
        exit 1
    fi
fi

if [ ! -f "$INSTALLER_SCRIPT" ]; then
    echo ""
    echo "ERRO: Script do instalador não encontrado: $INSTALLER_SCRIPT"
    exit 1
fi
echo "   OK - Script do instalador encontrado"

# ==========================================
# LISTAR ARQUIVOS
# ==========================================
echo "[4/5] Arquivos que serão incluídos no instalador:"
if ls "$BUILD_DIR"/*.exe "$BUILD_DIR"/*.dll 2>/dev/null 1>&2; then
    for file in "$BUILD_DIR"/*.exe "$BUILD_DIR"/*.dll; do
        if [ -f "$file" ]; then
            SIZE=$(du -h "$file" | cut -f1)
            echo "   - $(basename "$file") ($SIZE)"
        fi
    done
else
    echo "   AVISO: Nenhum arquivo .exe ou .dll encontrado!"
fi

# ==========================================
# CRIAR DIRETÓRIO DE SAÍDA
# ==========================================
echo "[5/5] Preparando e compilando..."
mkdir -p "$OUTPUT_DIR"

# ==========================================
# COMPILAR INSTALADOR COM WINE
# ==========================================
echo ""
echo "Compilando instalador com Inno Setup..."
echo "──────────────────────────────────────"

# Converter caminho Linux para Windows (Wine)
# Usar caminho absoluto
SCRIPT_PATH="$(pwd)/$INSTALLER_SCRIPT"

# Tentar winepath primeiro
if command -v winepath &>/dev/null; then
    WINE_SCRIPT_PATH=$(winepath -w "$SCRIPT_PATH" 2>/dev/null)
else
    # Fallback: conversão manual
    WINE_SCRIPT_PATH="Z:$SCRIPT_PATH"
fi

echo "Script: $WINE_SCRIPT_PATH"
echo ""

# Compilar com Wine (redirecionar output)
wine "$INNO_COMPILER" "$WINE_SCRIPT_PATH" 2>&1 | grep -v "^fixme:" | grep -v "^wine:" || true

EXIT_CODE=${PIPESTATUS[0]}

if [ $EXIT_CODE -ne 0 ]; then
    echo ""
    echo "=========================================="
    echo "  ERRO: Falha ao criar instalador!"
    echo "=========================================="
    echo ""
    echo "Código de saída: $EXIT_CODE"
    echo ""
    echo "Verifique:"
    echo "  1. Se o arquivo $INSTALLER_SCRIPT está correto"
    echo "  2. Se os caminhos no .iss estão corretos"
    echo "  3. Se todos os arquivos necessários existem"
    echo ""
    echo "Tente compilar manualmente:"
    echo "  wine \"$INNO_COMPILER\" \"$WINE_SCRIPT_PATH\""
    echo ""
    exit 1
fi

# ==========================================
# SUCESSO
# ==========================================
echo ""
echo "=========================================="
echo "  INSTALADOR CRIADO COM SUCESSO!"
echo "=========================================="
echo ""

# Procurar e mostrar arquivo gerado
INSTALLER_FOUND=0

# Procurar no diretório de saída especificado
if [ -d "$OUTPUT_DIR" ]; then
    for installer in "$OUTPUT_DIR"/*.exe; do
        if [ -f "$installer" ]; then
            SIZE=$(du -h "$installer" | cut -f1)
            echo "📦 Arquivo gerado:"
            echo "   Nome: $(basename "$installer")"
            echo "   Tamanho: $SIZE"
            echo "   Caminho: $(realpath "$installer")"
            echo ""
            INSTALLER_FOUND=1
        fi
    done
fi

# Se não encontrou, procurar em outros locais
if [ $INSTALLER_FOUND -eq 0 ]; then
    echo "Procurando instalador..."
    FOUND_FILES=$(find . -name "minimal-setup-*.exe" -type f -newer "$INSTALLER_SCRIPT" 2>/dev/null)

    if [ -n "$FOUND_FILES" ]; then
        echo "$FOUND_FILES" | while read -r file; do
            SIZE=$(du -h "$file" | cut -f1)
            echo "📦 Encontrado:"
            echo "   $file ($SIZE)"
        done
        echo ""
    else
        echo "⚠️  AVISO: Arquivo .exe não encontrado!"
        echo "   Verifique manualmente o diretório $OUTPUT_DIR"
        echo ""
    fi
fi

echo "=========================================="
echo "  PRÓXIMOS PASSOS"
echo "=========================================="
echo ""
echo "✓ Para testar no Windows:"
echo "  1. Copie o arquivo .exe para uma máquina Windows"
echo "  2. Execute como administrador"
echo ""
echo "✓ Para testar com Wine (teste básico):"
echo "  wine $OUTPUT_DIR/minimal-setup-*.exe"
echo ""
echo "✓ Para verificar conteúdo do instalador:"
echo "  7z l $OUTPUT_DIR/minimal-setup-*.exe"
echo ""
echo "=========================================="

exit 0
