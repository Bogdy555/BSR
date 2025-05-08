@echo off

echo on

if "%SolutionDir%" == "" call "..\..\Solution Items\Cmd Batches\BSR_Path.bat"

@echo off

pushd "%SolutionDir%"

echo on

if exist ".\Binaries\BSR\Release\AVR\" rmdir ".\Binaries\BSR\Release\AVR\" /s /q
if exist ".\Objects\BSR\Release\AVR\" rmdir ".\Objects\BSR\Release\AVR\" /s /q

@echo off

popd

echo on
