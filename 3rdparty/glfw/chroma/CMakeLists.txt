CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

PROJECT(glfw C)

SET(GLFW_LIB_NAME glfw)
SET(GLFW_VERSION_MAJOR "3")
SET(GLFW_VERSION_MINOR "2")
SET(GLFW_VERSION_PATCH "1")
SET(GLFW_VERSION_EXTRA "")
SET(GLFW_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
SET(GLFW_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
SET(GLFW_VERSION "${GLFW_VERSION_MAJOR}.${GLFW_VERSION_MINOR}")
SET(GLFW_VERSION_FULL "${GLFW_VERSION}.${GLFW_VERSION_PATCH}${GLFW_VERSION_EXTRA}")

LIST(APPEND CMAKE_MODULE_PATH "${GLFW_SOURCE_DIR}/CMake/modules")
FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(Vulkan)

IF(VULKAN_FOUND AND VULKAN_STATIC_LIBRARY)
    SET(_GLFW_VULKAN_STATIC 1)
    LIST(APPEND GLFW_LIBRARIES ${VULKAN_STATIC_LIBRARY})
ENDIF()

SET(COMMON_HEADERS 
        "${GLFW_SOURCE_DIR}/src/internal.h"
        "${GLFW_SOURCE_DIR}/src/GLFW_config.h"
        "${GLFW_SOURCE_DIR}/include/GLFW/glfw3.h"
        "${GLFW_SOURCE_DIR}/include/GLFW/glfw3native.h")
SET(COMMON_SOURCES 
        "${GLFW_SOURCE_DIR}/src/context.c" 
        "${GLFW_SOURCE_DIR}/src/init.c" 
        "${GLFW_SOURCE_DIR}/src/input.c" 
        "${GLFW_SOURCE_DIR}/src/monitor.c" 
        "${GLFW_SOURCE_DIR}/src/vulkan.c" 
        "${GLFW_SOURCE_DIR}/src/window.c")

IF(WIN32)
    SET(_GLFW_WIN32 1)
    SET(_GLFW_USE_HYBRID_HPG 1)
    SET(GLFW_HEADERS ${COMMON_HEADERS} 
            "${GLFW_SOURCE_DIR}/src/win32_platform.h" 
            "${GLFW_SOURCE_DIR}/src/win32_joystick.h" 
            "${GLFW_SOURCE_DIR}/src/wgl_context.h" 
            "${GLFW_SOURCE_DIR}/src/egl_context.h")
    SET(GLFW_SOURCES ${COMMON_SOURCES} 
            "${GLFW_SOURCE_DIR}/src/win32_init.c"
            "${GLFW_SOURCE_DIR}/src/win32_joystick.c" 
            "${GLFW_SOURCE_DIR}/src/win32_monitor.c" 
            "${GLFW_SOURCE_DIR}/src/win32_time.c" 
            "${GLFW_SOURCE_DIR}/src/win32_tls.c" 
            "${GLFW_SOURCE_DIR}/src/win32_window.c"
            "${GLFW_SOURCE_DIR}/src/wgl_context.c" 
            "${GLFW_SOURCE_DIR}/src/egl_context.c")

    LIST(APPEND GLFW_PKG_LIBS "-lgdi32")
ENDIF()

IF(APPLE)
    SET(_GLFW_COCOA 1)
    SET(_GLFW_USE_CHDIR 1)
    SET(_GLFW_USE_MENUBAR 1)
    SET(_GLFW_USE_RETINA 1)
    SET(GLFW_HEADERS ${COMMON_HEADERS} 
            "${GLFW_SOURCE_DIR}/src/cocoa_platform.h" 
            "${GLFW_SOURCE_DIR}/src/cocoa_joystick.h"
            "${GLFW_SOURCE_DIR}/src/posix_tls.h"
            "${GLFW_SOURCE_DIR}/src/nsgl_context.h")
    SET(GLFW_SOURCES ${COMMON_SOURCES} 
            "${GLFW_SOURCE_DIR}/src/cocoa_init.m" 
            "${GLFW_SOURCE_DIR}/src/cocoa_joystick.m"
            "${GLFW_SOURCE_DIR}/src/cocoa_monitor.m" 
            "${GLFW_SOURCE_DIR}/src/cocoa_window.m"
            "${GLFW_SOURCE_DIR}/src/cocoa_time.c" 
            "${GLFW_SOURCE_DIR}/src/posix_tls.c" 
            "${GLFW_SOURCE_DIR}/src/nsgl_context.m")

    SET_SOURCE_FILES_PROPERTIES(${GLFW_SOURCES} PROPERTIES LANGUAGE C)

    FIND_LIBRARY(COCOA_FRAMEWORK Cocoa)
    FIND_LIBRARY(IOKIT_FRAMEWORK IOKit)
    FIND_LIBRARY(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
    FIND_LIBRARY(CORE_VIDEO_FRAMEWORK CoreVideo)
    LIST(APPEND GLFW_LIBRARIES 
            "${COCOA_FRAMEWORK}"
            "${IOKIT_FRAMEWORK}"
            "${CORE_FOUNDATION_FRAMEWORK}"
            "${CORE_VIDEO_FRAMEWORK}")

    SET(GLFW_PKG_DEPS "")
    SET(GLFW_PKG_LIBS "-framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo")
ENDIF()

IF(UNIX AND NOT APPLE)
    SET(_GLFW_X11 1)
    SET(GLFW_HEADERS ${COMMON_HEADERS} 
            "${GLFW_SOURCE_DIR}/src/x11_platform.h" 
            "${GLFW_SOURCE_DIR}/src/xkb_unicode.h"
            "${GLFW_SOURCE_DIR}/src/linux_joystick.h" 
            "${GLFW_SOURCE_DIR}/src/posix_time.h" 
            "${GLFW_SOURCE_DIR}/src/posix_tls.h" 
            "${GLFW_SOURCE_DIR}/src/glx_context.h"
            "${GLFW_SOURCE_DIR}/src/egl_context.h")
    SET(GLFW_SOURCES ${COMMON_SOURCES} 
            "${GLFW_SOURCE_DIR}/src/x11_init.c"
            "${GLFW_SOURCE_DIR}/src/x11_monitor.c" 
            "${GLFW_SOURCE_DIR}/src/x11_window.c"
            "${GLFW_SOURCE_DIR}/src/xkb_unicode.c" 
            "${GLFW_SOURCE_DIR}/src/linux_joystick.c" 
            "${GLFW_SOURCE_DIR}/src/posix_time.c" 
            "${GLFW_SOURCE_DIR}/src/posix_tls.c"
            "${GLFW_SOURCE_DIR}/src/glx_context.c" 
            "${GLFW_SOURCE_DIR}/src/egl_context.c")

    FIND_LIBRARY(RT_LIBRARY rt)
    IF (RT_LIBRARY)
        LIST(APPEND GLFW_LIBRARIES "${RT_LIBRARY}")
        LIST(APPEND GLFW_PKG_LIBS "-lrt")
    ENDIF()

    FIND_LIBRARY(MATH_LIBRARY m)
    IF (MATH_LIBRARY)
        LIST(APPEND GLFW_LIBRARIES "${MATH_LIBRARY}")
        LIST(APPEND GLFW_PKG_LIBS "-lm")
    ENDIF()

    FIND_PACKAGE(X11 REQUIRED)
    LIST(APPEND GLFW_PKG_DEPS "x11")
    LIST(APPEND GLFW_INCLUDE_DIRS "${X11_X11_INCLUDE_PATH}")
    LIST(APPEND GLFW_LIBRARIES "${X11_X11_LIB}" "${CMAKE_THREAD_LIBS_INIT}")

    IF(NOT X11_Xrandr_FOUND)
        MESSAGE(FATAL_ERROR "The RandR library and headers were not found")
    ENDIF()
    LIST(APPEND GLFW_INCLUDE_DIRS "${X11_Xrandr_INCLUDE_PATH}")
    LIST(APPEND GLFW_LIBRARIES "${X11_Xrandr_LIB}")
    LIST(APPEND GLFW_PKG_DEPS "xrandr")

    IF(NOT X11_Xinerama_FOUND)
        MESSAGE(FATAL_ERROR "The Xinerama library and headers were not found")
    ENDIF()
    LIST(APPEND GLFW_INCLUDE_DIRS "${X11_Xinerama_INCLUDE_PATH}")
    LIST(APPEND GLFW_LIBRARIES "${X11_Xinerama_LIB}")
    LIST(APPEND GLFW_PKG_DEPS "xinerama")

    IF(X11_xf86vmode_FOUND)
        SET(_GLFW_HAS_XF86VM 1)
        LIST(APPEND GLFW_INCLUDE_DIRS "${X11_xf86vmode_INCLUDE_PATH}")
        LIST(APPEND GLFW_PKG_DEPS "xxf86vm")

        IF (X11_Xxf86vm_LIB)
            LIST(APPEND GLFW_LIBRARIES "${X11_Xxf86vm_LIB}")
        ELSE()
            LIST(APPEND GLFW_LIBRARIES Xxf86vm)
        ENDIF()
    ENDIF()

    IF (NOT X11_Xkb_FOUND)
        MESSAGE(FATAL_ERROR "The X keyboard extension headers were not found")
    ENDIF()
    LIST(APPEND GLFW_INCLUDE_DIR "${X11_Xkb_INCLUDE_PATH}")

    IF (NOT X11_Xcursor_FOUND)
        MESSAGE(FATAL_ERROR "The Xcursor libraries and headers were not found")
    ENDIF()
    LIST(APPEND GLFW_INCLUDE_DIR "${X11_Xcursor_INCLUDE_PATH}")
    LIST(APPEND GLFW_LIBRARIES "${X11_Xcursor_LIB}")
    LIST(APPEND GLFW_PKG_DEPS "xcursor")
ENDIF()

#--------------------------------------------------------------------
# Export GLFW library dependencies
#--------------------------------------------------------------------
FOREACH(ARG ${GLFW_PKG_DEPS})
    SET(GLFW_PKG_DEPS "${GLFW_PKG_DEPS} ${ARG}")
ENDFOREACH()

FOREACH(ARG ${GLFW_PKG_LIBS})
    SET(GLFW_PKG_LIBS "${GLFW_PKG_LIBS} ${ARG}")
ENDFOREACH()

#--------------------------------------------------------------------
# Create generated files
#--------------------------------------------------------------------
INCLUDE(CMakePackageConfigHelpers)
SET(GLFW_CONFIG_PATH "lib/cmake/glfw")

CONFIGURE_PACKAGE_CONFIG_FILE("${GLFW_SOURCE_DIR}/src/glfw3Config.cmake.in"
        "${GLFW_SOURCE_DIR}/src/glfw3Config.cmake"
        INSTALL_DESTINATION "${GLFW_CONFIG_PATH}"
        NO_CHECK_REQUIRED_COMPONENTS_MACRO)

WRITE_BASIC_PACKAGE_VERSION_FILE("${GLFW_SOURCE_DIR}/src/glfw3ConfigVersion.cmake"
        VERSION ${GLFW_VERSION_FULL}
        COMPATIBILITY SameMajorVersion)

CONFIGURE_FILE("${GLFW_SOURCE_DIR}/src/GLFW_config.h.in" "${GLFW_SOURCE_DIR}/src/GLFW_config.h" @ONLY)
CONFIGURE_FILE("${GLFW_SOURCE_DIR}/src/glfw3.pc.in" "${GLFW_SOURCE_DIR}/src/glfw3.pc" @ONLY)

#--------------------------------------------------------------------
# Add target
#--------------------------------------------------------------------
ADD_LIBRARY(glfw ${GLFW_SOURCES} ${GLFW_HEADERS})
SET_TARGET_PROPERTIES(glfw PROPERTIES
        OUTPUT_NAME ${GLFW_LIB_NAME}
        VERSION ${GLFW_VERSION}
        SOVERSION ${GLFW_VERSION_MAJOR}
        POSITION_INDEPENDENT_CODE ON
        FOLDER "ThirdParty")

TARGET_COMPILE_DEFINITIONS(glfw PRIVATE -D_GLFW_USE_CONFIG_H)
TARGET_INCLUDE_DIRECTORIES(glfw PUBLIC
        $<BUILD_INTERFACE:${GLFW_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)
TARGET_INCLUDE_DIRECTORIES(glfw PRIVATE
        "${GLFW_SOURCE_DIR}/src"
        "${GLFW_BINARY_DIR}/src"
        "${GLFW_INCLUDE_DIRS}")
TARGET_LINK_LIBRARIES(glfw INTERFACE ${GLFW_LIBRARIES})

