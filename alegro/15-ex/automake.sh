#!/usr/bin/env bash

# Script para executar make automaticamente quando arquivos são modificados
# Uso: ./automake.sh [diretório] [extensões]

set -e

# Configurações
WATCH_DIR="${1:-.}"
EXTENSIONS="${2:-c,cpp,h,hpp,cc,cxx}"
DEBOUNCE_TIME=0.5

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Verifica se inotifywait está instalado
if ! command -v inotifywait &>/dev/null; then
    echo -e "${RED}Erro: inotifywait não encontrado!${NC}"
    echo "Instale com: sudo apt-get install inotify-tools"
    exit 1
fi

# Verifica se existe Makefile
if [ ! -f "${WATCH_DIR}/Makefile" ]; then
    echo -e "${YELLOW}Aviso: Makefile não encontrado em ${WATCH_DIR}${NC}"
    read -p "Continuar mesmo assim? (s/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Ss]$ ]]; then
        exit 1
    fi
fi

# Converte extensões para regex do inotifywait
EXT_REGEX=$(echo "$EXTENSIONS" | sed 's/,/|/g')

echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${GREEN}🔍 Monitorando alterações em:${NC} $WATCH_DIR"
echo -e "${GREEN}📄 Extensões:${NC} $EXTENSIONS"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${YELLOW}Pressione Ctrl+C para parar${NC}\n"

# Executa make inicial
echo -e "${BLUE}[$(date '+%H:%M:%S')]${NC} Executando make inicial..."
if make -C "$WATCH_DIR"; then
    echo -e "${GREEN}✓ Build inicial concluído${NC}\n"
else
    echo -e "${RED}✗ Build inicial falhou${NC}\n"
fi

# Variável para controlar debounce
last_build_time=0

# Loop principal de monitoramento
inotifywait -m -r -e modify,create,delete,move \
    --exclude '(\.git|\.swp|\.o|\.d|\.tmp|build|bin|obj|\.vscode|\.idea)' \
    --format '%w%f %e' \
    "$WATCH_DIR" | while read -r file event; do

    # Filtra por extensão
    if [[ ! "$file" =~ \.($EXT_REGEX)$ ]]; then
        continue
    fi

    # Debounce: evita builds múltiplos em rápida sucessão
    current_time=$(date +%s.%N)
    time_diff=$(echo "$current_time - $last_build_time" | bc)

    if (($(echo "$time_diff < $DEBOUNCE_TIME" | bc -l))); then
        continue
    fi

    last_build_time=$current_time

    # Exibe informação sobre o arquivo modificado
    filename=$(basename "$file")
    echo -e "\n${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${YELLOW}📝 Detectada mudança:${NC} $filename ($event)"
    echo -e "${BLUE}[$(date '+%H:%M:%S')]${NC} Executando make..."

    # Executa make
    if make -C "$WATCH_DIR" 2>&1; then
        echo -e "${GREEN}✓ Build concluído com sucesso${NC}"
    else
        echo -e "${RED}✗ Build falhou - verifique os erros acima${NC}"
        # Opcional: notificação do sistema
        if command -v notify-send &>/dev/null; then
            notify-send -u critical "Build Falhou" "Erro ao compilar $filename"
        fi
    fi
    echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
done
