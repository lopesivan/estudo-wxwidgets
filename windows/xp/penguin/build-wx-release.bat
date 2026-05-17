@echo off
call "C:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86

rem set WXWIN=C:\Documents and Settings\ivan\My Documents\repo\wxWidgets-3.2.4
rem cd /d "%WXWIN%\build\msw"

nmake /f makefile.vc BUILD=release

pause
