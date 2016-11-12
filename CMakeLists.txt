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

#---
# m2etis CMake Configuration file
# This file builds the m2etis library and a test/evaluation environment
#---
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)


#---
# Global Build Configuration
#---

OPTION(M2ETIS_BUILD_SHARED "Build m2etis shared" ON)
OPTION(WITH_LOGGING "enables Logging" OFF)
OPTION(WITH_TESTING "builds with testing code" OFF)

set(ADDITIONAL_INCLUDE_DIRECTORIES "" CACHE STRING "Path to yourlib includes")

IF(ADDITIONAL_INCLUDE_DIRECTORIES)
	INCLUDE_DIRECTORIES(${ADDITIONAL_INCLUDE_DIRECTORIES})
ENDIF(ADDITIONAL_INCLUDE_DIRECTORIES)

project(m2etis)

include(cmake/variables.cmake)

IF(WIN32)
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /bigobj")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /bigobj")
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /bigobj")
ENDIF(WIN32)

# Setting last version
SET(LAST_VERSION_MAJOR 0)
SET(LAST_VERSION_MINOR 4)
SET(LAST_VERSION_PATCH 0)

# Setting current version
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 5)
SET(VERSION_PATCH 0)

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin)

#Setting global m2etis directories
SET(I6MC_SRC_DIR "${CMAKE_SOURCE_DIR}/library/src/")
SET(I6MC_INC_DIR "${CMAKE_SOURCE_DIR}/library/include/")
SET(I6MC_TEST_DIR "${CMAKE_SOURCE_DIR}/library/test/")

IF(WITH_LOGGING)
	ADD_DEFINITIONS("-DWITH_LOGGING")
ENDIF(WITH_LOGGING)

IF(WITH_TESTING)
	ADD_DEFINITIONS("-DWITH_TESTING")
ENDIF(WITH_TESTING)

IF(WIN32)
	ADD_DEFINITIONS("-D_WIN32_WINNT=0x0501")
ENDIF(WIN32)

find_package(Doxygen)
IF(DOXYGEN_FOUND)
	add_subdirectory(docs)
ENDIF(DOXYGEN_FOUND)

#------
#Build m2etis libs
#------

configure_file(${CMAKE_SOURCE_DIR}/config/Config.h.in ${CMAKE_BINARY_DIR}/include/m2etis/Config.h @ONLY)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)

include(cmake/searchDependencies.cmake)

ADD_SUBDIRECTORY(library)
ADD_SUBDIRECTORY(tools)

include(cmake/addToBinDir.cmake)

IF(WIN32)
	configure_file(${CMAKE_SOURCE_DIR}/config/build-archive.bat.in ${CMAKE_BINARY_DIR}/build-archive.bat @ONLY)
	
	add_custom_target(BuildReleaseSourceArchive COMMAND build-archive.bat WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	add_custom_target(BuildRevisionSourceArchive COMMAND build-archive.bat packageRevision WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

	# Script testing all channel configurations
	configure_file(${CMAKE_SOURCE_DIR}/config/test-channelconfigurations.bat.in ${CMAKE_BINARY_DIR}/test-channelconfigurations.bat @ONLY)
	configure_file(${CMAKE_SOURCE_DIR}/config/profile-channel.bat.in ${CMAKE_BINARY_DIR}/profile-channel.bat @ONLY)
	
	add_custom_target(TestChannelConfigurations COMMAND test-channelconfigurations.bat Release WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	add_custom_target(TestChannelConfigurationsWithProfiler COMMAND test-channelconfigurations.bat RelWithDebInfo profiler WORKING_DIRECTORY ${CMAKE_BINARY_DIR} DEPENDS csvToDotConverter)
ELSEIF(UNIX)
	# Script testing all channel configurations
	configure_file(${CMAKE_SOURCE_DIR}/config/test-channelconfigurations.sh.in ${CMAKE_BINARY_DIR}/test-channelconfigurations.sh @ONLY)
	
	add_custom_target(TestChannelConfigurations COMMAND ./test-channelconfigurations.sh Release WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	add_custom_target(TestChannelConfigurationsWithValgrind COMMAND ./test-channelconfigurations.sh RelWithDebInfo valgrind WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	add_custom_target(TestChannelConfigurationsWithProfiler COMMAND ./test-channelconfigurations.sh RelWithDebInfo profiler WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
ENDIF(WIN32)

# INSTALL
install(DIRECTORY ${CMAKE_BINARY_DIR}/include DESTINATION ${CMAKE_INSTALL_PREFIX})
