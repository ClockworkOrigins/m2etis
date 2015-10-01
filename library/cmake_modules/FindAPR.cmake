message(STATUS "Own APR (apache portable runtime) Find Module")
# - Try to find APR
# Find local APR includes and library
# This module defines:
#  	APR_FOUND - If false, do not try to use APR.
# On success this module defines:
#  	APR_INCLUDE_DIR - where to find apr.h, etc.
#  	APR_LIBRARY - where to find the libapr-1.so/libapr-1.dll library.
#  	APR_INCLUDE_DIRS - ALL include paths needed to use APR.
#  	APR_LIBRARIES - ALL libraries needed to use APR.
# See wiki for instructions.
#  	If you prefer a custom installation directory,
#	please set APR_CUSTOM_INCLUDE_DIR and APR_CUSTOM_LIBRARY accordingly.
# SET(APR_CUSTOM_INCLUDE_DIR .../apr/include)
# SET(APR_CUSTOM_LIBRARY .../apr/.libs/libapr-1.so)
# SET(APR-UTIL_CUSTOM_LIBRARY .../apr/Debug/libapr-1.lib|.dll)

# tbf: making this file a little bit version-independent by using
# APR_VERSION,APR_LIB_VERSION,APR_MAJOR_VERSION,APR_MINOR_VERSION,APR_SUBMINOR_VERSION
# NOTE: critical changes in dir-structure are NOT fixed by that !
# They cause this file to fail if the APR is not installed in standard system-paths.
# tbf: usage of APR_ROOT and FindPkgMacros.cmake


#INCLUDE(LibFindMacros)

# Use pkg-config to get hints about paths
#LIBFIND_PKG_CHECK_MODULES(APR_PKGCONF apr)

# Search Include dir if User didn't set APR_CUSTOM_INCLUDE_DIR
IF(APR_CUSTOM_INCLUDE_DIR)
	SET(APR_INCLUDE_DIR ${APR_CUSTOM_INCLUDE_DIR})
ELSE(APR_CUSTOM_INCLUDE_DIR)
	# different header files for UNIX and WIN32.
	# So to avoid errors confusing people I added UNIX/WIN32 seperation.
	IF(UNIX)
		FIND_PATH(APR_INCLUDE_DIR
			NAMES apr.h
			HINTS ${APR_PKGCONF_INCLUDE_DIRS}
			PATHS /usr/include/apr-1
				${CMAKE_SOURCE_DIR}/lib/log4cxx/log4cxxLocalNotSVN/apr-1.3.8/include
				${CMAKE_SOURCE_DIR}/lib/log4cxx/log4cxxLocalNotSVN/apr/include
		)
	ENDIF(UNIX)
	
	IF(WIN32)
		FIND_PATH(APR_INCLUDE_DIR
			NAMES apr.h
			HINTS ${APR_PKGCONF_INCLUDE_DIRS}
			PATHS ${CMAKE_SOURCE_DIR}/lib/log4cxx/includes/apr-1.3.8/include
		)
	ENDIF(WIN32)
ENDIF(APR_CUSTOM_INCLUDE_DIR)

IF(APR_CUSTOM_LIBRARY)
	SET(APR_LIBRARY ${APR_CUSTOM_LIBRARY})
ELSE(APR_CUSTOM_LIBRARY)
	IF (UNIX)
		# Finally the library itself
		FIND_LIBRARY(APR_LIBRARY
		  NAMES libapr-1.so
		  HINTS ${APR_PKGCONF_LIBRARY_DIRS}
		  PATHS ${CMAKE_SOURCE_DIR}/lib/log4cxx/log4cxxLocalNotSVN/apr-1.3.8/.libs
		  	${CMAKE_SOURCE_DIR}/lib/log4cxx/log4cxxLocalNotSVN/apr/.libs
		)
	ENDIF (UNIX)
	
	IF (WIN32)
		# Finally the library itself
		FIND_LIBRARY(APR_LIBRARY
		  NAMES libapr-1.lib libapr-1.dll
		  HINTS ${APR_PKGCONF_LIBRARY_DIRS}
		  PATHS ${CMAKE_SOURCE_DIR}/lib/log4cxx/win32
		)
	ENDIF (WIN32)
ENDIF(APR_CUSTOM_LIBRARY)

MARK_AS_ADVANCED(APR_LIBRARY APR_INCLUDE_DIR)

IF (APR_LIBRARY AND APR_INCLUDE_DIR)
  SET(APR_FOUND "TRUE")
ELSE (APR_LIBRARY AND APR_INCLUDE_DIR)
  SET(APR_FOUND "FALSE")
ENDIF (APR_LIBRARY AND APR_INCLUDE_DIR)

IF (APR_FOUND)
   IF (NOT APR_FIND_QUIETLY)
      MESSAGE(STATUS "Found APR: ${APR_LIBRARY}")
   ENDIF (NOT APR_FIND_QUIETLY)
ELSE (APR_FOUND)
   IF (APR_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "
			Could not find APR library. See wiki for instructions.
			If you prefer a custom installation directory,
			please set APR_CUSTOM_INCLUDE_DIR and APR_CUSTOM_LIBRARY accordingly.
		")
   ENDIF (APR_FIND_REQUIRED)
ENDIF (APR_FOUND)


# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this lib depends on.
SET(APR_INCLUDE_DIRS ${APR_INCLUDE_DIR})
SET(APR_LIBRARIES ${APR_LIBRARY})
#SET(APR_PROCESS_INCLUDES ${APR_INCLUDE_DIR})
#SET(APR_PROCESS_LIBS ${APR_LIBRARY})
#sets APR_INCLUDE_DIRS and APR_LIBRARIES
#LIBFIND_PROCESS(APR)

MARK_AS_ADVANCED(APR_LIBRARIES APR_INCLUDE_DIRS)