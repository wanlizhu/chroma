CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(benchmark)

SET(OUR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
SET(TARGET              benchmark)
SET(SRC_DIR             ${OUR_DIR}/src)
SET(PUBLIC_HDR_DIR      ${OUR_DIR}/include)
SET(PRIVATE_HDR_DIR     ${OUR_DIR})

# sources dependencies
FILE(GLOB
        SRCS
        ${SRC_DIR}/*.cc
        ${PUBLIC_HDR_DIR}/benchmark/*.h
        ${PRIVATE_HDR_DIR}/*.h)
FILE(GLOB BENCHMARK_MAIN "benchmark_main.cc")
FOREACH(item ${BENCHMARK_MAIN})
  LIST(REMOVE_ITEM SOURCE_FILES "${item}")
ENDFOREACH()

IF (WIN32)
    ADD_DEFINITIONS(/W0)
ELSE()
    ADD_DEFINITIONS(-Wno-deprecated-declarations)
ENDIF()

INCLUDE_DIRECTORIES(
        ${PUBLIC_HDR_DIR}
        ${PRIVATE_HDR_DIR})

# we're building a library
ADD_LIBRARY(${TARGET} STATIC ${SRCS})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES FOLDER ThirdParty)

IF (LINUX)
  SET(THREADS_PREFER_PTHREAD_FLAG ON)
  FIND_PACKAGE(Threads REQUIRED)
  TARGET_LINK_LIBRARIES(${TARGET} Threads::Threads)
ENDIF()

# specify where the public headers of this library are
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})

# Benchmark main library
ADD_LIBRARY(${TARGET}_main ${SRC_DIR}/benchmark_main.cc)
SET_TARGET_PROPERTIES(${TARGET}_main PROPERTIES FOLDER ThirdParty)
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})
TARGET_LINK_LIBRARIES(${TARGET}_main benchmark)
