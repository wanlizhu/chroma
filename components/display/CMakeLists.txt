CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(display)

SET(TARGET display)
SET(PUBLIC_HDR_DIR include)
SET(TARGET_DEPS glfw glad numeric system render)

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

ADD_EXECUTABLE(${TARGET} ${PUBLIC_HDRS} ${PRIVATE_HDRS} ${SRCS})
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})
TARGET_LINK_LIBRARIES(${TARGET} PUBLIC ${TARGET_DEPS})

# ===============================================
# Installation
# ===============================================
INSTALL(TARGETS ${TARGET} ${RUNTIME} DESTINATION bin)
INSTALL(FILES ${PUBLIC_HDRS} DESTINATION include/${TARGET})
