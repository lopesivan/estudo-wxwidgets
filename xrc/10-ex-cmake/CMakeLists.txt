# ==============================================================================
# CONFIGURAÇÃO BÁSICA DO PROJETO
# ==============================================================================
cmake_minimum_required(VERSION 3.16)
project(wxWidgets_XRC_App LANGUAGES CXX)

# Padrões do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opções de build para debug
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==============================================================================
# OPÇÕES CONFIGURÁVEIS DO USUÁRIO
# ==============================================================================

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

# ==============================================================================
# DETECÇÃO DO WXWIDGETS DO SISTEMA
# ==============================================================================

# Procura wx-config no PATH do sistema
find_program(
  WX_CONFIG_EXECUTABLE
  NAMES wx-config
  DOC "wx-config do sistema")

if(NOT WX_CONFIG_EXECUTABLE)
  message(
    FATAL_ERROR
      "wx-config não encontrado no sistema. Instale wxWidgets (ex: sudo apt install libwxgtk3.2-dev)"
  )
endif()

message(STATUS "========================================")
message(STATUS "Configuração do wxWidgets:")
message(STATUS "========================================")
message(STATUS "wx-config encontrado: ${WX_CONFIG_EXECUTABLE}")

# ==============================================================================
# OBTENÇÃO DAS FLAGS DO WXWIDGETS VIA WX-CONFIG
# ==============================================================================

# Obter CXXFLAGS do wx-config
execute_process(
  COMMAND ${WX_CONFIG_EXECUTABLE} --cxxflags
  OUTPUT_VARIABLE WX_CXXFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
  RESULT_VARIABLE WX_CONFIG_RESULT)

if(NOT WX_CONFIG_RESULT EQUAL 0)
  message(FATAL_ERROR "Falha ao executar wx-config --cxxflags")
endif()

# Obter LDFLAGS (libs) do wx-config
execute_process(
  COMMAND ${WX_CONFIG_EXECUTABLE} --libs std,xrc
  OUTPUT_VARIABLE WX_LDLIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE
  RESULT_VARIABLE WX_LIBS_RESULT)

if(NOT WX_LIBS_RESULT EQUAL 0)
  message(FATAL_ERROR "Falha ao executar wx-config --libs")
endif()

# Obter versão do wxWidgets
execute_process(
  COMMAND ${WX_CONFIG_EXECUTABLE} --version
  OUTPUT_VARIABLE WX_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "Versão do wxWidgets: ${WX_VERSION}")
message(STATUS "WX_CXXFLAGS: ${WX_CXXFLAGS}")
message(STATUS "WX_LDLIBS: ${WX_LDLIBS}")

# ==============================================================================
# COLETA DOS ARQUIVOS FONTE
# ==============================================================================

# Detecta todos os .cpp em src/
file(GLOB BASE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Remove ui_xrc.cpp da lista (será adicionado condicionalmente)
set(XRC_CPP "${CMAKE_CURRENT_SOURCE_DIR}/src/ui_xrc.cpp")
list(FILTER BASE_SOURCES EXCLUDE REGEX ".*ui_xrc\\.cpp$")

# Lista final de fontes
set(SOURCES ${BASE_SOURCES})

# ==============================================================================
# PROCESSAMENTO DO XRC (EMBED OU CÓPIA)
# ==============================================================================

if(EMBED_XRC)
  # Modo EMBED: Gera ui_xrc.cpp com wxrc

  # Procura wxrc no PATH do sistema
  find_program(
    WXRC_EXECUTABLE
    NAMES wxrc
    DOC "wxrc do sistema")

  if(NOT WXRC_EXECUTABLE)
    message(
      FATAL_ERROR "wxrc não encontrado no sistema. Necessário para EMBED_XRC=ON"
    )
  endif()

  if(NOT EXISTS "${XRC_FILE}")
    message(FATAL_ERROR "Arquivo XRC não encontrado: ${XRC_FILE}")
  endif()

  message(STATUS "EMBED_XRC = ON → Gerando ${XRC_CPP} com wxrc")
  message(STATUS "wxrc encontrado: ${WXRC_EXECUTABLE}")

  add_custom_command(
    OUTPUT "${XRC_CPP}"
    COMMAND "${WXRC_EXECUTABLE}" "${XRC_FILE}" -c -o "${XRC_CPP}"
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

# ==============================================================================
# CRIAÇÃO DO EXECUTÁVEL
# ==============================================================================

add_executable(${PROGRAM_NAME} ${SOURCES})

# ==============================================================================
# CONFIGURAÇÃO DE COMPILAÇÃO
# ==============================================================================

# Separar as flags do wx-config em componentes individuais
separate_arguments(WX_CXXFLAGS_LIST UNIX_COMMAND "${WX_CXXFLAGS}")
target_compile_options(${PROGRAM_NAME} PRIVATE ${WX_CXXFLAGS_LIST})

# Modo Debug ou Release
if(DEBUG_BUILD)
  target_compile_options(${PROGRAM_NAME} PRIVATE -O0 -g)
else()
  target_compile_options(${PROGRAM_NAME} PRIVATE -O3)
endif()

# ==============================================================================
# CONFIGURAÇÃO DE LINKAGEM
# ==============================================================================

# Separar as flags de link do wx-config
separate_arguments(WX_LDLIBS_LIST UNIX_COMMAND "${WX_LDLIBS}")

# Adicionar as bibliotecas
target_link_libraries(${PROGRAM_NAME} PRIVATE ${WX_LDLIBS_LIST})

# ==============================================================================
# CÓPIA DO ARQUIVO XRC PARA O DIRETÓRIO DO EXECUTÁVEL
# ==============================================================================

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

# ==============================================================================
# ALVOS PERSONALIZADOS
# ==============================================================================

# Alvo 'run': Executa o programa
add_custom_target(
  run
  COMMAND $<TARGET_FILE:${PROGRAM_NAME}>
  DEPENDS ${PROGRAM_NAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Executando ${PROGRAM_NAME}"
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

# ==============================================================================
# MENSAGENS DE DIAGNÓSTICO FINAL
# ==============================================================================

message(STATUS "========================================")
message(STATUS "Configuração do Build:")
message(STATUS "========================================")
message(STATUS "PROGRAM_NAME = ${PROGRAM_NAME}")
message(STATUS "EMBED_XRC = ${EMBED_XRC}")
message(STATUS "DEBUG_BUILD = ${DEBUG_BUILD}")
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
message(STATUS "  make run       - Executa o programa")
message(STATUS "  make ldd       - Mostra dependências dinâmicas")
message(STATUS "  make readelf   - Verifica RPATH/RUNPATH")
message(STATUS "========================================")
