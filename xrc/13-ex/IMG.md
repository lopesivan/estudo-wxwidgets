Aqui estão vários comandos ImageMagick para criar uma imagem PNG com as especificações exatas (115x280 pixels, RGB 8-bit, non-interlaced):

## Comando básico para imagem azul sólida:
```bash
convert -size 115x280 xc:blue wizard.png
```

## Comando com gradiente azul:
```bash
convert -size 115x280 gradient:blue-darkblue wizard.png
```

## Comando mais elaborado com texto:
```bash
convert -size 115x280 gradient:navy-blue \
        -pointsize 20 \
        -fill white \
        -gravity center \
        -annotate +0+0 "Wizard" \
        -strip \
        wizard.png
```

## Comando com detalhes visuais (estilo wizard):
```bash
convert -size 115x280 \
        gradient:royalblue-steelblue \
        -fill gold \
        -stroke black \
        -strokewidth 1 \
        -draw "circle 57,80 57,110" \
        -fill white \
        -pointsize 14 \
        -gravity center \
        -annotate +0+0 "Wizard" \
        -pointsize 10 \
        -annotate +0+20 "Setup" \
        -strip \
        wizard.png
```

## Comando para verificar as propriedades da imagem gerada:
```bash
identify wizard.png
```

## Comando mais avançado com múltiplos elementos:
```bash
convert -size 115x280 \
        gradient:darkblue-lightblue \
        -fill white \
        -stroke navy \
        -strokewidth 2 \
        -draw "roundrectangle 20,40 95,120 10,10" \
        -fill yellow \
        -draw "circle 57,80 57,65" \
        -fill white \
        -pointsize 12 \
        -gravity south \
        -annotate +0+50 "Assistente" \
        -pointsize 10 \
        -annotate +0+30 "de Configuração" \
        -strip \
        wizard.png
```

## Se você quiser usar uma imagem existente (zaul) como base:
```bash
convert zaul.jpg -resize 115x280! -colorspace RGB -strip wizard.png
```

O parâmetro `!` força o redimensionamento exato para 115x280 ignorando a proporção.

## Para garantir que é RGB 8-bit e non-interlaced:
```bash
convert -size 115x280 xc:blue -depth 8 -interlace none wizard.png
```

## Comando final recomendado (mais elaborado):
```bash
convert -size 115x280 \
        gradient:midnightblue-dodgerblue \
        -fill gold \
        -stroke black \
        -strokewidth 1 \
        -draw "ellipse 57,100 25,35 0,360" \
        -fill white \
        -pointsize 16 \
        -font Arial \
        -gravity north \
        -annotate +0+30 "Wizard" \
        -pointsize 10 \
        -annotate +0+60 "Click to Start" \
        -fill silver \
        -draw "rectangle 25,180 90,200" \
        -fill forestgreen \
        -draw "rectangle 25,210 90,230" \
        -strip \
        -depth 8 \
        -interlace none \
        wizard.png
```

**Notas importantes:**
- `-strip` remove metadados desnecessários
- `-depth 8` garante 8-bit por canal de cor
- `-interlace none` garante que a imagem seja non-interlaced
- `gradient:cor1-cor2` cria um gradiente vertical
- As coordenadas nos comandos `draw` são relativas ao tamanho 115x280

Escolha o comando que melhor atende às suas necessidades visuais!

