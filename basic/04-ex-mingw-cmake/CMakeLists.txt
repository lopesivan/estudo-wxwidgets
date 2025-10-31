cmake_minimum_required(VERSION 3.15)
project(WxWidgetsApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opções de build para debug
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================
# Configuração do wxWidgets
# =========================
set(WX_VERSION
    "3.3.1"
    CACHE STRING "wxWidgets version")
set(WX_OS
    "windows"
    CACHE STRING "OS type")
set(WX_BASE_DIR
    "$ENV{HOME}/wx"
    CACHE PATH "wxWidgets base directory")
set(WX_SRC_DIR
    "${WX_BASE_DIR}/wxWidgets-${WX_VERSION}-${WX_OS}"
    CACHE PATH "wxWidgets source directory")
set(WX_INSTALL_DIR
    "${WX_BASE_DIR}/${WX_OS}-wx-${WX_VERSION}"
    CACHE PATH "wxWidgets install directory")
set(WX_CONFIG_PATH
    "${WX_INSTALL_DIR}/bin/wx-config"
    CACHE FILEPATH "Path to wx-config")
set(WX_STATIC
    YES
    CACHE BOOL "Use static wxWidgets libraries")
set(USE_RPATH
    ON
    CACHE BOOL "Use RPATH for library location")

# Verificar se wx-config existe
if(NOT EXISTS ${WX_CONFIG_PATH})
  message(FATAL_ERROR "wx-config not found at: ${WX_CONFIG_PATH}")
endif()

# =========================
# Obter flags do wx-config
# =========================
if(WX_STATIC)
  set(WX_STATIC_FLAG "--static=yes")
else()
  set(WX_STATIC_FLAG "--static=no")
endif()

# Obter CXXFLAGS do wxWidgets
execute_process(
  COMMAND ${WX_CONFIG_PATH} --cppflags
  OUTPUT_VARIABLE WX_CXXFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE)

# Obter LDLIBS do wxWidgets
execute_process(
  COMMAND ${WX_CONFIG_PATH} ${WX_STATIC_FLAG} --libs
  OUTPUT_VARIABLE WX_LDLIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE)

# Separar flags em listas
string(REPLACE " " ";" WX_CXXFLAGS_LIST ${WX_CXXFLAGS})
string(REPLACE " " ";" WX_LDLIBS_LIST ${WX_LDLIBS})

# =========================
# Coletar arquivos fonte
# =========================
file(GLOB SOURCES "*.cpp")

if(NOT SOURCES)
  message(FATAL_ERROR "No .cpp files found in current directory")
endif()

# Criar executável para cada .cpp (como no Makefile original)
foreach(SOURCE_FILE ${SOURCES})
  get_filename_component(PROGRAM_NAME ${SOURCE_FILE} NAME_WE)

  add_executable(${PROGRAM_NAME} ${SOURCE_FILE})

  # Adicionar flags do wxWidgets
  target_compile_options(${PROGRAM_NAME} PRIVATE ${WX_CXXFLAGS_LIST})

  # Adicionar libraries do wxWidgets
  target_link_libraries(${PROGRAM_NAME} PRIVATE ${WX_LDLIBS_LIST})

  # Configurar RPATH se habilitado
  if(USE_RPATH)
    set_target_properties(
      ${PROGRAM_NAME} PROPERTIES BUILD_RPATH "${WX_INSTALL_DIR}/lib"
                                 INSTALL_RPATH "${WX_INSTALL_DIR}/lib")
  endif()

  # Flags de otimização/debug
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROGRAM_NAME} PRIVATE -O0 -g)
  else()
    target_compile_options(${PROGRAM_NAME} PRIVATE -O3)
  endif()

  # Warnings opcionais (descomente se desejar)
  # target_compile_options(${PROGRAM_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endforeach()

# =========================
# Target para copiar DLLs (MinGW cross-compile)
# =========================
set(MINGW_DLL_PATH
    "/usr/lib/gcc/x86_64-w64-mingw32/13-win32"
    CACHE PATH "MinGW DLL path")

add_custom_target(
  copy_dlls
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${MINGW_DLL_PATH}/libgcc_s_seh-1.dll ${CMAKE_CURRENT_BINARY_DIR}/
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${MINGW_DLL_PATH}/libstdc++-6.dll ${CMAKE_CURRENT_BINARY_DIR}/
  COMMENT "Copying MinGW DLLs")

# =========================
# Target customizado para run com Wine
# =========================
add_custom_target(
  run
  COMMAND wine ./${PROGRAM_NAME}.exe
  DEPENDS ${PROGRAM_NAME} copy_dlls
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Running program with Wine")

# =========================
# Informações de build
# =========================
message(STATUS "wxWidgets version: ${WX_VERSION}")
message(STATUS "wxWidgets install dir: ${WX_INSTALL_DIR}")
message(STATUS "wx-config path: ${WX_CONFIG_PATH}")
message(STATUS "Static linking: ${WX_STATIC}")
message(STATUS "Use RPATH: ${USE_RPATH}")
message(STATUS "Found sources: ${SOURCES}")
