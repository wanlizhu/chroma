CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(resc)

SET(TARGET resc)
SET(PUBLIC_HDR_DIR include)

# ===============================================
# Sources and headers
# ===============================================
FILE(GLOB_RECURSE PUBLIC_HDRS ${PUBLIC_HDR_DIR}/${TARGET}/*.h)
FILE(GLOB_RECURSE PRIVATE_HDRS src/*.h)
FILE(GLOB_RECURSE SRCS src/*.cpp)

# ===============================================
# Includes and target definition
# ===============================================
INCLUDE_DIRECTORIES(${PUBLIC_HDR_DIR})

# ===============================================
# Add executable
# ===============================================
ADD_EXECUTABLE(${TARGET} ${PUBLIC_HDRS} ${PRIVATE_HDRS} ${SRCS})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES FOLDER Tools)
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})
TARGET_LINK_LIBRARIES(${TARGET} math utils)

