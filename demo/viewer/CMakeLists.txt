CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(viewer)

SET(TARGET viewer)

# ===============================================
# Sources and headers
# ===============================================
FILE(GLOB_RECURSE HDRS src/*.h)
FILE(GLOB_RECURSE SRCS src/*.cpp)

# ===============================================
# Add target definition
# ===============================================
ADD_EXECUTABLE(${TARGET} ${HDRS} ${SRCS})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES FOLDER Demo)
TARGET_LINK_LIBRARIES(${TARGET} math utils display libchroma)

# ===============================================
# Installation
# ===============================================
SET(INSTALL_TYPE RUNTIME)
INSTALL(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION bin)
INSTALL(DIRECTORY "${CMAKE_SOURCE_DIR}/assets" DESTINATION bin)
INSTALL(FILES "${CMAKE_SOURCE_DIR}/README.md" DESTINATION doc)
