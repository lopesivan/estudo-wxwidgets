cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(settings_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------------------------------------------------
# wxWidgets — instalação local via awx
# ----------------------------------------------------------------------------
set(V
    "3.3.1"
    CACHE STRING "Versão do wxWidgets")
set(wxWidgets_ROOT_DIR "$ENV{HOME}/.local/wxwidgets/linux-cmake-wx-${V}")
set(wxWidgets_CONFIG_EXECUTABLE "${wxWidgets_ROOT_DIR}/bin/wx-config")

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# ----------------------------------------------------------------------------
set(SRCS src/app.cpp src/main_frame.cpp)

if(APPLE)
  add_executable(main MACOSX_BUNDLE ${SRCS})
  set_target_properties(main PROPERTIES MACOSX_BUNDLE_INFO_PLIST
                                        ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
elseif(WIN32)
  add_executable(main WIN32 ${SRCS} main.exe.manifest resources.rc)
else()
  add_executable(main ${SRCS})
endif()

target_link_libraries(main PRIVATE ${wxWidgets_LIBRARIES})
