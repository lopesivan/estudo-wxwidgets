#!/usr/bin/env bash

# Script interativo avançado para gerar ball.png
# Suporta dialog e permite personalizar cores

set -e

# Verifica ImageMagick
if ! command -v convert &> /dev/null; then
    echo "❌ Erro: ImageMagick não encontrado!"
    echo "Instale com: sudo apt-get install imagemagick"
    exit 1
fi

# Verifica dialog
USE_DIALOG=false
if command -v dialog &> /dev/null; then
    USE_DIALOG=true
fi

# Cores predefinidas
declare -A COLORS=(
    ["Azul (Padrão)"]="30,144,255"
    ["Vermelho"]="255,0,0"
    ["Verde"]="0,200,0"
    ["Amarelo"]="255,220,0"
    ["Laranja"]="255,140,0"
    ["Roxo"]="147,51,234"
    ["Rosa"]="255,105,180"
    ["Ciano"]="0,206,209"
)

# Variáveis globais
SELECTED_COLOR="30,144,255"
OUTPUT_FILE="ball.png"

# Método 1: Sólido
method1() {
    local color=$1
    echo "🎨 Gerando ${OUTPUT_FILE} - Método 1: Sólido"
    convert -size 128x128 xc:none \
        -fill "rgb(${color})" \
        -stroke "rgb(${color})" -strokewidth 1 \
        -draw "circle 64,64 64,4" \
        "${OUTPUT_FILE}"
    echo "✅ ${OUTPUT_FILE} criado (Sólido, rgb: ${color})"
}

# Método 2: Com Borda
method2() {
    local color=$1
    echo "🎨 Gerando ${OUTPUT_FILE} - Método 2: Com Borda"
    convert -size 128x128 xc:none \
        -fill "rgb(${color})" -stroke black -strokewidth 2 \
        -draw "circle 64,64 64,4" \
        "${OUTPUT_FILE}"
    echo "✅ ${OUTPUT_FILE} criado (Com Borda, rgb: ${color})"
}

# Método 3: Gradiente
method3() {
    local color=$1
    echo "🎨 Gerando ${OUTPUT_FILE} - Método 3: Gradiente"
    
    # Converte RGB para nome de cor (aproximado)
    local color_name="blue"
    case $color in
        "255,0,0") color_name="red" ;;
        "0,200,0"|"0,255,0") color_name="green" ;;
        "30,144,255") color_name="blue" ;;
        "255,220,0"|"255,255,0") color_name="yellow" ;;
        "255,140,0"|"255,165,0") color_name="orange" ;;
        "147,51,234"|"128,0,128") color_name="purple" ;;
        "255,105,180") color_name="pink" ;;
        "0,206,209"|"0,255,255") color_name="cyan" ;;
    esac
    
    convert -size 128x128 radial-gradient:${color_name}-light${color_name} \
        -alpha set -virtual-pixel transparent \
        -channel A -evaluate multiply 0.0 +channel \
        -fill none -draw "circle 64,64 64,4" \
        "${OUTPUT_FILE}"
    echo "✅ ${OUTPUT_FILE} criado (Gradiente, cor: ${color_name})"
}

# Seleciona cor com dialog
select_color_dialog() {
    local options=()
    local i=1
    for color_name in "${!COLORS[@]}"; do
        options+=("$i" "$color_name")
        ((i++))
    done
    
    choice=$(dialog --clear --title "Escolha a Cor" \
        --menu "Selecione uma cor:" 15 50 8 \
        "${options[@]}" \
        3>&1 1>&2 2>&3)
    
    if [ $? -eq 0 ]; then
        i=1
        for color_name in "${!COLORS[@]}"; do
            if [ "$i" -eq "$choice" ]; then
                SELECTED_COLOR="${COLORS[$color_name]}"
                return 0
            fi
            ((i++))
        done
    fi
    return 1
}

# Seleciona cor simples
select_color_simple() {
    echo ""
    echo "╔════════════════════════════════════════╗"
    echo "║         Escolha uma Cor                ║"
    echo "╚════════════════════════════════════════╝"
    echo ""
    
    local i=1
    local color_array=()
    for color_name in "${!COLORS[@]}"; do
        echo "  $i) $color_name"
        color_array[$i]="$color_name"
        ((i++))
    done
    
    echo ""
    read -p "Digite o número da cor [1-${#COLORS[@]}]: " choice
    
    if [[ "$choice" =~ ^[0-9]+$ ]] && [ "$choice" -ge 1 ] && [ "$choice" -le "${#COLORS[@]}" ]; then
        local selected_name="${color_array[$choice]}"
        SELECTED_COLOR="${COLORS[$selected_name]}"
        echo "✓ Cor selecionada: $selected_name (${SELECTED_COLOR})"
        return 0
    else
        echo "❌ Escolha inválida!"
        return 1
    fi
}

# Menu principal com dialog
main_dialog() {
    while true; do
        choice=$(dialog --clear --title "Gerador de ball.png" \
            --menu "Escolha uma opção:" 18 60 7 \
            1 "Método 1: Sólido Simples" \
            2 "Método 2: Com Borda Preta" \
            3 "Método 3: Gradiente Radial" \
            4 "Escolher Cor (atual: rgb ${SELECTED_COLOR})" \
            5 "Gerar Todas (3 métodos)" \
            6 "Ver arquivo gerado" \
            7 "Sair" \
            3>&1 1>&2 2>&3)
        
        exit_status=$?
        clear
        
        if [ $exit_status -ne 0 ]; then
            echo "Cancelado."
            exit 0
        fi
        
        case $choice in
            1) method1 "${SELECTED_COLOR}" ;;
            2) method2 "${SELECTED_COLOR}" ;;
            3) method3 "${SELECTED_COLOR}" ;;
            4) select_color_dialog ;;
            5)
                method1 "${SELECTED_COLOR}"
                cp ball.png ball_solid.png
                method2 "${SELECTED_COLOR}"
                cp ball.png ball_border.png
                method3 "${SELECTED_COLOR}"
                cp ball.png ball_gradient.png
                echo "✅ 3 versões criadas: ball_solid.png, ball_border.png, ball_gradient.png"
                ;;
            6)
                if [ -f ball.png ]; then
                    xdg-open ball.png 2>/dev/null || display ball.png 2>/dev/null || echo "$(pwd)/ball.png"
                else
                    dialog --msgbox "ball.png não encontrado!" 6 40
                fi
                ;;
            7) echo "👋 Até logo!"; exit 0 ;;
        esac
        
        echo ""
        read -p "Pressione ENTER para continuar..."
    done
}

# Menu principal simples
main_simple() {
    while true; do
        clear
        echo ""
        echo "╔════════════════════════════════════════════════╗"
        echo "║          Gerador de ball.png                   ║"
        echo "╚════════════════════════════════════════════════╝"
        echo ""
        echo "  Cor atual: rgb(${SELECTED_COLOR})"
        echo ""
        echo "  1) Método 1: Sólido Simples"
        echo "  2) Método 2: Com Borda Preta"
        echo "  3) Método 3: Gradiente Radial"
        echo "  4) Escolher Cor"
        echo "  5) Gerar Todas (3 métodos)"
        echo "  6) Ver arquivo gerado"
        echo "  7) Sair"
        echo ""
        echo "────────────────────────────────────────────────"
        read -p "Escolha [1-7]: " choice
        echo ""
        
        case $choice in
            1) method1 "${SELECTED_COLOR}" ;;
            2) method2 "${SELECTED_COLOR}" ;;
            3) method3 "${SELECTED_COLOR}" ;;
            4) select_color_simple ;;
            5)
                method1 "${SELECTED_COLOR}"
                cp ball.png ball_solid.png
                method2 "${SELECTED_COLOR}"
                cp ball.png ball_border.png
                method3 "${SELECTED_COLOR}"
                cp ball.png ball_gradient.png
                echo "✅ 3 versões: ball_solid.png, ball_border.png, ball_gradient.png"
                ;;
            6)
                if [ -f ball.png ]; then
                    xdg-open ball.png 2>/dev/null || display ball.png 2>/dev/null || \
                    echo "ℹ️  Arquivo: $(pwd)/ball.png"
                else
                    echo "❌ ball.png não encontrado!"
                fi
                ;;
            7) echo "👋 Até logo!"; exit 0 ;;
            *) echo "❌ Opção inválida!" ;;
        esac
        
        echo ""
        read -p "Pressione ENTER..."
    done
}

# Modo linha de comando
if [ $# -gt 0 ]; then
    case $1 in
        1|solid) method1 "${SELECTED_COLOR}" ;;
        2|border) method2 "${SELECTED_COLOR}" ;;
        3|gradient) method3 "${SELECTED_COLOR}" ;;
        all)
            method1 "${SELECTED_COLOR}"
            cp ball.png ball_solid.png
            method2 "${SELECTED_COLOR}"
            cp ball.png ball_border.png
            method3 "${SELECTED_COLOR}"
            cp ball.png ball_gradient.png
            echo "✅ 3 versões criadas"
            ;;
        -h|--help)
            echo "Uso: $0 [método] [cor_rgb]"
            echo ""
            echo "Métodos: 1|solid, 2|border, 3|gradient, all"
            echo "Cor RGB: formato R,G,B (ex: 255,0,0)"
            echo ""
            echo "Exemplos:"
            echo "  $0           # Modo interativo"
            echo "  $0 1         # Método 1 com cor padrão"
            echo "  $0 gradient  # Método 3 com cor padrão"
            echo "  $0 all       # Gera os 3 métodos"
            ;;
        *)
            echo "❌ Opção inválida: $1"
            exit 1
            ;;
    esac
    exit 0
fi

# Modo interativo
clear
if [ "$USE_DIALOG" = true ]; then
    main_dialog
else
    main_simple
fi

exit 0
