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

#GoogleTest
#-----------------------------------------------------
SET (GTest_ROOT "${DEP_DIR}/gmock")
FIND_PACKAGE(GTest REQUIRED)

INCLUDE_DIRECTORIES(SYSTEM ${GTest_INCLUDE_DIR})
LINK_DIRECTORIES(${GTest_LIB_DIR})


#gflags
#--------------------------------------

	SET(LIBNAME "GFLAGS")
	SET(LIBHEADER "gflags/gflags.h")
	SET(GFLAGS_ROOT ${DEP_DIR}/gflags)
	find_package(EasyFind REQUIRED COMPONENTS gflags)
	include_directories(SYSTEM ${GFLAGS_INCLUDE_DIR})

	LINK_DIRECTORIES(${LINK_DIRECTORIES} ${GFLAGS_LIB_DIR})


enable_testing()

file(GLOB_RECURSE unittests unit/*.h)
add_executable(m2etis_unit_tester unit_tester.cpp ${unittests})
target_link_libraries(m2etis_unit_tester ${GTest_LIBRARIES} m2etis)

IF(NOT WIN32)
	target_link_libraries(m2etis_unit_tester pthread)
ENDIF(NOT WIN32)

IF(WIN32)
	add_custom_command(TARGET m2etis_unit_tester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}/$<CONFIGURATION>/m2etis_unit_tester.exe ${CMAKE_BINARY_DIR}/dist)
ENDIF(WIN32)

add_test(M2etisUnitTests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/m2etis_unit_tester)

file(GLOB_RECURSE channeltests channel/*.h)
add_executable(m2etis_channel_tester channel_tester.cpp ${channeltests})
target_link_libraries(m2etis_channel_tester  ${GTest_LIBRARIES} m2etis)

IF(NOT WIN32)
	target_link_libraries(m2etis_channel_tester pthread)
ENDIF(NOT WIN32)

IF(WIN32)
	add_custom_command(TARGET m2etis_channel_tester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}/$<CONFIGURATION>/m2etis_channel_tester.exe ${CMAKE_BINARY_DIR}/dist)
ENDIF(WIN32)

add_subdirectory(integration)
