ADD_DEFINITIONS(-DI6E_EXPORTS)

add_executable(m2etis_node m2etis_node.cpp)
target_link_libraries(m2etis_node m2etis ${GFLAGS_LIBRARIES})

IF(NOT WIN32)
	target_link_libraries(m2etis_node pthread)
ENDIF(NOT WIN32)

IF(WIN32)
	add_custom_command(TARGET m2etis_node POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE}/$<CONFIGURATION>/m2etis_node.exe ${CMAKE_BINARY_DIR}/dist)
ENDIF(WIN32)
