#!/bin/bash

if [ -z "$SolutionDir" ]; then source "../../Solution Items/Cmd Batches/BSR_Path.sh"; fi

pushd "$SolutionDir"

if [ -d "./Binaries/BSR_APP_LINUX/Debug/x64/" ]; then rm -rf "./Binaries/BSR_APP_LINUX/Debug/x64/"; fi
if [ -d "./Objects/BSR_APP_LINUX/Debug/x64/" ]; then rm -rf "./Objects/BSR_APP_LINUX/Debug/x64/"; fi

if [ -d "./Binaries/BSR_APP_LINUX/Debug/x86/" ]; then rm -rf "./Binaries/BSR_APP_LINUX/Debug/x86/"; fi
if [ -d "./Objects/BSR_APP_LINUX/Debug/x86/" ]; then rm -rf "./Objects/BSR_APP_LINUX/Debug/x86/"; fi

if [ -d "./Binaries/BSR_APP_LINUX/Release/x64/" ]; then rm -rf "./Binaries/BSR_APP_LINUX/Release/x64/"; fi
if [ -d "./Objects/BSR_APP_LINUX/Release/x64/" ]; then rm -rf "./Objects/BSR_APP_LINUX/Release/x64/"; fi

if [ -d "./Binaries/BSR_APP_LINUX/Release/x86/" ]; then rm -rf "./Binaries/BSR_APP_LINUX/Release/x86/"; fi
if [ -d "./Objects/BSR_APP_LINUX/Release/x86/" ]; then rm -rf "./Objects/BSR_APP_LINUX/Release/x86/"; fi

popd
