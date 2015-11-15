#!/bin/bash

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
