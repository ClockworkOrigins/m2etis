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

ARCHIVE="glog-0.3.3.tar.gz"
BUILD_DIR="${BUILD_ROOT}/glog-0.3.3"
PREFIX="${DEP_OUT_DIR}/glog/"

if [ -d ${PREFIX} ]; then
	exit 0
fi

title "Compile GLog"

status "Extracting GLog"

downloadAndUnpack ${ARCHIVE}

status "Configuring GLog"

cd "${BUILD_DIR}"
if [[ "${C_COMPILER}" == "clang" ]]; then
	CXXFLAGS="-std=c++11 -Wno-reserved-user-defined-literal $STDLIB" CC=clang CXX=clang++ ./configure --prefix=${PREFIX}
else
	CXXFLAGS="-std=c++11" ./configure --prefix=${PREFIX}
fi

cmake \
	-DWITH_TESTING=OFF \
	-DCMAKE_INSTALL_PREFIX="${PREFIX}" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_COMPILER=${C_COMPILER} \
	-DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
.

status "Building GLog"

make -j ${CPU_CORES}

status "Installing GLog"

make install

status "Cleaning up"

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}"

