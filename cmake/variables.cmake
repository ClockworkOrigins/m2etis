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

IF(ANDROID)
	ADD_DEFINITIONS(-DANDROID)
	SET(ANDROID_STL "gnustl_shared")
	SET(DEP_DIR_BUILD android)
	SET(ANDROID_SDK $ENV{ANDROID_HOME} CACHE PATH "Path to ANDROID SDK")
ELSEIF(UNIX)
	IF(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
		SET(DEP_DIR_BUILD "gcc")
		SET(UNIX_COMPILER "gcc")
		IF(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "4.8")
			SET(UNIX_COMPILER "gcc-4.7")
			SET(DEP_DIR_BUILD "gcc-4.7")
		ENDIF()
	ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
		SET(DEP_DIR_BUILD "clang")
		SET(UNIX_COMPILER "clang")
	ENDIF()
ELSEIF(WIN32)
	IF(CMAKE_GENERATOR MATCHES "Visual Studio 12 2013 Win64")
		SET(DEP_DIR_BUILD "msvc13_64")
		SET(VS_TOOLCHAIN "msvc13")
		SET(VSENV VS12)
		SET(VS_ARCH "64")
		SET(VSSCRIPTARCH "amd64")
	ELSEIF(CMAKE_GENERATOR MATCHES "Visual Studio 12 2013")
		SET(DEP_DIR_BUILD "msvc13_32")
		SET(VS_TOOLCHAIN "msvc13")
		SET(VSENV VS12)
		SET(VS_ARCH "32")
		SET(VSSCRIPTARCH "x86")
	ELSEIF(CMAKE_GENERATOR MATCHES "Visual Studio 14 2015 Win64")
		SET(DEP_DIR_BUILD "msvc15_64")
		SET(VS_TOOLCHAIN "msvc15")
		SET(VSENV VS15)
		SET(VS_ARCH "64")
		SET(VSSCRIPTARCH "amd64")
	ELSEIF(CMAKE_GENERATOR MATCHES "Visual Studio 14 2015")
		SET(DEP_DIR_BUILD "msvc15_32")
		SET(VS_TOOLCHAIN "msvc15")
		SET(VSENV VS15)
		SET(VS_ARCH "32")
		SET(VSSCRIPTARCH "x86")
	ENDIF()
ENDIF()

SET(M2ETIS_DEP_DIR "${CMAKE_SOURCE_DIR}/dependencies/${DEP_DIR_BUILD}" CACHE STRING "path to deps for m2etis")

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug CACHE STRING
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
		FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

#########################################################################
# Global Macros and Definitions
#########################################################################

# Sets appropriate Compilerflags

SET(CXX_FLAGS "-pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wnon-virtual-dtor -Wformat=2 -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wredundant-decls -Wctor-dtor-privacy -Wno-sign-conversion -Wno-unused-parameter -Wno-long-long -fPIC -std=c++11")
SET(CLANG_FLAGS "-Wstring-plus-int")

IF(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	SET(CMAKE_CXX_FLAGS_DEBUG		"${CXX_FLAGS} -g")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL		"${CXX_FLAGS} -0s -DNDEBUG")
	SET(CMAKE_CXX_FLAGS_RELEASE		"${CXX_FLAGS} -O3 -DNDEBUG")
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CXX_FLAGS} -O2 -g")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
	SET(CMAKE_CXX_FLAGS_DEBUG		"${CXX_FLAGS} ${CLANG_FLAGS} -g")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL		"${CXX_FLAGS} ${CLANG_FLAGS} -0s -DNDEBUG")
	SET(CMAKE_CXX_FLAGS_RELEASE		"${CXX_FLAGS} ${CLANG_FLAGS} -O3 -DNDEBUG")
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CXX_FLAGS} ${CLANG_FLAGS} -O2 -g")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	SET(CXX_FLAGS "/MP /W4 /wd4127 /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /D_WINSOCK_DEPRECATED_NO_WARNINGS")
	SET(CMAKE_CXX_FLAGS_DEBUG		"${CMAKE_CXX_FLAGS_DEBUG} ${CXX_FLAGS}")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL		"${CMAKE_CXX_FLAGS_MINSIZEREL} ${CXX_FLAGS}")
	SET(CMAKE_CXX_FLAGS_RELEASE		"${CMAKE_CXX_FLAGS_RELEASE} ${CXX_FLAGS}")
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CXX_FLAGS}")
ELSE()
	MESSAGE(SEND_FATAL "Unknown C++ compiler \"${CMAKE_CXX_COMPILER_ID}\".")
ENDIF()

#########################################################################
# Global Directory Definitions
#########################################################################

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
