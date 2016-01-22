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

# Find M2ETIS includes and library
#
# This module defines
#  M2ETIS_INCLUDE_DIR
#  M2ETIS_LIBRARIES
#  M2ETIS_LIB_DIR, the location of the libraries
#  M2ETIS_FOUND


IF (M2ETIS_LIBRARIES AND M2ETIS_INCLUDE_DIR)
	SET(M2ETIS_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (M2ETIS_LIBRARIES AND M2ETIS_INCLUDE_DIR)

IF (WIN32) #Windows
	MESSAGE(STATUS "Looking for M2etis")
	SET(M2ETIS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../library/include/)
	SET(M2ETIS_LIB_DIR ${CMAKE_SOURCE_DIR}/../library/dist/Release/) 
	SET(M2ETIS_LIBRARIES debug M2ETIS optimized M2ETIS)
ELSE (WIN32) #Unix
	MESSAGE(STATUS "Looking for M2etis")
	SET(M2ETIS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../library/include/)
	SET(M2ETIS_LIB_DIR ${CMAKE_SOURCE_DIR}/../library/dist/Release/)
	SET(M2ETIS_LIBRARIES ${M2ETIS_LIB_DIR}libm2etis.so CACHE STRING "")
ENDIF (WIN32)


SET(M2ETIS_INCLUDE_DIR ${M2ETIS_INCLUDE_DIR} CACHE PATH "" FORCE)
SET(M2ETIS_LIB_DIR ${M2ETIS_LIB_DIR} CACHE PATH "" FORCE)
SET(M2ETIS_LIBRARIES ${M2ETIS_LIBRARIES} CACHE STRING "" FORCE)

MARK_AS_ADVANCED(M2ETIS_INCLUDE_DIR)
MARK_AS_ADVANCED(M2ETIS_LIB_DIR)
MARK_AS_ADVANCED(M2ETIS_LIBRARIES)

IF (M2ETIS_INCLUDE_DIR AND M2ETIS_LIBRARIES)
	SET(M2ETIS_FOUND TRUE)
ENDIF (M2ETIS_INCLUDE_DIR AND M2ETIS_LIBRARIES)

IF (M2ETIS_FOUND)
	IF (NOT M2ETIS_FIND_QUIETLY)
		MESSAGE(STATUS "  libraries : ${M2ETIS_LIBRARIES} from ${M2ETIS_LIB_DIR}")
		MESSAGE(STATUS "  includes  : ${M2ETIS_INCLUDE_DIR}")
	ENDIF (NOT M2ETIS_FIND_QUIETLY)
ELSE (M2ETIS_FOUND)
	IF (M2ETIS_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could not find M2etis")
	ENDIF (M2ETIS_FIND_REQUIRED)
ENDIF (M2ETIS_FOUND)
