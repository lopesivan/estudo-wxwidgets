@echo off
REM ==========================================
REM Script para compilar e criar instalador
REM do Minimal Application
REM ==========================================
SETLOCAL EnableDelayedExpansion

echo ==========================================
echo   Build e Instalador - Minimal App
echo ==========================================
echo.

REM ==========================================
REM CONFIGURAÇÕES
REM ==========================================
set APP_NAME=minimal
set INNO_SETUP="C:\Program Files (x86)\Inno Setup 6\ISCC.exe"
set BUILD_DIR=build-msvc-x64
set INSTALLER_SCRIPT=minimal_installer.iss
set OUTPUT_DIR=installer_output

%INNO_SETUP% "%INSTALLER_SCRIPT%"

pause

