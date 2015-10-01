set(chimera_sources chimera.c
 dllist.c
 dtime.c
 host.c
 job_queue.c
 jrb.c
 jval.c
 key.c
 log.c
 message.c
 network.c
 priqueue.c
 route.c
 semaphore.c)

# without the next line, the build will fail because the compiler can't locate chimera.h
INCLUDE_DIRECTORIES(../include/)

OPTION(CHIMERA_BUILD_SHARED
  "Build chimera shared" ON)

IF(CHIMERA_BUILD_SHARED)
	add_library(chimera SHARED ${chimera_sources})
ELSE(CHIMERA_BUILD_SHARED)
	add_library(chimera STATIC ${chimera_sources})
ENDIF(CHIMERA_BUILD_SHARED)
INSTALL(TARGETS chimera
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

target_link_libraries(chimera ssl crypto)
set_target_properties(chimera PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(chimera PROPERTIES VERSION 1.4)
#target_link_libraries(chimera ${PTHREADS_LIBRARY} ${OPENSSL_LIBRARIES})

