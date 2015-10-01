message(STATUS "i6 log4cxx Find Module")
# - Try to find log4cxx
# Find local log4cxx includes and library
# This module defines:
#  	LOG4CXX_FOUND - If false, do not try to use LOG4CXX.
# On success this module defines:
#  	LOG4CXX_INCLUDE_DIR - where to find log4cxx/log4cxx.h, etc.
#  	LOG4CXX_LIBRARY - where to find the liblog4cxx.so/log4cxx.dll library.
#  	LOG4CXX_INCLUDE_DIRS - ALL include paths needed to use log4cxx.
#  	LOG4CXX_LIBRARIES - ALL libraries needed to use log4cxx.
# See wiki for instructions.
#  	If you prefer a custom installation directory,
#	please set LOG4CXX_CUSTOM_INCLUDE_DIR and LOG4CXX_CUSTOM_LIBRARY accordingly.
# SET(LOG4CXX_CUSTOM_INCLUDE_DIR .../apache-log4cxx/src/main/include)
# SET(LOG4CXX_CUSTOM_LIBRARY .../apache-log4cxx/src/main/cpp/.libs/liblog4cxx.so)
# SET(LOG4CXX_CUSTOM_LIBRARY .../apache-log4cxx/projects/Debug/log4cxx.lib|log4cxx.dll)

# tbf: making this file a little bit version-independent by using
# LOG4CXX_VERSION,LOG4CXX_LIB_VERSION,LOG4CXX_MAJOR_VERSION,LOG4CXX_MINOR_VERSION,LOG4CXX_SUBMINOR_VERSION
# NOTE: critical changes in dir-structure are NOT fixed by that !
# They cause this file to fail if the log4cxx is not installed in standard system-paths.
# tbf: LOG4CXX_ROOT usage and FindPkgMacros.cmake

INCLUDE(LibFindMacros)

IF (NOT APR_INCLUDE_DIRS OR NOT APR_LIBRARIES)
	IF (LOG4CXX_FIND_REQUIRED)
		FIND_PACKAGE(APR REQUIRED)
	ELSE (LOG4CXX_FIND_REQUIRED)
		FIND_PACKAGE(APR QUIETLY)
	ENDIF (LOG4CXX_FIND_REQUIRED)
ENDIF (NOT APR_INCLUDE_DIRS OR NOT APR_LIBRARIES)

IF (NOT APR-UTIL_INCLUDE_DIRS OR NOT APR-UTIL_LIBRARIES)
	IF (LOG4CXX_FIND_REQUIRED)
		FIND_PACKAGE(APR-UTIL REQUIRED)
	ELSE (LOG4CXX_FIND_REQUIRED)
		FIND_PACKAGE(APR-UTIL QUIETLY)
	ENDIF (LOG4CXX_FIND_REQUIRED)
ENDIF (NOT APR-UTIL_INCLUDE_DIRS OR NOT APR-UTIL_LIBRARIES)


# Dependencies
#LIBFIND_PACKAGE(LOG4CXX apr)

# Use pkg-config to get hints about paths
#LIBFIND_PKG_CHECK_MODULES(LOG4CXX_PKGCONF apr)

# Search Include dir if User didn't set LOG4CXX_CUSTOM_INCLUDE_DIR
IF(LOG4CXX_CUSTOM_INCLUDE_DIR)
	SET(LOG4CXX_INCLUDE_DIR ${LOG4CXX_CUSTOM_INCLUDE_DIR})
ELSE(LOG4CXX_CUSTOM_INCLUDE_DIR)
	# UNIX system fail to build from the 'includes' directory
	# (only header files no cpp files,this may be the reason)
	# especially includes/.../include/log4cxx/helpers/objectptr.h causes errors.
	# So to avoid errors confusing people I added UNIX/WIN32 seperation.
	
	IF(UNIX)
		FIND_PATH(LOG4CXX_INCLUDE_DIR
			NAMES log4cxx.h
			HINTS ${LOG4CXX_PKGCONF_INCLUDE_DIRS}
			PATHS /usr/include/log4cxx /proj/mmog/lib_prak/log4cxx/include/log4cxx
				${CMAKE_SOURCE_DIR}/lib/log4cxx/log4cxxLocalNotSVN/apache-log4cxx-0.10.0/src/main/include/log4cxx
		)
	ENDIF(UNIX)
	
	IF(WIN32)
		FIND_PATH(LOG4CXX_INCLUDE_DIR
			NAMES log4cxx.h
			HINTS ${LOG4CXX_PKGCONF_INCLUDE_DIRS}
			PATHS ${CMAKE_SOURCE_DIR}/lib/log4cxx/includes/apache-log4cxx-0.10.0/src/main/include/log4cxx
		)
	ENDIF(WIN32)
ENDIF(LOG4CXX_CUSTOM_INCLUDE_DIR)

# Correct FIND_PATH here because log4cxx.h is in the 'include/log4cxx' subdir
# and FIND_PATH sets LOG4CXX_INCLUDE_DIR to the dir-path where it found 
# log4cxx.h . But we want to have the 'include' directory.
IF(LOG4CXX_INCLUDE_DIR MATCHES .*/include/log4cxx)
	STRING(REPLACE "/include/log4cxx"
         "/include" LOG4CXX_INCLUDE_DIR
         ${LOG4CXX_INCLUDE_DIR})
ELSEIF(LOG4CXX_INCLUDE_DIR MATCHES .*/includes/log4cxx)
	STRING(REPLACE "/includes/log4cxx"
         "/includes" LOG4CXX_INCLUDE_DIR
         ${LOG4CXX_INCLUDE_DIR})
ENDIF()

IF(LOG4CXX_CUSTOM_LIBRARY)
	SET(LOG4CXX_LIBRARY ${LOG4CXX_CUSTOM_LIBRARY})
ELSE(LOG4CXX_CUSTOM_LIBRARY)
	# find libraries liblog4cxx.so (UNIX)
	# log4cxx.lib log4cxx.dll (WIN32)
	FIND_LIBRARY(LOG4CXX_LIBRARY
		NAMES log4cxx
		HINTS ${LOG4CXX_PKGCONF_LIBRARY_DIRS}
		PATHS ${CMAKE_SOURCE_DIR}/lib/log4cxx/win32
			/proj/mmog/lib_prak/log4cxx/lib
			${CMAKE_SOURCE_DIR}/lib/log4cxx/log4cxxLocalNotSVN/apache-log4cxx-0.10.0/src/main/cpp/.libs/
	)
ENDIF(LOG4CXX_CUSTOM_LIBRARY)

MARK_AS_ADVANCED(LOG4CXX_LIBRARY LOG4CXX_INCLUDE_DIR)

IF (LOG4CXX_LIBRARY AND LOG4CXX_INCLUDE_DIR)
  SET(LOG4CXX_FOUND "TRUE")
ELSE (LOG4CXX_LIBRARY AND LOG4CXX_INCLUDE_DIR)
  SET(LOG4CXX_FOUND "FALSE")
ENDIF (LOG4CXX_LIBRARY AND LOG4CXX_INCLUDE_DIR)

IF (LOG4CXX_FOUND)
   IF (NOT LOG4CXX_FIND_QUIETLY)
      MESSAGE(STATUS "Found LOG4CXX: ${LOG4CXX_LIBRARY}")
   ENDIF (NOT LOG4CXX_FIND_QUIETLY)
ELSE (LOG4CXX_FOUND)
   IF (LOG4CXX_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "
			Could not find log4cxx library. See wiki for instructions.
			If you prefer a custom installation directory,
			please set LOG4CXX_CUSTOM_INCLUDE_DIR and LOG4CXX_CUSTOM_LIBRARY accordingly.
		")
   ENDIF (LOG4CXX_FIND_REQUIRED)
ENDIF (LOG4CXX_FOUND)


# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this lib depends on.
SET(LOG4CXX_INCLUDE_DIRS ${LOG4CXX_INCLUDE_DIR} ${APR_INCLUDE_DIRS} ${APR-UTIL_INCLUDE_DIRS})
SET(LOG4CXX_LIBRARIES ${LOG4CXX_LIBRARY} ${APR_LIBRARIES} ${APR-UTIL_LIBRARIES})
#SET(LOG4CXX_PROCESS_INCLUDES ${LOG4CXX_INCLUDE_DIR} ${APR_INCLUDE_DIRS} ${APR-UTIL_INCLUDE_DIRS})
#SET(LOG4CXX_PROCESS_LIBS ${LOG4CXX_LIBRARY} ${APR_LIBRARIES} ${APR-UTIL_LIBRARIES})
#sets LOG4CXX_INCLUDE_DIRS and LOG4CXX_LIBRARIES
#LIBFIND_PROCESS(LOG4CXX)

MARK_AS_ADVANCED(LOG4CXX_LIBRARIES LOG4CXX_INCLUDE_DIRS)
