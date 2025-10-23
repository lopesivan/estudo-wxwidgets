# ============================================================================
# CONFIGURAÇÃO BÁSICA DO PROJETO
# ============================================================================
cmake_minimum_required(VERSION 3.16)
project(wxWidgets_App LANGUAGES CXX)

# Padrões do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opções de build para debug
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# OPÇÕES CONFIGURÁVEIS DO USUÁRIO
# ============================================================================

# Nome do executável
set(PROGRAM_NAME
    "app"
    CACHE STRING "Nome do executável")

# Modo Debug
option(DEBUG_BUILD "Build em modo debug" OFF)

# Usar RPATH para garantir uso das libs locais
option(USE_RPATH "Usar RPATH para libs locais" ON)

# ============================================================================
# DETECÇÃO DO WXWIDGETS LOCAL
# ============================================================================

# Versão e configurações do wxWidgets
set(WX_VERSION
    "3.2.4"
    CACHE STRING "Versão do wxWidgets")
set(WX_OS
    "linux"
    CACHE STRING "Sistema operacional (linux/windows)")

# Diretório base do wxWidgets
if(NOT DEFINED WX_BASE_DIR)
  set(WX_BASE_DIR "$ENV{HOME}/wx")
endif()

# Diretório de instalação do wxWidgets
set(WX_INSTALL_DIR
    "${WX_BASE_DIR}/${WX_OS}-wx-${WX_VERSION}"
    CACHE PATH "Diretório de instalação do wxWidgets")

# Caminhos importantes
set(WX_LIB_PATH "${WX_INSTALL_DIR}/lib")
set(WX_CONFIG "${WX_INSTALL_DIR}/bin/wx-config")

message(STATUS "========================================")
message(STATUS "Configuração do wxWidgets:")
message(STATUS "  WX_INSTALL_DIR = ${WX_INSTALL_DIR}")
message(STATUS "  WX_CONFIG      = ${WX_CONFIG}")
message(STATUS "========================================")

# ============================================================================
# VERIFICAÇÃO DO WX-CONFIG
# ============================================================================

if(NOT EXISTS "${WX_CONFIG}")
  message(FATAL_ERROR "wx-config não encontrado em: ${WX_CONFIG}")
endif()

if(NOT EXISTS "${WX_LIB_PATH}")
  message(
    FATAL_ERROR "Diretório de libs do wxWidgets não encontrado: ${WX_LIB_PATH}")
endif()

# ============================================================================
# OBTENÇÃO DAS FLAGS DO WXWIDGETS VIA WX-CONFIG
# ============================================================================

# Opções do wx-config
set(WX_CONFIG_OPTS "--unicode=yes --static=no --toolkit=gtk3 --version=3.2")

# Obter CXXFLAGS do wx-config
execute_process(
  COMMAND ${WX_CONFIG} --cxxflags ${WX_CONFIG_OPTS}
  OUTPUT_VARIABLE WX_CXXFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
  RESULT_VARIABLE WX_CONFIG_RESULT)
if(NOT WX_CONFIG_RESULT EQUAL 0)
  message(FATAL_ERROR "Falha ao executar wx-config --cxxflags")
endif()

# Obter LDFLAGS (libs) do wx-config
execute_process(
  COMMAND ${WX_CONFIG} ${WX_CONFIG_OPTS} --libs std,core
  OUTPUT_VARIABLE WX_LDLIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE
  RESULT_VARIABLE WX_LIBS_RESULT)
if(NOT WX_LIBS_RESULT EQUAL 0)
  message(FATAL_ERROR "Falha ao executar wx-config --libs")
endif()

message(STATUS "WX_CXXFLAGS = ${WX_CXXFLAGS}")
message(STATUS "WX_LDLIBS   = ${WX_LDLIBS}")

# ============================================================================
# COLETA DOS ARQUIVOS FONTE
# ============================================================================

# Único fonte: src/main.cpp
set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# ============================================================================
# CRIAÇÃO DO EXECUTÁVEL
# ============================================================================

add_executable(${PROGRAM_NAME} ${SOURCES})

# ============================================================================
# CONFIGURAÇÃO DE COMPILAÇÃO
# ============================================================================

# Separe as flags do wx-config
separate_arguments(WX_CXXFLAGS_LIST UNIX_COMMAND "${WX_CXXFLAGS}")
target_compile_options(${PROGRAM_NAME} PRIVATE ${WX_CXXFLAGS_LIST})

# Modo Debug ou Release
if(DEBUG_BUILD)
  target_compile_options(${PROGRAM_NAME} PRIVATE -O0 -g)
else()
  target_compile_options(${PROGRAM_NAME} PRIVATE -O3)
endif()

# ============================================================================
# CONFIGURAÇÃO DE LINKAGEM
# ============================================================================

# Separe as flags de link do wx-config
separate_arguments(WX_LDLIBS_LIST UNIX_COMMAND "${WX_LDLIBS}")

# Adicionar diretório de libs
target_link_directories(${PROGRAM_NAME} PRIVATE "${WX_LIB_PATH}")

# Linkar com as libs do wx-config
target_link_libraries(${PROGRAM_NAME} PRIVATE ${WX_LDLIBS_LIST})

# ============================================================================
# CONFIGURAÇÃO DO RPATH
# ============================================================================

if(USE_RPATH
   AND UNIX
   AND NOT APPLE)
  set_target_properties(
    ${PROGRAM_NAME}
    PROPERTIES BUILD_RPATH "${WX_LIB_PATH}"
               INSTALL_RPATH "${WX_LIB_PATH};\$ORIGIN"
               BUILD_RPATH_USE_ORIGIN TRUE
               INSTALL_RPATH_USE_LINK_PATH FALSE)
  target_link_options(${PROGRAM_NAME} PRIVATE "LINKER:--disable-new-dtags")
  message(STATUS "USE_RPATH = ON → RPATH configurado para ${WX_LIB_PATH}")
else()
  message(STATUS "USE_RPATH = OFF")
endif()

# ============================================================================
# ALVOS PERSONALIZADOS
# ============================================================================

# Alvo 'run': Executa com LD_LIBRARY_PATH ajustado
add_custom_target(
  run
  COMMAND
    ${CMAKE_COMMAND} -E env
    "LD_LIBRARY_PATH=${WX_LIB_PATH}:$ENV{LD_LIBRARY_PATH}"
    $<TARGET_FILE:${PROGRAM_NAME}>
  DEPENDS ${PROGRAM_NAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Executando ${PROGRAM_NAME} com LD_LIBRARY_PATH ajustado")

# Alvo 'ldd': Mostra dependências dinâmicas
add_custom_target(
  ldd
  COMMAND ldd $<TARGET_FILE:${PROGRAM_NAME}> | grep wx || true
  DEPENDS ${PROGRAM_NAME}
  COMMENT "Verificando dependências dinâmicas (ldd)")

# Alvo 'readelf': Verifica RPATH/RUNPATH
add_custom_target(
  readelf
  COMMAND readelf -d $<TARGET_FILE:${PROGRAM_NAME}> | grep -E "(RPATH|RUNPATH)"
          || true
  DEPENDS ${PROGRAM_NAME}
  COMMENT "Verificando RPATH/RUNPATH (readelf)")

# ============================================================================
# MENSAGENS DE DIAGNÓSTICO FINAL
# ============================================================================

message(STATUS "========================================")
message(STATUS "Configuração do Build:")
message(STATUS "  PROGRAM_NAME = ${PROGRAM_NAME}")
message(STATUS "  DEBUG_BUILD  = ${DEBUG_BUILD}")
message(STATUS "  USE_RPATH    = ${USE_RPATH}")
message(STATUS "  Fonte única  = ${SOURCES}")
message(STATUS "Alvos disponíveis:")
message(STATUS "  make           - compila o projeto")
message(STATUS "  make run       - executa o programa")
message(STATUS "  make ldd       - mostra dependências dinâmicas")
message(STATUS "  make readelf   - verifica RPATH/RUNPATH")
message(STATUS "========================================")
