@echo off
setlocal

set "WX_BIN=C:\Users\ivan\source\repos\SDKs\MinGW64\wx-3.3.1-gcc-x64\bin\gcc_x64_dll"
set "MINGW_BIN=C:\mingw64\bin"
set "APP_DIR=build-mingw64"
set "APP=%APP_DIR%\minimal.exe"

rem Adiciona as pastas de DLLs ao PATH *somente durante este script*
set "PATH=%WX_BIN%;%MINGW_BIN%;%PATH%"

if not exist "%APP%" (
  echo [ERRO] Nao encontrei o executavel: %APP%
  exit /b 1
)

echo [RUN] Executando %APP% ...
pushd "%APP_DIR%"
minimal.exe
set "RC=%ERRORLEVEL%"
popd

endlocal & exit /b %RC%
