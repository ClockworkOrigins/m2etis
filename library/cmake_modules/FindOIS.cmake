# Find OIS includes and library
#
# This module defines
#  OIS_INCLUDE_DIR
#  OIS_LIBRARIES, the libraries to link against to use OGRE.
#  OIS_LIB_DIR, the location of the libraries
#  OIS_FOUND, If false, do not try to use OGRE
#
# Copyright � 2007, Matt Williams
#
# Redistribution and use is allowed according to the terms of the BSD license.

IF (OIS_LIBRARIES AND OIS_INCLUDE_DIR)
    SET(OIS_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (OIS_LIBRARIES AND OIS_INCLUDE_DIR)

IF (WIN32) #Windows
    MESSAGE(STATUS "Looking for OIS")
    SET(OIS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dependencies/ois/include/)
    SET(OIS_LIB_DIR ${CMAKE_SOURCE_DIR}/dependencies/ois/lib/win32/) 
    SET(OIS_LIBRARIES debug OIS optimized OIS)
ELSE (WIN32) #Unix
    MESSAGE(STATUS "Looking for OIS")
    SET(OIS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dependencies/ois/include/)
    SET(OIS_LIB_DIR ${CMAKE_SOURCE_DIR}/dependencies/ois/lib/) 
    SET(OIS_LIBRARIES OIS CACHE STRING "")
ENDIF (WIN32)

SET(OIS_INCLUDE_DIR ${OIS_INCLUDE_DIR} CACHE PATH "" FORCE)
SET(OIS_LIB_DIR ${OIS_LIB_DIR} CACHE PATH "" FORCE)
SET(OIS_LIBRARIES ${OIS_LIBRARIES} CACHE STRING "" FORCE)

MARK_AS_ADVANCED(OIS_INCLUDE_DIR)
MARK_AS_ADVANCED(OIS_LIB_DIR)
MARK_AS_ADVANCED(OIS_LIBRARIES)

IF (OIS_INCLUDE_DIR AND OIS_LIBRARIES)
    SET(OIS_FOUND TRUE)
ENDIF (OIS_INCLUDE_DIR AND OIS_LIBRARIES)

IF (OIS_FOUND)
    IF (NOT OIS_FIND_QUIETLY)
        MESSAGE(STATUS "  libraries : ${OIS_LIBRARIES} from ${OIS_LIB_DIR}")
        MESSAGE(STATUS "  includes  : ${OIS_INCLUDE_DIR}")
    ENDIF (NOT OIS_FIND_QUIETLY)
ELSE (OIS_FOUND)
    IF (OIS_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find OIS")
    ENDIF (OIS_FIND_REQUIRED)
ENDIF (OIS_FOUND)
