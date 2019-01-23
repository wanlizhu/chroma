CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(openexr)

SET(OUR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
SET(TARGET openexr)
SET(ILM_BASE_DIR ${OUR_DIR}/IlmBase)
SET(ILM_OPENEXR_DIR ${OUR_DIR}/OpenEXR)

SET(OPENEXR_VERSION_MAJOR 2)
SET(OPENEXR_VERSION_MINOR 3)
SET(OPENEXR_VERSION_PATCH 0)
SET(OPENEXR_VERSION ${OPENEXR_VERSION_MAJOR}.${OPENEXR_VERSION_MINOR}.${OPENEXR_VERSION_PATCH})
SET(OPENEXR_VERSION_API ${OPENEXR_VERSION_MAJOR}_${OPENEXR_VERSION_MINOR})
SET(OPENEXR_SOVERSION 24)


#################################################
# Configurate ILM_BASE library
#################################################
SET(ILMBASE_BASEVERSION   ${OPENEXR_BASEVERSION})
SET(ILMBASE_VERSION_MAJOR ${OPENEXR_VERSION_MAJOR})
SET(ILMBASE_VERSION_MINOR ${OPENEXR_VERSION_MINOR})
SET(ILMBASE_VERSION_PATCH ${OPENEXR_VERSION_PATCH})
SET(ILMBASE_VERSION       ${OPENEXR_VERSION})
SET(ILMBASE_VERSION_API   ${OPENEXR_VERSION_API})

INCLUDE_DIRECTORIES(
    ${EXTERNALS}/zlib
    ${ILM_BASE_DIR}/Iex 
    ${ILM_BASE_DIR}/IexMath 
    ${ILM_BASE_DIR}/Imath 
    ${ILM_BASE_DIR}/Half 
    ${ILM_BASE_DIR}/IlmThread  
    ${ILM_OPENEXR_DIR}/IlmImf
    ${ILM_OPENEXR_DIR}/IlmImfUtil
    ${ILM_BASE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/config)

#################################################
# IlmBaseConfig.h generation
#################################################
SET(ILMBASE_TMP_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/config/IlmBaseConfig.h.in)
IF(WIN32)
    FILE(WRITE  ${ILMBASE_TMP_CONFIG} "#ifdef HAVE_PTHREAD\n")
    FILE(APPEND ${ILMBASE_TMP_CONFIG} "# undef HAVE_PTHREAD\n")
    FILE(APPEND ${ILMBASE_TMP_CONFIG} "#endif\n")
    FILE(APPEND ${ILMBASE_TMP_CONFIG} "#ifdef HAVE_POSIX_SEMAPHORES\n")
    FILE(APPEND ${ILMBASE_TMP_CONFIG} "# undef HAVE_POSIX_SEMAPHORES\n")
    FILE(APPEND ${ILMBASE_TMP_CONFIG} "#endif\n")
    FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define PLATFORM_WINDOWS 1\n")
ELSE()
    IF(APPLE)
        FILE(WRITE  ${ILMBASE_TMP_CONFIG} "#define HAVE_PTHREAD 1\n")
    ELSE()
        FILE(WRITE  ${ILMBASE_TMP_CONFIG} "#define HAVE_PTHREAD 1\n")
        FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define ILMBASE_HAVE_LARGE_STACK  1\n")
        FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define HAVE_POSIX_SEMAPHORES 1\n")
        FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define ILMBASE_HAVE_CONTROL_REGISTER_SUPPORT 1\n")
    ENDIF()
ENDIF()

FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define ILMBASE_INTERNAL_NAMESPACE_CUSTOM 0\n")
FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define IMATH_INTERNAL_NAMESPACE ILM\n")
FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define IEX_INTERNAL_NAMESPACE ILM\n")
FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define ILMTHREAD_INTERNAL_NAMESPACE ILM\n")

FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define IMATH_NAMESPACE ILM\n")
FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define IEX_NAMESPACE ILM\n")
FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define ILMTHREAD_NAMESPACE ILM\n")
FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define ILMBASE_VERSION_STRING \"${ILMBASE_VERSION}\"\n")
FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define ILMBASE_PACKAGE_STRING \"ILMBASE ${ILMBASE_VERSION}\"\n")
FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define ILMBASE_VERSION_MAJOR ${ILMBASE_VERSION_MAJOR}\n")
FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define ILMBASE_VERSION_MINOR ${ILMBASE_VERSION_MINOR}\n")
FILE(APPEND ${ILMBASE_TMP_CONFIG} "#define ILMBASE_VERSION_PATCH ${ILMBASE_VERSION_PATCH}\n")
FILE(APPEND ${ILMBASE_TMP_CONFIG} "// Version as a single hex number, e.g. 0x01000300 == 1.0.3
#define ILMBASE_VERSION_HEX ((ILMBASE_VERSION_MAJOR << 24) | (ILMBASE_VERSION_MINOR << 16) | (ILMBASE_VERSION_PATCH <<  8))")

CONFIGURE_FILE(${ILMBASE_TMP_CONFIG} ${CMAKE_CURRENT_BINARY_DIR}/config/IlmBaseConfig.h COPYONLY)
FILE(REMOVE ${ILMBASE_TMP_CONFIG})
UNSET(ILMBASE_TMP_CONFIG)


#################################################
# OpenEXRConfig.h generation
#################################################
SET(OPENEXR_TMP_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/config/OpenEXRConfig.h.in)
IF(WIN32)
    FILE(WRITE ${OPENEXR_TMP_CONFIG} "#define OPENEXR_IMF_HAVE_COMPLETE_IOMANIP 1\n")
ELSEIF(APPLE)
    FILE(WRITE ${OPENEXR_TMP_CONFIG} "#define OPENEXR_IMF_HAVE_DARWIN 1\n")
    FILE(APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_IMF_HAVE_COMPLETE_IOMANIP 1\n")
    FILE(APPEND ${OPENEXR_TMP_CONFIG} "#include <string.h>\n")
ELSE()
    # Linux
    FILE(WRITE ${OPENEXR_TMP_CONFIG} "#define OPENEXR_IMF_HAVE_LINUX_PROCFS 1\n")
    FILE(APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_IMF_HAVE_COMPLETE_IOMANIP 1\n")
    FILE(APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_IMF_HAVE_LARGE_STACK 1\n")
ENDIF()

FILE(APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_IMF_INTERNAL_NAMESPACE_CUSTOM 0\n")
FILE(APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_IMF_NAMESPACE ILM \n")
FILE(APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_IMF_INTERNAL_NAMESPACE ILM \n\n")

FILE(APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_VERSION_STRING \"${OPENEXR_VERSION}\"\n")
FILE(APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_PACKAGE_STRING \"ILMIMF ${OPENEXR_VERSION}\"\n")

FILE(APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_VERSION_MAJOR ${OPENEXR_VERSION_MAJOR}\n")
FILE(APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_VERSION_MINOR ${OPENEXR_VERSION_MINOR}\n")
FILE(APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_VERSION_PATCH ${OPENEXR_VERSION_PATCH}\n")

  FILE(APPEND ${OPENEXR_TMP_CONFIG} "
// Version as a single hex number, e.g. 0x01000300 == 1.0.3
#define OPENEXR_VERSION_HEX ((OPENEXR_VERSION_MAJOR << 24) | (OPENEXR_VERSION_MINOR << 16) | (OPENEXR_VERSION_PATCH <<  8))\n
")

IF(HAVE_GCC_INLINE_ASM_AVX)
    FILE( APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_IMF_HAVE_GCC_INLINE_ASM_AVX 1\n")
ENDIF()

IF(HAVE_SYSCONF_NPROCESSORS_ONLN)
    FILE(APPEND ${OPENEXR_TMP_CONFIG} "#define OPENEXR_IMF_HAVE_SYSCONF_NPROCESSORS_ONLN 1\n")
ENDIF()

CONFIGURE_FILE(${OPENEXR_TMP_CONFIG} ${CMAKE_CURRENT_BINARY_DIR}/config/OpenEXRConfig.h COPYONLY)
FILE(REMOVE ${OPENEXR_TMP_CONFIG})
UNSET(OPENEXR_TMP_CONFIG)


#################################################
# OpenEXR_ILMBASE Library
#################################################
ADD_EXECUTABLE(OpenEXR_Tool_eLut ${ILM_BASE_DIR}/Half/eLut.cpp)
ADD_EXECUTABLE(OpenEXR_Tool_toFloat ${ILM_BASE_DIR}/Half/toFloat.cpp)

SET_TARGET_PROPERTIES(OpenEXR_Tool_eLut PROPERTIES FOLDER ThirdParty/OpenEXR)
SET_TARGET_PROPERTIES(OpenEXR_Tool_toFloat PROPERTIES FOLDER ThirdParty/OpenEXR)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/eLut.h ${CMAKE_CURRENT_BINARY_DIR}/toFloat.h
    COMMAND $<TARGET_FILE:OpenEXR_Tool_eLut> ARGS > ${CMAKE_CURRENT_BINARY_DIR}/eLut.h
    COMMAND $<TARGET_FILE:OpenEXR_Tool_toFloat> ARGS > ${CMAKE_CURRENT_BINARY_DIR}/toFloat.h
    DEPENDS OpenEXR_Tool_eLut OpenEXR_Tool_toFloat)

SET(OpenEXR_ILMBASE_HEADERS
    ${ILM_BASE_DIR}/Half/half.h
    ${ILM_BASE_DIR}/Half/halfFunction.h
    ${ILM_BASE_DIR}/Half/halfExport.h
    ${ILM_BASE_DIR}/Half/halfLimits.h
    ${ILM_BASE_DIR}/Iex/IexBaseExc.h
    ${ILM_BASE_DIR}/Iex/IexMathExc.h
    ${ILM_BASE_DIR}/Iex/IexThrowErrnoExc.h
    ${ILM_BASE_DIR}/Iex/IexErrnoExc.h
    ${ILM_BASE_DIR}/Iex/IexMacros.h
    ${ILM_BASE_DIR}/Iex/Iex.h
    ${ILM_BASE_DIR}/Iex/IexNamespace.h
    ${ILM_BASE_DIR}/Iex/IexExport.h
    ${ILM_BASE_DIR}/Iex/IexForward.h
    ${ILM_BASE_DIR}/Iexmath/IexMathFloatExc.h
    ${ILM_BASE_DIR}/Iexmath/IexMathFpu.h
    ${ILM_BASE_DIR}/Iexmath/IexMathIeeeExc.h
    ${ILM_BASE_DIR}/IlmThread/IlmThreadPool.h
    ${ILM_BASE_DIR}/IlmThread/IlmThread.h
    ${ILM_BASE_DIR}/IlmThread/IlmThreadSemaphore.h
    ${ILM_BASE_DIR}/IlmThread/IlmThreadMutex.h
    ${ILM_BASE_DIR}/IlmThread/IlmThreadNamespace.h
    ${ILM_BASE_DIR}/IlmThread/IlmThreadExport.h
    ${ILM_BASE_DIR}/IlmThread/IlmThreadForward.h
    ${ILM_BASE_DIR}/Imath/ImathBoxAlgo.h
    ${ILM_BASE_DIR}/Imath/ImathBox.h
    ${ILM_BASE_DIR}/Imath/ImathColorAlgo.h
    ${ILM_BASE_DIR}/Imath/ImathColor.h
    ${ILM_BASE_DIR}/Imath/ImathEuler.h
    ${ILM_BASE_DIR}/Imath/ImathExc.h
    ${ILM_BASE_DIR}/Imath/ImathExport.h
    ${ILM_BASE_DIR}/Imath/ImathForward.h
    ${ILM_BASE_DIR}/Imath/ImathFrame.h
    ${ILM_BASE_DIR}/Imath/ImathFrustum.h
    ${ILM_BASE_DIR}/Imath/ImathFrustumTest.h
    ${ILM_BASE_DIR}/Imath/ImathFun.h
    ${ILM_BASE_DIR}/Imath/ImathGL.h
    ${ILM_BASE_DIR}/Imath/ImathGLU.h
    ${ILM_BASE_DIR}/Imath/ImathHalfLimits.h
    ${ILM_BASE_DIR}/Imath/ImathInt64.h
    ${ILM_BASE_DIR}/Imath/ImathInterval.h
    ${ILM_BASE_DIR}/Imath/ImathLimits.h
    ${ILM_BASE_DIR}/Imath/ImathLineAlgo.h
    ${ILM_BASE_DIR}/Imath/ImathLine.h
    ${ILM_BASE_DIR}/Imath/ImathMath.h
    ${ILM_BASE_DIR}/Imath/ImathMatrixAlgo.h
    ${ILM_BASE_DIR}/Imath/ImathMatrix.h
    ${ILM_BASE_DIR}/Imath/ImathNamespace.h
    ${ILM_BASE_DIR}/Imath/ImathPlane.h
    ${ILM_BASE_DIR}/Imath/ImathPlatform.h
    ${ILM_BASE_DIR}/Imath/ImathQuat.h
    ${ILM_BASE_DIR}/Imath/ImathRandom.h
    ${ILM_BASE_DIR}/Imath/ImathRoots.h
    ${ILM_BASE_DIR}/Imath/ImathShear.h
    ${ILM_BASE_DIR}/Imath/ImathSphere.h
    ${ILM_BASE_DIR}/Imath/ImathVecAlgo.h
    ${ILM_BASE_DIR}/Imath/ImathVec.h
    )

SET(OpenEXR_ILMBASE_SOURCES
    ${ILM_BASE_DIR}/Half/half.cpp
    ${ILM_BASE_DIR}/Iex/IexBaseExc.cpp
    ${ILM_BASE_DIR}/Iex/IexThrowErrnoExc.cpp
    ${ILM_BASE_DIR}/IexMath/IexMathFloatExc.cpp
    ${ILM_BASE_DIR}/IexMath/IexMathFpu.cpp
    ${ILM_BASE_DIR}/Imath/ImathRandom.cpp
    ${ILM_BASE_DIR}/Imath/ImathColorAlgo.cpp
    ${ILM_BASE_DIR}/Imath/ImathFun.cpp
    ${ILM_BASE_DIR}/Imath/ImathVec.cpp
    ${ILM_BASE_DIR}/Imath/ImathMatrixAlgo.cpp
    ${ILM_BASE_DIR}/IlmThread/IlmThread.cpp
    ${ILM_BASE_DIR}/IlmThread/IlmThreadMutex.cpp
    ${ILM_BASE_DIR}/IlmThread/IlmThreadMutexPosix.cpp
    ${ILM_BASE_DIR}/IlmThread/IlmThreadPool.cpp
    ${ILM_BASE_DIR}/IlmThread/IlmThreadPosix.cpp
    ${ILM_BASE_DIR}/IlmThread/IlmThreadSemaphore.cpp
    ${ILM_BASE_DIR}/IlmThread/IlmThreadSemaphorePosixCompat.cpp
    ${ILM_BASE_DIR}/IlmThread/IlmThreadSemaphorePosix.cpp)
IF(WIN32)
    SET(OpenEXR_ILMBASE_SOURCES 
        ${OpenEXR_ILMBASE_SOURCES}
        ${ILM_BASE_DIR}/IlmThread/IlmThreadMutexWin32.cpp
        ${ILM_BASE_DIR}/IlmThread/IlmThreadSemaphoreWin32.cpp
        ${ILM_BASE_DIR}/IlmThread/IlmThreadWin32.cpp)
ENDIF()

ADD_LIBRARY(OpenEXR_ILMBASE STATIC ${OpenEXR_ILMBASE_SOURCES} ${OpenEXR_ILMBASE_HEADERS})
SET_TARGET_PROPERTIES(OpenEXR_ILMBASE
    PROPERTIES
    VERSION ${ILMBASE_VERSION_MAJOR}.${ILMBASE_VERSION_MINOR}.${ILMBASE_VERSION_PATCH}
    OUTPUT_NAME "OpenEXR_ILMBASE"
    FOLDER ThirdParty/OpenEXR)
TARGET_INCLUDE_DIRECTORIES(OpenEXR_ILMBASE PUBLIC ${ILM_BASE_DIR})
TARGET_LINK_LIBRARIES(OpenEXR_ILMBASE PRIVATE zlib)
    
SET_SOURCE_FILES_PROPERTIES(${ILM_BASE_DIR}/Half/half.cpp
    PROPERTIES
    OBJECT_DEPENDS
    "${CMAKE_CURRENT_BINARY_DIR}/eLut.h;${CMAKE_CURRENT_BINARY_DIR}/toFloat.h")

#################################################
# OpenEXR_ILMIMF Library
#################################################
IF(WIN32)
    SET(PTHREAD_LIB "")
ELSE()
    SET(PTHREAD_LIB pthread)
ENDIF()

IF(NOT WIN32 AND NOT APPLE)
    # Test for GCC-style inline asm support with AVX instructions
    INCLUDE(CheckCXXSourceCompiles)
    CHECK_CXX_SOURCE_COMPILES(
        "
        int main()
        {
        #if defined(__GNUC__) && defined(__SSE2__)
            int n   = 0;
            int eax = 0;
            int edx = 0;
            __asm__(
                \"xgetbv     ;\"
                \"vzeroupper  \"
                : \"=a\"(eax), \"=d\"(edx) : \"c\"(n) :);
        #else
            #error No GCC style inline asm supported for AVX instructions
        #endif
        }
        " HAVE_GCC_INLINE_ASM_AVX)
    
    # Check if sysconf(_SC_NPROCESSORS_ONLN) can be used for CPU count
    CHECK_CXX_SOURCE_COMPILES(
        "
        #include <unistd.h>
        int main()
        {
            sysconf(_SC_NPROCESSORS_ONLN);
        }
        " HAVE_SYSCONF_NPROCESSORS_ONLN)
ENDIF()

ADD_EXECUTABLE(OpenEXR_Tool_b44ExpLogTable ${ILM_OPENEXR_DIR}/IlmImf/b44ExpLogTable.cpp)
ADD_EXECUTABLE(OpenEXR_Tool_dwaLookups ${ILM_OPENEXR_DIR}/IlmImf/dwaLookups.cpp)

SET_TARGET_PROPERTIES(OpenEXR_Tool_b44ExpLogTable PROPERTIES FOLDER ThirdParty/OpenEXR)
SET_TARGET_PROPERTIES(OpenEXR_Tool_dwaLookups PROPERTIES FOLDER ThirdParty/OpenEXR)

TARGET_LINK_LIBRARIES(OpenEXR_Tool_b44ExpLogTable OpenEXR_ILMBASE ${PTHREAD_LIB})
TARGET_LINK_LIBRARIES(OpenEXR_Tool_dwaLookups OpenEXR_ILMBASE ${PTHREAD_LIB})

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/b44ExpLogTable.h ${CMAKE_CURRENT_BINARY_DIR}/dwaLookups.h
    COMMAND $<TARGET_FILE:OpenEXR_Tool_b44ExpLogTable> ARGS > ${CMAKE_CURRENT_BINARY_DIR}/b44ExpLogTable.h
    COMMAND $<TARGET_FILE:OpenEXR_Tool_dwaLookups> ARGS > ${CMAKE_CURRENT_BINARY_DIR}/dwaLookups.h
    DEPENDS OpenEXR_Tool_b44ExpLogTable OpenEXR_Tool_dwaLookups)
    
SET(OpenEXR_ILMIMF_HEADERS
    ${ILM_OPENEXR_DIR}/IlmImf/ImfForward.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfExport.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfBoxAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfCRgbaFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfChannelList.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfChannelListAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfCompressionAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDoubleAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfFloatAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfFrameBuffer.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfHeader.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfIO.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfInputFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfIntAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfLineOrderAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfMatrixAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfOpaqueAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfOutputFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfRgbaFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfStringAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfVecAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfHuf.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfWav.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfLut.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfArray.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfCompression.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfLineOrder.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfName.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfPixelType.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfVersion.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfXdr.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfConvert.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfPreviewImage.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfPreviewImageAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfChromaticities.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfChromaticitiesAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfKeyCode.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfKeyCodeAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTimeCode.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTimeCodeAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfRational.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfRationalAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfFramesPerSecond.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfStandardAttributes.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfStdIO.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfEnvmap.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfEnvmapAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfInt64.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfRgba.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTileDescription.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTileDescriptionAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTiledInputFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTiledOutputFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTiledRgbaFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfRgbaYca.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTestFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfThreading.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfB44Compressor.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfStringVectorAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfMultiView.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfAcesFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfMultiPartOutputFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfGenericOutputFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfMultiPartInputFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfGenericInputFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfPartType.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfPartHelper.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfOutputPart.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTiledOutputPart.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfInputPart.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTiledInputPart.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepScanLineOutputFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepScanLineOutputPart.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepScanLineInputFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepScanLineInputPart.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepTiledInputFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepTiledInputPart.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepTiledOutputFile.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepTiledOutputPart.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepFrameBuffer.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepCompositing.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfCompositeDeepScanLine.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfNamespace.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfMisc.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepImageState.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepImageStateAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImf/ImfFloatVectorAttribute.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfImageChannel.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfFlatImageChannel.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfDeepImageChannel.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfSampleCountChannel.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfImageLevel.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfFlatImageLevel.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfDeepImageLevel.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfImage.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfFlatImage.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfDeepImage.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfImageIO.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfFlatImageIO.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfDeepImageIO.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfImageDataWindow.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfImageChannelRenaming.h
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfUtilExport.h)
    
SET(OpenEXR_ILMIMF_SOURCES
    ${ILM_OPENEXR_DIR}/IlmImf/ImfAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfBoxAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfCRgbaFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfChannelList.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfChannelListAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfFloatAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfFrameBuffer.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfHeader.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfIO.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfInputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfIntAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfLineOrderAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfMatrixAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfOpaqueAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfOutputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfRgbaFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfStringAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfVecAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfHuf.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfThreading.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfWav.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfLut.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfCompressor.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfRleCompressor.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfZipCompressor.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfPizCompressor.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfB44Compressor.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDwaCompressor.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfMisc.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfCompressionAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDoubleAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfConvert.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfPreviewImage.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfPreviewImageAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfVersion.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfChromaticities.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfChromaticitiesAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfKeyCode.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfKeyCodeAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTimeCode.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTimeCodeAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfRational.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfRationalAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfFramesPerSecond.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfStandardAttributes.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfStdIO.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfEnvmap.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfEnvmapAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfScanLineInputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTiledInputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTiledMisc.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTiledOutputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTiledRgbaFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTileDescriptionAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTileOffsets.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfRgbaYca.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfPxr24Compressor.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTestFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfStringVectorAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfMultiView.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfAcesFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfMultiPartOutputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfGenericOutputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfOutputPartData.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfMultiPartInputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfGenericInputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfPartType.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfInputPartData.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfOutputPart.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTiledOutputPart.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfInputPart.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfTiledInputPart.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepScanLineInputPart.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepScanLineOutputPart.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepScanLineInputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepScanLineOutputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepTiledInputPart.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepTiledOutputPart.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepTiledInputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepTiledOutputFile.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepFrameBuffer.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepCompositing.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfCompositeDeepScanLine.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfDeepImageStateAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfFastHuf.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfFloatVectorAttribute.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfRle.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfSystemSpecific.cpp
    ${ILM_OPENEXR_DIR}/IlmImf/ImfZip.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfImageChannel.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfFlatImageChannel.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfDeepImageChannel.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfSampleCountChannel.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfImageLevel.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfFlatImageLevel.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfDeepImageLevel.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfImage.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfFlatImage.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfDeepImage.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfImageIO.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfFlatImageIO.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfDeepImageIO.cpp
    ${ILM_OPENEXR_DIR}/IlmImfUtil/ImfImageDataWindow.cpp)

ADD_LIBRARY(OpenEXR_ILMIMF STATIC ${OpenEXR_ILMIMF_SOURCES} ${OpenEXR_ILMIMF_HEADERS})
SET_TARGET_PROPERTIES(OpenEXR_ILMIMF
    PROPERTIES
    VERSION ${OPENEXR_VERSION_MAJOR}.${OPENEXR_VERSION_MINOR}.${OPENEXR_VERSION_PATCH}
    OUTPUT_NAME "OpenEXR_ILMIMF"
    FOLDER ThirdParty/OpenEXR)
TARGET_INCLUDE_DIRECTORIES(OpenEXR_ILMIMF PUBLIC ${ILM_OPENEXR_DIR}/IlmImf ${ILM_OPENEXR_DIR}/IlmImfUtil)
TARGET_LINK_LIBRARIES(OpenEXR_ILMIMF PUBLIC OpenEXR_ILMBASE)
ADD_DEPENDENCIES(OpenEXR_ILMIMF OpenEXR_ILMBASE)

IF(WIN32)
    TARGET_COMPILE_DEFINITIONS(OpenEXR_ILMIMF PUBLIC _CRT_SECURE_NO_WARNINGS)
ENDIF()

SET_SOURCE_FILES_PROPERTIES (${ILM_OPENEXR_DIR}/IlmImf/ImfB44Compressor.cpp
    PROPERTIES
    OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/b44ExpLogTable.h)
SET_SOURCE_FILES_PROPERTIES (${ILM_OPENEXR_DIR}/IlmImf/ImfDwaCompressor.cpp
    PROPERTIES
    OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/dwaLookups.h)
