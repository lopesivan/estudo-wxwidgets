@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86

cmake -S . -B build-msvc-x64 -G "Visual Studio 17 2022" -A x64 ^
  -D wxBUILD_TOOLKIT=msw ^
  -D wxBUILD_SHARED=ON ^
  -D wxBUILD_USE_STATIC_RUNTIME=OFF ^
  -D wxUSE_WEBVIEW=OFF -D wxUSE_WEBVIEW_EDGE=OFF  ^
  -D wxBUILD_SAMPLES=OFF -D wxBUILD_DEMOS=OFF -D wxBUILD_TESTS=OFF ^
  -D CMAKE_INSTALL_PREFIX="C:/Users/ivan/source/repos/SDKs/VC/wx-3.3.1-msvc-x64"

cmake --build build-msvc-x64 --config Release --target install

pause
