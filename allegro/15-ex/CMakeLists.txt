cmake_minimum_required(VERSION 3.14)
project(app LANGUAGES CXX)

# ----------------------------------------------------------------------
# Configurações gerais
# ----------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ----------------------------------------------------------------------
# wxWidgets
# ----------------------------------------------------------------------
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# ----------------------------------------------------------------------
# Allegro (via pkg-config)
# ----------------------------------------------------------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED allegro-5 allegro_primitives-5
                  allegro_font-5 allegro_ttf-5)

# ----------------------------------------------------------------------
# Arquivos do projeto
# ----------------------------------------------------------------------
add_executable(
  app
  ui/App.cpp
  ui/RenderView.hpp
  ui/AppFrame.hpp
  render/AllegroBackend.cpp
  render/AllegroBackend.hpp
  core/BackendFactory.cpp
  core/BackendFactory.hpp
  core/IRenderBackend.hpp
  core/RenderModel.hpp)
# ----------------------------------------------------------------------
# Includes e linkagem
# ----------------------------------------------------------------------
target_include_directories(
  app PRIVATE ${ALLEGRO_INCLUDE_DIRS} ${wxWidgets_INCLUDE_DIRS}
              ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(app PRIVATE ${wxWidgets_LIBRARIES} ${ALLEGRO_LIBRARIES})

# ----------------------------------------------------------------------
# Otimização opcional de build (remova se quiser ver os warnings)
# ----------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(app PRIVATE -O2)
else()
  target_compile_options(app PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

# ----------------------------------------------------------------------
# Pós-build: copiar a fonte TTF para o diretório do executável
# ----------------------------------------------------------------------
add_custom_command(
  TARGET app
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts/SauceCodeProNerdFontMono-Regular.ttf
    $<TARGET_FILE_DIR:app>)
