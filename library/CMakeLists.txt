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
OPTION(WITH_SIM "builds with simulator" OFF)
OPTION(WITH_TESTING "builds with testing code" ON)
OPTION(WITH_CHIMERA "builds system with Chimera network backend" OFF)
OPTION(WITH_PROFILING "builds system with Chimera network backend" OFF)
OPTION(WITH_MESSAGECOMPRESSION "enables compression of messages with zlib" ON)
set(DEP_DIR "${CMAKE_SOURCE_DIR}/../dependencies" CACHE STRING "path to dependencies for m2etis")

set(ADDITIONAL_INCLUDE_DIRECTORIES "" CACHE STRING "Path to yourlib includes")

IF(ADDITIONAL_INCLUDE_DIRECTORIES)
	MESSAGE("${ADDITIONAL_INCLUDE_DIRECTORIES}")
	INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} "${ADDITIONAL_INCLUDE_DIRECTORIES}")

	MESSAGE("${INCLUDE_DIRECTORIES}")
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

#Exports Compilerflag for platform independency (used for declspec macro)
IF(WIN32)
	IF(I6MC_BUILD_SHARED)
		SET(I6MC_DLL 1)
	ENDIF(I6MC_BUILD_SHARED)
ENDIF(WIN32)

IF(I6MC_DLL)
	ADD_DEFINITIONS(-DI6MC_EXPORTS)
	ADD_DEFINITIONS(-DISIXE_BUILD_SHARED)
ENDIF(I6MC_DLL)

#Directory definitions
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/" ${CMAKE_MODULE_PATH})

#Setting Output Directories
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}")
SET(I6MC_DEP_LIB_DIR "${CMAKE_BINARY_DIR}/dist/lib")

#Workaround for Xcode Output Directory Bug
IF(CMAKE_GENERATOR STREQUAL "Xcode")
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/")
ENDIF(CMAKE_GENERATOR STREQUAL "Xcode")

#Setting global m2etis directories
SET(I6MC_DEP_DIR "${CMAKE_SOURCE_DIR}/../dependencies/")
SET(I6MC_SRC_DIR "${CMAKE_SOURCE_DIR}/src/")
SET(I6MC_INC_DIR "${CMAKE_SOURCE_DIR}/include/")
SET(I6MC_TEST_DIR "${CMAKE_SOURCE_DIR}/test/")

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

#
# Dependencies
#
#----------------------------------------------------

#Boost
#---------------------------------------------------
IF(APPLE)
	SET(COMPONENTS thread filesystem serialization system date_time iostreams)
	FIND_PACKAGE(Boost REQUIRED ${COMPONENTS})

	#copy libs to expected dir for os x:
	IF(APPLE)
		foreach(comp ${COMPONENTS})
			configure_file(${Boost_LIBRARY_DIRS}/libboost_${comp}.dylib ${I6MC_DEP_LIB_DIR}/libboost_${comp}.dylib COPYONLY)
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

	SET(BOOST_LIBRARIES ${Boost_LIBRARIES})
ELSE(APPLE)
	SET(LIBNAME "BOOST")
	SET(LIBHEADER "boost/thread.hpp")
	SET(BOOST_ROOT ${DEP_DIR}/boost)
	IF(WITH_MESSAGECOMPRESSION)
		find_package(EasyFind REQUIRED COMPONENTS boost_chrono boost_date_time boost_filesystem boost_iostreams boost_regex boost_serialization boost_system boost_thread)
	ELSE(WITH_MESSAGECOMPRESSION)
		find_package(EasyFind REQUIRED COMPONENTS boost_chrono boost_date_time boost_filesystem boost_regex boost_serialization boost_system boost_thread)
	ENDIF(WITH_MESSAGECOMPRESSION)
	include_directories(SYSTEM ${BOOST_INCLUDE_DIR})
ENDIF(APPLE)

#Chimera
#----------------------------------------------------
IF(WITH_CHIMERA)
	FIND_PACKAGE(Chimera REQUIRED)

	LINK_DIRECTORIES(${LINK_DIRECTORIES} ${CHIMERA_LIB_DIR})
	IF(APPLE)
		configure_file(${CHIMERA_LIB_DIR}/libchimera.1.4.dylib ${I6MC_DEP_LIB_DIR}/libchimera.1.4.dylib COPYONLY)
	        SET(CMAKE_CXX_FLAGS_DEBUG "-isystem ${CHIMERA_INCLUDE_DIR} ${CMAKE_CXX_FLAGS_DEBUG}")
			SET(CMAKE_CXX_FLAGS_RELEASE "-isystem ${CHIMERA_INCLUDE_DIR} ${CMAKE_CXX_FLAGS_RELEASE}")
	ENDIF(APPLE)
	IF(WIN32)
	        INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} "${CHIMERA_INCLUDE_DIR}")
	ENDIF(WIN32)
	IF(UNIX AND NOT APPLE)
		configure_file(${CHIMERA_LIB_DIR}/libchimera.so ${I6MC_DEP_LIB_DIR}/libchimera.so COPYONLY)
		SET(CMAKE_CXX_FLAGS_DEBUG "-isystem ${CHIMERA_INCLUDE_DIR} ${CMAKE_CXX_FLAGS_DEBUG}")
		SET(CMAKE_CXX_FLAGS_RELEASE "-isystem ${CHIMERA_INCLUDE_DIR} ${CMAKE_CXX_FLAGS_RELEASE}")
	ENDIF(UNIX AND NOT APPLE)
ENDIF(WITH_CHIMERA)

#glog
#--------------------------------------
IF(WITH_LOGGING)
	LINK_DIRECTORIES(${LINK_DIRECTORIES} ${GLOG_LIB_DIR})
	SET(LIBNAME "GLOG")
	SET(LIBHEADER "glog/logging.h")
	SET(GLOG_ROOT ${DEP_DIR}/glog)
	IF(NOT WIN32)
		find_package(EasyFind REQUIRED COMPONENTS glog)
	ELSE(NOT WIN32)
		find_package(EasyFind REQUIRED COMPONENTS libglog)
	ENDIF(NOT WIN32)
	include_directories(SYSTEM ${GLOG_INCLUDE_DIR})
ENDIF(WITH_LOGGING)

IF(WITH_PROFILING)
	SET(LIBNAME "GPerfTools")
	SET(LIBHEADER "gperftools/tcmalloc.h")
	SET(GPerfTools_ROOT ${DEP_DIR}/gperftools)
	IF(WIN32)
		find_package(EasyFind REQUIRED COMPONENTS libtcmalloc_minimal)
	ELSE(WIN32)
		find_package(EasyFind REQUIRED COMPONENTS tcmalloc profiler)
	ENDIF(WIN32)
	include_directories(SYSTEM ${GPerfTools_INCLUDE_DIR})

	IF(NOT WIN32)
		SET(CMAKE_CXX_FLAGS_DEBUG "-isystem ${GPerfTools_INCLUDE_DIR} ${CMAKE_CXX_FLAGS}")
		SET(CMAKE_CXX_FLAGS_RELEASE "-isystem ${GPerfTools_INCLUDE_DIR} ${CMAKE_CXX_FLAGS}")
	ENDIF(NOT WIN32)
ENDIF(WITH_PROFILING)

# clockUtils
#----------------------------------------------------
SET(LIBNAME "CLOCKUTILS")
SET(LIBHEADER "clockUtils/errors.h")
SET(CLOCKUTILS_ROOT ${DEP_DIR}/clockUtils)
find_package(EasyFind REQUIRED COMPONENTS clock_sockets)
include_directories(SYSTEM ${CLOCKUTILS_INCLUDE_DIR})

#------
#Build m2etis libs
#------
file(GLOB_RECURSE utils ${I6MC_SRC_DIR}/util/*.cpp)
file(GLOB_RECURSE utils_header ${I6MC_INC_DIR}/util/*.h)

file(GLOB_RECURSE net ${I6MC_SRC_DIR}/net/*.cpp)
file(GLOB_RECURSE net_header ${I6MC_INC_DIR}/m2etis/net/*.h)

file(GLOB_RECURSE message ${I6MC_SRC_DIR}/message/*.cpp)
file(GLOB_RECURSE message_header ${I6MC_INC_DIR}/m2etis/message/*.h)

file(GLOB_RECURSE m2etis_pubsub ${I6MC_SRC_DIR}/pubsub/*.cpp)
file(GLOB_RECURSE m2etis_pubsub_header ${I6MC_INC_DIR}/m2etis/pubsub/*.h)

file(GLOB_RECURSE m2etis_config ${I6MC_SRC_DIR}/config/*.cpp)
file(GLOB_RECURSE m2etis_config_header ${I6MC_INC_DIR}/m2etis/config/*.h)

file(GLOB_RECURSE m2etis_chimera_wrapper ${I6MC_SRC_DIR}/wrapper/chimera/*.cpp)
file(GLOB_RECURSE m2etis_chimera_wrapper_header ${I6MC_INC_DIR}/m2etis/wrapper/chimera/*.h)

file(GLOB_RECURSE m2etis_tcp_wrapper ${I6MC_SRC_DIR}/wrapper/tcp/*.cpp)
file(GLOB_RECURSE m2etis_tcp_wrapper_header ${I6MC_INC_DIR}/m2etis/wrapper/tcp/*.h)

file(GLOB_RECURSE m2etis_clocktcp_wrapper ${I6MC_SRC_DIR}/wrapper/clocktcp/*.cpp)
file(GLOB_RECURSE m2etis_clocktcp_wrapper_header ${I6MC_INC_DIR}/m2etis/wrapper/clocktcp/*.h)

file(GLOB_RECURSE m2etis_udp_wrapper ${I6MC_SRC_DIR}/wrapper/udp/*.cpp)
file(GLOB_RECURSE m2etis_udp_wrapper_header ${I6MC_INC_DIR}/m2etis/wrapper/udp/*.h)

file(GLOB_RECURSE m2etis_simulator_bindings ${I6MC_SRC_DIR}/sim/*.cpp)
file(GLOB_RECURSE m2etis_simulator_bindings_header ${I6MC_INC_DIR}/m2etis/sim/*.h)

#Define IDE source groups
SOURCE_GROUP(util FILES ${utils_header} ${utils})

SOURCE_GROUP(net FILES ${net_header} ${net})

SOURCE_GROUP(message FILES ${message_header} ${message})

SOURCE_GROUP(sim FILES ${m2etis_simulator_bindings_header} ${m2etis_simulator_bindings})

SOURCE_GROUP(pubsub FILES ${m2etis_pubsub_header} ${m2etis_pubsub})

SOURCE_GROUP(config FILES ${m2etis_config_header} ${m2etis_config})

SOURCE_GROUP(wrapper\\tcp FILES ${m2etis_tcp_wrapper_header} ${m2etis_tcp_wrapper})
SOURCE_GROUP(wrapper\\clocktcp FILES ${m2etis_clocktcp_wrapper_header} ${m2etis_clocktcp_wrapper})
SOURCE_GROUP(wrapper\\self FILES ${m2etis_self_wrapper_header} ${m2etis_self_wrapper})
SOURCE_GROUP(wrapper\\udp FILES ${m2etis_udp_wrapper_header} ${m2etis_udp_wrapper})
SOURCE_GROUP(wrapper\\chimera FILES ${m2etis_chimera_wrapper_header} ${m2etis_chimera_wrapper})

#Defines an Option to build i6m2etis_net as shared lib
IF(WIN32)
	IF(I6MC_BUILD_SHARED)
		SET(I6MC_DLL 1)
	ENDIF(I6MC_BUILD_SHARED)
ENDIF(WIN32)
IF(I6MC_DLL)
	ADD_DEFINITIONS(-DI6EC_EXPORTS)
ENDIF(I6MC_DLL)

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} "${I6MC_INC_DIR}")

SET(m2etis_source ${utils_header} ${utils} ${net_header} ${net} ${message_header} ${message} ${m2etis_pubsub_header} ${m2etis_pubsub} ${m2etis_config_header} ${m2etis_config} ${m2etis_tcp_wrapper_header} ${m2etis_tcp_wrapper} ${m2etis_clocktcp_wrapper_header} ${m2etis_clocktcp_wrapper} ${m2etis_udp_wrapper_header} ${m2etis_udp_wrapper})

IF(WITH_SIM)
	SET(m2etis_source ${m2etis_source} ${m2etis_simulator_bindings_header} ${m2etis_simulator_bindings})
ENDIF(WITH_SIM)

IF(WITH_CHIMERA)
	SET(m2etis_source ${m2etis_source} ${m2etis_chimera_wrapper_header} ${m2etis_chimera_wrapper})
ENDIF(WITH_CHIMERA)

IF(I6MC_BUILD_SHARED)
	add_library(m2etis SHARED ${m2etis_source})
ELSE(I6MC_BUILD_SHARED)
	add_library(m2etis STATIC ${m2etis_source})
ENDIF(I6MC_BUILD_SHARED)

IF(WIN32)
	SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /FORCE:MULTIPLE")
ENDIF(WIN32)

target_link_libraries(m2etis ${BOOST_LIBRARIES} ${CLOCKUTILS_LIBRARIES})

IF(WITH_CHIMERA)
	target_link_libraries(m2etis chimera)
ENDIF(WITH_CHIMERA)
IF(WITH_LOGGING)
	target_link_libraries(m2etis ${GLOG_LIBRARIES})
ENDIF(WITH_LOGGING)
IF(WITH_PROFILING)
	target_link_libraries(m2etis ${GPerfTools_LIBRARIES})
ENDIF(WITH_PROFILING)

IF(WIN32)
	add_custom_command(TARGET m2etis POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}/$<CONFIGURATION>/m2etis.dll ${CMAKE_BINARY_DIR}/dist)
ENDIF(WIN32)

# TESTING
IF(WITH_TESTING)
	add_subdirectory(${I6MC_TEST_DIR})
ENDIF(WITH_TESTING)

IF(WIN32)
	file(GLOB_RECURSE files "${DEP_DIR}/*.dll")
	foreach(filename ${files})
		execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${filename} ${CMAKE_BINARY_DIR}/dist)
	endforeach()

	configure_file(${CMAKE_SOURCE_DIR}/../config/build-archive.bat.in ${CMAKE_BINARY_DIR}/build-archive.bat @ONLY)
	
	add_custom_target(BuildReleaseSourceArchive COMMAND build-archive.bat WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	add_custom_target(BuildRevisionSourceArchive COMMAND build-archive.bat packageRevision WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
ENDIF(WIN32)

# INSTALL
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS m2etis
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
