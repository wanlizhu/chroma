CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(getopt)

SET(TARGET getopt)
SET(PUBLIC_HDR_DIR ../include)

# ===============================================
# Sources and headers
# ===============================================
SET(PUBLIC_HDRS ../include/getopt/getopt.h)
SET(PRIVATE_HDRS ../include/getopt/getopt.h)
SET(SRCS ../src/getopt.c ../src/getopt_long.c)

# ===============================================
# Include and target definitions
# ===============================================
INCLUDE_DIRECTORIES(${PUBLIC_HDR_DIR})

ADD_LIBRARY(${TARGET} STATIC ${PRIVATE_HDRS} ${PUBLIC_HDRS} ${SRCS})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES FOLDER Externals)
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})
