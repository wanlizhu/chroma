CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(numeric)

SET(TARGET numeric)
SET(PUBLIC_HDR_DIR include)
SET(TARGET_DEPS "")

IF (MSVC)
    SET(OPTIMIZATION_FLAGS /fp:fast)
ELSE()
    SET(OPTIMIZATION_FLAGS -ffast-math -ffp-contract=fast)
ENDIF()

# ===============================================
# Sources and headers
# ===============================================
FILE(GLOB_RECURSE PUBLIC_HEADERS ${PUBLIC_HDR_DIR}/${TARGET}/*.h)
SET(SRCS dummy.cpp)

# ===============================================
# Include and target definitions
# ===============================================
INCLUDE_DIRECTORIES(${PUBLIC_HDR_DIR})
ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
ADD_LIBRARY(${TARGET} STATIC ${PUBLIC_HEADERS} ${SRCS})
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})
TARGET_LINK_LIBRARIES(${TARGET} PRIVATE ${TARGET_DEPS})

# ===============================================
# Installation
# ===============================================
INSTALL(DIRECTORY ${PUBLIC_HDR_DIR}/${TARGET} DESTINATION include)

# ===============================================
# Test executables
# ===============================================
FILE(GLOB_RECURSE TEST_SRCS test/*.cpp)
ADD_EXECUTABLE(test_${TARGET} ${TEST_SRCS})
SET_TARGET_PROPERTIES(test_${TARGET} PROPERTIES FOLDER Test)
TARGET_LINK_LIBRARIES(test_${TARGET} PRIVATE ${TARGET} gtest)
