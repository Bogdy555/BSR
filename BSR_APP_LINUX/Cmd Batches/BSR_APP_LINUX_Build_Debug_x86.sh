#!/bin/bash

if [ -z "$SolutionDir" ]; then source "../../Solution Items/Cmd Batches/BSR_Path.sh"; fi

export CPP_FLAGS="-c -std=gnu++17 -I\"./BSR/Headers/\" -m32 -g"
export LINK_FLAGS="-L\"./Binaries/BSR/Debug/x86/\""

pushd "$SolutionDir"

if [ -d "./Binaries/BSR_APP_LINUX/Debug/x86/" ]; then rm -rf "./Binaries/BSR_APP_LINUX/Debug/x86/"; fi
if [ -d "./Objects/BSR_APP_LINUX/Debug/x86/" ]; then rm -rf "./Objects/BSR_APP_LINUX/Debug/x86/"; fi

mkdir -p "./Binaries/BSR_APP_LINUX/Debug/x86/"
mkdir -p "./Objects/BSR_APP_LINUX/Debug/x86/"

g++ $CPP_FLAGS -o "./Objects/BSR_APP_LINUX/Debug/x86/BSR_APP_LINUX_EntryPoint.o" "./BSR_APP_LINUX/Sources/BSR_APP_LINUX_EntryPoint.cpp"

g++ $LINK_FLAGS -o "./Binaries/BSR_APP_LINUX/Debug/x86/BSR_APP_LINUX" "./Objects/BSR_APP_LINUX/Debug/x86/BSR_APP_LINUX_EntryPoint.o"

if [ -d "./BSR_APP_LINUX/Files/" ]; then cp -r "./BSR_APP_LINUX/Files/" "./Binaries/BSR_APP_LINUX/Debug/x86/"; fi

popd
