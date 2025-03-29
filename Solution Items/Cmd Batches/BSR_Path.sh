#!/bin/bash

if [ -z "$SolutionDir" ]; then export SolutionDir="/home/matei/Programming/C++/Others/BSR/"; fi

pushd "$SolutionDir"

chmod +x "./BSR/Cmd Batches/BSR_Build_Debug_x64.sh"
chmod +x "./BSR/Cmd Batches/BSR_Build_Debug_x86.sh"
chmod +x "./BSR/Cmd Batches/BSR_Build_Release_x64.sh"
chmod +x "./BSR/Cmd Batches/BSR_Build_Release_x86.sh"
chmod +x "./Solution Items/Cmd Batches/BSR_Path.sh"

popd
