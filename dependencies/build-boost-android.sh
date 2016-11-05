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

. ./build-common.sh android

ARCHIVE="boost_1_58_0.tar.bz2"
BUILD_DIR="${BUILD_ROOT}/boost_1_58_0"
PREFIX="${DEP_OUT_DIR}/boost/"

if [ -d ${PREFIX} ]; then
	exit 0
fi

title "Compile Boost"

. ./download-dependency.sh ${ARCHIVE}

status "Extracting Boost"

cd "${BUILD_ROOT}"
tar xfj "${ARCHIVE}"

status "Configuring Boost"

cd "${BUILD_DIR}"
cp "${PATCH_DIR}/boost/user-config-AndroidLinux.jam" "${BUILD_DIR}/tools/build/src/user-config.jam"
./bootstrap.sh  --prefix="${PREFIX}" --with-libraries=chrono,date_time,filesystem,regex,serialization,system,thread

status "Building & Installing Boost"

./bjam -d2 \
	cxxflags=-fPIC \
	-j ${CPU_CORES} \
	variant=release \
	--layout=system \
	threading=multi \
	link=static \
	toolset=gcc-android \
	install >/dev/null

status "Cleaning up"

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}"

