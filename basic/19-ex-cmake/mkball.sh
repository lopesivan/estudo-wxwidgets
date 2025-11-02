#!/usr/bin/env bash

# Script interativo para gerar ball.png com diferentes métodos
# Usa dialog se disponível, senão usa menu simples

set -e

# Verifica se ImageMagick está instalado
if ! command -v convert &>/dev/null; then
    echo "❌ Erro: ImageMagick não encontrado!"
    echo "Instale com: sudo apt-get install imagemagick"
    exit 1
fi

# Verifica se dialog está disponível
USE_DIALOG=false
if command -v dialog &>/dev/null; then
    USE_DIALOG=true
fi

# Função para gerar com método 1: Sólido Simples
method1_solid() {
    echo "🎨 Gerando ball.png - Método 1: Círculo Sólido Simples"
    convert -size 128x128 xc:none \
        -fill "rgb(30,144,255)" \
        -stroke "rgb(30,144,255)" -strokewidth 1 \
        -draw "circle 64,64 64,4" \
        ball.png
    echo "✅ ball.png criado com sucesso (Método 1: Sólido)"
}

# Função para gerar com método 2: Com Borda
method2_border() {
    echo "🎨 Gerando ball.png - Método 2: Com Borda Preta"
    convert -size 128x128 xc:none \
        -fill red -stroke black -strokewidth 2 \
        -draw "circle 64,64 64,4" \
        ball.png
    echo "✅ ball.png criado com sucesso (Método 2: Com Borda)"
}

# Função para gerar com método 3: Gradiente
method3_gradient() {
    echo "🎨 Gerando ball.png - Método 3: Gradiente Radial"
    convert -size 128x128 radial-gradient:blue-lightblue \
        -alpha set -virtual-pixel transparent \
        -channel A -evaluate multiply 0.0 +channel \
        -fill none -draw "circle 64,64 64,4" \
        ball.png
    echo "✅ ball.png criado com sucesso (Método 3: Gradiente)"
}

# Função para visualizar o resultado
view_result() {
    if [ -f ball.png ]; then
        if command -v xdg-open &>/dev/null; then
            xdg-open ball.png 2>/dev/null &
        elif command -v display &>/dev/null; then
            display ball.png &
        else
            echo "ℹ️  Arquivo gerado: $(pwd)/ball.png"
            echo "   Tamanho: $(stat -f%z ball.png 2>/dev/null || stat -c%s ball.png) bytes"
        fi
    fi
}

# Menu com dialog
show_dialog_menu() {
    while true; do
        choice=$(dialog --clear --title "Gerador de ball.png" \
            --menu "Escolha o método de geração:" 15 60 4 \
            1 "Círculo Sólido Simples (Azul)" \
            2 "Com Borda Preta (Vermelho)" \
            3 "Gradiente Radial (Azul)" \
            4 "Sair" \
            3>&1 1>&2 2>&3)

        exit_status=$?

        # Limpa a tela
        clear

        # Se cancelou, sai
        if [ $exit_status -ne 0 ]; then
            echo "Cancelado pelo usuário."
            exit 0
        fi

        case $choice in
            1)
                method1_solid
                view_result
                ;;
            2)
                method2_border
                view_result
                ;;
            3)
                method3_gradient
                view_result
                ;;
            4)
                echo "👋 Até logo!"
                exit 0
                ;;
            *)
                echo "❌ Opção inválida!"
                ;;
        esac

        echo ""
        read -p "Pressione ENTER para continuar..."
    done
}

# Menu simples (sem dialog)
show_simple_menu() {
    while true; do
        echo ""
        echo "╔════════════════════════════════════════════════╗"
        echo "║       Gerador de ball.png - Escolha um        ║"
        echo "╚════════════════════════════════════════════════╝"
        echo ""
        echo "  1) Círculo Sólido Simples (Azul)"
        echo "     └─ Cor uniforme sem efeitos"
        echo ""
        echo "  2) Com Borda Preta (Vermelho)"
        echo "     └─ Preenchimento vermelho + borda preta"
        echo ""
        echo "  3) Gradiente Radial (Azul)"
        echo "     └─ Efeito 3D com gradiente"
        echo ""
        echo "  4) Sair"
        echo ""
        echo "────────────────────────────────────────────────"
        read -p "Digite sua escolha [1-4]: " choice
        echo ""

        case $choice in
            1)
                method1_solid
                view_result
                ;;
            2)
                method2_border
                view_result
                ;;
            3)
                method3_gradient
                view_result
                ;;
            4)
                echo "👋 Até logo!"
                exit 0
                ;;
            *)
                echo "❌ Opção inválida! Digite um número de 1 a 4."
                ;;
        esac

        echo ""
        read -p "Pressione ENTER para continuar..."
        clear
    done
}

# Modo não-interativo (com argumentos)
if [ $# -gt 0 ]; then
    case $1 in
        1 | solid | simples)
            method1_solid
            ;;
        2 | border | borda)
            method2_border
            ;;
        3 | gradient | gradiente)
            method3_gradient
            ;;
        -h | --help | help)
            echo "Uso: $0 [método]"
            echo ""
            echo "Métodos disponíveis:"
            echo "  1, solid, simples    - Círculo sólido simples"
            echo "  2, border, borda     - Com borda preta"
            echo "  3, gradient, gradiente - Gradiente radial"
            echo ""
            echo "Exemplos:"
            echo "  $0              # Modo interativo"
            echo "  $0 1            # Gera método 1 direto"
            echo "  $0 gradient     # Gera método 3 direto"
            exit 0
            ;;
        *)
            echo "❌ Método inválido: $1"
            echo "Use: $0 --help para ver as opções"
            exit 1
            ;;
    esac
    exit 0
fi

# Modo interativo
clear
if [ "$USE_DIALOG" = true ]; then
    show_dialog_menu
else
    show_simple_menu
fi

exit 0
