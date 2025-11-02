#!/bin/bash

# Script para gerar bolas coloridas em PNG
# Uso: ./generate_balls.sh

# Verifica se ImageMagick está instalado
if ! command -v convert &> /dev/null; then
    echo "Erro: ImageMagick não encontrado!"
    echo "Instale com: sudo apt-get install imagemagick"
    exit 1
fi

# Tamanho das bolas
SIZE=128
RADIUS=64
CENTER=64
EDGE=4

echo "Gerando bolas coloridas..."

# Array com cores e nomes
declare -A COLORS=(
    ["red"]="255,0,0"
    ["green"]="0,255,0"
    ["blue"]="30,144,255"
    ["yellow"]="255,255,0"
    ["orange"]="255,165,0"
    ["purple"]="128,0,128"
    ["pink"]="255,192,203"
    ["cyan"]="0,255,255"
    ["lime"]="50,205,50"
    ["magenta"]="255,0,255"
    ["gold"]="255,215,0"
    ["coral"]="255,127,80"
    ["turquoise"]="64,224,208"
    ["violet"]="238,130,238"
    ["indigo"]="75,0,130"
    ["brown"]="165,42,42"
    ["gray"]="128,128,128"
    ["silver"]="192,192,192"
)

# Gera cada bola
for name in "${!COLORS[@]}"; do
    rgb="${COLORS[$name]}"
    output="ball_${name}.png"
    
    convert -size ${SIZE}x${SIZE} xc:none \
        -fill "rgb(${rgb})" \
        -stroke "rgb(${rgb})" -strokewidth 1 \
        -draw "circle ${CENTER},${CENTER} ${CENTER},${EDGE}" \
        "${output}"
    
    echo "✓ Criado: ${output} (rgb: ${rgb})"
done

# Gera a bola padrão (ball.png)
echo ""
echo "Gerando ball.png padrão (azul)..."
convert -size ${SIZE}x${SIZE} xc:none \
    -fill "rgb(30,144,255)" \
    -stroke "rgb(30,144,255)" -strokewidth 1 \
    -draw "circle ${CENTER},${CENTER} ${CENTER},${EDGE}" \
    ball.png

echo "✓ Criado: ball.png"
echo ""
echo "Concluído! ${#COLORS[@]} bolas coloridas + 1 padrão geradas."
echo "Total: $((${#COLORS[@]} + 1)) arquivos PNG"

