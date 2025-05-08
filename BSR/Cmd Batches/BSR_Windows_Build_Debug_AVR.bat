@echo off

echo on

if "%SolutionDir%" == "" call "..\..\Solution Items\Cmd Batches\BSR_Path.bat"

@echo off

set CPP_FLAGS=-c -g -Os -std=gnu++17 -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p
set CPP_DEFINES=-DBSR_ARDUINO

pushd "%SolutionDir%"

echo on

if exist ".\Binaries\BSR\Release\AVR\" rmdir ".\Binaries\BSR\Release\AVR\" /s /q
if exist ".\Objects\BSR\Release\AVR\" rmdir ".\Objects\BSR\Release\AVR\" /s /q

@echo off

mkdir ".\Binaries\BSR\Release\AVR\"
mkdir ".\Objects\BSR\Release\AVR\"

echo on

avr-g++ %CPP_FLAGS% %CPP_DEFINES% -o ".\Objects\BSR\Release\AVR\BSR_Image.o" ".\BSR\Sources\BSR_Image.cpp"
avr-g++ %CPP_FLAGS% %CPP_DEFINES% -o ".\Objects\BSR\Release\AVR\BSR_Math.o" ".\BSR\Sources\BSR_Math.cpp"
avr-g++ %CPP_FLAGS% %CPP_DEFINES% -o ".\Objects\BSR\Release\AVR\BSR_Math_Matrix.o" ".\BSR\Sources\BSR_Math_Matrix.cpp"
avr-g++ %CPP_FLAGS% %CPP_DEFINES% -o ".\Objects\BSR\Release\AVR\BSR_Math_Vector.o" ".\BSR\Sources\BSR_Math_Vector.cpp"
avr-g++ %CPP_FLAGS% %CPP_DEFINES% -o ".\Objects\BSR\Release\AVR\BSR_Rasterizer.o" ".\BSR\Sources\BSR_Rasterizer.cpp"
avr-g++ %CPP_FLAGS% %CPP_DEFINES% -o ".\Objects\BSR\Release\AVR\BSR_Renderer.o" ".\BSR\Sources\BSR_Renderer.cpp"

avr-gcc-ar rcs ".\Binaries\BSR\Release\AVR\BSR.a" ".\Objects\BSR\Release\AVR\BSR_Image.o"
avr-gcc-ar rcs ".\Binaries\BSR\Release\AVR\BSR.a" ".\Objects\BSR\Release\AVR\BSR_Math.o"
avr-gcc-ar rcs ".\Binaries\BSR\Release\AVR\BSR.a" ".\Objects\BSR\Release\AVR\BSR_Math_Matrix.o"
avr-gcc-ar rcs ".\Binaries\BSR\Release\AVR\BSR.a" ".\Objects\BSR\Release\AVR\BSR_Math_Vector.o"
avr-gcc-ar rcs ".\Binaries\BSR\Release\AVR\BSR.a" ".\Objects\BSR\Release\AVR\BSR_Rasterizer.o"
avr-gcc-ar rcs ".\Binaries\BSR\Release\AVR\BSR.a" ".\Objects\BSR\Release\AVR\BSR_Renderer.o"

@echo off

popd

echo on
