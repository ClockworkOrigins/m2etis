#!/bin/bash

cd "$(readlink -f "$(dirname "${0}")")"

. ./build-common.sh

# boost
ARCHIVE="boost_1_60_0.tar.bz2"
BUILD_DIR="${BUILD_ROOT}/boost_1_60_0"

PREFIX="${DEP_DIR}/boost/"
DEBUG_FLAG="variant=debug"
RELEASE_FLAG="variant=release"
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
	status "Cleaning Boost"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile Boost"

#if ! uptodate "${ARCHIVE}" "${PREFIX}" && ! uptodate "${BOOST_LOG_ARCHIVE}" "${PREFIX}"; then
#	status "Boost seems to be up to date, skipping build"
#	exit 0
#fi

./download-dependency.sh ${ARCHIVE}

status "Cleaning Boost"
rm -rf "${PREFIX}" >/dev/null

status "Extracting Boost"
cd "${BUILD_ROOT}"
tar xfj "${ARCHIVE}" >/dev/null

status "Bootstrapping Boost"
cd "${BUILD_DIR}"

./bootstrap.sh --prefix="${PREFIX}" --with-libraries=chrono,filesystem,thread,date_time,regex,system,serialization,iostreams > /dev/null

status "Building & Installing Boost"

if [[ $OSTYPE =~ "darwin" ]]; then
  status "Compiling Boost for OS X"
./bjam \
        ${PARALLEL_FLAG}\
        ${BUILD_TYPE}\
        link=shared\
        --layout=system\
        threading=multi\
        architecture=x86\
        address-model=32_64\
	toolset=$COMPILER cxxflags="-std=c++11 $STDLIB" linkflags="$STDLIB"\
        install >/dev/null
else
  ./bjam \
        ${PARALLEL_FLAG}\
        ${BUILD_TYPE}\
        threading=multi\
        --layout=system\
        link=shared\
	toolset=$COMPILER cxxflags="-std=c++11"\
        install >/dev/null
fi

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_DIR}" >/dev/null

if [[ $OSTYPE =~ "darwin" ]]; then
        status "OS X detected: Setting install_names of libs"
        cd ${PREFIX}/lib
        for file in $( ls );
        do
        if  [[ $file =~ ".dylib" ]]; then
            install_name_tool -id @executable_path/../lib/${file} ${file};
        fi
        done
        #install_name_tool -change libboost_thread-mt.dylib @executable_path/../lib/libboost_thread-mt.dylib libboost_log-mt.dylib
    #install_name_tool -change libboost_filesystem-mt.dylib @executable_path/../lib/libboost_filesystem-mt.dylib libboost_log-mt.dylib
    #install_name_tool -change libboost_system-mt.dylib @executable_path/../lib/libboost_system-mt.dylib libboost_log-mt.dylib
    #install_name_tool -change libboost_date_time-mt.dylib @executable_path/../lib/libboost_date_time-mt.dylib libboost_log-mt.dylib

    install_name_tool -change libboost_system-mt.dylib @executable_path/../lib/libboost_system-mt.dylib libboost_filesystem-mt.dylib
	install_name_tool -change libboost_system-mt.dylib @executable_path/../lib/libboost_system-mt.dylib libboost_thread-mt.dylib
fi


touch "${PREFIX}"
rm -rf ${BUILD_ROOT}
