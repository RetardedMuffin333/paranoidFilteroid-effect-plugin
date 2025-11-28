@echo off
cd /d "%~dp0"

REM Delete build folder completely
if exist build (
    echo Cleaning build folder...
    rmdir /s /q build 2>nul
    timeout /t 1 >nul
)

mkdir build
cd build

REM Setup Visual Studio environment
echo Setting up Visual Studio 2026 environment...
call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"

REM Configure
echo.
echo Configuring CMake...
cmake .. -G "Visual Studio 18 2026" -A x64 -DCMAKE_CXX_FLAGS="/permissive-"

if not exist CMakeCache.txt (
    echo Configuration failed!
    timeout /t 10
    exit /b 1
)

REM Build
echo.
echo Building plugin...
cmake --build . --config Debug --parallel 4

if %ERRORLEVEL% equ 0 (
    echo.
    echo ===============================================
    echo SUCCESS! Plugin built successfully
    echo ===============================================
    echo.
    echo Plugin location:
    echo C:\Program Files\Common Files\VST3\paranoidFilteroid.vst3
    echo.
) else (
    echo.
    echo BUILD FAILED - Check errors above
)

timeout /t 10
