#!/bin/bash

if [ -z "$SolutionDir" ]; then source "../../Solution Items/Cmd Batches/BSR_Path.sh"; fi

pushd "$SolutionDir"

if [ ! -f "./Binaries/BSR/Debug/x64/libBSR.a" ]; then source "./BSR/Cmd Batches/BSR_Build_Debug_x64.sh"; fi

popd

CPP_FLAGS="-c -std=gnu++17 -I./BSR/Headers/ -g"
LINK_FLAGS="-L./Binaries/BSR/Debug/x64/ -lBSR"

pushd "$SolutionDir"

if [ -d "./Binaries/BSR_APP_LINUX/Debug/x64/" ]; then rm -rf "./Binaries/BSR_APP_LINUX/Debug/x64/"; fi
if [ -d "./Objects/BSR_APP_LINUX/Debug/x64/" ]; then rm -rf "./Objects/BSR_APP_LINUX/Debug/x64/"; fi

mkdir -p "./Binaries/BSR_APP_LINUX/Debug/x64/"
mkdir -p "./Objects/BSR_APP_LINUX/Debug/x64/"

g++ $CPP_FLAGS -o "./Objects/BSR_APP_LINUX/Debug/x64/BSR_APP_LINUX_EntryPoint.o" "./BSR_APP_LINUX/Sources/BSR_APP_LINUX_EntryPoint.cpp"

g++ "./Objects/BSR_APP_LINUX/Debug/x64/BSR_APP_LINUX_EntryPoint.o" $LINK_FLAGS -o "./Binaries/BSR_APP_LINUX/Debug/x64/BSR_APP_LINUX"

if [ -d "./BSR_APP_LINUX/Files/" ]; then cp -r "./BSR_APP_LINUX/Files/." "./Binaries/BSR_APP_LINUX/Debug/x64/"; fi

popd
