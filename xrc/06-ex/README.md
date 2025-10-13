Isso é **mismatch de bibliotecas do wx** em runtime. Seu binário está pegando **algumas libs do sistema** (`/lib/x86_64-linux-gnu/...`) em vez das que você construiu em `~/wx/linux-wx-3.2.4/lib`. Aí rola a quebra de ABI/símbolos (ex.: `wxTrapInAssert` versionado para WXU_3.2).

### Conserto rápido (sem rebuild)

Rode com o `LD_LIBRARY_PATH` apontando para suas libs locais:

```bash
export LD_LIBRARY_PATH="$HOME/wx/linux-wx-3.2.4/lib:$LD_LIBRARY_PATH"
./app
```

### Deixar permanente no Makefile (preferível)

O seu `-Wl,-rpath,...` está gerando **RUNPATH** por padrão (new-dtags). RUNPATH **não vale para dependências indiretas**, e o loader acaba caindo nas libs do sistema. Forçe **RPATH clássico**:

1. Adicione a flag para desabilitar new-dtags:

```make
LDFLAGS += -Wl,--disable-new-dtags
```

2. Garanta o rpath para o diretório local:

```make
LDFLAGS += -Wl,-rpath,$(WX_LIB_PATH)
```

3. (Opcional, útil se um dia colocar as libs ao lado do binário)

```make
# Se futuramente copiar as .so para o mesmo diretório do app:
LDFLAGS += -Wl,-rpath,'$$ORIGIN'
```

Seu bloco pode ficar assim:

```make
ifeq ($(RPATH),1)
  LDFLAGS     ?= -Wl,-rpath,$(WX_LIB_PATH) -Wl,--disable-new-dtags
else
  LDFLAGS     ?=
endif
```

Recompile:

```bash
make clean
RPATH=1 EMBED_XRC=1 make
./app
```

### Dicas de diagnóstico (se quiser conferir)

* Ver quais libs foram resolvidas:

  ```bash
  ldd ./app | grep wx
  ```

  (todas devem apontar para `~/wx/linux-wx-3.2.4/lib`)

* Ver se gravou RPATH ou RUNPATH:

  ```bash
  readelf -d ./app | grep -E 'RPATH|RUNPATH'
  ```

### Alternativas

* **Sem rebuild**: ajustar RPATH pós-build

  ```bash
  patchelf --set-rpath "$HOME/wx/linux-wx-3.2.4/lib" ./app
  ```
* **Garantir tudo estático (quando possível)**: construir wx **estático** e linkar estaticamente (elimina dor de cabeça com .so), mas aumenta o tamanho do binário.

> Observação: o `-DwxDEBUG_LEVEL=0` nos seus CXXFLAGS não é o culpado aqui. O problema é mesmo a mistura de versões/instalações em runtime.

