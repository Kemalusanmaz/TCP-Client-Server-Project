@REM If there is an error during the exit, stop the script.
@echo off
setlocal

@REM Eğer hata olursa devam etme
echo Starting build process...

@REM Doğru dizine git
cd ..  

@REM go to build file and create cmake
if not exist build mkdir build
cd build

@REM CMake ile proje oluştur
cmake .. -G "MinGW Makefiles"

@REM Çok çekirdekli derleme yap
mingw32-make -j4

echo "Build is completed."
endlocal