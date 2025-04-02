#!/bin/bash

if [ -z "$SolutionDir" ]; then SolutionDir="/workspaces/BSR/"; fi

pushd "$SolutionDir"

chmod +x "./BSR/Cmd Batches/BSR_Build_Debug_x64.sh"
chmod +x "./BSR/Cmd Batches/BSR_Build_Debug_x86.sh"
chmod +x "./BSR/Cmd Batches/BSR_Build_Release_x64.sh"
chmod +x "./BSR/Cmd Batches/BSR_Build_Release_x86.sh"
chmod +x "./BSR/Cmd Batches/BSR_Clean.sh"
chmod +x "./BSR_APP_LINUX/Cmd Batches/BSR_APP_LINUX_Build_Debug_x64.sh"
chmod +x "./BSR_APP_LINUX/Cmd Batches/BSR_APP_LINUX_Build_Debug_x86.sh"
chmod +x "./BSR_APP_LINUX/Cmd Batches/BSR_APP_LINUX_Build_Release_x64.sh"
chmod +x "./BSR_APP_LINUX/Cmd Batches/BSR_APP_LINUX_Build_Release_x86.sh"
chmod +x "./BSR_APP_LINUX/Cmd Batches/BSR_APP_LINUX_Clean.sh"
chmod +x "./Solution Items/Cmd Batches/BSR_Path.sh"

popd
