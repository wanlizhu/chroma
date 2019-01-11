CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(chroma)

SET(TARGET chroma)
SET(PUBLIC_HDR_DIR include)
SET(GENERATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
SET(COMPILED_RES_HDR "${GENERATION_DIR}/compiled_res.h")
SET(COMPILED_RES_SRC "${GENERATION_DIR}/compiled_res.cpp")

# ===============================================
# Sources and headers
# ===============================================
FILE(GLOB_RECURSE PUBLIC_HDRS ${PUBLIC_HDR_DIR}/${TARGET}/*.h)
FILE(GLOB_RECURSE PRIVATE_HDRS src/*.h)
FILE(GLOB_RECURSE SRCS src/*.cpp)
FILE(GLOB_RECURSE RES_LIST res/*)
FILE(GLOB_RECURSE MAT_LIST mat/*)

# Embed the binary resource blob and materials.
LIST(APPEND PRIVATE_HDRS ${COMPILED_RES_HDR})
LIST(APPEND SRCS ${COMPILED_RES_SRC})
FILE(MAKE_DIRECTORY ${GENERATION_DIR})

FOREACH (RES_FILE ${RES_LIST})
    GET_FILENAME_COMPONENT(FULL_NAME "${RES_FILE}" ABSOLUTE)

    ADD_CUSTOM_COMMAND(
            OUTPUT ${COMPILED_RES_HDR}
            COMMAND resc -s -o ${COMPILED_RES_HDR} ${FULL_NAME}
            DEPENDS ${FULL_NAME} resc
            COMMENT "Compiling resource '${RES_FILE}' to '${COMPILED_RES_HDR}'")
ENDFOREACH()

FOREACH (MAT_FILE ${MAT_LIST})
    GET_FILENAME_COMPONENT(LOCAL_NAME "${MAT_FILE}" NAME_WE)
    GET_FILENAME_COMPONENT(FULL_NAME "${MAT_FILE}" ABSOLUTE)
    SET(OUTPUT_PATH "${GENERATION_DIR}/materials/${LOCAL_NAME}.h")

    ADD_CUSTOM_COMMAND(
            OUTPUT ${OUTPUT_PATH}
            COMMAND matc -r ${COMPILED_RES_HDR} -o ${OUTPUT_PATH} ${FULL_NAME}
            DEPENDS ${FULL_NAME} matc
            COMMENT "Compiling material '${MAT_FILE}' to '${OUTPUT_PATH}'")
    LIST(APPEND PRIVATE_HDRS ${OUTPUT_PATH})
ENDFOREACH()

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

# ===============================================
# Dependencies
# ===============================================
IF(APPLE AND NOT IOS)
    TARGET_LINK_LIBRARIES(${TARGET} PRIVATE "-framework Cocoa")
ENDIF()

TARGET_LINK_LIBRARIES(${TARGET} PUBLIC math)
TARGET_LINK_LIBRARIES(${TARGET} PUBLIC utils)
TARGET_LINK_LIBRARIES(${TARGET} PUBLIC minigl)

IF(LINUX)
    TARGET_LINK_LIBRARIES(${TARGET} PRIVATE dl)
ENDIF()

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

SET(LINUX_LINKER_OPTIMIZATION_FLAGS -Wl,--exclude-libs,minigl)
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
INSTALL(FILES "README.md" DESTINATION .)

# ===============================================
# Sub-projects
# ===============================================
#ADD_SUBDIRECTORY(test)
#ADD_SUBDIRECTORY(benchmark)
