#!/bin/bash

# Copyright (2016) Michael Baer, Daniel Bonrath, All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
