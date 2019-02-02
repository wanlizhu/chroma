IF(WIN32)
    ADD_SUBDIRECTORY(OSDependent/Windows)
ELSEIF(UNIX)
    ADD_SUBDIRECTORY(OSDependent/Unix)
ELSE()
    MESSAGE("Unknown Platform")
ENDIF()

SET(SOURCES
    ${GLSLANG_DIR}/glslang/MachineIndependent/glslang.y
    ${GLSLANG_DIR}/glslang/MachineIndependent/glslang_tab.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/attribute.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/Constant.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/iomapper.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/InfoSink.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/Initialize.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/IntermTraverse.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/Intermediate.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/ParseContextBase.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/ParseHelper.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/PoolAlloc.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/RemoveTree.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/Scan.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/ShaderLang.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/SymbolTable.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/Versions.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/intermOut.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/limits.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/linkValidate.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/parseConst.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/reflection.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/preprocessor/Pp.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/preprocessor/PpAtom.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/preprocessor/PpContext.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/preprocessor/PpScanner.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/preprocessor/PpTokens.cpp
    ${GLSLANG_DIR}/glslang/MachineIndependent/propagateNoContraction.cpp
    ${GLSLANG_DIR}/glslang/GenericCodeGen/CodeGen.cpp
    ${GLSLANG_DIR}/glslang/GenericCodeGen/Link.cpp)

SET(HEADERS
    ${GLSLANG_DIR}/glslang/Public/ShaderLang.h
    ${GLSLANG_DIR}/glslang/Include/arrays.h
    ${GLSLANG_DIR}/glslang/Include/BaseTypes.h
    ${GLSLANG_DIR}/glslang/Include/Common.h
    ${GLSLANG_DIR}/glslang/Include/ConstantUnion.h
    ${GLSLANG_DIR}/glslang/Include/InfoSink.h
    ${GLSLANG_DIR}/glslang/Include/InitializeGlobals.h
    ${GLSLANG_DIR}/glslang/Include/intermediate.h
    ${GLSLANG_DIR}/glslang/Include/PoolAlloc.h
    ${GLSLANG_DIR}/glslang/Include/ResourceLimits.h
    ${GLSLANG_DIR}/glslang/Include/revision.h
    ${GLSLANG_DIR}/glslang/Include/ShHandle.h
    ${GLSLANG_DIR}/glslang/Include/Types.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/attribute.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/glslang_tab.cpp.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/gl_types.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/Initialize.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/iomapper.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/LiveTraverser.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/localintermediate.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/ParseHelper.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/reflection.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/RemoveTree.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/Scan.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/ScanContext.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/SymbolTable.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/Versions.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/parseVersions.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/propagateNoContraction.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/preprocessor/PpContext.h
    ${GLSLANG_DIR}/glslang/MachineIndependent/preprocessor/PpTokens.h)

glslang_pch(SOURCES ${GLSLANG_DIR}/glslang/MachineIndependent/pch.cpp)

ADD_LIBRARY(glslang ${LIB_TYPE} ${BISON_GLSLParser_OUTPUT_SOURCE} ${SOURCES} ${HEADERS})
SET_PROPERTY(TARGET glslang PROPERTY FOLDER Externals/glslang)
SET_PROPERTY(TARGET glslang PROPERTY POSITION_INDEPENDENT_CODE ON)
TARGET_LINK_LIBRARIES(glslang OGLCompiler OSDependent HLSL)
TARGET_INCLUDE_DIRECTORIES(glslang PUBLIC ${GLSLANG_DIR}/glslang/Include ${GLSLANG_DIR}/glslang/..)

IF(WIN32)
    SOURCE_GROUP("Public" REGULAR_EXPRESSION "Public/*")
    SOURCE_GROUP("MachineIndependent" REGULAR_EXPRESSION "MachineIndependent/[^/]*")
    SOURCE_GROUP("Include" REGULAR_EXPRESSION "Include/[^/]*")
    SOURCE_GROUP("GenericCodeGen" REGULAR_EXPRESSION "GenericCodeGen/*")
    SOURCE_GROUP("MachineIndependent\\Preprocessor" REGULAR_EXPRESSION "MachineIndependent/preprocessor/*")
ENDIF()
