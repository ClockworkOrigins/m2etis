#---
# m2etis CMake Configuration file
# This file builds the m2etis library and a test/evaluation environment
#---
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)


#---
# Global Build Configuration
#---

OPTION(I6MC_BUILD_SHARED "Build i6m2etis_shared" ON)
OPTION(WITH_LOGGING "enables Logging" ON)
OPTION(WITH_PROFILING "builds system with Chimera network backend" OFF)
OPTION(WITH_MESSAGECOMPRESSION "enables compression of messages with zlib" ON)

project(m2etis_integrationtest)

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug CACHE STRING
			"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
		FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

#---
# Global Macros and Definitions
#---

#set standard library
SET(STDLIB "")

#Apple specific stuff
if(APPLE)
	#crappy xcode hack
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

	SET(STDLIB "-stdlib=libc++")

	SET(CMAKE_INSTALL_NAME_DIR @executable_path)
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
endif(APPLE)

SET (CXX_FLAGS "-pedantic -ansi -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wnon-virtual-dtor -Wformat=2 -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wredundant-decls -Wctor-dtor-privacy -Wold-style-cast -Wsign-conversion -Wno-unused-parameter -Wno-long-long -std=c++11 ${STDLIB}")

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
ELSE()
	message (SEND_INFO "Unknown C++ compiler \"${COMPILER_NAME}\". Only GNU and Clang are completely supported.")
ENDIF()

#Exports Compilerflag for platform independency (used for declspec macro)
IF(WIN32)
	IF(I6MC_BUILD_SHARED)
		SET(I6MC_DLL 1)
	ENDIF(I6MC_BUILD_SHARED)
ENDIF(WIN32)

IF(I6MC_DLL)
	ADD_DEFINITIONS(-DI6MC_EXPORTS)
ENDIF(I6MC_DLL)

#Directory definitions
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../library/cmake_modules/" "${CMAKE_SOURCE_DIR}/cmake_modules/" ${CMAKE_MODULE_PATH})

#Setting Output Directories
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
SET(I6MC_DEP_LIB_DIR "${CMAKE_BINARY_DIR}/dist/lib")

#Workaround for Xcode Output Directory Bug
IF(CMAKE_GENERATOR STREQUAL "Xcode")
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
ENDIF(CMAKE_GENERATOR STREQUAL "Xcode")

#Setting global m2etis directories
SET(I6MC_DEP_DIR "${CMAKE_SOURCE_DIR}/../dependencies/")
SET(I6MC_SRC_DIR "${CMAKE_SOURCE_DIR}/src/")
SET(I6MC_INC_DIR "${CMAKE_SOURCE_DIR}/include/")

IF(WITH_LOGGING)
	ADD_DEFINITIONS("-DWITH_LOGGING")
ENDIF(WITH_LOGGING)

IF(WITH_TESTING)
	ADD_DEFINITIONS("-DWITH_TESTING")
ENDIF(WITH_TESTING)

IF(WITH_SIM)
	ADD_DEFINITIONS("-DWITH_SIM")
ENDIF(WITH_SIM)

IF(WITH_MESSAGECOMPRESSION)
	ADD_DEFINITIONS("-DWITH_MESSAGECOMPRESSION")
ENDIF(WITH_MESSAGECOMPRESSION)

#
# Dependencies
#
#----------------------------------------------------


#Boost
#---------------------------------------------------
IF(NOT BOOST_ROOT)
	MESSAGE("Taking default directory to find boost")
	SET (BOOST_ROOT "${CMAKE_SOURCE_DIR}/../dependencies/boost")
ENDIF(NOT BOOST_ROOT)
SET(COMPONENTS thread filesystem serialization system date_time iostreams)
FIND_PACKAGE(Boost REQUIRED ${COMPONENTS})

#copy libs to expected dir for os x:
IF(APPLE)
	foreach(comp ${COMPONENTS})
		configure_file(${Boost_LIBRARY_DIRS}/libboost_${comp}-mt.dylib ${I6MC_DEP_LIB_DIR}/libboost_${comp}-mt.dylib COPYONLY)
	endforeach(comp)
ENDIF(APPLE)

SET(CMAKE_INCLUDE_SYSTEM_FLAG_C "-isystem ")
SET(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-isystem ")

#WORKAROUND FOR XCODE Generator BUG in CMake
IF(CMAKE_GENERATOR STREQUAL "Xcode")
 SET(CMAKE_CXX_FLAGS_DEBUG "-isystem ${Boost_INCLUDE_DIR} ${CMAKE_CXX_FLAGS_DEBUG}")
 SET(CMAKE_CXX_FLAGS_RELEASE "-isystem ${Boost_INCLUDE_DIR} ${CMAKE_CXX_FLAGS_RELEASE}")
ELSE()
 INCLUDE_DIRECTORIES(SYSTEM "${Boost_INCLUDE_DIR}")
ENDIF()

#glog
#--------------------------------------
IF(WITH_LOGGING)
	SET(GLOG_INCLUDE_DIR ${I6MC_DEP_DIR}/glog/include)
	IF(WIN32)
	        INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} "${GLOG_INCLUDE_DIR}")
	ELSE(WIN32)
	        SET(CMAKE_CXX_FLAGS_DEBUG "-isystem ${GLOG_INCLUDE_DIR} ${CMAKE_CXX_FLAGS_DEBUG}")
			SET(CMAKE_CXX_FLAGS_RELEASE "-isystem ${GLOG_INCLUDE_DIR} ${MAKE_CXX_FLAGS_RELEASE}")
	ENDIF(WIN32)

	IF(APPLE)
	       SET(GLOG_LIB_DIR ${I6MC_DEP_DIR}/glog/lib)
	       configure_file(${GLOG_LIB_DIR}/libglog.dylib ${I6MC_DEP_LIB_DIR}/libglog.dylib COPYONLY)
	ENDIF(APPLE)
	IF(UNIX AND NOT APPLE)
	       IF(EXISTS "${I6MC_DEP_DIR}/glog/lib64" AND IS_DIRECTORY "${I6MC_DEP_DIR}/glog/lib64")
	            SET(GLOG_LIB_DIR ${I6MC_DEP_DIR}/glog/lib64)
	       ELSE()
	            SET(GLOG_LIB_DIR ${I6MC_DEP_DIR}/glog/lib)
	       ENDIF()
	            configure_file(${GLOG_LIB_DIR}/libglog.so ${I6MC_DEP_LIB_DIR}/libglog.so COPYONLY)
	ENDIF(UNIX AND NOT APPLE)

	LINK_DIRECTORIES(${LINK_DIRECTORIES} ${GLOG_LIB_DIR})
ENDIF(WITH_LOGGING)

SET(GPerfTools_ROOT ${CMAKE_SOURCE_DIR}/../dependencies/gperftools)
IF(WITH_PROFILING)
	FIND_PACKAGE(GPerftools REQUIRED)

	LINK_DIRECTORIES(${LINK_DIRECTORIES} ${GPERFTOOLS_LIB_DIR})

	IF(WIN32)
		INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} "${GPerfTools_INCLUDE_DIR}")
	ELSE(WIN32)
		SET(CMAKE_CXX_FLAGS_DEBUG "-isystem ${GPerfTools_INCLUDE_DIR} ${CMAKE_CXX_FLAGS}")
		SET(CMAKE_CXX_FLAGS_RELEASE "-isystem ${GPerfTools_INCLUDE_DIR} ${CMAKE_CXX_FLAGS}")
	ENDIF(WIN32)
ENDIF(WITH_PROFILING)

find_package(M2etis REQUIRED)
INCLUDE_DIRECTORIES(SYSTEM ${M2ETIS_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} "${I6MC_INC_DIR}")

SET(integrationtest_source
	${integrationtest_source}
	${CMAKE_SOURCE_DIR}/src/main.cpp
	${CMAKE_SOURCE_DIR}/src/TupleFeeder.cpp
)

add_executable(TupleFeeder ${integrationtest_source})

set_target_properties(TupleFeeder PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(TupleFeeder ${Boost_LIBRARIES} ${M2ETIS_LIBRARIES})

IF(WITH_LOGGING)
	target_link_libraries(TupleFeeder glog)
ENDIF(WITH_LOGGING)
IF(WITH_PROFILING)
	target_link_libraries(TupleFeeder ${GPerfTools_LIBRARIES})
ENDIF(WITH_PROFILING)
