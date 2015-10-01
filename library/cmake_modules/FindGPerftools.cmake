# easyFind version 1.0 - 25.12.2012 - # 009

# Find GPerfTools includes and library
#
# --------------------
# This modules works correctly with these find_package parameters
# --------------------
#	QUIET					no messages are print
#	REQUIRED [comp...]		cmake fails if not found
#	COMPONENTS [comp...]	only look for the compoents comp. if not specified, look for all
#
# --------------------
# This modules uses the following variables
# --------------------
#	GPerfTools_ROOT			Look here first for library / headers
#
# --------------------
# This modules defines the following variables
# --------------------
#	GPerfTools_INCLUDE_DIR	Path to the include files
#	GPerfTools_LIBRARIES	List of all libraries
#	GPerfTools_LIB_DIR		location of the libraries
#	GPerfTools_FOUND		TRUE if GPerfTools was found, FALSE otherwise

# sets all components needed
IF (GPerfTools_FIND_COMPONENTS)
	SET (COMPONENTS ${GPerfTools_FIND_COMPONENTS})
ELSE (GPerfTools_FIND_COMPONENTS)
	SET (COMPONENTS "profiler" "tcmalloc")
ENDIF (GPerfTools_FIND_COMPONENTS)

IF (NOT GPerfTools_FIND_QUIETLY)
	message(STATUS "Looking for GPerfTools:")
ENDIF (NOT GPerfTools_FIND_QUIETLY)

# find the libraries
	FOREACH (COMPONENT ${COMPONENTS})
		string(TOUPPER ${COMPONENT} UPPERCOMPONENT)

		find_library(
			GPerfTools_${UPPERCOMPONENT}_LIBRARY	# return value
			NAMES ${COMPONENT}						# list of possible names, currently only exactly the name
			HINTS ${GPerfTools_ROOT}/lib			# first search here
		)
	
		IF (GPerfTools_${UPPERCOMPONENT}_LIBRARY)
			SET(GPerfTools_LIBRARIES ${GPerfTools_LIBRARIES} ${GPerfTools_${UPPERCOMPONENT}_LIBRARY})
			message(STATUS "	${COMPONENT}	found")
		ELSE (GPerfTools_${UPPERCOMPONENT}_LIBRARY)
			SET (GPerfTools_notFoundList ${GPerfTools_notFoundList} ${COMPONENT})
			message(STATUS "	${COMPONENT}	not found")
		ENDIF (GPerfTools_${UPPERCOMPONENT}_LIBRARY)
	ENDFOREACH (COMPONENT)

	IF (GPerfTools_notFoundList)
		SET (GPerfTools_FOUND FALSE)
		IF (GPerfTools_FIND_REQUIRED)
			message(SEND_ERROR "GPerfTools marked as required, but not all components were found.")
		ELSE (GPerfTools_FIND_REQUIRED)
			message(WARNING "Not all components were found. Continuing anyways")
		ENDIF (GPerfTools_FIND_REQUIRED)
	ELSEIF (GPerfTools_notFoundList)
		SET (GPerfTools_FOUND TRUE)
	ENDIF (GPerfTools_notFoundList)

# find the header
	find_path(
		GPerfTools_HEADER					# return value
		NAMES gperftools/profiler.h			# file to find
		HINTS ${GPerfTools_ROOT}/include	# first search here
	)

	IF (GPerfTools_HEADER)
		SET (GPerfTools_INCLUDE_DIR ${GPerfTools_ROOT}/include)
	ELSE (GPerfTools_HEADER)
		SET (GPerfTools_FOUND FALSE)
		IF (GPerfTools_FIND_REQUIRED)
			message(SEND_ERROR "GPerfTools marked as required but header not found")
		ELSE (GPerfTools_FIND_REQUIRED)
			IF (NOT GPerfTools_FIND_QUIETLY)
				message(WARNING "GPerfTools header not found")
			ENDIF (NOT GPerfTools_FIND_QUIETLY)
		ENDIF (GPerfTools_FIND_REQUIRED)
	ENDIF (GPerfTools_HEADER)

# find dependencies from GPerfTools
	# TODO

