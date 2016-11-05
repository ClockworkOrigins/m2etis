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

SET(CMAKE_LIBRARY_PATH ${M2ETIS_DEP_DIR})

#----------------------------------------------------
# Boost
#----------------------------------------------------

IF(WIN32 AND NOT ANDROID)
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-boost.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(WIN32 AND NOT ANDROID)
IF(CMAKE_HOST_WIN32 AND ANDROID)
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-boost-android.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(CMAKE_HOST_WIN32 AND ANDROID)
IF(UNIX AND NOT ANDROID)
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-boost.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND NOT ANDROID)
IF(UNIX AND ANDROID AND NOT CMAKE_HOST_WIN32)
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-boost-android.sh WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND ANDROID AND NOT CMAKE_HOST_WIN32)
SET(LIBNAME "BOOST")
SET(LIBHEADER "boost/thread.hpp")
SET(BOOST_ROOT ${M2ETIS_DEP_DIR}/boost)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_chrono)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_date_time)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_filesystem)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_regex)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_serialization)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_system)
SET(BOOST_COMPONENT ${BOOST_COMPONENT} boost_thread)
find_package(EasyFind REQUIRED COMPONENTS ${BOOST_COMPONENT})
include_directories(SYSTEM ${BOOST_INCLUDE_DIR})

#----------------------------------------------------
# clockUtils
#----------------------------------------------------

IF(WIN32 AND NOT ANDROID)
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-clockUtils.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(WIN32 AND NOT ANDROID)
IF(CMAKE_HOST_WIN32 AND ANDROID)
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-clockUtils-android.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(CMAKE_HOST_WIN32 AND ANDROID)
IF(UNIX AND NOT ANDROID)
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-clockUtils.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND NOT ANDROID)
IF(UNIX AND ANDROID AND NOT CMAKE_HOST_WIN32)
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-clockUtils-android.sh WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
ENDIF(UNIX AND ANDROID AND NOT CMAKE_HOST_WIN32)
SET(LIBNAME "CLOCKUTILS")
SET(LIBHEADER "clockUtils/errors.h")
SET(CLOCKUTILS_ROOT ${M2ETIS_DEP_DIR}/clockUtils)
SET(CLOCKUTILS_COMPONENT ${CLOCKUTILS_COMPONENT} clock_sockets)

find_package(EasyFind REQUIRED COMPONENTS ${CLOCKUTILS_COMPONENT})
include_directories(SYSTEM ${CLOCKUTILS_INCLUDE_DIR})

#----------------------------------------------------
# GFlags
#----------------------------------------------------

IF(WITH_TESTING)
	IF(WIN32 AND NOT ANDROID)
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-gflag.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(WIN32 AND NOT ANDROID)
	IF(UNIX AND NOT ANDROID)
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-gflag.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(UNIX AND NOT ANDROID)
	SET(LIBNAME "GFLAGS")
	SET(LIBHEADER "gflags/gflags.h")
	SET(GFLAGS_ROOT ${M2ETIS_DEP_DIR}/gflags)
	find_package(EasyFind REQUIRED COMPONENTS gflags)
	include_directories(SYSTEM ${GFLAGS_INCLUDE_DIR})
ENDIF(WITH_TESTING)

#----------------------------------------------------
# GLog
#----------------------------------------------------

IF(WITH_LOGGING)
	IF(WIN32 AND NOT ANDROID)
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-glog.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(WIN32 AND NOT ANDROID)
	IF(UNIX AND NOT ANDROID)
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-glog.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(UNIX AND NOT ANDROID)
	SET(LIBNAME "GLOG")
	SET(LIBHEADER "glog/logging.h")
	SET(GLOG_ROOT ${M2ETIS_DEP_DIR}/glog)
	find_package(EasyFind REQUIRED COMPONENTS glog)
	include_directories(SYSTEM ${GLOG_INCLUDE_DIR})
ENDIF(WITH_LOGGING)

#----------------------------------------------------
# GMock
#----------------------------------------------------

IF(WITH_TESTING)
	IF(WIN32 AND NOT ANDROID)
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-gmock.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(WIN32 AND NOT ANDROID)
	IF(UNIX AND NOT ANDROID)
		execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dependencies/build-gmock.sh ${UNIX_COMPILER} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies)
	ENDIF(UNIX AND NOT ANDROID)
	SET(LIBNAME "GTEST")
	SET(LIBHEADER "gtest/gtest.h")
	SET(GTEST_ROOT ${M2ETIS_DEP_DIR}/gmock)
	SET(GTEST_COMPONENT ${GTEST_COMPONENT} gtest)

	find_package(EasyFind REQUIRED COMPONENTS ${GTEST_COMPONENT})
	include_directories(SYSTEM ${GTEST_INCLUDE_DIR})
ENDIF(WITH_TESTING)

#----------------------------------------------------
# PThread
#----------------------------------------------------

IF(UNIX)
	find_package(Threads REQUIRED)
	include_directories(SYSTEM ${PTHREADS_INCLUDE_DIR})
ENDIF(UNIX)
