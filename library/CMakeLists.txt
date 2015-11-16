#---
# m2etis CMake Configuration file
# This file builds the m2etis library and a test/evaluation environment
#---
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
	find_package(EasyFind REQUIRED COMPONENTS glog)
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

	IF(APPLE)
		SET(CMAKE_CXX_FLAGS_DEBUG "-isystem ${GPerfTools_INCLUDE_DIR} ${CMAKE_CXX_FLAGS}")
		SET(CMAKE_CXX_FLAGS_RELEASE "-isystem ${GPerfTools_INCLUDE_DIR} ${CMAKE_CXX_FLAGS}")
	ENDIF(APPLE)
ENDIF(WITH_PROFILING)

# clockUtils
#----------------------------------------------------
SET(LIBNAME "CLOCKUTILS")
SET(LIBHEADER "clockUtils/errors.h")
SET(CLOCKUTILS_ROOT ${DEP_DIR}/clockUtils)
find_package(EasyFind REQUIRED COMPONENTS clock_sockets)
include_directories(SYSTEM ${CLOCKUTILS_INCLUDE_DIR})

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

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} "${I6MC_INC_DIR}")

SET(m2etis_source ${utils_header} ${utils} ${net_header} ${net} ${message_header} ${message} ${m2etis_pubsub_header} ${m2etis_pubsub} ${m2etis_config_header} ${m2etis_config} ${m2etis_tcp_wrapper_header} ${m2etis_tcp_wrapper} ${m2etis_clocktcp_wrapper_header} ${m2etis_clocktcp_wrapper} ${m2etis_udp_wrapper_header} ${m2etis_udp_wrapper})

IF(WITH_SIM)
	SET(m2etis_source ${m2etis_source} ${m2etis_simulator_bindings_header} ${m2etis_simulator_bindings})
ENDIF(WITH_SIM)

IF(WITH_CHIMERA)
	SET(m2etis_source ${m2etis_source} ${m2etis_chimera_wrapper_header} ${m2etis_chimera_wrapper})
ENDIF(WITH_CHIMERA)

IF(M2ETIS_BUILD_SHARED)
	add_library(m2etis SHARED ${m2etis_source})
ELSE(M2ETIS_BUILD_SHARED)
	add_library(m2etis STATIC ${m2etis_source})
ENDIF(M2ETIS_BUILD_SHARED)

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

IF(WIN32 AND M2ETIS_BUILD_SHARED)
	add_custom_command(TARGET m2etis POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}/$<CONFIGURATION>/m2etis.dll ${CMAKE_BINARY_DIR}/dist/)
ENDIF(WIN32 AND M2ETIS_BUILD_SHARED)

# TESTING
IF(WITH_TESTING)
	add_subdirectory(${I6MC_TEST_DIR})
ENDIF(WITH_TESTING)

# INSTALL
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS m2etis
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
