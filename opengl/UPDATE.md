Use `find + xargs + cp -v` para ver cada arquivo sendo copiado:

```bash id="b4t8kc"
SRC="/home/ivan/.local/wxwidgets/wxWidgets-3.3.1-linux-cmake/samples/opengl"
DST="/workspace/work/estudo-wxwidgets/opengl"

cd "$SRC" || exit 1

find . \
    -type f \
    \( -name '*.cpp' -o -name '*.c' -o -name '*.h' \) \
    -print0 |
xargs -0 -I{} cp -v --parents "{}" "$DST"
```

O importante aqui é:

```bash id="p6x2eu"
cd "$SRC"
```

porque o `--parents` do `cp` preserva o caminho relativo atual.

Então:

```text id="6xwhzm"
./cube/main.cpp
```

vira:

```text id="8i9q0d"
/workspace/work/estudo-wxwidgets/opengl/cube/main.cpp
```

E o `-v` mostra:

```text id="6kx0aj"
'./cube/main.cpp' -> '/workspace/work/estudo-wxwidgets/opengl/./cube/main.cpp'
```

O `-print0` + `xargs -0` evita problemas com espaços em nomes de arquivos.

Você pode fazer isso preservando a estrutura das subpastas e copiando apenas `.cpp`, `.c` e `.h`.

No seu caso:

```bash id="x3m7ab"
SRC="/home/ivan/.local/wxwidgets/wxWidgets-3.3.1-linux-cmake/samples/opengl"
DST="/workspace/work/estudo-wxwidgets/opengl"

find "$SRC" \
    \( -name '*.cpp' -o -name '*.c' -o -name '*.h' \) \
    -type f \
    -exec cp --parents {} "$DST" \;
```

Mas isso criará o caminho absoluto inteiro dentro do destino.

Melhor usar `rsync`:

```bash id="w9l2fd"
SRC="/home/ivan/.local/wxwidgets/wxWidgets-3.3.1-linux-cmake/samples/opengl/"
DST="/workspace/work/estudo-wxwidgets/opengl/"

rsync -av \
    --include='*/' \
    --include='*.cpp' \
    --include='*.c' \
    --include='*.h' \
    --exclude='*' \
    "$SRC" "$DST"
```

Isso irá:

* entrar em `cube`, `isosurf`, `penguin`, `pyramid`
* copiar apenas:

  * `.cpp`
  * `.c`
  * `.h`
* sobrescrever os arquivos antigos no destino
* manter a estrutura original

Resultado esperado:

```text id="jj6ejm"
/workspace/work/estudo-wxwidgets/opengl/cube/*.cpp
/workspace/work/estudo-wxwidgets/opengl/isosurf/*.cpp
/workspace/work/estudo-wxwidgets/opengl/penguin/*.cpp
...
```

Sem copiar:

* `.o`
* `.a`
* `.so`
* `CMakeFiles`
* builds
* assets extras
* `.git` etc.

