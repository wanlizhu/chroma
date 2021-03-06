CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(system)

SET(TARGET system)
SET(PUBLIC_HDR_DIR include)
SET(TARGET_DEPS jsoncpp rapidxml)

# ===============================================
# Sources and headers
# ===============================================
FILE(GLOB_RECURSE PUBLIC_HDRS ${PUBLIC_HDR_DIR}/${TARGET}/*.h)
FILE(GLOB_RECURSE PRIVATE_HDRS src/*.h)
FILE(GLOB_RECURSE SRCS src/*.cpp)

# ===============================================
# Target definition
# ===============================================
INCLUDE_DIRECTORIES(${PUBLIC_HDR_DIR})

ADD_LIBRARY(${TARGET} STATIC ${PUBLIC_HDRS} ${PRIVATE_HDRS} ${SRCS})
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})
TARGET_COMPILE_OPTIONS(${TARGET} PRIVATE $<$<PLATFORM_ID:Linux>:-fPIC>)
        
IF (WIN32)
    # Needed for shlwapi.h (GetModuleFileName)
    TARGET_LINK_LIBRARIES(${TARGET} PRIVATE Shlwapi ${TARGET_DEPS})
ELSE()
    SET(THREADS_PREFER_PTHREAD_FLAG ON)
    FIND_PACKAGE(Threads REQUIRED)
    TARGET_LINK_LIBRARIES(${TARGET} PRIVATE Threads::Threads ${TARGET_DEPS})
ENDIF()

# ===============================================
# Installation
# ===============================================
SET(INSTALL_TYPE ARCHIVE)
INSTALL(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib)
INSTALL(FILES ${PUBLIC_HDRS} DESTINATION include/${TARGET})

# ===============================================
# Test executables
# ===============================================
FILE(GLOB_RECURSE TEST_SRCS test/*.cpp)
ADD_EXECUTABLE(test_${TARGET} ${TEST_SRCS})
SET_TARGET_PROPERTIES(test_${TARGET} PROPERTIES FOLDER Test)
TARGET_LINK_LIBRARIES(test_${TARGET} PRIVATE gtest ${TARGET} numeric)
