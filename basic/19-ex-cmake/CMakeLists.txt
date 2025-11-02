cmake_minimum_required(VERSION 3.16)
project(app) # Nome do projeto, pode ser alterado com -DPROGRAM=nome

# =========================
# Configurações do projeto
# =========================
set(PROGRAM
    "app"
    CACHE STRING "Nome do executável")
set(SRC_DIR
    "."
    CACHE PATH "Diretório dos fontes")
set(BIN_DIR
    "bin"
    CACHE PATH "Diretório de saída dos binários")

# =========================
# Configuração do wxWidgets
# =========================
set(V
    "3.3.1"
    CACHE STRING "Versão do wxWidgets")
set(WX_BASE_DIR
    "$ENV{HOME}/wx"
    CACHE PATH "Diretório base do wxWidgets")
set(WX_INSTALL_DIR
    "${WX_BASE_DIR}/linux-wx-${V}"
    CACHE PATH "Diretório de instalação do wxWidgets")
set(WX_CONFIG
    "${WX_INSTALL_DIR}/bin/wx-config"
    CACHE FILEPATH "Caminho para wx-config")

# Opções do wx-config
set(WX_STATIC
    "no"
    CACHE STRING "Build estático (yes/no)")
set(WX_CONFIG_OPT "")
if(WX_STATIC STREQUAL "yes")
  set(WX_CONFIG_OPT "--static=yes")
endif()

# =========================
# Opções de build
# =========================
set(DEBUG
    0
    CACHE BOOL "Build de debug")
set(RPATH
    1
    CACHE BOOL "Usar RPATH")
set(WARNINGS
    "-Wall -Wextra -Wpedantic"
    CACHE STRING "Flags de warning")

# =========================
# Configuração do compilador
# =========================
# Padrões do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opções de build para debug
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Diretório de saída
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# =========================
# Obter flags do wx-config
# =========================
if(EXISTS ${WX_CONFIG})
  # Obter CXX do wx-config
  execute_process(
    COMMAND ${WX_CONFIG} --cxx
    OUTPUT_VARIABLE WX_CXX_COMPILER
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  # Obter flags de compilação
  execute_process(
    COMMAND ${WX_CONFIG} --cxxflags ${WX_CONFIG_OPT}
    OUTPUT_VARIABLE WX_CXXFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  # Obter flags de link
  execute_process(
    COMMAND ${WX_CONFIG} --libs core,base ${WX_CONFIG_OPT}
    OUTPUT_VARIABLE WX_LDLIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  # Configurar compilador se encontrado
  if(WX_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER ${WX_CXX_COMPILER})
  endif()
else()
  message(FATAL_ERROR "wx-config não encontrado em: ${WX_CONFIG}")
endif()

# =========================
# Configuração de flags
# =========================
if(DEBUG)
  set(CMAKE_BUILD_TYPE Debug)
  set(OPTIMIZE "-O0 -g")
else()
  set(CMAKE_BUILD_TYPE Release)
  set(OPTIMIZE "-O3")
endif()

# Flags de compilação
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WX_CXXFLAGS} ${WARNINGS} ${OPTIMIZE}")

# RPATH
if(RPATH)
  set(CMAKE_BUILD_RPATH ${WX_INSTALL_DIR}/lib)
  set(CMAKE_INSTALL_RPATH ${WX_INSTALL_DIR}/lib)
endif()

# =========================
# Executável
# =========================
# Encontrar todos os arquivos .cpp
file(GLOB SRCS "${SRC_DIR}/*.cpp")

if(NOT SRCS)
  message(FATAL_ERROR "Nenhum arquivo .cpp encontrado em: ${SRC_DIR}")
endif()

add_executable(${PROGRAM} ${SRCS})

# Flags de link
target_link_libraries(${PROGRAM} ${WX_LDLIBS})

# =========================
# Copiar recursos (imagens, etc)
# =========================
# Inclui o módulo e gera a "ball.png" padrão
include(cmake/GenerateBalls.cmake)
# Método A
# Personalize por linha de comando,
# ex: -DBALL_COLOR=255,0,0 -DBALL_SIZE_PX=192

# Método B
# gen_ball(ball_blue  "30,144,255" 128)
# gen_ball(ball_red   "220,20,60" 128)
# gen_ball(ball_green "46,139,87" 128)
# Você pode ainda chamar generate_default_ball() se quiser "ball.png" também
generate_default_ball()

# =========================
# Targets customizados
# =========================
# Target run
add_custom_target(
  run
  COMMAND ./${PROGRAM}
  DEPENDS ${PROGRAM}
  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  COMMENT "Executando ${PROGRAM}")

# Target info
add_custom_target(
  info
  COMMAND ${CMAKE_COMMAND} -E echo "PROGRAM    = ${PROGRAM}"
  COMMAND ${CMAKE_COMMAND} -E echo "SRCS       = ${SRCS}"
  COMMAND ${CMAKE_COMMAND} -E echo "CXX        = ${CMAKE_CXX_COMPILER}"
  COMMAND ${CMAKE_COMMAND} -E echo "WX_CONFIG  = ${WX_CONFIG}"
  COMMAND ${CMAKE_COMMAND} -E echo "CXXFLAGS   = ${CMAKE_CXX_FLAGS}"
  COMMAND ${CMAKE_COMMAND} -E echo "LDLIBS     = ${WX_LDLIBS}"
  COMMENT "Mostrando informações do build")

# =========================
# Configurações de instalação (opcional)
# =========================
# install(TARGETS ${PROGRAM} DESTINATION bin)

# =========================
# Mensagem de status
# =========================
message(STATUS "=== Configuração do Projeto ===")
message(STATUS "Programa: ${PROGRAM}")
message(STATUS "Fontes: ${SRCS}")
message(STATUS "wxWidgets: ${WX_INSTALL_DIR}")
message(STATUS "Modo: ${CMAKE_BUILD_TYPE}")
message(STATUS "RPATH: ${RPATH}")
message(STATUS "================================")
