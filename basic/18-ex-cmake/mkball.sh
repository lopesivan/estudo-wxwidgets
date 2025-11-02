#!/usr/bin/env bash

convert -size 128x128 xc:none \
    -fill "rgb(30,144,255)" \
    -stroke "rgb(30,144,255)" -strokewidth 1 \
    -draw "circle 64,64 64,4" \
    ball.png

convert -size 128x128 xc:none \
    -fill "rgb(30,144,255)" \
    -stroke "rgb(30,144,5)" -strokewidth 1 \
    -draw "circle 64,64 64,4" \
    ball1.png
