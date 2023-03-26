@echo off

call gccvarsx64.bat

mkdir ".\Binaries\"
mkdir ".\Binaries\Software Rasterizer\"
mkdir ".\Binaries\Software Rasterizer\C++98\"

echo on

g++ -static -std=c++98 ".\Software Rasterizer\Sources\*.cpp" -o ".\Binaries\Software Rasterizer\C++98\Software Rasterizer.exe"

@echo off

if exist ".\Software Rasterizer\Files\" xcopy ".\Software Rasterizer\Files\*" ".\Binaries\Software Rasterizer\C++98\*" /h /e /y /q

pushd ".\Binaries\Software Rasterizer\C++98\"

echo on

".\Software Rasterizer.exe"

@echo off

popd

echo on
