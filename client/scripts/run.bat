@echo off
REM If there is an error during the exit, stop the script.
setlocal enabledelayedexpansion

cd ..

REM Go to build directory.
cd build

REM Check if there is an executable named Client (Client.exe for Windows).
if exist "Client.exe" (
    echo Program is working...
    Client.exe
) else (
    echo Error: The binary file cannot be found. First, run build.bat
    exit /b 1
)

endlocal