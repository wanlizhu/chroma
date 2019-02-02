CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(jsoncpp)

SET(TARGET jsoncpp)
IF(MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4 ")
ENDIF()

IF(APPLE)
    SET(CMAKE_MACOSX_RPATH 1)
ENDIF()

IF(NOT WIN32 AND NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release FORCE)
ENDIF()

# Adhere to GNU filesystem layout conventions
INCLUDE(GNUInstallDirs)

SET(JSONCPP_VERSION 1.8.4)
SET(JSONCPP_SOVERSION 19)
SET(JSONCPP_USE_SECURE_MEMORY "0")
SET(JSONCPP_INCLUDE_DIR ../include )
SET(PUBLIC_HEADERS
    ${JSONCPP_INCLUDE_DIR}/json/config.h
    ${JSONCPP_INCLUDE_DIR}/json/forwards.h
    ${JSONCPP_INCLUDE_DIR}/json/features.h
    ${JSONCPP_INCLUDE_DIR}/json/value.h
    ${JSONCPP_INCLUDE_DIR}/json/reader.h
    ${JSONCPP_INCLUDE_DIR}/json/writer.h
    ${JSONCPP_INCLUDE_DIR}/json/assertions.h
    ${JSONCPP_INCLUDE_DIR}/json/version.h)
SOURCE_GROUP("Public API" FILES ${PUBLIC_HEADERS} )
SET(jsoncpp_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lib_json/json_tool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lib_json/json_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lib_json/json_valueiterator.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lib_json/json_value.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lib_json/json_writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lib_json/version.h.in)

# File version.h is only regenerated on CMake configure step
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/../src/lib_json/version.h.in"
                "${PROJECT_SOURCE_DIR}/../include/json/version.h"
                NEWLINE_STYLE UNIX )
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/../version.in"
                "${PROJECT_SOURCE_DIR}/../version"
                NEWLINE_STYLE UNIX )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../include )

IF(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wconversion -Wshadow -Werror=conversion -Werror=sign-compare")
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wconversion -Wshadow -Wextra")
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wconversion -Wshadow -Wextra -Werror=conversion")
ENDIF()

INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckTypeSize)
INCLUDE(CheckStructHasMember)
INCLUDE(CheckCXXSymbolExists)

check_include_file_cxx(clocale HAVE_CLOCALE)
check_cxx_symbol_exists(localeconv clocale HAVE_LOCALECONV)

SET(CMAKE_EXTRA_INCLUDE_FILES clocale)
check_type_size(lconv LCONV_SIZE LANGUAGE CXX)
UNSET(CMAKE_EXTRA_INCLUDE_FILES)
check_struct_has_member(lconv decimal_point clocale HAVE_DECIMAL_POINT LANGUAGE CXX)
    
IF(NOT (HAVE_CLOCALE AND HAVE_LCONV_SIZE AND HAVE_DECIMAL_POINT AND HAVE_LOCALECONV))
    MESSAGE(WARNING "Locale functionality is not supported")
    ADD_DEFINITIONS(-DJSONCPP_NO_LOCALE_SUPPORT)
ENDIF()

# Target definition
ADD_LIBRARY(${TARGET} STATIC ${PUBLIC_HEADERS} ${jsoncpp_sources})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES 
                      VERSION ${JSONCPP_VERSION} 
                      SOVERSION ${JSONCPP_SOVERSION}
                      OUTPUT_NAME ${TARGET}
                      DEBUG_OUTPUT_NAME ${TARGET}
                      FOLDER Externals)
TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${JSONCPP_INCLUDE_DIR})
