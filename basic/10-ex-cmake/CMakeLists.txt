# ==============================================================================
# CONFIGURAÇÃO BÁSICA DO PROJETO (SEM XRC)
# ==============================================================================
cmake_minimum_required(VERSION 3.16)
project(HelloWx LANGUAGES CXX)

# ----------------------------------------
# Padrões de C++
# ----------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------------
# Opções de build para debug
# ----------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ----------------------------------------
# Log do build
# ----------------------------------------
message(STATUS "========================================")
message(STATUS "  🔍 Build System Info")
message(
  STATUS "    • OS:           ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")
message(STATUS "    • Arch:         ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "    • Generator:    ${CMAKE_GENERATOR}")
message(
  STATUS
    "    • C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
)
message(STATUS "========================================")

# ==============================================================================
# LOCALIZAÇÃO E LINKAGEM DO WXWIDGETS
# ==============================================================================

# Procura o pacote wxWidgets (apenas base + core)
find_package(wxWidgets REQUIRED COMPONENTS core base)

# Inclui as variáveis do módulo FindwxWidgets
include(${wxWidgets_USE_FILE})

# ==============================================================================
# FONTES E EXECUTÁVEL
# ==============================================================================

# Só temos um único fonte: src/main.cpp
add_executable(${PROJECT_NAME} src/main.cpp)

# Aplica flags de compilação e define includes
target_compile_options(${PROJECT_NAME} PRIVATE ${wxWidgets_CXX_FLAGS})
target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})

# Linka com as libs do wxWidgets encontradas
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})

# ==============================================================================
# DIAGNÓSTICO FINAL
# ==============================================================================
message(STATUS "→ Projeto:        ${PROJECT_NAME}")
message(STATUS "→ Fonte:          src/main.cpp")
message(STATUS "→ wxWidgets ver:  ${wxWidgets_VERSION}")
message(STATUS "→ wx_INCLUDE_DIR: ${wxWidgets_INCLUDE_DIRS}")
message(STATUS "→ wx_LIBRARIES:   ${wxWidgets_LIBRARIES}")
message(STATUS "========================================")
