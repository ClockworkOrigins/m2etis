#!/bin/bash

cd "$(readlink "$(dirname "${0}")")"

. ./build-common.sh

# chimera
ARCHIVE="chimera-1.20.tar.gz"
BUILD_DIR="${BUILD_ROOT}/chimera"

PREFIX="${DEP_DIR}/chimera/"
DEBUG_FLAG="-DCMAKE_BUILD_TYPE=Debug"
RELEASE_FLAG="-DCMAKE_BUILD_TYPE=Release"
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
	status "Cleaning Chimera"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile Chimera"

./download-dependency.sh ${ARCHIVE}

status "Extracting Chimera"
tar xfz "${ARCHIVE}" >/dev/null

cd "${BUILD_ROOT}"

status "Configuring Chimera"
cd "${BUILD_DIR}"

# the autotool scripts in this package of chimera are broken and don't work on OpenSuSE,
# due to a version conflict in libtool
cp "${DEP_DIR}/resources/chimera/CMakeLists.txt" .
cp "${DEP_DIR}/resources/chimera/src/CMakeLists.txt" "./src/"

status "Patching Sources"
patch -p0 < "${DEP_DIR}/resources/chimera/routing_fix.diff" >/dev/null
patch -p0 < "${DEP_DIR}/resources/chimera/sparse_leafset_delete.diff" >/dev/null

cmake \
	-DCMAKE_INSTALL_PREFIX="${PREFIX}" \
	${BUILD_TYPE} \
	. >/dev/null

status "Building Chimera"
make ${PARALLEL_FLAG} >/dev/null

status "Installing Chimera"
make ${PARALLEL_FLAG} install >/dev/null

status "Patching Headers"
cd "${PREFIX}"
patch -p0 < "${DEP_DIR}/resources/chimera/global_variables_key_h.diff" >/dev/null

status "Cleaning up"
cd "${DEP_DIR}"
rm -r "${BUILD_DIR}" >/dev/null

if [[ $OSTYPE =~ "darwin" ]]; then
	status "OS X detected: Setting install_name"
	cd ${PREFIX}/lib
	install_name_tool -id @executable_path/../lib/libchimera.1.4.dylib libchimera.1.4.dylib
fi

touch "${PREFIX}"
