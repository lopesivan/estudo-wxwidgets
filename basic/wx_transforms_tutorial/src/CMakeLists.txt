cmake_minimum_required(VERSION 3.6 FATAL_ERROR)
project(wx_transforms_tutorial_core LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# hack for buggy CMake's FindwxWidgets script
if(DEFINED ENV_WX_CONFIG)
    set(ENV{WX_CONFIG} ${ENV_WX_CONFIG})
endif()

set(wxWidgets_USE_STATIC 1)

# Se wxWidgets já foi encontrado no modo customizado, não precisa find_package
if(NOT wxWidgets_FOUND)
    find_package(wxWidgets REQUIRED xml core base)
    include(${wxWidgets_USE_FILE})
endif()

set(SRCS main.cpp canvas/drawingcanvas.cpp canvas/objectspace.cpp canvas/selectionbox.cpp drawingdocument.cpp drawingview.cpp)

if(APPLE)
    add_executable(main MACOSX_BUNDLE ${SRCS})
    set_target_properties(main PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
else()
    add_executable(main WIN32 ${SRCS} main.exe.manifest)
endif()

target_link_libraries(main PRIVATE ${wxWidgets_LIBRARIES})

# Se usando wxWidgets customizado, adicionar flags manualmente
if(DEFINED wxWidgets_CXX_FLAGS)
    target_compile_options(main PRIVATE ${wxWidgets_CXX_FLAGS})
endif()
