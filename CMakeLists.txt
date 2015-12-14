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
OPTION(WITH_SIM "builds with simulator" OFF)
OPTION(WITH_TESTING "builds with testing code" OFF)
OPTION(WITH_CHIMERA "builds system with Chimera network backend" OFF)
OPTION(WITH_PROFILING "builds system with Chimera network backend" OFF)
OPTION(WITH_MESSAGECOMPRESSION "enables compression of messages with zlib" OFF)
OPTION(WITH_DOCU "enables docu build" OFF)

IF(WIN32)
	IF(CMAKE_GENERATOR MATCHES "Visual Studio 12")
		SET(DEP_DIR_BUILD "msvc12_")
	ELSEIF(CMAKE_GENERATOR MATCHES "Visual Studio 14")
		SET(DEP_DIR_BUILD "msvc14_")
	ELSE()
		MESSAGE([WARNING] "Toolchain not supported yet!")
	ENDIF()

	IF(CMAKE_GENERATOR MATCHES "Win64")
		SET(DEP_DIR_BUILD "${DEP_DIR_BUILD}x64")
	ELSEIF(CMAKE_GENERATOR MATCHES "ARM")
		SET(DEP_DIR_BUILD "${DEP_DIR_BUILD}arm")
	ELSE()
		SET(DEP_DIR_BUILD "${DEP_DIR_BUILD}x86")
	ENDIF()
ENDIF(WIN32)

set(DEP_DIR "${CMAKE_SOURCE_DIR}/dependencies/${DEP_DIR_BUILD}" CACHE STRING "path to dependencies for m2etis")

set(ADDITIONAL_INCLUDE_DIRECTORIES "" CACHE STRING "Path to yourlib includes")

IF(ADDITIONAL_INCLUDE_DIRECTORIES)
	INCLUDE_DIRECTORIES(${ADDITIONAL_INCLUDE_DIRECTORIES})
ENDIF(ADDITIONAL_INCLUDE_DIRECTORIES)

project(m2etis)

IF(WITH_SIM)
	SET(WITH_TESTING OFF)
	message("INFO disabled Testing because Simulator is used")
ENDIF(WITH_SIM)

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug CACHE STRING
			"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
		FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# Setting last version
SET(LAST_VERSION_MAJOR 0)
SET(LAST_VERSION_MINOR 3)
SET(LAST_VERSION_PATCH 0)

# Setting current version
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 4)
SET(VERSION_PATCH 0)

#---
# Global Macros and Definitions
#---

# set standard library
SET(STDLIB "")

# Apple specific stuff
if(APPLE)
	#crappy xcode hack
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

	SET(STDLIB "-stdlib=libc++")

	SET(CMAKE_INSTALL_NAME_DIR @executable_path)
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
endif(APPLE)

SET (CXX_FLAGS "-pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wnon-virtual-dtor -Wformat=2 -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wredundant-decls -Wctor-dtor-privacy -Wold-style-cast -Wsign-conversion -Wno-unused-parameter -Wno-long-long -std=c++11 ${STDLIB}")

IF(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
	SET (CMAKE_CXX_FLAGS			"${CXX_FLAGS} -O0")
	SET (CMAKE_CXX_FLAGS_DEBUG		"${CXX_FLAGS} -O0 -g")
	SET (CMAKE_CXX_FLAGS_MINSIZEREL		"${CXX_FLAGS} -0s -DNDEBUG")
	SET (CMAKE_CXX_FLAGS_RELEASE		"${CXX_FLAGS} -O3 -DNDEBUG")
	SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CXX_FLAGS} -O2 -g")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	SET (CMAKE_CXX_FLAGS			"${CXX_FLAGS} -O0")
	SET (CMAKE_CXX_FLAGS_DEBUG		"${CXX_FLAGS} -O0 -g")
	SET (CMAKE_CXX_FLAGS_MINSIZEREL		"${CXX_FLAGS} -0s -DNDEBUG")
	SET (CMAKE_CXX_FLAGS_RELEASE		"${CXX_FLAGS} -O3 -DNDEBUG")
	SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CXX_FLAGS} -O2 -g")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	set (CMAKE_CXX_FLAGS_DEBUG		"${CMAKE_CXX_FLAGS_DEBUG} /MP /bigobj /W4")
	set (CMAKE_CXX_FLAGS_MINSIZEREL		"${CMAKE_CXX_FLAGS_MINSIZEREL} /MP /bigobj /W4")
	set (CMAKE_CXX_FLAGS_RELEASE		"${CMAKE_CXX_FLAGS_RELEASE} /MP /bigobj /W4")
	set (CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MP /bigobj /W4")
ELSE()
	message (SEND_INFO "Unknown C++ compiler \"${CMAKE_CXX_COMPILER_ID}\". Only GNU, Clang and MSVC are completely supported.")
ENDIF()

#Directory definitions
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/library/cmake_modules/" ${CMAKE_MODULE_PATH})

#Setting Output Directories
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}")

#Workaround for Xcode Output Directory Bug
IF(CMAKE_GENERATOR STREQUAL "Xcode")
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/")
ENDIF(CMAKE_GENERATOR STREQUAL "Xcode")

#Setting global m2etis directories
SET(I6MC_DEP_DIR "${DEP_DIR}")
SET(I6MC_SRC_DIR "${CMAKE_SOURCE_DIR}/library/src/")
SET(I6MC_INC_DIR "${CMAKE_SOURCE_DIR}/library/include/")
SET(I6MC_TEST_DIR "${CMAKE_SOURCE_DIR}/library/test/")

IF(WITH_LOGGING)
	ADD_DEFINITIONS("-DWITH_LOGGING")
ENDIF(WITH_LOGGING)

IF(WITH_TESTING)
	ADD_DEFINITIONS("-DWITH_TESTING")
ENDIF(WITH_TESTING)

IF(WITH_SIM)
	ADD_DEFINITIONS("-DWITH_SIM")
ENDIF(WITH_SIM)

IF(WITH_CHIMERA)
	ADD_DEFINITIONS("-DWITH_CHIMERA")
ENDIF(WITH_CHIMERA)

IF(WITH_MESSAGECOMPRESSION)
	ADD_DEFINITIONS("-DWITH_MESSAGECOMPRESSION")
ENDIF(WITH_MESSAGECOMPRESSION)

IF(WITH_DOCU)
	find_package(Doxygen)
	IF(DOXYGEN_FOUND)
		add_subdirectory(docs)
	ENDIF(DOXYGEN_FOUND)
ENDIF(WITH_DOCU)

#------
#Build m2etis libs
#------

configure_file(${CMAKE_SOURCE_DIR}/config/Config.h.in ${CMAKE_BINARY_DIR}/include/m2etis/Config.h @ONLY)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)

ADD_SUBDIRECTORY(library)

IF(WIN32)
	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

	file(GLOB_RECURSE files "${DEP_DIR}/*.dll")
	foreach(filename ${files})
		execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${filename} ${CMAKE_BINARY_DIR}/dist)
	endforeach()

	configure_file(${CMAKE_SOURCE_DIR}/config/build-archive.bat.in ${CMAKE_BINARY_DIR}/build-archive.bat @ONLY)
	
	add_custom_target(BuildReleaseSourceArchive COMMAND build-archive.bat WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	add_custom_target(BuildRevisionSourceArchive COMMAND build-archive.bat packageRevision WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

	# Script testing all channel configurations
	configure_file(${CMAKE_SOURCE_DIR}/config/test-channelconfigurations.bat.in ${CMAKE_BINARY_DIR}/test-channelconfigurations.bat @ONLY)
	
	add_custom_target(TestChannelConfigurations COMMAND test-channelconfigurations.bat WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
ELSEIF(UNIX)
	# Script testing all channel configurations
	configure_file(${CMAKE_SOURCE_DIR}/config/test-channelconfigurations.sh.in ${CMAKE_BINARY_DIR}/test-channelconfigurations.sh @ONLY)
	
	add_custom_target(TestChannelConfigurations COMMAND ./test-channelconfigurations.sh Release WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	add_custom_target(TestChannelConfigurationsWithValgrind COMMAND ./test-channelconfigurations.sh RelWithDebInfo valgrind WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
ENDIF(WIN32)

# INSTALL
install(DIRECTORY ${CMAKE_BINARY_DIR}/include DESTINATION ${CMAKE_INSTALL_PREFIX})
