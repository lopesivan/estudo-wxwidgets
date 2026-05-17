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


