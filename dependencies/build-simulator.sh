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

PREFIX="${DEP_DIR}/../simulator/"

#OMNET++ Variables
OPP_ARCHIVE="omnetpp-4.3rc1b-src.tgz"
OPPBUILD="${PREFIX}/omnetpp-4.3rc1"

#INET-Oversim Variables
IOVER_ARCHIVE="inet-20111118-src.tgz"
IOVERBUILD="${PREFIX}/inet"

#Oversim Variables
OVER_ARCHIVE="OverSim-20121206.tgz"
OVERBUILD="${PREFIX}/OverSim"

DEBUG_FLAG="MODE=debug"
RELEASE_FLAG="MODE=release"
PARALLEL_FLAG=""

if [ -d ${PREFIX} ]; then
	exit 0
fi

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi
if [ -z "${DEBUG}" ]; then
	BUILD_TYPE="${RELEASE_FLAG}"
else
	BUILD_TYPE="${DEBUG_FLAG}"
fi
if [ ! -z "${CLEAN}" ]; then
	status "Cleaning Simulator"
	rm -rf "${OVERBUILD}"
	rm -rf "${IOVERBUILD}"
	rm -rf "${OPPBUILD}"
	exit 0
fi

title "Compile OMNET++ & Oversim"

#if ! uptodate "${EX_DIR}/${OPP_ARCHIVE}" "${PREFIX}" && ! uptodate "${EX_DIR}/${OVER_ARCHIVE}" "${PREFIX}"; then
#	status "Simulator seems to be up to date, skipping build"
#	exit 0
#fi

./download-dependency.sh ${OPP_ARCHIVE}
./download-dependency.sh ${OVER_ARCHIVE}
./download-dependency.sh ${IOVER_ARCHIVE}

status "Extracting OMNET++"
mkdir -p "${PREFIX}"
cd "${PREFIX}"
tar zxvf "${BUILD_ROOT}/${OPP_ARCHIVE}" > /dev/null

status "Set up build environment for OMNET++"
cd ${OPPBUILD}
#copied from setenv script of omnet #NON persistent environment changes
if [ ! -f configure.user -o ! -f include/omnetpp.h ]; then
    echo "Error: current working directory does not look like an OMNeT++ root directory"
    # no exit -- it would close the shell
else
    omnetpp_root=`pwd`
    echo $omnetpp_root
    export PATH=$omnetpp_root/bin:$PATH
    export LD_LIBRARY_PATH=$omnetpp_root/lib:$LD_LIBRARY_PATH
fi

#end of copied

status "Patching OMNET++"

status "Configure build scripts for OMNET++"
if [[ "$COMPILER" == "clang" ]]; then
	echo "-----------------------------------------------"
	echo "Clang is not yet supported for the simulator."
	echo "Falling back to gcc."
	echo "This will break OS X builds."
	echo "-----------------------------------------------"
	CFLAGS_RELEASE="-g -O0" CFLAGS_DEBUG="-g -O0" CC=gcc CXX=g++ ./configure --prefix=${PREFIX}
else
	CFLAGS_RELEASE="-g -O0" CFLAGS_DEBUG="-g -O0" ./configure --prefix=${PREFIX}
fi

status "Compiling OMNET++"
echo "#########################"
echo ${BUILD_TYPE}
BUILD_TYPE='MODE=debug'
make ${BUILD_TYPE} #> /dev/null

status "Installing OMNET++"
make install-menu-item > /dev/null
make install-desktop-icon > /dev/null

status "Extracting INET-Oversim"
cd ${PREFIX}
tar xzvf "${BUILD_ROOT}/${IOVER_ARCHIVE}" > /dev/null

status "Compiling INET-Oversim"
cd ${IOVERBUILD}
make makefiles
make #> /dev/null

status "Extracting Oversim"
cd ${PREFIX}
tar xzvf "${BUILD_ROOT}/${OVER_ARCHIVE}" > /dev/null

status "Compiling Oversim"
cd ${OVERBUILD}
#replace with working makefile
cp ${DEP_DIR}/resources/oversim/Makefile .
make #> /dev/null

# dirty, dirty hack because of omnet not compiling with c++11
cd ${OPPBUILD}
CFLAGS_RELEASE="-std=c++11 -g -O0" CFLAGS_DEBUG="-std=c++11 -g -O0" CC=gcc CXX=g++ ./configure --prefix=${PREFIX}
