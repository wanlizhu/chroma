ADD_LIBRARY(glslang-default-resource-limits STATIC
            ${GLSLANG_DIR}/StandAlone/ResourceLimits.cpp)
SET_PROPERTY(TARGET glslang-default-resource-limits PROPERTY FOLDER Externals/glslang)
SET_PROPERTY(TARGET glslang-default-resource-limits PROPERTY POSITION_INDEPENDENT_CODE ON)

TARGET_INCLUDE_DIRECTORIES(glslang-default-resource-limits
                           PUBLIC ${GLSLANG_DIR}/StandAlone)

SET(SOURCES ${GLSLANG_DIR}/StandAlone/StandAlone.cpp ${GLSLANG_DIR}/StandAlone/DirStackFileIncluder.h)
SET(REMAPPER_SOURCES ${GLSLANG_DIR}/StandAlone/spirv-remap.cpp)

ADD_EXECUTABLE(glslangValidator ${SOURCES})
ADD_EXECUTABLE(spirv-remap ${REMAPPER_SOURCES})
SET_PROPERTY(TARGET glslangValidator PROPERTY FOLDER Externals/glslang/tools)
SET_PROPERTY(TARGET spirv-remap PROPERTY FOLDER Externals/glslang/tools)
glslang_set_link_args(glslangValidator)
glslang_set_link_args(spirv-remap)

SET(LIBRARIES
    glslang
    SPIRV
    SPVRemapper
    glslang-default-resource-limits)

IF(WIN32)
    SET(LIBRARIES ${LIBRARIES} psapi)
ELSEIF(UNIX)
    IF(NOT ANDROID)
        SET(LIBRARIES ${LIBRARIES} pthread)
    ENDIF()
ENDIF()

TARGET_LINK_LIBRARIES(glslangValidator ${LIBRARIES})
TARGET_LINK_LIBRARIES(spirv-remap ${LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(glslangValidator PUBLIC ${GLSLANG_DIR}/StandAlone/../External)

IF(WIN32)
    SOURCE_GROUP("Source" FILES ${SOURCES})
ENDIF()
