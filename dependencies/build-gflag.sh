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

ARCHIVE="gflags-2.1.2.zip"
BUILD_DIR="${BUILD_ROOT}/gflags-2.1.2"
PREFIX="${DEP_OUT_DIR}/gflags/"

if [ -d ${PREFIX} ]; then
	exit 0
fi

title "Compile GFlags"

. ./download-dependency.sh ${ARCHIVE}

status "Extracting GFlags"

cd "${BUILD_ROOT}"
unzip "${ARCHIVE}"

status "Configuring GFlags"

cd "${BUILD_DIR}"
cmake \
	-DBUILD_SHARED_LIBS=ON \
	-DCMAKE_INSTALL_PREFIX="${PREFIX}" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_COMPILER=${C_COMPILER} \
	-DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
.

status "Building GFlags"

make -j ${CPU_CORES}

status "Installing GFlags"

make install

status "Cleaning up"

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}"

