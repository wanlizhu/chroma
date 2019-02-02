CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(glslang)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(GLSLANG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

MACRO(glslang_pch SRCS PCHCPP)
    IF(MSVC)
        IF(CMAKE_GENERATOR MATCHES "^Visual Studio")
            SET(PCH_NAME "$(IntDir)\\pch.pch")
        ELSE()
            SET(PCH_NAME "${CMAKE_CURRENT_BINARY_DIR}/../pch.pch")
        ENDIF()
    
        SET_SOURCE_FILES_PROPERTIES(${${SRCS}} PROPERTIES 
                                    COMPILE_FLAGS "/Yupch.h /FIpch.h /Fp${PCH_NAME} /Zm300" 
                                    OBJECT_DEPENDS "${PCH_NAME}")

        SET_SOURCE_FILES_PROPERTIES(${PCHCPP} PROPERTIES 
                                    COMPILE_FLAGS "/Ycpch.h /Fp${PCH_NAME} /Zm300"
                                    OBJECT_OUTPUTS "${PCH_NAME}")
        LIST(APPEND ${SRCS} "${PCHCPP}")
  ENDIF()
ENDMACRO()

FUNCTION(glslang_set_link_args TARGET)
    IF(WIN32 AND ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
        SET_TARGET_PROPERTIES(${TARGET} PROPERTIES
                              LINK_FLAGS "-static -static-libgcc -static-libstdc++")
    ENDIF()
ENDFUNCTION()


ADD_DEFINITIONS(-DAMD_EXTENSIONS
                -DNV_EXTENSIONS
                -DENABLE_HLSL
                -DENABLE_OPT=1
                -DENABLE_OPT=0)

IF(WIN32)
    ADD_DEFINITIONS(-DGLSLANG_OSINCLUDE_WIN32)
ELSEIF(UNIX)
    ADD_DEFINITIONS(-DGLSLANG_OSINCLUDE_UNIX)
ELSE()
    MESSAGE("Unknown Platform")
ENDIF()

IF(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    ADD_COMPILE_OPTIONS(-Wall -Wmaybe-uninitialized -Wuninitialized -Wunused -Wunused-local-typedefs
                        -Wunused-parameter -Wunused-value  -Wunused-variable -Wunused-but-set-parameter -Wunused-but-set-variable -fno-exceptions)
    ADD_COMPILE_OPTIONS(-Wno-reorder)   
ELSEIF(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    ADD_COMPILE_OPTIONS(-Wall -Wuninitialized -Wunused -Wunused-local-typedefs
                        -Wunused-parameter -Wunused-value  -Wunused-variable)
    ADD_COMPILE_OPTIONS(-Wno-reorder)  
ENDIF()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)
ADD_SUBDIRECTORY(glslang)
ADD_SUBDIRECTORY(OGLCompilersDLL)
ADD_SUBDIRECTORY(StandAlone)
ADD_SUBDIRECTORY(SPIRV)
ADD_SUBDIRECTORY(hlsl)
