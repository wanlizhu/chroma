CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(render)

SET(TARGET render)
SET(PUBLIC_HDR_DIR include)
SET(TARGET_DEPS numeric system glad)
SET(GENERATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
SET(RESOURCES_BIN_HDR "${GENERATION_DIR}/resources.h")
SET(RESOURCES_BIN_SRC "${GENERATION_DIR}/resources.cpp")

# ===============================================
# Sources and headers
# ===============================================
FILE(GLOB_RECURSE PUBLIC_HDRS ${PUBLIC_HDR_DIR}/${TARGET}/*.h)
FILE(GLOB_RECURSE PRIVATE_HDRS src/*.h)
FILE(GLOB_RECURSE SRCS src/*.cpp)
FILE(GLOB_RECURSE RES_LIST resources/*)

# Embed the binary resource blob and materials.
IF(RES_LIST)
    FILE(MAKE_DIRECTORY ${GENERATION_DIR})
    
    ADD_CUSTOM_COMMAND(
            OUTPUT ${RESOURCES_BIN_HDR}
            COMMAND resc -o ${RESOURCES_BIN_HDR} "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            DEPENDS resc
            COMMENT "Compiling resources folder to '${output_path}'")
            
    LIST(APPEND PRIVATE_HDRS ${RESOURCES_BIN_HDR})
    LIST(APPEND SRCS ${RESOURCES_BIN_SRC})
ENDIF()

# ===============================================
# Includes & target definition
# ===============================================
# specify where our headers are
INCLUDE_DIRECTORIES(${PUBLIC_HDR_DIR})
INCLUDE_DIRECTORIES(${GENERATION_DIR})
INCLUDE_DIRECTORIES(src)

# we're building a library
ADD_LIBRARY(${TARGET} STATIC ${PRIVATE_HDRS} ${PUBLIC_HDRS} ${SRCS})
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})
TARGET_LINK_LIBRARIES(${TARGET} PUBLIC ${TARGET_DEPS})
ADD_DEPENDENCIES(${TARGET} resc)

# ===============================================
# Compiler flags
# ===============================================
IF(MSVC)
    SET(OPTIMIZATION_FLAGS /fp:fast)
ELSE()
    SET(OPTIMIZATION_FLAGS
        -ffast-math
        -ffp-contract=fast
        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -fstrict-vtable-pointers)
ENDIF()

SET(LINUX_COMPILER_FLAGS -fPIC)
TARGET_COMPILE_OPTIONS(${TARGET} PRIVATE
        ${FILAMENT_WARNINGS}
        $<$<CONFIG:Release>:${OPTIMIZATION_FLAGS}>
        $<$<AND:$<PLATFORM_ID:Darwin>,$<CONFIG:Release>>:${DARWIN_OPTIMIZATION_FLAGS}>
        $<$<PLATFORM_ID:Linux>:${LINUX_COMPILER_FLAGS}>)

# ===============================================
# Installation
# ===============================================
SET(INSTALL_TYPE ARCHIVE)
INSTALL(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib/${DIST_DIR})
INSTALL(DIRECTORY ${PUBLIC_HDR_DIR}/chroma DESTINATION include)

# ===============================================
# Sub-projects
# ===============================================
#ADD_SUBDIRECTORY(test)
#ADD_SUBDIRECTORY(benchmark)
