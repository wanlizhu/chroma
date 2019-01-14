CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(glad)

SET(OUR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
SET(TARGET glad)
SET(SRC_DIR ${OUR_DIR})
SET(PUBLIC_HDR_DIR ${OUR_DIR})
SET(PRIVATE_HDR_DIR ${OUR_DIR})

# public headers dependencies
SET(PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/glad.h)

# private headers dependencies
SET(PRIVATE_HDRS
    ${PRIVATE_HDR_DIR}/khrplatform.h)

# sources dependencies
SET(SRCS
    ${SRC_DIR}/glad.c)

# we're building a library
ADD_LIBRARY(${TARGET} STATIC ${PRIVATE_HDRS} ${PUBLIC_HDRS} ${SRCS})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES FOLDER ThirdParty)

# specify where the public headers of this library are
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})
