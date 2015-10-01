#!/bin/bash

##
# Copyright  2011-2012  Prof. Dr. Richard Lenz, Thomas Fischer
#
# Licensed  under  the  Apache  License,  Version  2.0  (the  "License");
# you  may  not  use  this  file  except  in  compliance  with  the  License.
# You  may  obtain  a  copy  of  the  License  at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless  required  by  applicable  law  or  agreed  to  in  writing,  software
# distributed  under  the  License  is  distributed  on  an  "AS IS"  BASIS,
# WITHOUT  WARRANTIES  OR  CONDITIONS  OF  ANY  KIND,  either  express  or  implied.
# See  the  License  for  the  specific  language  governing  permissions  and
# limitations  under  the  License.
#
##

cd "$(readlink -f "$(dirname "${0}")")"

. ./build-common.sh

# google perftools
ARCHIVE="gperftools-2.4.tar.gz"
BUILD_DIR="${BUILD_ROOT}/gperftools-2.4"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${DEP_DIR}/gperftools/"
RELEASE_FLAG=""
DEBUG_FLAG="CFLAGS=-g"
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
	status "Cleaning GPerfTools"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile GPerfTools"

status "Extracting GPerfTools"

./download-dependency.sh ${ARCHIVE}

cd "${BUILD_ROOT}"
tar xfz "${ARCHIVE}" >/dev/null
#status
#status "Bootstrapping "
cd "${BUILD_DIR}"

# set readable
chmod -R u+w .

patch -p0 -i "${EX_DIR}/gperftools-gcc-4.7-siginfo.patch"

status "Configuring GPerfTools"
./configure --libdir="${PREFIX}/lib" --prefix="${PREFIX}" --enable-frame-pointers ${BUILD_TYPE} >/dev/null

status "Building GPerfTools"
make ${PARALLEL_FLAG} >/dev/null

status "Installing GPerfTools"
make ${PARALLEL_FLAG} install >/dev/null

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_DIR}" >/dev/null

touch "${PREFIX}"

