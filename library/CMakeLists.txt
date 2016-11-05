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

# Generated Channels
configure_file(${CMAKE_SOURCE_DIR}/library/generated/GeneratedChannelConfiguration.h ${CMAKE_BINARY_DIR}/include/m2etis/config/GeneratedChannelConfiguration.h COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/library/generated/GeneratedChannelName.h ${CMAKE_BINARY_DIR}/include/m2etis/config/GeneratedChannelName.h COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/library/generated/GeneratedEventTypes.h ${CMAKE_BINARY_DIR}/include/m2etis/config/GeneratedEventTypes.h COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/library/generated/GeneratedMessageConfiguration.h ${CMAKE_BINARY_DIR}/include/m2etis/config/GeneratedMessageConfiguration.h COPYONLY)


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

file(GLOB_RECURSE m2etis_tcp_wrapper ${I6MC_SRC_DIR}/wrapper/tcp/*.cpp)
file(GLOB_RECURSE m2etis_tcp_wrapper_header ${I6MC_INC_DIR}/m2etis/wrapper/tcp/*.h)

file(GLOB_RECURSE m2etis_clocktcp_wrapper ${I6MC_SRC_DIR}/wrapper/clocktcp/*.cpp)
file(GLOB_RECURSE m2etis_clocktcp_wrapper_header ${I6MC_INC_DIR}/m2etis/wrapper/clocktcp/*.h)

file(GLOB_RECURSE m2etis_udp_wrapper ${I6MC_SRC_DIR}/wrapper/udp/*.cpp)
file(GLOB_RECURSE m2etis_udp_wrapper_header ${I6MC_INC_DIR}/m2etis/wrapper/udp/*.h)

#Define IDE source groups
SOURCE_GROUP(util FILES ${utils_header} ${utils})

SOURCE_GROUP(net FILES ${net_header} ${net})

SOURCE_GROUP(message FILES ${message_header} ${message})

SOURCE_GROUP(pubsub FILES ${m2etis_pubsub_header} ${m2etis_pubsub})

SOURCE_GROUP(config FILES ${m2etis_config_header} ${m2etis_config})

SOURCE_GROUP(wrapper\\tcp FILES ${m2etis_tcp_wrapper_header} ${m2etis_tcp_wrapper})
SOURCE_GROUP(wrapper\\clocktcp FILES ${m2etis_clocktcp_wrapper_header} ${m2etis_clocktcp_wrapper})
SOURCE_GROUP(wrapper\\udp FILES ${m2etis_udp_wrapper_header} ${m2etis_udp_wrapper})

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} "${I6MC_INC_DIR}")

SET(m2etis_source ${utils_header} ${utils} ${net_header} ${net} ${message_header} ${message} ${m2etis_pubsub_header} ${m2etis_pubsub} ${m2etis_config_header} ${m2etis_config} ${m2etis_tcp_wrapper_header} ${m2etis_tcp_wrapper} ${m2etis_clocktcp_wrapper_header} ${m2etis_clocktcp_wrapper} ${m2etis_udp_wrapper_header} ${m2etis_udp_wrapper})

IF(M2ETIS_BUILD_SHARED)
	add_library(m2etis SHARED ${m2etis_source})
ELSE(M2ETIS_BUILD_SHARED)
	add_library(m2etis STATIC ${m2etis_source})
ENDIF(M2ETIS_BUILD_SHARED)

IF(WIN32)
	SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /FORCE:MULTIPLE")
ENDIF(WIN32)

target_link_libraries(m2etis ${BOOST_LIBRARIES} ${CLOCKUTILS_LIBRARIES})

IF(WITH_LOGGING)
	target_link_libraries(m2etis ${GLOG_LIBRARIES})
ENDIF(WITH_LOGGING)

IF(WIN32 AND M2ETIS_BUILD_SHARED)
	add_custom_command(TARGET m2etis POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/m2etis.dll ${CMAKE_BINARY_DIR}/bin/)
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
