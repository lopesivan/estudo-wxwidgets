#!/bin/bash

# Script para gerar bolas REALISTAS com gradiente e sombra
# Uso: ./generate_balls_realistic.sh

if ! command -v convert &> /dev/null; then
    echo "Erro: ImageMagick não encontrado!"
    echo "Instale com: sudo apt-get install imagemagick"
    exit 1
fi

SIZE=128
RADIUS=60
CENTER=64

echo "Gerando bolas realistas com gradiente e sombra..."

# Cores base
declare -A COLORS=(
    ["red"]="255,0,0"
    ["green"]="0,200,0"
    ["blue"]="30,144,255"
    ["yellow"]="255,220,0"
    ["orange"]="255,140,0"
    ["purple"]="147,51,234"
    ["pink"]="255,105,180"
    ["cyan"]="0,206,209"
    ["lime"]="50,205,50"
    ["magenta"]="255,0,255"
    ["gold"]="255,215,0"
    ["coral"]="255,127,80"
    ["turquoise"]="64,224,208"
    ["violet"]="138,43,226"
    ["indigo"]="75,0,130"
    ["teal"]="0,128,128"
    ["crimson"]="220,20,60"
    ["emerald"]="80,200,120"
)

# Função para criar bola com gradiente radial
create_ball() {
    local name=$1
    local r=$2
    local g=$3
    local b=$4
    local output=$5
    
    # Calcula cor mais clara (highlight)
    local hr=$(( r + (255 - r) * 60 / 100 ))
    local hg=$(( g + (255 - g) * 60 / 100 ))
    local hb=$(( b + (255 - b) * 60 / 100 ))
    
    # Calcula cor mais escura (sombra)
    local sr=$(( r * 40 / 100 ))
    local sg=$(( g * 40 / 100 ))
    local sb=$(( b * 40 / 100 ))
    
    convert -size ${SIZE}x${SIZE} xc:none \
        \( -size ${SIZE}x${SIZE} \
           radial-gradient:"rgb(${hr},${hg},${hb})-rgb(${r},${g},${b})" \
           -gravity northwest -crop ${SIZE}x${SIZE}+20+20 +repage \
        \) \
        \( -size ${SIZE}x${SIZE} xc:none -fill white \
           -draw "circle ${CENTER},${CENTER} ${CENTER},$((CENTER - RADIUS))" \
        \) \
        -compose Over -composite \
        \( +clone -background "rgb(${sr},${sg},${sb})" \
           -shadow 80x3+2+2 \
        \) +swap \
        -background none -compose Over -flatten \
        "${output}"
}

# Gera cada bola realista
for name in "${!COLORS[@]}"; do
    IFS=',' read -r r g b <<< "${COLORS[$name]}"
    output="ball_${name}.png"
    
    create_ball "${name}" "${r}" "${g}" "${b}" "${output}"
    echo "✓ Criado: ${output} (${r},${g},${b})"
done

# Gera a bola padrão (ball.png - azul)
echo ""
echo "Gerando ball.png padrão (azul realista)..."
create_ball "default" 30 144 255 "ball.png"
echo "✓ Criado: ball.png"

echo ""
echo "Concluído! ${#COLORS[@]} bolas realistas + 1 padrão geradas."
echo "Total: $((${#COLORS[@]} + 1)) arquivos PNG"

