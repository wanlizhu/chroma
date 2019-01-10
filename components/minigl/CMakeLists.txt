CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(minigl ASM)

SET(TARGET minigl)
SET(PUBLIC_HDR_DIR include)

IF (WIN32)
    ENABLE_LANGUAGE(ASM_MASM)
    SET_PROPERTY(SOURCE src/minigl_core_windows_impl.s PROPERTY LANGUAGE ASM_MASM)
ENDIF()

# public headers are always in include/${TARGET}/
file(GLOB_RECURSE PUBLIC_HDRS ${PUBLIC_HDR_DIR}/${TARGET}/*.h)

# list each source file individually
SET(SRCS src/minigl.cpp)

IF (WIN32)
    SET(SRCS ${SRCS} src/minigl_windows.cpp)
    SET(SRCS ${SRCS} src/minigl_core_windows_impl.s)
ELSEIF (APPLE AND NOT IOS)
    SET(SRCS ${SRCS} src/minigl_darwin.cpp)
    SET(SRCS ${SRCS} src/minigl_core_darwin_impl.s)
ELSEIF(LINUX)
    SET(SRCS ${SRCS} src/minigl_linux.cpp)
    SET(SRCS ${SRCS} src/minigl_core_linux_impl.s)
ELSE()
    MESSAGE(FATAL_ERROR "Platform not supported. MiniGL supports Windows, Linux, and MacOS X.")
ENDIF()

IF (NOT WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
ENDIF()

# Only linux, clang doesn't want to use a shared library that is not PIC.
# /usr/bin/ld: ../minigl/libminigl.a(minigl.cpp.o): relocation R_X86_64_32S
# against `.bss' can not be used when making a shared object; recompile with -fPIC
IF (LINUX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF()

# specify where our headers are
INCLUDE_DIRECTORIES(${PUBLIC_HDR_DIR})

# we're building a library
ADD_LIBRARY(${TARGET} STATIC ${PUBLIC_HDRS} ${SRCS})

# specify where the public headers of this library are
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})

IF (WIN32)
    TARGET_LINK_LIBRARIES(${TARGET} PRIVATE opengl32 gdi32)
ENDIF()

INSTALL(TARGETS ${TARGET} ARCHIVE DESTINATION lib/${DIST_DIR})

# Build the tests...
ADD_EXECUTABLE(test_${TARGET}
    tests/opengl_support.cpp
    tests/opengl_support.hpp
    tests/test_minigl.cpp)
SET_TARGET_PROPERTIES(test_${TARGET} PROPERTIES FOLDER TestAndBenchmark)

IF (LINUX)
    TARGET_LINK_LIBRARIES(test_${TARGET} PUBLIC dl)
ENDIF()

# and we're linking against the libraries below, importing their public headers
TARGET_LINK_LIBRARIES(test_${TARGET} LINK_PUBLIC ${TARGET})
TARGET_LINK_LIBRARIES(test_${TARGET} LINK_PUBLIC gtest)
