CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(rapidxml)

SET(TARGET rapidxml)
SET(PUBLIC_HDR_DIR include)

# ===============================================
# Sources and headers
# ===============================================
FILE(GLOB_RECURSE PUBLIC_HEADERS ${PUBLIC_HDR_DIR}/${TARGET}/*.h)
SET(SRCS dummy.cpp)

# ===============================================
# Include and target definitions
# ===============================================
INCLUDE_DIRECTORIES(${PUBLIC_HDR_DIR})
ADD_LIBRARY(${TARGET} STATIC ${PUBLIC_HEADERS} ${SRCS})

TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})

# ===============================================
# Installation
# ===============================================
INSTALL(DIRECTORY ${PUBLIC_HDR_DIR}/${TARGET} DESTINATION include)