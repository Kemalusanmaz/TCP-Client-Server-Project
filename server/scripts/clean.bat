@echo off
REM If there is an error during the exit, stop the script.
setlocal enabledelayedexpansion

cd ..

echo Cleaning build directory...

REM Remove if there is a build directory
if exist "build" (
    rmdir /s /q "build"
    echo Removed build directory.
)

REM Remove if there is a CMakeCache.txt file created by CMake
if exist "CMakeCache.txt" (
    del "CMakeCache.txt"
    echo Removed CMakeCache.txt
)

REM Remove if there is a CMakeFiles directory created by CMake
if exist "CMakeFiles" (
    rmdir /s /q "CMakeFiles"
    echo Removed CMakeFiles directory.
)

REM Remove if there is an executable file (Server)
if exist "Server.exe" (
    del "Server.exe"
    echo Removed Server executable.
)

echo Cleanup complete!
endlocal