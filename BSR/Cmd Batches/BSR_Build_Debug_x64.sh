#!/bin/bash

if [ -z "$SolutionDir" ]; then source "../../Solution Items/Cmd Batches/BSR_Path.sh"; fi

export CPP_FLAGS="-c -std=gnu++17 -g"

pushd "$SolutionDir"

if [ -d "./Binaries/BSR/Debug/x64/" ]; then rm -rf "./Binaries/BSR/Debug/x64/"; fi
if [ -d "./Objects/BSR/Debug/x64/" ]; then rm -rf "./Objects/BSR/Debug/x64/"; fi

mkdir -p "./Binaries/BSR/Debug/x64/"
mkdir -p "./Objects/BSR/Debug/x64/"

g++ $CPP_FLAGS -o "./Objects/BSR/Debug/x64/BSR_Image.o" "./BSR/Sources/BSR_Image.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Debug/x64/BSR_Math.o" "./BSR/Sources/BSR_Math.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Debug/x64/BSR_Math_Matrix.o" "./BSR/Sources/BSR_Math_Matrix.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Debug/x64/BSR_Math_Vector.o" "./BSR/Sources/BSR_Math_Vector.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Debug/x64/BSR_Rasterizer.o" "./BSR/Sources/BSR_Rasterizer.cpp"
g++ $CPP_FLAGS -o "./Objects/BSR/Debug/x64/BSR_Renderer.o" "./BSR/Sources/BSR_Renderer.cpp"

ar rcs "./Binaries/BSR/Debug/x64/BSR.a" "./Objects/BSR/Debug/x64/BSR_Image.o"
ar rcs "./Binaries/BSR/Debug/x64/BSR.a" "./Objects/BSR/Debug/x64/BSR_Math.o"
ar rcs "./Binaries/BSR/Debug/x64/BSR.a" "./Objects/BSR/Debug/x64/BSR_Math_Matrix.o"
ar rcs "./Binaries/BSR/Debug/x64/BSR.a" "./Objects/BSR/Debug/x64/BSR_Math_Vector.o"
ar rcs "./Binaries/BSR/Debug/x64/BSR.a" "./Objects/BSR/Debug/x64/BSR_Rasterizer.o"
ar rcs "./Binaries/BSR/Debug/x64/BSR.a" "./Objects/BSR/Debug/x64/BSR_Renderer.o"

popd
