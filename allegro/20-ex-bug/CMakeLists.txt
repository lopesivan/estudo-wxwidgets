cmake_minimum_required(VERSION 3.16)
project(wx_allegro_paint)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------- wxWidgets ----------
find_package(wxWidgets 3.2 REQUIRED COMPONENTS core base gl)
include(${wxWidgets_USE_FILE})

# ---------- Allegro 5 ----------
find_package(Allegro 5.2 REQUIRED COMPONENTS primitives font ttf image)

# ---------- nosso executável ----------
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(
  ${PROJECT_NAME}
  ${wxWidgets_LIBRARIES}
  Allegro::allegro
  Allegro::primitives
  Allegro::font
  Allegro::ttf
  Allegro::image)

# ---------- Windows: subsystem windows ----------
if(WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
