@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86

set "WX_DIR=C:/Users/ivan/source/repos/SDKs/VC/wx-3.3.1-msvc-x64/lib/cmake/wxWidgets"

cmake -S .. -B build-msvc-x64 -G "Visual Studio 17 2022" -A x64 ^
  -D CMAKE_BUILD_TYPE=Release ^
  -D wxBUILD_TOOLKIT=msw ^
  -D wxBUILD_SHARED=ON ^
  -D wxBUILD_USE_STATIC_RUNTIME=OFF ^
  -D wxUSE_WEBVIEW=OFF ^
  -D wxUSE_WEBVIEW_EDGE=OFF ^
  -D wxWidgets_DIR="%WX_DIR%" ^
  -D CMAKE_PROJECT_TOP_LEVEL_INCLUDES:FILEPATH="%~dp0run.cmake"

cmake --build build-msvc-x64 --config Release

rem
