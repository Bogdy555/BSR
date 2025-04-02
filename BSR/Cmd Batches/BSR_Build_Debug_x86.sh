#!/bin/bash

if [ -z "$SolutionDir" ]; then source "../../Solution Items/Cmd Batches/BSR_Path.sh"; fi

CPP_FLAGS="-c -std=gnu++17 -m32 -g"

pushd "$SolutionDir"

if [ -d "./Binaries/BSR/Debug/x86/" ]; then rm -rf "./Binaries/BSR/Debug/x86/"; fi
if [ -d "./Objects/BSR/Debug/x86/" ]; then rm -rf "./Objects/BSR/Debug/x86/"; fi

mkdir -p "./Binaries/BSR/Debug/x86/"
mkdir -p "./Objects/BSR/Debug/x86/"

g++ $CPP_FLAGS -o "./Objects/BSR/Debug/x86/BSR_Image.o" "./BSR/Sources/BSR_Image.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Debug/x86/BSR_Math.o" "./BSR/Sources/BSR_Math.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Debug/x86/BSR_Math_Matrix.o" "./BSR/Sources/BSR_Math_Matrix.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Debug/x86/BSR_Math_Vector.o" "./BSR/Sources/BSR_Math_Vector.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Debug/x86/BSR_Rasterizer.o" "./BSR/Sources/BSR_Rasterizer.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Debug/x86/BSR_Renderer.o" "./BSR/Sources/BSR_Renderer.cpp"

ar rcs "./Binaries/BSR/Debug/x86/libBSR.a" "./Objects/BSR/Debug/x86/BSR_Image.o" "./Objects/BSR/Debug/x86/BSR_Math.o" "./Objects/BSR/Debug/x86/BSR_Math_Matrix.o" "./Objects/BSR/Debug/x86/BSR_Math_Vector.o" "./Objects/BSR/Debug/x86/BSR_Rasterizer.o" "./Objects/BSR/Debug/x86/BSR_Renderer.o"

popd
