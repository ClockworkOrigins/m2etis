# easyFind version 1.0 - 25.12.2012 - # 008

# Find GTest includes and library
#
# --------------------
# This modules works correctly with these find_package parameters
# --------------------
#	QUIET			no messages are print
#	REQUIRED [comp...]	cmake fails if not found
#	COMPONENTS [comp...]	only look for the compoents comp. if not specified, look for all
#
# --------------------
# This modules uses the following variables
# --------------------
#	GTest_ROOT		Look here first for library / headers
#
# --------------------
# This modules defines the following variables
# --------------------
#	GTest_INCLUDE_DIR	Path to the include files
#	GTest_LIBRARIES	List of all libraries
#	GTest_LIB_DIR	location of the libraries
#	GTest_FOUND		TRUE if GTest was found, FALSE otherwise

# sets all components needed
IF (GTest_FIND_COMPONENTS)
	SET (COMPONENTS ${GTest_FIND_COMPONENTS})
ELSE (GTest_FIND_COMPONENTS)
	SET (COMPONENTS "gtest" "gtest_main" "gmock" "gmock_main")
ENDIF (GTest_FIND_COMPONENTS)

IF (NOT GTest_FIND_QUIETLY)
	message(STATUS "Looking for GTest:")
ENDIF (NOT GTest_FIND_QUIETLY)

# find the libraries
	FOREACH (COMPONENT ${COMPONENTS})
		string(TOUPPER ${COMPONENT} UPPERCOMPONENT)

		find_library(
			GTest_${UPPERCOMPONENT}_LIBRARY	# return value
			NAMES ${COMPONENT}		# list of possible names, currently only exactly the name
			HINTS ${GTest_ROOT}/lib		# first search here
		)
	
		IF (GTest_${UPPERCOMPONENT}_LIBRARY)
			SET(GTest_LIBRARIES ${GTest_LIBRARIES} ${GTest_${UPPERCOMPONENT}_LIBRARY})
			message(STATUS "	${COMPONENT}	found")
		ELSE (GTest_${UPPERCOMPONENT}_LIBRARY)
			SET (notFoundList ${notFoundList} ${COMPONENT})
			message(STATUS "	${COMPONENT}	not found")
		ENDIF (GTest_${UPPERCOMPONENT}_LIBRARY)
	ENDFOREACH (COMPONENT)

	IF (notFoundList)
		SET (GTest_FOUND FALSE)
		IF (GTest_FIND_REQUIRED)
			message(SEND_ERROR "GTest marked as required, but not all components were found.")
		ELSE (GTest_FIND_REQUIRED)
			message(WARNING "Not all components were found. Continuing anyways")
		ENDIF (GTest_FIND_REQUIRED)
	ELSEIF (notFoundList)
		SET (GTest_FOUND TRUE)
	ENDIF (notFoundList)

# find the header
	find_path(
		GTest_HEADER			# return value
		NAMES gtest/gtest.h		# file to find
		HINTS ${GTest_ROOT}/include	# first search here
	)

	IF (GTest_HEADER)
		SET (GTest_INCLUDE_DIR ${GTest_ROOT}/include)
	ELSE (GTest_HEADER)
		SET (GTest_FOUND FALSE)
		IF (GTest_FIND_REQUIRED)
			message(SEND_ERROR "GTest marked as required but header not found")
		ELSE (GTest_FIND_REQUIRED)
			IF (NOT GTest_FIND_QUIETLY)
				message(WARNING "GTest header not found")
			ENDIF (NOT GTest_FIND_QUIETLY)
		ENDIF (GTest_FIND_REQUIRED)
	ENDIF (GTest_HEADER)

# find dependencies from GTest
	# none

