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
# Target definition
# ===============================================
INCLUDE_DIRECTORIES(${PUBLIC_INCLUDE_DIRS} ${GENERATION_DIR} src)

IF(CHROMA_USE_VULKAN)
    FIND_PACKAGE(Vulkan REQUIRED)
    ADD_DEFINITIONS(-DCHROMA_USE_VULKAN)
    SET(PRIVATE_INCLUDE_DIRS ${PRIVATE_INCLUDE_DIRS} ${Vulkan_INCLUDE_DIRS})
    SET(PRIVATE_LINK_DEPS ${PRIVATE_LINK_DEPS} ${Vulkan_LIBRARIES})
ENDIF()

ADD_LIBRARY(${TARGET} STATIC ${PRIVATE_HDRS} ${PUBLIC_HDRS} ${SRCS})
TARGET_INCLUDE_DIRECTORIES(${TARGET} 
                           PUBLIC ${PUBLIC_INCLUDE_DIRS}
                           #PRIVATE ${PRIVATE_INCLUDE_DIRS})
                           PUBLIC ${PRIVATE_INCLUDE_DIRS}) # FOR DEBUG 
TARGET_LINK_LIBRARIES(${TARGET} 
                           PUBLIC ${PUBLIC_LINK_DEPS}
                           PRIVATE ${PRIVATE_LINK_DEPS})


# ===============================================
# Installation
# ===============================================
SET(INSTALL_TYPE ARCHIVE)
INSTALL(TARGETS ${TARGET} ${INSTALL_TYPE} DESTINATION lib)
INSTALL(DIRECTORY ${PUBLIC_INCLUDE_DIRS}/chroma DESTINATION include)

# ===============================================
# Sub-projects
# ===============================================
#ADD_SUBDIRECTORY(test)
#ADD_SUBDIRECTORY(benchmark)
