#!/bin/bash

if [ -z "$SolutionDir" ]; then source "../../Solution Items/Cmd Batches/BSR_Path.sh"; fi

pushd "$SolutionDir"

if [ -d "./Binaries/BSR/Debug/x64/" ]; then rm -rf "./Binaries/BSR/Debug/x64/"; fi
if [ -d "./Objects/BSR/Debug/x64/" ]; then rm -rf "./Objects/BSR/Debug/x64/"; fi

if [ -d "./Binaries/BSR/Debug/x86/" ]; then rm -rf "./Binaries/BSR/Debug/x86/"; fi
if [ -d "./Objects/BSR/Debug/x86/" ]; then rm -rf "./Objects/BSR/Debug/x86/"; fi

if [ -d "./Binaries/BSR/Release/x64/" ]; then rm -rf "./Binaries/BSR/Release/x64/"; fi
if [ -d "./Objects/BSR/Release/x64/" ]; then rm -rf "./Objects/BSR/Release/x64/"; fi

if [ -d "./Binaries/BSR/Release/x86/" ]; then rm -rf "./Binaries/BSR/Release/x86/"; fi
if [ -d "./Objects/BSR/Release/x86/" ]; then rm -rf "./Objects/BSR/Release/x86/"; fi

popd
