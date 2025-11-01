# Opção para usar wxWidgets pré-instalado customizado
set(WX_CUSTOM_INSTALL_DIR
    "$ENV{HOME}/wx/linux-wx-3.2.4"
    CACHE PATH "Diretório customizado do wxWidgets")

set(USE_CUSTOM_WXWIDGETS
    OFF
    CACHE BOOL "Usar wxWidgets customizado ao invés de buscar no sistema")

if(USE_CUSTOM_WXWIDGETS AND EXISTS ${WX_CUSTOM_INSTALL_DIR})
  message(
    STATUS "Using custom wxWidgets installation at ${WX_CUSTOM_INSTALL_DIR}")

  set(WX_CONFIG_EXECUTABLE ${WX_CUSTOM_INSTALL_DIR}/bin/wx-config)

  # Verificar se wx-config existe
  if(NOT EXISTS ${WX_CONFIG_EXECUTABLE})
    message(FATAL_ERROR "wx-config not found at ${WX_CONFIG_EXECUTABLE}")
  endif()

  # Obter flags diretamente do wx-config
  execute_process(
    COMMAND ${WX_CONFIG_EXECUTABLE} --cxxflags
    OUTPUT_VARIABLE wxWidgets_CXX_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  execute_process(
    COMMAND ${WX_CONFIG_EXECUTABLE} --libs
    OUTPUT_VARIABLE wxWidgets_LIBRARIES
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  execute_process(
    COMMAND ${WX_CONFIG_EXECUTABLE} --prefix
    OUTPUT_VARIABLE wxWidgets_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  # Criar target interface
  add_library(wxWidgets_external INTERFACE)

  # Simular variáveis do find_package para compatibilidade
  set(wxWidgets_FOUND
      TRUE
      CACHE BOOL "wxWidgets found")
  set(wxWidgets_ROOT_DIR
      ${WX_CUSTOM_INSTALL_DIR}
      CACHE INTERNAL "wxWidgets installation dir")
  set(ENV_WX_CONFIG
      ${WX_CONFIG_EXECUTABLE}
      CACHE INTERNAL "wx-config dir")

  message(STATUS "wxWidgets CXX FLAGS: ${wxWidgets_CXX_FLAGS}")
  message(STATUS "wxWidgets LIBRARIES: ${wxWidgets_LIBRARIES}")

  # check if wxWidgets is already installed in the system
elseif(NOT USE_CUSTOM_WXWIDGETS)
  find_package(wxWidgets QUIET)
  if(wxWidgets_FOUND)
    message(
      STATUS "Found preinstalled wxWidgets libraries at ${wxWidgets_LIBRARIES}")
    add_library(wxWidgets_external INTERFACE)
  else()
    message(STATUS "Preinstalled wxWidgets not found.")
    message(
      STATUS "Will download and install wxWidgets in ${STAGED_INSTALL_PREFIX}")
    include(ExternalProject)
    ExternalProject_Add(
      wxWidgets_external
      GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
      GIT_TAG v3.2.2
      UPDATE_COMMAND ""
      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
                 -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DwxBUILD_SHARED=OFF
      TEST_AFTER_INSTALL 0
      DOWNLOAD_NO_PROGRESS 1
      LOG_CONFIGURE 1
      LOG_BUILD 1
      LOG_INSTALL 1)
    set(wxWidgets_ROOT_DIR ${STAGED_INSTALL_PREFIX})
    file(TO_NATIVE_PATH "${wxWidgets_ROOT_DIR}" wxWidgets_ROOT_DIR)
    set(wxWidgets_ROOT_DIR
        ${wxWidgets_ROOT_DIR}
        CACHE INTERNAL "wxWidgets installation dir")
    set(ENV_WX_CONFIG ${STAGED_INSTALL_PREFIX}/bin/wx-config)
    file(TO_NATIVE_PATH "${ENV_WX_CONFIG}" ENV_WX_CONFIG)
    set(ENV_WX_CONFIG
        ${ENV_WX_CONFIG}
        CACHE INTERNAL "wx-config dir")
  endif()
endif()
