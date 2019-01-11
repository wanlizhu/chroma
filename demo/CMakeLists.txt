CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(chroma-demo)

SET(TARGET chroma-demo)

# ===============================================
# Sources and headers
# ===============================================
FILE(GLOB_RECURSE HDRS src/*.h)
FILE(GLOB_RECURSE SRCS src/*.cpp)

# ===============================================
# Includes and target definition
# ===============================================
ADD_EXECUTABLE(${TARGET} ${HDRS} ${SRCS})
TARGET_LINK_LIBRARIES(${TARGET} math utils gui)
