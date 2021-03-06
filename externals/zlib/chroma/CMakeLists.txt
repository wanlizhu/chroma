CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(zlib)

SET(OUR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
SET(TARGET zlib)
SET(SRC_DIR ${OUR_DIR})
SET(PUBLIC_HDR_DIR ${OUR_DIR})
SET(PRIVATE_HDR_DIR ${OUR_DIR})

# public headers dependencies
SET(PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/zlib.h)

# private headers dependencies
SET(PRIVATE_HDRS
    ${PRIVATE_HDR_DIR}/crc32.h
    ${PRIVATE_HDR_DIR}/deflate.h
    ${PRIVATE_HDR_DIR}/gzguts.h
    ${PRIVATE_HDR_DIR}/inffast.h
    ${PRIVATE_HDR_DIR}/inffixed.h
    ${PRIVATE_HDR_DIR}/inflate.h
    ${PRIVATE_HDR_DIR}/inftrees.h
    ${PRIVATE_HDR_DIR}/trees.h
    ${PRIVATE_HDR_DIR}/zutil.h)

# sources dependencies
SET(SRCS
    ${SRC_DIR}/adler32.c
    ${SRC_DIR}/compress.c
    ${SRC_DIR}/crc32.c
    ${SRC_DIR}/deflate.c
    ${SRC_DIR}/gzclose.c
    ${SRC_DIR}/gzlib.c
    ${SRC_DIR}/gzread.c
    ${SRC_DIR}/gzwrite.c
    ${SRC_DIR}/inflate.c
    ${SRC_DIR}/infback.c
    ${SRC_DIR}/inftrees.c
    ${SRC_DIR}/inffast.c
    ${SRC_DIR}/trees.c
    ${SRC_DIR}/uncompr.c
    ${SRC_DIR}/zutil.c)

IF (NOT WIN32)
    ADD_DEFINITIONS(
        -Wno-implicit-function-declaration
        -Wno-shift-negative-value)
ENDIF()

# we're building a library
IF (BUILD_SHARED_ZLIB)
    ADD_LIBRARY(${TARGET} SHARED ${PRIVATE_HDRS} ${PUBLIC_HDRS} ${SRCS})
ELSE()
    ADD_LIBRARY(${TARGET} STATIC ${PRIVATE_HDRS} ${PUBLIC_HDRS} ${SRCS})
ENDIF()
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES FOLDER Externals)
# specify where the public headers of this library are
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})
