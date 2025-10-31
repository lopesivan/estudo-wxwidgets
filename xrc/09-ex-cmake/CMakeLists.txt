# ============================================================================
# CONFIGURAÇÃO BÁSICA DO PROJETO
# ============================================================================
cmake_minimum_required(VERSION 3.16)
project(wxWidgets_XRC_App LANGUAGES CXX)

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

# Modo de embed do XRC
option(EMBED_XRC "Embutir arquivo XRC no executável" OFF)

# Arquivo XRC de entrada
set(XRC_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ui.xrc"
    CACHE FILEPATH "Arquivo XRC")

# Modo Debug
option(DEBUG_BUILD "Build em modo debug" OFF)

# Usar RPATH para garantir uso das libs locais
option(USE_RPATH "Usar RPATH para libs locais" ON)

# ============================================================================
# DETECÇÃO DO WXWIDGETS LOCAL
# ============================================================================

# Versão e configurações do wxWidgets
set(WX_VERSION
    "3.3.1"
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
set(WXRC "${WX_INSTALL_DIR}/bin/wxrc")

message(STATUS "========================================")
message(STATUS "Configuração do wxWidgets:")
message(STATUS "========================================")
message(STATUS "WX_INSTALL_DIR = ${WX_INSTALL_DIR}")
message(STATUS "WX_CONFIG = ${WX_CONFIG}")
message(STATUS "WXRC = ${WXRC}")

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
set(WX_CONFIG_OPTS "--static=no")

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
  COMMAND ${WX_CONFIG} ${WX_CONFIG_OPTS} --libs std,xrc
  OUTPUT_VARIABLE WX_LDLIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE
  RESULT_VARIABLE WX_LIBS_RESULT)

if(NOT WX_LIBS_RESULT EQUAL 0)
  message(FATAL_ERROR "Falha ao executar wx-config --libs")
endif()

message(STATUS "WX_CXXFLAGS = ${WX_CXXFLAGS}")
message(STATUS "WX_LDLIBS = ${WX_LDLIBS}")

# ============================================================================
# COLETA DOS ARQUIVOS FONTE
# ============================================================================

# Detecta todos os .cpp em src/
file(GLOB BASE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Remove ui_xrc.cpp da lista (será adicionado condicionalmente)
set(XRC_CPP "${CMAKE_CURRENT_SOURCE_DIR}/src/ui_xrc.cpp")
list(FILTER BASE_SOURCES EXCLUDE REGEX ".*ui_xrc\\.cpp$")

# Lista final de fontes
set(SOURCES ${BASE_SOURCES})

# ============================================================================
# PROCESSAMENTO DO XRC (EMBED OU CÓPIA)
# ============================================================================

if(EMBED_XRC)
  # Modo EMBED: Gera ui_xrc.cpp com wxrc
  if(NOT EXISTS "${WXRC}")
    message(
      FATAL_ERROR
        "wxrc não encontrado em: ${WXRC}. Necessário para EMBED_XRC=ON")
  endif()

  if(NOT EXISTS "${XRC_FILE}")
    message(FATAL_ERROR "Arquivo XRC não encontrado: ${XRC_FILE}")
  endif()

  message(STATUS "EMBED_XRC = ON → Gerando ${XRC_CPP} com wxrc")

  add_custom_command(
    OUTPUT "${XRC_CPP}"
    COMMAND "${WXRC}" "${XRC_FILE}" -c -o "${XRC_CPP}"
    DEPENDS "${XRC_FILE}"
    COMMENT "Compilando XRC → ${XRC_CPP}"
    VERBATIM)

  # Adiciona o arquivo gerado às fontes
  list(APPEND SOURCES "${XRC_CPP}")

else()
  # Modo SEM EMBED: Copia ui.xrc para o diretório do executável
  message(
    STATUS "EMBED_XRC = OFF → ui.xrc será copiado para pasta do executável")
endif()

# ============================================================================
# CRIAÇÃO DO EXECUTÁVEL
# ============================================================================

add_executable(${PROGRAM_NAME} ${SOURCES})

# ============================================================================
# CONFIGURAÇÃO DE COMPILAÇÃO
# ============================================================================

# Separar as flags do wx-config em componentes individuais
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

# Separar as flags de link do wx-config
separate_arguments(WX_LDLIBS_LIST UNIX_COMMAND "${WX_LDLIBS}")

# Adicionar o diretório de libs
target_link_directories(${PROGRAM_NAME} PRIVATE "${WX_LIB_PATH}")

# Adicionar as bibliotecas
target_link_libraries(${PROGRAM_NAME} PRIVATE ${WX_LDLIBS_LIST})

# ============================================================================
# CONFIGURAÇÃO DO RPATH
# ============================================================================

if(USE_RPATH
   AND UNIX
   AND NOT APPLE)
  # RPATH "clássico" para garantir uso das libs locais em runtime Equivalente a:
  # -Wl,-rpath,$(WX_LIB_PATH) -Wl,-rpath,'$ORIGIN' -Wl,--disable-new-dtags
  set_target_properties(
    ${PROGRAM_NAME}
    PROPERTIES BUILD_RPATH "${WX_LIB_PATH}"
               INSTALL_RPATH "${WX_LIB_PATH};\$ORIGIN"
               BUILD_RPATH_USE_ORIGIN TRUE
               SKIP_BUILD_RPATH FALSE
               BUILD_WITH_INSTALL_RPATH FALSE
               INSTALL_RPATH_USE_LINK_PATH FALSE)

  # Força RPATH em vez de RUNPATH (--disable-new-dtags)
  target_link_options(${PROGRAM_NAME} PRIVATE "LINKER:--disable-new-dtags")

  message(STATUS "USE_RPATH = ON → RPATH configurado para ${WX_LIB_PATH}")
else()
  message(STATUS "USE_RPATH = OFF")
endif()

# ============================================================================
# CÓPIA DO ARQUIVO XRC PARA O DIRETÓRIO DO EXECUTÁVEL
# ============================================================================

# Sempre copia o XRC para o diretório do executável (mesmo se embutido) Isso
# replica o comportamento do Makefile: cp src/ui.xrc .
if(EXISTS "${XRC_FILE}")
  add_custom_command(
    TARGET ${PROGRAM_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${XRC_FILE}"
            $<TARGET_FILE_DIR:${PROGRAM_NAME}>
    COMMENT "Copiando ${XRC_FILE} para pasta do executável")
endif()

# ============================================================================
# ALVOS PERSONALIZADOS (EQUIVALENTES AOS DO MAKEFILE)
# ============================================================================

# Alvo 'run': Executa o programa com LD_LIBRARY_PATH ajustado
add_custom_target(
  run
  COMMAND
    ${CMAKE_COMMAND} -E env
    "LD_LIBRARY_PATH=${WX_LIB_PATH}:$ENV{LD_LIBRARY_PATH}"
    $<TARGET_FILE:${PROGRAM_NAME}>
  DEPENDS ${PROGRAM_NAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Executando ${PROGRAM_NAME} com LD_LIBRARY_PATH ajustado"
  VERBATIM)

# Alvo 'ldd': Mostra as dependências dinâmicas
add_custom_target(
  ldd
  COMMAND ldd $<TARGET_FILE:${PROGRAM_NAME}> | grep wx || true
  DEPENDS ${PROGRAM_NAME}
  COMMENT "Verificando dependências dinâmicas (ldd)"
  VERBATIM)

# Alvo 'readelf': Verifica RPATH/RUNPATH
add_custom_target(
  readelf
  COMMAND readelf -d $<TARGET_FILE:${PROGRAM_NAME}> | grep -E "(RPATH|RUNPATH)"
          || true
  DEPENDS ${PROGRAM_NAME}
  COMMENT "Verificando RPATH/RUNPATH (readelf)"
  VERBATIM)

# ============================================================================
# MENSAGENS DE DIAGNÓSTICO FINAL
# ============================================================================

message(STATUS "========================================")
message(STATUS "Configuração do Build:")
message(STATUS "========================================")
message(STATUS "PROGRAM_NAME = ${PROGRAM_NAME}")
message(STATUS "EMBED_XRC = ${EMBED_XRC}")
message(STATUS "DEBUG_BUILD = ${DEBUG_BUILD}")
message(STATUS "USE_RPATH = ${USE_RPATH}")
message(STATUS "XRC_FILE = ${XRC_FILE}")

if(EMBED_XRC)
  message(STATUS "XRC_CPP = ${XRC_CPP} (será gerado)")
else()
  message(STATUS "XRC será copiado para pasta do executável")
endif()

message(STATUS "")
message(STATUS "Fontes do projeto:")
foreach(src ${SOURCES})
  message(STATUS "  - ${src}")
endforeach()

message(STATUS "")
message(STATUS "Alvos disponíveis:")
message(STATUS "  make           - Compila o projeto")
message(STATUS "  make run       - Executa com LD_LIBRARY_PATH ajustado")
message(STATUS "  make ldd       - Mostra dependências dinâmicas")
message(STATUS "  make readelf   - Verifica RPATH/RUNPATH")
message(STATUS "========================================")
