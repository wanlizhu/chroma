CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(demo)

SET(TARGET demo)
SET(TARGET_DEPS numeric system render)

FILE(GLOB_RECURSE HDRS *.h)
FILE(GLOB_RECURSE SRCS *.cpp)

# ===============================================
# Target definition
# ===============================================
ADD_EXECUTABLE(${TARGET} ${HDRS} ${SRCS})
TARGET_INCLUDE_DIRECTORIES(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
TARGET_LINK_LIBRARIES(${TARGET} PRIVATE ${TARGET_DEPS})

# ===============================================
# Installation
# ===============================================
INSTALL(TARGETS ${TARGET} ${RUNTIME} DESTINATION bin)
