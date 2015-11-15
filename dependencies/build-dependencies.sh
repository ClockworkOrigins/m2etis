#!/bin/bash

set -e

#
# Automatic build script to compile system dependencies
#

# all paths are relative to this file's directory
cd "$(readlink "$(dirname "${0}")")"

. build-common.sh

if [ ! -z "${CLEAN}" ]; then
	title "Cleaning all dependencies..."
else
	title "Compiling all dependencies..."
fi

#./build-configurator.sh
./build-boost.sh
#./build-chimera.sh # until it is needed again
./build-clockUtils.sh
./build-gmock.sh
./build-gflags.sh
./build-glog.sh
./build-gperftools.sh
#Source Simulator Buildscript
#./build-simulator.sh
