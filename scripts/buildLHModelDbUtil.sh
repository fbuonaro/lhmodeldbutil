#!/bin/bash

TARGET_STAGE="$1"
if [[ "x${TARGET_STAGE}" == "x" ]];
then
    TARGET_STAGE="dist"
fi

# lhmodel
pushd ./modules/lhmodel
./scripts/buildLHModelImage.sh
popd

# libcppdb
./scripts/buildLibCppdbLHDistImage.sh

./modules/lhscriptutil/scripts/buildImage.sh ./Dockerfiles/Dockerfile.lhmodeldbutil lhmodeldbutil "${TARGET_STAGE}"