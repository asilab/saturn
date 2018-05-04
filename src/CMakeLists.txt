cmake_minimum_required(VERSION 2.8.4)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
IF(UNIX)
 link_libraries(m)
ENDIF(UNIX)

project (saturn) 
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
SET(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_BUILD_TYPE Release)

add_executable (SATURN saturn.c mem.c msg.c alphabet.c hash.c array.c pmodels.c tolerant.c common.c buffer.c levels.c context.c turing.c sequence.c bitio.c arith.c arith_aux.c defs.h) 
TARGET_LINK_LIBRARIES(SATURN pthread)
