#!/bin/bash

if [ -z "$SolutionDir" ]; then source "../../Solution Items/Cmd Batches/BSR_Path.sh"; fi

CPP_FLAGS="-c -std=gnu++17 -I./BSR/Headers/ -m32"
LINK_FLAGS="-m32 -L./Binaries/BSR/Release/x86/ -lBSR"

pushd "$SolutionDir"

if [ -d "./Binaries/BSR_APP_LINUX/Release/x86/" ]; then rm -rf "./Binaries/BSR_APP_LINUX/Release/x86/"; fi
if [ -d "./Objects/BSR_APP_LINUX/Release/x86/" ]; then rm -rf "./Objects/BSR_APP_LINUX/Release/x86/"; fi

mkdir -p "./Binaries/BSR_APP_LINUX/Release/x86/"
mkdir -p "./Objects/BSR_APP_LINUX/Release/x86/"

g++ $CPP_FLAGS -o "./Objects/BSR_APP_LINUX/Release/x86/BSR_APP_LINUX_EntryPoint.o" "./BSR_APP_LINUX/Sources/BSR_APP_LINUX_EntryPoint.cpp"

g++ "./Objects/BSR_APP_LINUX/Release/x86/BSR_APP_LINUX_EntryPoint.o" $LINK_FLAGS -o "./Binaries/BSR_APP_LINUX/Release/x86/BSR_APP_LINUX"

if [ -d "./BSR_APP_LINUX/Files/" ]; then cp -r "./BSR_APP_LINUX/Files/." "./Binaries/BSR_APP_LINUX/Release/x86/"; fi

popd
