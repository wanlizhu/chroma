SET(TARGET jsoncpp)

IF( CMAKE_COMPILER_IS_GNUCXX )
    #Get compiler version.
    EXECUTE_PROCESS( COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                     OUTPUT_VARIABLE GNUCXX_VERSION )

    #-Werror=* was introduced -after- GCC 4.1.2
    IF( GNUCXX_VERSION VERSION_GREATER 4.1.2 )
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=strict-aliasing")
    ENDIF()
ENDIF( CMAKE_COMPILER_IS_GNUCXX )

INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckTypeSize)
INCLUDE(CheckStructHasMember)
INCLUDE(CheckCXXSymbolExists)

check_include_file_cxx(clocale HAVE_CLOCALE)
check_cxx_symbol_exists(localeconv clocale HAVE_LOCALECONV)

IF(CMAKE_VERSION VERSION_LESS 3.0.0)
    # The "LANGUAGE CXX" parameter is not supported in CMake versions below 3,
    # so the C compiler and header has to be used.
    check_include_file(locale.h HAVE_LOCALE_H)
    SET(CMAKE_EXTRA_INCLUDE_FILES locale.h)
    check_type_size("struct lconv" LCONV_SIZE)
    UNSET(CMAKE_EXTRA_INCLUDE_FILES)
    check_struct_has_member("struct lconv" decimal_point locale.h HAVE_DECIMAL_POINT)
ELSE()
    SET(CMAKE_EXTRA_INCLUDE_FILES clocale)
    check_type_size(lconv LCONV_SIZE LANGUAGE CXX)
    UNSET(CMAKE_EXTRA_INCLUDE_FILES)
    check_struct_has_member(lconv decimal_point clocale HAVE_DECIMAL_POINT LANGUAGE CXX)
ENDIF()

IF(NOT (HAVE_CLOCALE AND HAVE_LCONV_SIZE AND HAVE_DECIMAL_POINT AND HAVE_LOCALECONV))
    MESSAGE(WARNING "Locale functionality is not supported")
    ADD_DEFINITIONS(-DJSONCPP_NO_LOCALE_SUPPORT)
ENDIF()

SET( JSONCPP_INCLUDE_DIR ../../include )

SET( PUBLIC_HEADERS
    ${JSONCPP_INCLUDE_DIR}/json/config.h
    ${JSONCPP_INCLUDE_DIR}/json/forwards.h
    ${JSONCPP_INCLUDE_DIR}/json/features.h
    ${JSONCPP_INCLUDE_DIR}/json/value.h
    ${JSONCPP_INCLUDE_DIR}/json/reader.h
    ${JSONCPP_INCLUDE_DIR}/json/writer.h
    ${JSONCPP_INCLUDE_DIR}/json/assertions.h
    ${JSONCPP_INCLUDE_DIR}/json/version.h
    )

SOURCE_GROUP( "Public API" FILES ${PUBLIC_HEADERS} )

SET(jsoncpp_sources
                json_tool.h
                json_reader.cpp
                json_valueiterator.inl
                json_value.cpp
                json_writer.cpp
                version.h.in)

# Install instructions for this target
IF(JSONCPP_WITH_CMAKE_PACKAGE)
    SET(INSTALL_EXPORT EXPORT ${TARGET})
ELSE(JSONCPP_WITH_CMAKE_PACKAGE)
    SET(INSTALL_EXPORT)
ENDIF()

IF(BUILD_STATIC_LIBS)
    ADD_LIBRARY(${TARGET} STATIC ${PUBLIC_HEADERS} ${jsoncpp_sources})
    SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES 
        VERSION ${JSONCPP_VERSION} 
        SOVERSION ${JSONCPP_SOVERSION}
        FOLDER "Externals")
        
    # avoid name clashes on windows as the shared import lib is also named ${TARGET}.lib
    if (NOT DEFINED STATIC_SUFFIX AND BUILD_SHARED_LIBS)
      set (STATIC_SUFFIX "")
    endif ()
    set_target_properties (${TARGET} PROPERTIES OUTPUT_NAME ${TARGET}${STATIC_SUFFIX}
                                                         DEBUG_OUTPUT_NAME ${TARGET}${STATIC_SUFFIX}${DEBUG_LIBNAME_SUFFIX})

    INSTALL( TARGETS ${TARGET} ${INSTALL_EXPORT}
             RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
             LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
             ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

    IF(NOT CMAKE_VERSION VERSION_LESS 2.8.11)
        TARGET_INCLUDE_DIRECTORIES( ${TARGET} PUBLIC
                                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${JSONCPP_INCLUDE_DIR}>
                                )
    ENDIF()

ENDIF()
