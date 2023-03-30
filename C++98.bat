@echo off

call gccvarsx64.bat

if not exist ".\Binaries\" mkdir ".\Binaries\"
if not exist ".\Binaries\Software Rasterizer\" mkdir ".\Binaries\Software Rasterizer\"
if not exist ".\Binaries\Software Rasterizer\C++98\" mkdir ".\Binaries\Software Rasterizer\C++98\"

echo on

g++ -static -std=c++98 ".\Software Rasterizer\Sources\*.cpp" -o ".\Binaries\Software Rasterizer\C++98\Software Rasterizer.exe" -l kernel32 -l user32 -l gdi32 -l winspool -l comdlg32 -l advapi32 -l shell32 -l ole32 -l oleaut32 -l uuid -l odbc32 -l odbccp32

@echo off

if exist ".\Software Rasterizer\Files\" xcopy ".\Software Rasterizer\Files\*" ".\Binaries\Software Rasterizer\C++98\*" /h /e /y /q

pushd ".\Binaries\Software Rasterizer\C++98\"

echo on

".\Software Rasterizer.exe"

@echo off

popd

echo on
