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

set -e

# make sure this doesn't get called directly
if [ "`basename "${0}"`" == "build-common.sh" ]; then
	echo "This script should not be called directly. Please execute build-dependencies.sh instead"
	exit 1
fi

# shared variables
DEP_DIR="${PWD}"
PATCH_DIR="${PWD}/../ext/patches"
BUILD_ROOT="/tmp/`whoami`/m2etis"
CPU_CORES=`grep -c processor /proc/cpuinfo`

mkdir -p "${BUILD_ROOT}"

if [ "${1}" = "gcc" ] ; then
	C_COMPILER=gcc
	CXX_COMPILER=g++
	DEP_OUT_DIR=${DEP_DIR}/gcc
fi
if [ "${1}" = "gcc-4.7" ] ; then
	C_COMPILER=gcc-4.7
	CXX_COMPILER=g++-4.7
	DEP_OUT_DIR=${DEP_DIR}/gcc-4.7
fi
if [ "${1}" = "clang" ] ; then
	C_COMPILER=clang
	CXX_COMPILER=clang++
	DEP_OUT_DIR=${DEP_DIR}/clang
fi
if [ "${1}" = "android" ] ; then
	DEP_OUT_DIR=${DEP_DIR}/android
fi

# print titles
title() {
	text="$1"
	echo
	echo
	echo "${text}"
	echo
	echo
}

# print status text
status() {
	text="${1}"
	echo "	${text}"
}

# downloads and unpacks a dependency
downloadAndUnpack() {
	FILE=${1}
	URL=http://www.clockwork-origins.de/dependencies/
	if [ -n "$2" ]; then
		URL=${2}
	fi

	if ! [ -f "${BUILD_ROOT}/${FILE}" ]; then
		wget ${URL}/${FILE} -P ${BUILD_ROOT}
	fi
	FILENAME=$(basename "${ARCHIVE}")
	EXTENSION="${FILENAME##*.}"
	
	cd "${BUILD_ROOT}"
	if [ "${EXTENSION}" == "zip" ]; then
		unzip "${ARCHIVE}"
	fi
	if [ "${EXTENSION}" == "gz" ]; then
		tar xfz "${ARCHIVE}"
	fi
	if [ "${EXTENSION}" == "bz2" ]; then
		tar xfj "${ARCHIVE}"
	fi
}

