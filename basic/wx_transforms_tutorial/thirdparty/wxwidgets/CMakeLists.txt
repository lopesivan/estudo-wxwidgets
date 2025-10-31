# Opção para usar wxWidgets pré-instalado customizado
set(WX_CUSTOM_INSTALL_DIR
    "$ENV{HOME}/wx/linux-wx-3.3.1"
    CACHE PATH "Diretório customizado do wxWidgets")

set(USE_CUSTOM_WXWIDGETS
    OFF
    CACHE BOOL "Usar wxWidgets customizado ao invés de buscar no sistema")

if(USE_CUSTOM_WXWIDGETS AND EXISTS ${WX_CUSTOM_INSTALL_DIR})
  message(
    STATUS "Using custom wxWidgets installation at ${WX_CUSTOM_INSTALL_DIR}")

  # Configurar caminhos para o FindwxWidgets encontrar
  set(wxWidgets_ROOT_DIR ${WX_CUSTOM_INSTALL_DIR})
  set(wxWidgets_CONFIG_EXECUTABLE ${WX_CUSTOM_INSTALL_DIR}/bin/wx-config)

  # Forçar busca no diretório customizado
  find_package(wxWidgets REQUIRED PATHS ${WX_CUSTOM_INSTALL_DIR}
               NO_DEFAULT_PATH)

  if(wxWidgets_FOUND)
    message(STATUS "Found custom wxWidgets at ${WX_CUSTOM_INSTALL_DIR}")
    add_library(wxWidgets_external INTERFACE)

    # Exportar variáveis para outros CMakeLists
    set(wxWidgets_ROOT_DIR
        ${WX_CUSTOM_INSTALL_DIR}
        CACHE INTERNAL "wxWidgets installation dir")
    set(ENV_WX_CONFIG
        ${WX_CUSTOM_INSTALL_DIR}/bin/wx-config
        CACHE INTERNAL "wx-config dir")
  else()
    message(
      FATAL_ERROR "Custom wxWidgets not found at ${WX_CUSTOM_INSTALL_DIR}")
  endif()

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
