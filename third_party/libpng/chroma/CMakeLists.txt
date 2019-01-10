CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(png)

SET(OUR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
SET(TARGET png)
SET(SRC_DIR ${OUR_DIR})
SET(PUBLIC_HDR_DIR ${OUR_DIR})
SET(PRIVATE_HDR_DIR ${OUR_DIR})

# public headers dependencies
SET(PUBLIC_HDRS
    ${PUBLIC_HDR_DIR}/png.h
    ${PUBLIC_HDR_DIR}/pngconf.h
    ${PUBLIC_HDR_DIR}/pnglibconf.h)

# private headers dependencies
SET(PRIVATE_HDRS
    ${PRIVATE_HDR_DIR}/pngdebug.h
    ${PRIVATE_HDR_DIR}/pnginfo.h
    ${PRIVATE_HDR_DIR}/pngpriv.h
    ${PRIVATE_HDR_DIR}/pngstruct.h)

# sources dependencies
SET(SRCS
    ${SRC_DIR}/png.c
    ${SRC_DIR}/pngerror.c
    ${SRC_DIR}/pngget.c
    ${SRC_DIR}/pngmem.c
    ${SRC_DIR}/pngpread.c
    ${SRC_DIR}/pngread.c
    ${SRC_DIR}/pngrio.c
    ${SRC_DIR}/pngrtran.c
    ${SRC_DIR}/pngrutil.c
    ${SRC_DIR}/pngset.c
    ${SRC_DIR}/pngtrans.c
    ${SRC_DIR}/pngwio.c
    ${SRC_DIR}/pngwrite.c
    ${SRC_DIR}/pngwtran.c
    ${SRC_DIR}/pngwutil.c)

IF (NOT LINUX AND NOT WIN32)
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
ENDIF()

# we're building a library
ADD_LIBRARY(${TARGET} STATIC ${PRIVATE_HDRS} ${PUBLIC_HDRS} ${SRCS})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES FOLDER ThirdParty)
TARGET_LINK_LIBRARIES(${TARGET} PUBLIC z)

# specify where the public headers of this library are
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})
