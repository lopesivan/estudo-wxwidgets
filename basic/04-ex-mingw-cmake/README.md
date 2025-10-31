Aqui está um CMakeLists.txt equivalente ao seu Makefile:##
Como usar:

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


Vou ajustar o CMakeLists.txt para usar o compilador
cross-compile do MinGW:Agora o CMake está configurado para
usar o compilador MinGW cross-compile.

## Como usar:

**Build:**
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

**Ou com toolchain file separado (método alternativo mais limpo):**

Crie um arquivo `mingw-w64-toolchain.cmake`:

```cmake
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER /bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER /bin/x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER /bin/x86_64-w64-mingw32-windres)

set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
```

E então use:
```bash
cmake -DCMAKE_TOOLCHAIN_FILE=../mingw-w64-toolchain.cmake ..
```

## Verificar se está usando o compilador correto:

```bash
cd build
cmake .. --debug-output | grep CXX_COMPILER
```

Deve mostrar `/bin/x86_64-w64-mingw32-g++`

Prefere que eu crie também o arquivo de toolchain separado
como artifact?
