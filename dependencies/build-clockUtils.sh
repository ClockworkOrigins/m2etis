#!/bin/bash

##
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
##

cd "$(readlink "$(dirname "${0}")")"

. ./build-common.sh ${1}

ARCHIVE="clockutils-1.1.0-src.zip"
BUILD_DIR="${BUILD_ROOT}/clockutils-1.1.0-src"
PREFIX="${DEP_OUT_DIR}/clockUtils/"

if [ -d ${PREFIX} ]; then
	exit 0
fi

title "Compile clockUtils"

status "Extracting clockUtils"

downloadAndUnpack ${ARCHIVE} http://clockwork-origins.de/clockUtils/downloads/

status "Configuring clockUtils"

cd "${BUILD_DIR}"
cmake \
	-DWITH_TESTING=OFF \
	-DWITH_LIBRARY_ARGPARSER=ON \
	-DWITH_LIBRARY_COMPRESSION=OFF \
	-DWITH_LIBRARY_CONTAINER=OFF \
	-DWITH_LIBRARY_INIPARSER=OFF \
	-DWITH_LIBRARY_SOCKETS=ON \
	-DCLOCKUTILS_BUILD_SHARED=ON \
	-DCMAKE_INSTALL_PREFIX="${PREFIX}" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_COMPILER=${C_COMPILER} \
	-DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
.

status "Building clockUtils"

make -j ${CPU_CORES}

status "Installing clockUtils"

make install

status "Cleaning up"

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}"

