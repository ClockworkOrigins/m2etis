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

cd "$(readlink -f "$(dirname "${0}")")"

. ./build-common.sh

ARCHIVE="clockutils-1.0.0-src.zip"
BUILD_DIR="${BUILD_ROOT}/clockutils-1.0.0-src"

PREFIX="${DEP_DIR}/clockUtils"
DEST_DIR="${PREFIX}"
DEBUG_FLAG="DEBUG"
RELEASE_FLAG="RELEASE"
PARALLEL_FLAG=""
BUILD_TYPE="Release"

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi

if [ ! -z "${CLEAN}" ]; then
	rm -rf "${PREFIX}"
	exit 0
fi

if [ -d ${DEST_DIR} ]; then
	exit 0
fi

rm -rf "${PREFIX}"

./download-dependency.sh ${ARCHIVE} http://clockwork-origins.de/clockUtils/downloads/

cd "${BUILD_ROOT}"
unzip "${ARCHIVE}" >/dev/null

cd "${BUILD_DIR}"

if [ -e "CMakeCache.txt" ]; then
	rm "CMakeCache.txt"
	make clean
fi

cmake -G 'Unix Makefiles'\
 -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"\
 -DCMAKE_INSTALL_PREFIX="${PREFIX}"\
 -DWITH_TESTING=OFF\
 -DWITH_LIBRARY_ARGPARSER=OFF\
 -DWITH_LIBRARY_COMPRESSION=OFF\
 -DWITH_LIBRARY_CONTAINER=ON\
 -DWITH_LIBRARY_INIPARSER=OFF\
 -DWITH_LIBRARY_SOCKETS=ON\
 -DCMAKE_CXX_COMPILER=${COMPILER}\
 .

make -j 8

make install

cd "${DEP_DIR}"
rm -r "${BUILD_DIR}" >/dev/null

touch "${PREFIX}"
