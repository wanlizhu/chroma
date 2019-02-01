CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(chroma)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

OPTION(CHROMA_BUILD_DEMO "Build demo application" ON)
OPTION(CHROMA_BUILD_TEST "Build test for all components" ON)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(BUILD_SHARED_LIBS OFF)
SET(BUILD_STATIC_LIBS ON)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# ===============================================
#  Paths
# ===============================================
SET(EXTERNALS ${CMAKE_CURRENT_SOURCE_DIR}/externals)
SET(COMPONENTS ${CMAKE_CURRENT_SOURCE_DIR}/components)
SET(TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools)

# ===============================================
# Sub-directories
# ===============================================
ADD_SUBDIRECTORY(${EXTERNALS}/zlib/chroma)
ADD_SUBDIRECTORY(${EXTERNALS}/libgtest/chroma)
ADD_SUBDIRECTORY(${EXTERNALS}/glad/chroma)
ADD_SUBDIRECTORY(${EXTERNALS}/glfw/chroma)
ADD_SUBDIRECTORY(${EXTERNALS}/getopt/chroma)
ADD_SUBDIRECTORY(${EXTERNALS}/jsoncpp/chroma)
ADD_SUBDIRECTORY(${EXTERNALS}/rapidxml/chroma)
ADD_SUBDIRECTORY(${EXTERNALS}/tinyobjloader/chroma)
ADD_SUBDIRECTORY(${EXTERNALS}/glslang/chroma)
ADD_SUBDIRECTORY(${COMPONENTS}/numeric)
ADD_SUBDIRECTORY(${COMPONENTS}/system)
ADD_SUBDIRECTORY(${COMPONENTS}/render)
IF(CHROMA_BUILD_DEMO)
    ADD_SUBDIRECTORY(demo)
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
MESSAGE(STATUS "  Vulkan SDK:                $ENV{VULKAN_SDK}")
