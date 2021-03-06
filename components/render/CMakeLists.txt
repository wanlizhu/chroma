CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(render)

SET(TARGET render)
SET(PUBLIC_INCLUDE_DIRS include)
SET(PRIVATE_INCLUDE_DIRS "src")
SET(PUBLIC_LINK_DEPS numeric system)
SET(PRIVATE_LINK_DEPS glad)

# ===============================================
# Sources and headers
# ===============================================
FILE(GLOB_RECURSE PUBLIC_HDRS ${PUBLIC_INCLUDE_DIRS}/${TARGET}/*.h)
FILE(GLOB_RECURSE PRIVATE_HDRS src/*.h)
FILE(GLOB_RECURSE SRCS src/*.cpp)

# ===============================================
# Find Vulkan
# ===============================================
FIND_PACKAGE(Vulkan REQUIRED)
SET(PRIVATE_LINK_DEPS ${PRIVATE_LINK_DEPS} Vulkan::Vulkan)
IF(WIN32)
    ADD_DEFINITIONS(-DVK_USE_PLATFORM_WIN32_KHR 
                    -DVK_USE_PLATFORM_WIN32_KHX 
                    -DWIN32_LEAN_AND_MEAN)
ENDIF()

# ===============================================
# Target definition
# ===============================================
ADD_LIBRARY(${TARGET} STATIC ${PRIVATE_HDRS} ${PUBLIC_HDRS} ${SRCS})
TARGET_INCLUDE_DIRECTORIES(${TARGET} 
                           PUBLIC ${PUBLIC_INCLUDE_DIRS}
                           PRIVATE ${PRIVATE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${TARGET} 
                      PUBLIC ${PUBLIC_LINK_DEPS}
                      PRIVATE ${PRIVATE_LINK_DEPS})


# ===============================================
# Installation
# ===============================================
SET(INSTALL_TYPE ARCHIVE)
INSTALL(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib)
INSTALL(DIRECTORY ${PUBLIC_INCLUDE_DIRS}/${TARGET} DESTINATION include)

