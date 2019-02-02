set(SOURCES
    ${GLSLANG_DIR}/hlsl/hlslAttributes.cpp
    ${GLSLANG_DIR}/hlsl/hlslParseHelper.cpp
    ${GLSLANG_DIR}/hlsl/hlslScanContext.cpp
    ${GLSLANG_DIR}/hlsl/hlslOpMap.cpp
    ${GLSLANG_DIR}/hlsl/hlslTokenStream.cpp
    ${GLSLANG_DIR}/hlsl/hlslGrammar.cpp
    ${GLSLANG_DIR}/hlsl/hlslParseables.cpp)

set(HEADERS
    ${GLSLANG_DIR}/hlsl/hlslAttributes.h
    ${GLSLANG_DIR}/hlsl/hlslParseHelper.h
    ${GLSLANG_DIR}/hlsl/hlslTokens.h
    ${GLSLANG_DIR}/hlsl/hlslScanContext.h
    ${GLSLANG_DIR}/hlsl/hlslOpMap.h
    ${GLSLANG_DIR}/hlsl/hlslTokenStream.h
    ${GLSLANG_DIR}/hlsl/hlslGrammar.h
    ${GLSLANG_DIR}/hlsl/hlslParseables.h)

glslang_pch(SOURCES ${GLSLANG_DIR}/hlsl/pch.cpp)

ADD_LIBRARY(HLSL STATIC ${LIB_TYPE} ${SOURCES} ${HEADERS})
SET_PROPERTY(TARGET HLSL PROPERTY FOLDER Externals/glslang)
SET_PROPERTY(TARGET HLSL PROPERTY POSITION_INDEPENDENT_CODE ON)

IF(WIN32)
    SOURCE_GROUP("Source" FILES ${SOURCES} ${HEADERS})
ENDIF()
