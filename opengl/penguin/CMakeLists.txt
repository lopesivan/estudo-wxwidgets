cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(PROGRAM_NAME
    "penguin"
    CACHE STRING "Nome do executável")

project(${PROGRAM_NAME} LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ----------------------------------------------------------------------------
# wxWidgets — instalação local via awx
# ----------------------------------------------------------------------------
set(V
    "3.3.1"
    CACHE STRING "Versão do wxWidgets")
set(wxWidgets_ROOT_DIR "$ENV{HOME}/.local/wxwidgets/linux-cmake-wx-${V}")
set(wxWidgets_CONFIG_EXECUTABLE "${wxWidgets_ROOT_DIR}/bin/wx-config")

find_package(wxWidgets 3.3 REQUIRED COMPONENTS core base gl)
find_package(OpenGL REQUIRED)

if(wxWidgets_USE_FILE)
  include(${wxWidgets_USE_FILE})
endif()

# ----------------------------------------------------------------------------
set(SRCS penguin.cpp dxfrenderer.cpp trackball.c)

add_executable(${PROGRAM_NAME} ${SRCS})
target_link_libraries(${PROGRAM_NAME} PRIVATE ${wxWidgets_LIBRARIES} OpenGL::GL
                                              OpenGL::GLU)
