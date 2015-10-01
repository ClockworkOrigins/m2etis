IF (BULLET_LIBRARIES AND BULLET_INCLUDE_DIR)
    SET(BULLET_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (BULLET_LIBRARIES AND BULLET_INCLUDE_DIR)

IF (WIN32) #Windows
    MESSAGE(STATUS "Looking for Bullet")
    SET(BULLET_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dependencies/bullet/include/bullet/)
    SET(BULLET_LIB_DIR ${CMAKE_SOURCE_DIR}/dependencies/bullet/lib_win32/) 
    SET(BULLET_LIBRARIES debug BulletCollision_d  BulletDynamics_d BulletSoftBody_d  LinearMath_d   optimized BulletCollision  BulletDynamics  BulletSoftBody  LinearMath   )
ELSE (WIN32) #Unix
    MESSAGE(STATUS "Looking for Bullet")
    SET(BULLET_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dependencies/bullet/include/bullet/)
    SET(BULLET_LIB_DIR ${CMAKE_SOURCE_DIR}/dependencies/bullet/lib/) 
    SET(BULLET_LIBRARIES BulletCollision  BulletDynamics  BulletSoftBody  LinearMath CACHE STRING "")
ENDIF (WIN32)

SET(BULLET_INCLUDE_DIR ${BULLET_INCLUDE_DIR} CACHE PATH "" FORCE)
SET(BULLET_LIB_DIR ${BULLET_LIB_DIR} CACHE PATH "" FORCE)
SET(BULLET_LIBRARIES ${BULLET_LIBRARIES} CACHE STRING "" FORCE)

MARK_AS_ADVANCED(BULLET_INCLUDE_DIR)
MARK_AS_ADVANCED(BULLET_LIB_DIR)
MARK_AS_ADVANCED(BULLET_LIBRARIES)

IF (BULLET_INCLUDE_DIR AND BULLET_LIBRARIES)
    SET(BULLET_FOUND TRUE)
ENDIF (BULLET_INCLUDE_DIR AND BULLET_LIBRARIES)

IF (BULLET_FOUND)
    IF (NOT BULLET_FIND_QUIETLY)
        MESSAGE(STATUS "  libraries : ${BULLET_LIBRARIES} from ${BULLET_LIB_DIR}")
        MESSAGE(STATUS "  includes  : ${BULLET_INCLUDE_DIR}")
    ENDIF (NOT BULLET_FIND_QUIETLY)
ELSE (BULLET_FOUND)
    IF (BULLET_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find Bullet")
    ENDIF (BULLET_FIND_REQUIRED)
ENDIF (BULLET_FOUND)
