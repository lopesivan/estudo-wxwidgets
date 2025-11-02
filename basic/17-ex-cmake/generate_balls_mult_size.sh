#!/bin/bash

# Script para gerar bolas em múltiplos tamanhos e cores
# Uso: ./generate_balls_multi_size.sh

if ! command -v convert &> /dev/null; then
    echo "Erro: ImageMagick não encontrado!"
    exit 1
fi

echo "Gerando bolas em múltiplos tamanhos e cores..."

# Cores disponíveis
declare -A COLORS=(
    ["red"]="255,0,0"
    ["green"]="0,200,0"
    ["blue"]="30,144,255"
    ["yellow"]="255,220,0"
    ["orange"]="255,140,0"
    ["purple"]="147,51,234"
    ["pink"]="255,105,180"
    ["cyan"]="0,206,209"
)

# Tamanhos disponíveis
SIZES=(64 128 256)

# Gera bolas em todos os tamanhos e cores
for size in "${SIZES[@]}"; do
    radius=$((size / 2))
    center=$((size / 2))
    edge=4
    
    echo ""
    echo "=== Tamanho: ${size}x${size} ==="
    
    for name in "${!COLORS[@]}"; do
        rgb="${COLORS[$name]}"
        output="ball_${name}_${size}.png"
        
        convert -size ${size}x${size} xc:none \
            -fill "rgb(${rgb})" \
            -stroke "rgb(${rgb})" -strokewidth 1 \
            -draw "circle ${center},${center} ${center},${edge}" \
            "${output}"
        
        echo "  ✓ ${output}"
    done
done

# Gera ball.png padrão (128x128 azul)
echo ""
echo "Gerando ball.png padrão..."
convert -size 128x128 xc:none \
    -fill "rgb(30,144,255)" \
    -stroke "rgb(30,144,255)" -strokewidth 1 \
    -draw "circle 64,64 64,4" \
    ball.png
echo "  ✓ ball.png"

total=$((${#COLORS[@]} * ${#SIZES[@]} + 1))
echo ""
echo "Concluído! Total: ${total} arquivos PNG gerados"
echo "  - ${#COLORS[@]} cores"
echo "  - ${#SIZES[@]} tamanhos (${SIZES[*]})"

