cmake_minimum_required(VERSION 3.14)
project(app LANGUAGES CXX)

# Padrões do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opções de build para debug
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Allegro
find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED allegro-5 allegro_primitives-5
                  allegro_font-5 allegro_ttf-5)

add_executable(
  app
  ui/App.cpp
  ui/RenderView.hpp
  ui/AppFrame.hpp
  core/IRenderBackend.hpp
  core/RenderModel.hpp
  core/BackendFactory.hpp
  render/AllegroBackend.hpp
  render/AllegroBackend.cpp)

target_include_directories(app PRIVATE ${ALLEGRO_INCLUDE_DIRS})

target_link_libraries(app ${wxWidgets_LIBRARIES} ${ALLEGRO_LIBRARIES})
