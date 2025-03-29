#!/bin/bash

# Automatically print each command and its exit code
trap 'LAST_COMMAND=$BASH_COMMAND' DEBUG
trap 'RET=$?; echo "[🧩] \"$LAST_COMMAND\" exited with code $RET"; if [ $RET -ne 0 ]; then echo "❌ Script exiting due to error."; exit $RET; fi' EXIT

if [ -z "$SolutionDir" ]; then
    source "../../Solution Items/Cmd Batches/BSR_Path.sh"
fi

export CPP_FLAGS='-c -std=gnu++17 -I./BSR/Headers/'
export LINK_FLAGS='-L./Binaries/BSR/Release/x64/ -lBSR'

pushd "$SolutionDir" || { echo "❌ Failed to pushd into $SolutionDir"; exit 1; }

echo "🛠️  Working in: $(pwd)"

# Clean build directories
rm -rf "./Binaries/BSR_APP_LINUX/Release/x64/"
rm -rf "./Objects/BSR_APP_LINUX/Release/x64/"

# Recreate directories
mkdir -p "./Binaries/BSR_APP_LINUX/Release/x64/"
mkdir -p "./Objects/BSR_APP_LINUX/Release/x64/"

# Compile entry point
g++ $CPP_FLAGS -o "./Objects/BSR_APP_LINUX/Release/x64/BSR_APP_LINUX_EntryPoint.o" \
    "./BSR_APP_LINUX/Sources/BSR_APP_LINUX_EntryPoint.cpp"

# Link final binary
g++ ./Objects/BSR_APP_LINUX/Release/x64/BSR_APP_LINUX_EntryPoint.o \
    -L./Binaries/BSR/Release/x64/ -lBSR \
    -o ./Binaries/BSR_APP_LINUX/Release/x64/BSR_APP_LINUX

# Copy runtime files
if [ -d "./BSR_APP_LINUX/Files/" ]; then
    cp -r "./BSR_APP_LINUX/Files/" "./Binaries/BSR_APP_LINUX/Release/x64/"
fi

popd
