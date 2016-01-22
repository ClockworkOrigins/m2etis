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

cd "$(readlink "$(dirname "${0}")")"

. ./build-common.sh

# googletest
ARCHIVE="glog-rev188.zip"
BUILD_DIR="${BUILD_ROOT}/glog-master"

PREFIX="${DEP_DIR}/glog/"
DEBUG_FLAG=""
RELEASE_FLAG=""
PARALLEL_FLAG=""

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi

if [ -z "${DEBUG}" ]; then
	BUILD_TYPE="${RELEASE_FLAG}"
else
	BUILD_TYPE="${DEBUG_FLAG}"
fi

if [ ! -z "${CLEAN}" ]; then
	status "Cleaning Google Logger"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile Google Logger"

status "Extracting Google Logger"

./download-dependency.sh ${ARCHIVE}

cd "${BUILD_ROOT}"
rm -rf "${BUILD_DIR}" >/dev/null
unzip "${ARCHIVE}" #>/dev/null

status "Configuring Google Logger"
cd "${BUILD_DIR}"

cmake -DWITH_GFLAGS=OFF -DCMAKE_INSTALL_PREFIX=${PREFIX} .
status "Building Google Logger"
make ${PARALLEL_FLAG} &> ${DEP_DIR}/out #>/dev/null

status "Installing Google Logger"
make ${PARALLEL_FLAG} install 

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_DIR}" >/dev/null

touch "${PREFIX}"
