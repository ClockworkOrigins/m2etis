#!/bin/bash

cd "$(readlink "$(dirname "${0}")")"

. ./build-common.sh

# googletest
ARCHIVE="glog-0.3.3.tar.gz"
BUILD_DIR="${BUILD_ROOT}/glog-0.3.3"

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
tar xfvz "${ARCHIVE}" #>/dev/null

status "Configuring Google Logger"
cd "${BUILD_DIR}"

if [[ "$COMPILER" == "clang" ]]; then
	CXXFLAGS="-std=c++11 -Wno-reserved-user-defined-literal $STDLIB" CC=clang CXX=clang++ ./configure --prefix=${PREFIX}
else
	CXXFLAGS="-std=c++11" ./configure --prefix=${PREFIX}
fi
status "Building Google Logger"
make ${PARALLEL_FLAG} #>/dev/null

status "Installing Google Logger"
make ${PARALLEL_FLAG} install 

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_DIR}" >/dev/null

touch "${PREFIX}"
