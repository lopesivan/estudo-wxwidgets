cmake_minimum_required(VERSION 3.16)
project(HelloWx_ExistingInstall LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Componentes que seu app precisa
set(WX_COMPONENTS base core xrc)

# --- Escolha automática do diretório raiz do wxWidgets por SO ---
# Você pode sobrescrever com: -DWX_ROOT=/caminho/personalizado
if(NOT DEFINED WX_ROOT)
  if(WIN32)
    set(WX_ROOT "C:/wxwidgets-3.2.4")
  else()
    # ~ expande para $ENV{HOME} de forma portátil:
    set(WX_ROOT "$ENV{HOME}/wx/linux-wx-3.2.4")
  endif()
endif()

message(STATUS "Tentando usar wxWidgets em: ${WX_ROOT}")

# --- Tentativa 1: modo CONFIG (wxWidgetsConfig.cmake) ---
# Normalmente fica em: <prefix>/lib/cmake/wxWidgets Você pode forçar com:
# -DwxWidgets_DIR=".../lib/cmake/wxWidgets"
if(NOT DEFINED wxWidgets_DIR)
  set(_guess_cfg "${WX_ROOT}/lib/cmake/wxWidgets")
  if(EXISTS "${_guess_cfg}/wxWidgetsConfig.cmake")
    set(wxWidgets_DIR
        "${_guess_cfg}"
        CACHE PATH "wxWidgets CMake config dir" FORCE)
  endif()
endif()

# Ajuda o CMake a encontrar pacotes instalados em WX_ROOT
list(APPEND CMAKE_PREFIX_PATH "${WX_ROOT}")

# Tenta CONFIG silenciosamente primeiro
set(_WX_FOUND_CONFIG FALSE)
find_package(wxWidgets QUIET CONFIG COMPONENTS ${WX_COMPONENTS})
if(wxWidgets_FOUND)
  set(_WX_FOUND_CONFIG TRUE)
  message(STATUS "wxWidgets encontrado via CONFIG em: ${wxWidgets_DIR}")
else()
  message(
    STATUS "wxWidgets (CONFIG) não encontrado, tentando módulo FindwxWidgets..."
  )
endif()

# --- Tentativa 2: módulo FindwxWidgets (usa wx-config no Linux) ---
# Permite sobrescrever o wx-config:
# -DWX_CONFIG_EXECUTABLE=/caminho/para/wx-config
if(NOT _WX_FOUND_CONFIG)
  if(NOT DEFINED WX_CONFIG_EXECUTABLE)
    # guesses comuns de wx-config no Linux
    if(EXISTS "${WX_ROOT}/bin/wx-config")
      set(WX_CONFIG_EXECUTABLE "${WX_ROOT}/bin/wx-config")
    elseif(EXISTS "${WX_ROOT}/lib/wx/config/gtk3-unicode-3.2")
      # alguns distros guardam scripts aqui; ajuste se necessário
      set(WX_CONFIG_EXECUTABLE "${WX_ROOT}/lib/wx/config/gtk3-unicode-3.2")
    endif()
  endif()

  if(DEFINED WX_CONFIG_EXECUTABLE)
    set(ENV{WX_CONFIG} "${WX_CONFIG_EXECUTABLE}") # alguns FindwxWidgets
                                                  # respeitam isso
    set(wxWidgets_CONFIG_EXECUTABLE
        "${WX_CONFIG_EXECUTABLE}"
        CACHE FILEPATH "wx-config path" FORCE)
    message(STATUS "Usando wx-config explícito: ${WX_CONFIG_EXECUTABLE}")
  endif()

  # Agora tente o módulo "clássico"
  find_package(wxWidgets REQUIRED COMPONENTS ${WX_COMPONENTS})
  message(
    STATUS
      "wxWidgets encontrado via módulo FindwxWidgets. Versão: ${wxWidgets_VERSION}"
  )
endif()

# --- Alvo da aplicação ---
add_executable(hello_app src/main.cpp)

# Link de acordo com o modo encontrado
if(_WX_FOUND_CONFIG)
  # CONFIG mode: targets modernos
  target_link_libraries(hello_app PRIVATE wx::base wx::core wx::xrc)
else()
  # Module mode: use as variáveis do FindwxWidgets
  if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
  endif()
  target_link_libraries(hello_app PRIVATE ${wxWidgets_LIBRARIES})
  target_include_directories(hello_app PRIVATE ${wxWidgets_INCLUDE_DIRS})
  # (Opcional) compile defs/flags vindos do módulo
  if(wxWidgets_DEFINITIONS)
    target_compile_definitions(hello_app PRIVATE ${wxWidgets_DEFINITIONS})
  endif()
  if(wxWidgets_CXX_FLAGS)
    target_compile_options(hello_app PRIVATE ${wxWidgets_CXX_FLAGS})
  endif()
endif()

# Warnings e propriedades úteis
if(MSVC)
  target_compile_options(hello_app PRIVATE /W4 /permissive-)
else()
  target_compile_options(hello_app PRIVATE -Wall -Wextra -Wpedantic)
endif()

# App GUI no Windows (sem console). Remova se quiser console.
if(WIN32)
  set_target_properties(hello_app PROPERTIES WIN32_EXECUTABLE YES)
endif()

# Mensagens de diagnóstico
message(STATUS "WX_ROOT='${WX_ROOT}'")
if(DEFINED wxWidgets_DIR)
  message(STATUS "wxWidgets_DIR='${wxWidgets_DIR}'")
endif()
if(DEFINED WX_CONFIG_EXECUTABLE)
  message(STATUS "WX_CONFIG_EXECUTABLE='${WX_CONFIG_EXECUTABLE}'")
endif()
