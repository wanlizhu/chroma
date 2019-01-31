CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(chroma)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

OPTION(CHROMA_BUILD_EDITOR "Build editor applications" ON)
OPTION(CHROMA_BUILD_TEST "Build test for all submodules" ON)
OPTION(CHROMA_USE_VULKAN "Use Vulkan library" ON)

SET(CMAKE_CXX_STANDARD 14)
SET(BUILD_SHARED_LIBS OFF)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET(BUILD_SHARED_ZLIB OFF)

# ===============================================
#  Paths
# ===============================================
SET(EXTERNALS ${CMAKE_CURRENT_SOURCE_DIR}/externals)
SET(COMPONENTS ${CMAKE_CURRENT_SOURCE_DIR}/components)
SET(TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools)
SET(ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets)

# ===============================================
# General compiler flags
# ===============================================
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_STANDARD}")

# ===============================================
# Project flags
# ===============================================
IF (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    ADD_DEFINITIONS(-DCHROMA_DEV)
ENDIF()

IF(WIN32)
    MESSAGE(STATUS "Configurate for Windows")
    ADD_DEFINITIONS(/W0 
                    -DWINDOWS 
                    -D_CRT_SECURE_NO_WARNINGS
                    -D_CRT_NONSTDC_NO_DEPRECATE)
    SET(CRT_FLAGS_RELEASE "/MT")
    SET(CRT_FLAGS_DEBUG "/MTd") 
ELSEIF(UNIX AND NOT APPLE)
    MESSAGE(STATUS "Configurate for Linux")
     ADD_DEFINITIONS(-DLINUX)
    FIND_PACKAGE(X11 REQUIRED)
ELSEIF(APPLE)
    MESSAGE(STATUS "Configurate for Apple")
    SET(GC_SECTIONS "-Wl,-dead_strip")
    ADD_DEFINITIONS(-DAPPLE)
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported platform : ${CMAKE_SYSTEM_NAME}")
ENDIF()


# ===============================================
# Sub-projects
# ===============================================
# Common to all platforms
ADD_SUBDIRECTORY(${EXTERNALS}/zlib/chroma)
ADD_SUBDIRECTORY(${EXTERNALS}/libgtest/chroma)
ADD_SUBDIRECTORY(${EXTERNALS}/benchmark/chroma)
ADD_SUBDIRECTORY(${EXTERNALS}/glad/chroma)
ADD_SUBDIRECTORY(${EXTERNALS}/getopt)
ADD_SUBDIRECTORY(${EXTERNALS}/jsoncpp)
ADD_SUBDIRECTORY(${EXTERNALS}/rapidxml)
ADD_SUBDIRECTORY(${EXTERNALS}/tinyobjloader)
ADD_SUBDIRECTORY(${EXTERNALS}/openexr/chroma)
ADD_SUBDIRECTORY(${TOOLS}/resc)
ADD_SUBDIRECTORY(${COMPONENTS}/numeric)
ADD_SUBDIRECTORY(${COMPONENTS}/system)
ADD_SUBDIRECTORY(${COMPONENTS}/render)
IF(CHROMA_BUILD_EDITOR)
    ADD_SUBDIRECTORY(${EXTERNALS}/glfw/chroma)
    ADD_SUBDIRECTORY(${COMPONENTS}/display)
ENDIF()


MESSAGE(STATUS "Chroma Configuration Summary")
MESSAGE(STATUS "--------------------------------")
MESSAGE(STATUS "")
MESSAGE(STATUS "  Version:                   1.0")
MESSAGE(STATUS "  Library interface version: 1.0")
MESSAGE(STATUS "")
MESSAGE(STATUS "  Installation directory:    ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "  C compiler:                ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "  C++ compiler:              ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "")
MESSAGE(STATUS "  Build shared libraries:    ${BUILD_SHARED_LIBS}")
MESSAGE(STATUS "  Mutex Manager:             C++ STL")
MESSAGE(STATUS "  Vulkan SDK:                $ENV{VULKAN_SDK}")
