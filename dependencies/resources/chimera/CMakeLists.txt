cmake_minimum_required(VERSION 2.8)
SET(CMAKE_OSX_ARCHITECTURES "i386;x86_64")

add_subdirectory(src)

FILE(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
INSTALL(FILES ${headers} DESTINATION include/chimera)

