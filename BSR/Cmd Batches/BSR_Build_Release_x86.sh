#!/bin/bash

if [ -z "$SolutionDir" ]; then source "../../Solution Items/Cmd Batches/BSR_Path.sh"; fi

export CPP_FLAGS="-c -std=gnu++17 -m32"

pushd "$SolutionDir"

if [ -d "./Binaries/BSR/Release/x86/" ]; then rm -rf "./Binaries/BSR/Release/x86/"; fi
if [ -d "./Objects/BSR/Release/x86/" ]; then rm -rf "./Objects/BSR/Release/x86/"; fi

mkdir -p "./Binaries/BSR/Release/x86/"
mkdir -p "./Objects/BSR/Release/x86/"

g++ $CPP_FLAGS -o "./Objects/BSR/Release/x86/BSR_Image.o" "./BSR/Sources/BSR_Image.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Release/x86/BSR_Math.o" "./BSR/Sources/BSR_Math.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Release/x86/BSR_Math_Matrix.o" "./BSR/Sources/BSR_Math_Matrix.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Release/x86/BSR_Math_Vector.o" "./BSR/Sources/BSR_Math_Vector.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Release/x86/BSR_Rasterizer.o" "./BSR/Sources/BSR_Rasterizer.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Release/x86/BSR_Renderer.o" "./BSR/Sources/BSR_Renderer.cpp"

ar rcs "./Binaries/BSR/Release/x86/BSR.a" "./Objects/BSR/Release/x86/BSR_Image.o"
ar rcs "./Binaries/BSR/Release/x86/BSR.a" "./Objects/BSR/Release/x86/BSR_Math.o"
ar rcs "./Binaries/BSR/Release/x86/BSR.a" "./Objects/BSR/Release/x86/BSR_Math_Matrix.o"
ar rcs "./Binaries/BSR/Release/x86/BSR.a" "./Objects/BSR/Release/x86/BSR_Math_Vector.o"
ar rcs "./Binaries/BSR/Release/x86/BSR.a" "./Objects/BSR/Release/x86/BSR_Rasterizer.o"
ar rcs "./Binaries/BSR/Release/x86/BSR.a" "./Objects/BSR/Release/x86/BSR_Renderer.o"

popd
