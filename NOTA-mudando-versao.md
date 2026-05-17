Faça assim:

```bash id="vqgl5w"
find . -type f \
    -exec sed -i 's/3\.2\.4/3.3.1/g' {} +
```

Depois confira:

```bash id="otqfjx"
find . -type f -exec grep '3\.3\.1' {} +
```

ou veja se ainda restou algo:

```bash id="t1f2c3"
find . -type f -exec grep '3\.2\.4' {} +
```

O escape `\.` é importante porque no regex do `sed` o `.` significa “qualquer caractere”.


Então exclua o diretório `.git` do `find`:

```bash id="g7a0qj"
find . \
    -path './.git' -prune -o \
    -type f \
    -exec sed -i 's/3\.2\.4/3.3.1/g' {} +
```

Explicando:

```bash id="bjlwm4"
-path './.git' -prune
```

significa:

* quando encontrar `.git`
* não entre nele (`prune`)

Assim o `sed` não modifica:

* histórico
* refs
* logs
* objetos git

Depois confira:

```bash id="3zm9g0"
find . \
    -path './.git' -prune -o \
    -type f \
    -exec grep '3\.2\.4' {} +
```

Se não aparecer nada, terminou corretamente.


Use:

```bash id="c3e1pk"
find . \
    -path './.git' -prune -o \
    -type f -name 'CMakeLists.txt' \
    -exec sed -i \
    's/find_package(wxWidgets 3\.2/find_package(wxWidgets 3.3/g' {} +
```

Depois confira:

```bash id="2q8r7n"
find . \
    -path './.git' -prune -o \
    -type f -name 'CMakeLists.txt' \
    -exec grep 'find_package(wxWidgets' {} +
```

Isso altera somente:

```cmake id="s64w5f"
find_package(wxWidgets 3.2
```

para:

```cmake id="g5nq4k"
find_package(wxWidgets 3.3
```

sem tocar no `.git`.
