Aqui está um CMakeLists.txt equivalente ao seu Makefile:## Como usar:

**Build básico:**
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

**Build com configurações customizadas:**
```bash
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DWX_VERSION=3.3.1 \
      -DWX_OS=linux \
      -DUSE_RPATH=ON \
      ..
```

**Executar com Wine:**
```bash
cmake --build . --target run
```

**Copiar apenas as DLLs:**
```bash
cmake --build . --target copy_dlls
```

**Limpar:**
```bash
cmake --build . --target clean
# ou simplesmente deletar a pasta build
rm -rf build
```

## Principais diferenças do Makefile:

1. **Variáveis cache**: podem ser sobrescritas via `-D` no cmake
2. **Detecção automática**: CMake gerencia dependências automaticamente
3. **Multiplataforma**: funciona melhor em diferentes sistemas
4. **Build out-of-source**: mantém código fonte limpo

## Opções configuráveis:

- `WX_VERSION`: versão do wxWidgets
- `WX_OS`: sistema operacional alvo
- `WX_STATIC`: YES/NO para linking estático
- `USE_RPATH`: ON/OFF para RPATH
- `CMAKE_BUILD_TYPE`: Debug ou Release
- `MINGW_DLL_PATH`: caminho das DLLs do MinGW
