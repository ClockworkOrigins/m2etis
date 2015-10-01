message(STATUS "Own APR-util (apache portable runtime utilities) Find Module")
# - Try to find APR-util
# Find local APR-util includes and library
# This module defines:
#  	APR-UTIL_FOUND - If false, do not try to use APR-UTIL.
# On success this module defines:
#  	APR-UTIL_INCLUDE_DIR - where to find apr_base64.h, etc.
#  	APR-UTIL_LIBRARY - where to find the libaprutil-1.so/libaprutil-1.dll library.
#  	APR-UTIL_INCLUDE_DIRS - ALL include paths needed to use APR-util.
#  	APR-UTIL_LIBRARIES - ALL libraries needed to use APR-util.
# See wiki for instructions.
#  	If you prefer a custom installation directory,
#	please set APR-UTIL_CUSTOM_INCLUDE_DIR and APR-UTIL_CUSTOM_LIBRARY accordingly.
# SET(APR-UTIL_CUSTOM_INCLUDE_DIR .../apr-util/include)
# SET(APR-UTIL_CUSTOM_LIBRARY .../apr-util/.libs/libaprutil-1.so)
# SET(APR-UTIL_CUSTOM_LIBRARY .../apr-util/Debug/libaprutil-1.lib|.dll)

# tbf: making this file a little bit version-independent by using
# APR-UTIL_VERSION,APR-UTIL_LIB_VERSION,APR-UTIL_MAJOR_VERSION,APR-UTIL_MINOR_VERSION,APR-UTIL_SUBMINOR_VERSION
# NOTE: critical changes in dir-structure are NOT fixed by that !
# They cause this file to fail if the APR-util is not installed in standard system-paths.
# tbf: usage of APR-UTIL_ROOT and FindPkgMacros.cmake

#INCLUDE(LibFindMacros)

IF (NOT APR_INCLUDE_DIRS OR NOT APR_LIBRARIES)
	IF (APR-UTIL_FIND_REQUIRED)
		FIND_PACKAGE(APR REQUIRED)
	ELSE (APR-UTIL_FIND_REQUIRED)
		FIND_PACKAGE(APR QUIETLY)
	ENDIF (APR-UTIL_FIND_REQUIRED)
ENDIF (NOT APR_INCLUDE_DIRS OR NOT APR_LIBRARIES)

# Dependencies
#LIBFIND_PACKAGE(APR-UTIL apr)

# Use pkg-config to get hints about paths
#LIBFIND_PKG_CHECK_MODULES(APR-UTIL_PKGCONF apr)

# Search Include dir if User didn't set APR-UTIL_CUSTOM_INCLUDE_DIR
IF(APR-UTIL_CUSTOM_INCLUDE_DIR)
	SET(APR-UTIL_INCLUDE_DIR ${APR-UTIL_CUSTOM_INCLUDE_DIR})
ELSE(APR-UTIL_CUSTOM_INCLUDE_DIR)
	# different header files for UNIX and WIN32.
	# So to avoid errors confusing people I added UNIX/WIN32 seperation.
	IF(UNIX)
		FIND_PATH(APR-UTIL_INCLUDE_DIR
			NAMES apr_base64.h
			HINTS ${APR-UTIL_PKGCONF_INCLUDE_DIRS}
			PATHS /usr/include/apr-1
				${CMAKE_SOURCE_DIR}/lib/log4cxx/log4cxxLocalNotSVN/apr-util-1.3.9/include
				${CMAKE_SOURCE_DIR}/lib/log4cxx/log4cxxLocalNotSVN/apr-util/include
		)
	ENDIF(UNIX)
	
	IF(WIN32)
		FIND_PATH(APR-UTIL_INCLUDE_DIR
			NAMES apr_base64.h
			HINTS ${APR-UTIL_PKGCONF_INCLUDE_DIRS}
			PATHS ${CMAKE_SOURCE_DIR}/lib/log4cxx/includes/apr-util-1.3.9/include
		)
	ENDIF(WIN32)

ENDIF(APR-UTIL_CUSTOM_INCLUDE_DIR)

IF(APR-UTIL_CUSTOM_LIBRARY)
	SET(APR-UTIL_LIBRARY ${APR-UTIL_CUSTOM_LIBRARY})
ELSE(APR-UTIL_CUSTOM_LIBRARY)
	IF (UNIX)
		# Finally the library itself
		FIND_LIBRARY(APR-UTIL_LIBRARY
		  NAMES libaprutil-1.so
		  HINTS ${APR-UTIL_PKGCONF_LIBRARY_DIRS}
		  PATHS ${CMAKE_SOURCE_DIR}/lib/log4cxx/log4cxxLocalNotSVN/apr-util-1.3.9/.libs
		  	${CMAKE_SOURCE_DIR}/lib/log4cxx/log4cxxLocalNotSVN/apr-util/.libs
		)
	ENDIF (UNIX)
	
	IF (WIN32)
		# Finally the library itself
		FIND_LIBRARY(APR-UTIL_LIBRARY
		  NAMES libaprutil-1.lib libaprutil-1.dll
		  HINTS ${APR-UTIL_PKGCONF_LIBRARY_DIRS}
		  PATHS ${CMAKE_SOURCE_DIR}/lib/log4cxx/win32
		)
	ENDIF (WIN32)
ENDIF(APR-UTIL_CUSTOM_LIBRARY)

MARK_AS_ADVANCED(APR-UTIL_LIBRARY APR-UTIL_INCLUDE_DIR)

IF (APR-UTIL_LIBRARY AND APR-UTIL_INCLUDE_DIR)
  SET(APR-UTIL_FOUND "TRUE")
ELSE (APR-UTIL_LIBRARY AND APR-UTIL_INCLUDE_DIR)
  SET(APR-UTIL_FOUND "FALSE")
ENDIF (APR-UTIL_LIBRARY AND APR-UTIL_INCLUDE_DIR)

IF (APR-UTIL_FOUND)
   IF (NOT APR-UTIL_FIND_QUIETLY)
      MESSAGE(STATUS "Found APR-UTIL: ${APR-UTIL_LIBRARY}")
   ENDIF (NOT APR-UTIL_FIND_QUIETLY)
ELSE (APR-UTIL_FOUND)
   IF (APR-UTIL_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "
			Could not find APR-util library. See wiki for instructions.
			If you prefer a custom installation directory,
			please set APR-UTIL_CUSTOM_INCLUDE_DIR and APR-UTIL_CUSTOM_LIBRARY 
			accordingly.
		")
   ENDIF (APR-UTIL_FIND_REQUIRED)
ENDIF (APR-UTIL_FOUND)


# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this lib depends on.
SET(APR-UTIL_INCLUDE_DIRS ${APR-UTIL_INCLUDE_DIR} ${APR_INCLUDE_DIRS})
SET(APR-UTIL_LIBRARIES ${APR-UTIL_LIBRARY} ${APR_LIBRARIES})
#SET(APR-UTIL_PROCESS_INCLUDES ${APR-UTIL_INCLUDE_DIR} ${APR_INCLUDE_DIRS})
#SET(APR-UTIL_PROCESS_LIBS ${APR-UTIL_LIBRARY} ${APR_LIBRARIES})
#sets APR-UTIL_INCLUDE_DIRS and APR-UTIL_LIBRARIES
#LIBFIND_PROCESS(APR-UTIL)

MARK_AS_ADVANCED(APR-UTIL_LIBRARIES APR-UTIL_INCLUDE_DIRS)