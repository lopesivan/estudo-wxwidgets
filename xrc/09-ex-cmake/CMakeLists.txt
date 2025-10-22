cmake_minimum_required(VERSION 3.16)
project(HelloWx_ExistingInstall LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================================
# Opções
# =========================================
# XRC_LINK: linkar a lib xrc (necessário quando usa wxXmlResource) XRC_EMBED:
# embutir .xrc com wxrc (gera resource.cpp)
option(XRC_LINK "Linkar wxXRC (necessário se usa wxXmlResource)" ON)
set(XRC_EMBED
    "AUTO"
    CACHE STRING "Embutir .xrc com wxrc (ON/OFF/AUTO)")
set_property(CACHE XRC_EMBED PROPERTY STRINGS ON OFF AUTO)

# Seus .xrc estão em src/
set(XRC_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    CACHE PATH "Directory with .xrc files")
file(GLOB XRC_FILES CONFIGURE_DEPENDS "${XRC_DIR}/*.xrc")

# =========================================
# Raiz do wx por SO (pode sobrescrever com -DWX_ROOT=...)
# =========================================
if(NOT DEFINED WX_ROOT)
  if(WIN32)
    set(WX_ROOT "C:/wxwidgets-3.2.4")
  else()
    set(WX_ROOT "$ENV{HOME}/wx/linux-wxwidgets-3.2.4")
  endif()
endif()
message(STATUS "Tentando usar wxWidgets em: ${WX_ROOT}")

# =========================================
# Tenta CONFIG primeiro
# =========================================
if(NOT DEFINED wxWidgets_DIR)
  set(_guess_cfg "${WX_ROOT}/lib/cmake/wxWidgets")
  if(EXISTS "${_guess_cfg}/wxWidgetsConfig.cmake")
    set(wxWidgets_DIR
        "${_guess_cfg}"
        CACHE PATH "wxWidgets CMake config dir" FORCE)
  endif()
endif()
list(APPEND CMAKE_PREFIX_PATH "${WX_ROOT}")

# Sempre peça base/core; se XRC_LINK estiver ON, peça xrc também
set(_WX_COMPONENTS base core)
if(XRC_LINK)
  list(APPEND _WX_COMPONENTS xrc)
endif()

set(_WX_FOUND_CONFIG FALSE)
find_package(wxWidgets QUIET CONFIG COMPONENTS ${_WX_COMPONENTS})
if(wxWidgets_FOUND)
  set(_WX_FOUND_CONFIG TRUE)
  message(STATUS "wxWidgets encontrado via CONFIG em: ${wxWidgets_DIR}")
else()
  message(
    STATUS "wxWidgets (CONFIG) não encontrado, tentando módulo FindwxWidgets..."
  )
endif()

# =========================================
# Fallback: módulo FindwxWidgets (wx-config)
# =========================================
if(NOT _WX_FOUND_CONFIG)
  if(NOT DEFINED WX_CONFIG_EXECUTABLE)
    if(EXISTS "${WX_ROOT}/bin/wx-config")
      set(WX_CONFIG_EXECUTABLE "${WX_ROOT}/bin/wx-config")
    elseif(EXISTS "${WX_ROOT}/lib/wx/config/gtk3-unicode-3.2")
      set(WX_CONFIG_EXECUTABLE "${WX_ROOT}/lib/wx/config/gtk3-unicode-3.2")
    endif()
  endif()

  if(DEFINED WX_CONFIG_EXECUTABLE)
    set(ENV{WX_CONFIG} "${WX_CONFIG_EXECUTABLE}")
    set(wxWidgets_CONFIG_EXECUTABLE
        "${WX_CONFIG_EXECUTABLE}"
        CACHE FILEPATH "wx-config path" FORCE)
    message(STATUS "Usando wx-config explícito: ${WX_CONFIG_EXECUTABLE}")
  endif()

  find_package(wxWidgets REQUIRED COMPONENTS ${_WX_COMPONENTS})
  message(
    STATUS
      "wxWidgets encontrado via módulo FindwxWidgets. Versão: ${wxWidgets_VERSION}"
  )
endif()

# =========================================
# Descobrir wxrc (apenas se for EMBED)
# =========================================
set(WXRC_PROGRAM "")
if(TARGET wxrc)
  set(WXRC_PROGRAM "$<TARGET_FILE:wxrc>")
else()
  find_program(
    WXRC_PROGRAM
    NAMES wxrc
    HINTS "${WX_ROOT}/bin")
endif()

# Decisão de EMBED
set(_DO_EMBED FALSE)
if(XRC_EMBED STREQUAL "ON")
  if(XRC_FILES AND WXRC_PROGRAM)
    set(_DO_EMBED TRUE)
  else()
    message(
      WARNING "XRC_EMBED=ON mas não há .xrc ou wxrc; EMBED será ignorado.")
  endif()
elseif(XRC_EMBED STREQUAL "AUTO")
  if(XRC_FILES AND WXRC_PROGRAM)
    set(_DO_EMBED TRUE)
    message(
      STATUS "XRC_EMBED=AUTO → EMBED ativado (arquivos e wxrc encontrados).")
  else()
    message(STATUS "XRC_EMBED=AUTO → EMBED não ativado.")
  endif()
else()
  message(
    STATUS "XRC_EMBED=OFF → sem embed; você deve carregar o .xrc do disco.")
endif()

# =========================================
# Alvo da aplicação
# =========================================
add_executable(hello_app src/main.cpp src/main_frame.cpp src/main_frame.h)

# EMBED (gera resource.cpp) — opcional
if(_DO_EMBED)
  set(RESOURCE_CPP "${CMAKE_CURRENT_BINARY_DIR}/xrc_resource.cpp")
  add_custom_command(
    OUTPUT "${RESOURCE_CPP}"
    COMMAND "${WXRC_PROGRAM}" -c -o "${RESOURCE_CPP}" ${XRC_FILES}
    DEPENDS ${XRC_FILES}
    COMMENT "Compilando XRC (wxrc) → ${RESOURCE_CPP}"
    VERBATIM)
  target_sources(hello_app PRIVATE "${RESOURCE_CPP}")
else()
  # Se não embutir, garanta que o .xrc estará ao lado do binário (cópia simples)
  if(XRC_FILES)
    add_custom_command(
      TARGET hello_app
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${XRC_FILES}
              $<TARGET_FILE_DIR:hello_app>
      COMMENT "Copiando .xrc para a pasta do executável")
  endif()
endif()

# Link com wx — em CONFIG usamos targets modernos; no módulo, variáveis do find
if(_WX_FOUND_CONFIG)
  target_link_libraries(hello_app PRIVATE wx::base wx::core)
  if(XRC_LINK)
    target_link_libraries(hello_app PRIVATE wx::xrc)
  endif()
else()
  if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
  endif()
  target_link_libraries(hello_app PRIVATE ${wxWidgets_LIBRARIES})
  target_include_directories(hello_app PRIVATE ${wxWidgets_INCLUDE_DIRS})
  if(wxWidgets_DEFINITIONS)
    target_compile_definitions(hello_app PRIVATE ${wxWidgets_DEFINITIONS})
  endif()
  if(wxWidgets_CXX_FLAGS)
    target_compile_options(hello_app PRIVATE ${wxWidgets_CXX_FLAGS})
  endif()
endif()

# Warnings/props
if(MSVC)
  target_compile_options(hello_app PRIVATE /W4 /permissive-)
else()
  target_compile_options(hello_app PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
  set_target_properties(hello_app PROPERTIES WIN32_EXECUTABLE YES)
endif()

# Diagnóstico
message(STATUS "WX_ROOT='${WX_ROOT}'")
if(DEFINED wxWidgets_DIR)
  message(STATUS "wxWidgets_DIR='${wxWidgets_DIR}'")
endif()
if(DEFINED WX_CONFIG_EXECUTABLE)
  message(STATUS "WX_CONFIG_EXECUTABLE='${WX_CONFIG_EXECUTABLE}'")
endif()
message(
  STATUS
    "XRC_LINK=${XRC_LINK}  |  XRC_EMBED='${XRC_EMBED}'  → EMBED=${_DO_EMBED}")
message(STATUS "WXRC_PROGRAM='${WXRC_PROGRAM}'")
if(XRC_FILES)
  message(STATUS "XRC_FILES: ${XRC_FILES}")
else()
  message(STATUS "XRC_FILES: (nenhum .xrc encontrado em ${XRC_DIR})")
endif()
