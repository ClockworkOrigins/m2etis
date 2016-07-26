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

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}")
IF(CMAKE_GENERATOR STREQUAL "Xcode")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist/")
ENDIF(CMAKE_GENERATOR STREQUAL "Xcode")

IF(WITH_LOGGING)
        SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DWITH_LOGGING")
ENDIF(WITH_LOGGING)

IF(WITH_TESTING)
        SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DWITH_TESTING")
ENDIF(WITH_TESTING)

IF(WITH_SIM)
        SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DWITH_SIM")
ENDIF(WITH_SIM)

# GoogleTest
#-----------------------------------------------------
IF(WIN32)
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/deps/build-gmock.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps)
ELSEIF(UNIX)
	execute_process(COMMAND /bin/bash ${CMAKE_SOURCE_DIR}/deps/build-gmock.sh WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps)
ENDIF()

SET(LIBNAME "GTEST")
SET(LIBHEADER "gtest/gtest.h")
SET(GTEST_ROOT ${DEP_DIR}/gmock)
find_package(EasyFind REQUIRED COMPONENTS gmock_main gmock gtest gtest_main)
include_directories(SYSTEM ${GTEST_INCLUDE_DIR})

# gflags
#--------------------------------------

IF(WIN32)
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/deps/build-gflags.bat ${VS_TOOLCHAIN} ${VS_ARCH} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps)
ELSEIF(UNIX)
	execute_process(COMMAND /bin/bash ${CMAKE_SOURCE_DIR}/deps/build-gflags.sh WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps)
ENDIF()

SET(LIBNAME "GFLAGS")
SET(LIBHEADER "gflags/gflags.h")
SET(GFLAGS_ROOT ${DEP_DIR}/gflags)
find_package(EasyFind REQUIRED COMPONENTS gflags)
include_directories(SYSTEM ${GFLAGS_INCLUDE_DIR})

file(GLOB_RECURSE unittests unit/*.h)
add_executable(m2etis_unit_tester unit_tester.cpp ${unittests})
target_link_libraries(m2etis_unit_tester m2etis ${GTEST_LIBRARIES})

IF(NOT WIN32)
	target_link_libraries(m2etis_unit_tester pthread)
ENDIF(NOT WIN32)

IF(WIN32)
	add_custom_command(TARGET m2etis_unit_tester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}/$<CONFIGURATION>/m2etis_unit_tester.exe ${CMAKE_BINARY_DIR}/dist)
ENDIF(WIN32)

add_test(M2etisUnitTests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/m2etis_unit_tester)

file(GLOB_RECURSE channeltests channel/*.h)
add_executable(m2etis_channel_tester channel_tester.cpp ${channeltests})
target_link_libraries(m2etis_channel_tester m2etis ${GTEST_LIBRARIES})

IF(NOT WIN32)
	target_link_libraries(m2etis_channel_tester pthread)
ENDIF(NOT WIN32)

IF(WIN32)
	add_custom_command(TARGET m2etis_channel_tester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}/$<CONFIGURATION>/m2etis_channel_tester.exe ${CMAKE_BINARY_DIR}/dist)
ENDIF(WIN32)

add_subdirectory(integration)
