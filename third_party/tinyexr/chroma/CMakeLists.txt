CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(tinyexr)

SET(TARGET tinyexr)
SET(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
SET(PUBLIC_HDR_DIR ${SRC_DIR})

# ==================================================================================================
# Sources and headers
# ==================================================================================================
SET(PUBLIC_HDRS ${SRC_DIR}/tinyexr.h)
SET(SRCS ${SRC_DIR}/chroma/tinyexr.cc)

# ==================================================================================================
# Include and target definitions
# ==================================================================================================
INCLUDE_DIRECTORIES(${PUBLIC_HDR_DIR})

ADD_LIBRARY(${TARGET} STATIC ${PUBLIC_HDRS} ${SRCS})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES FOLDER ThirdParty)
TARGET_INCLUDE_DIRECTORIES (${TARGET} PUBLIC ${PUBLIC_HDR_DIR})
TARGET_LINK_LIBRARIES(${TARGET} LINK_PUBLIC z)

IF (NOT WIN32)
    TARGET_COMPILE_OPTIONS(${TARGET} PRIVATE -Wno-unused-variable)
ENDIF()
