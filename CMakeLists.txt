CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(chroma)

OPTION(BUILD_DEMO "Build demo." ON)

IF (WIN32)
    # Link statically against c/c++ lib to avoid missing redistriburable such as "VCRUNTIME140.dll not found."
    SET(USE_STATIC_CRT ON)

    # CL.EXE will not generate .lib without .def file (or without pragma __declspec(dllexport) in front of each functions).
    SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

    # IN RELEASE_WITH_DEBUG_INFO, generate debug info in .obj, no in pdb.
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CRT_FLAGS_RELEASE} /Z7")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${CRT_FLAGS_RELEASE} /Z7")

    # In RELEASE, also generate PDBs.
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CRT_FLAGS_RELEASE} /Zi")
    SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${CRT_FLAGS_RELEASE} /Zi")

    # In DEBUG, avoid generating a PDB file which seems to mess with incremental compilation.
    # Instead generate debug info directly inside obj files.
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CRT_FLAGS_DEBUG} /Z7")
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${CRT_FLAGS_DEBUG} /Z7")

    IF (${USE_STATIC_CRT})
        SET(CRT_FLAGS_RELEASE "/MT")
        SET(CRT_FLAGS_DEBUG "/MTd")
    ELSE()
        SET(CRT_FLAGS_RELEASE "/MD")
        SET(CRT_FLAGS_DEBUG "/MDd")
    ENDIF()
ENDIF()

# ===============================================
#  Paths
# ===============================================
SET(EXTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
SET(COMPONENTS ${CMAKE_CURRENT_SOURCE_DIR}/global)
SET(TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/tools)
SET(ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets)

# ===============================================
# General compiler flags
# ===============================================
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_STANDARD}")

IF(WIN32)
    MESSAGE(STATUS "Configurate for Windows")
    ADD_DEFINITIONS(/W0)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWINDOWS")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES=1 /std:c++14")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE")
ELSEIF(UNIX AND NOT APPLE)
    MESSAGE(STATUS "Configurate for Linux")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLINUX")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++14")
    # To distribute our binaries, we must remove the dependency on libc++ and libgcc.
    IF (CMAKE_BUILD_TYPE STREQUAL "Release")
        LINK_LIBRARIES("-static-libgcc -static-libstdc++")
    ENDIF()
    
    FIND_PACKAGE(X11)
    IF(NOT X11_FOUND)
        SET(BUILD_DEMO OFF)
    ENDIF()
ELSEIF(APPLE)
    MESSAGE(STATUS "Configurate for Apple")
    SET(GC_SECTIONS "-Wl,-dead_strip")
    SET(B_SYMBOLIC_FUNCTIONS "")

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAPPLE")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    # tell ranlib to ignore empty compilation units
    SET(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols <TARGET>")
    SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols <TARGET>")
    # prevents ar from invoking ranlib, let CMake do it
    SET(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> qc -S <TARGET> <LINK_FLAGS> <OBJECTS>")
    SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> qc -S <TARGET> <LINK_FLAGS> <OBJECTS>")
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported platform : ${CMAKE_SYSTEM_NAME}")
ENDIF()

# ===============================================
# Project flags
# ===============================================
IF (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    SET(CHROMA_DEV true)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCHROMA_DEV")
ENDIF()

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# ===============================================
# Distribution
# ===============================================
# choose where to put the objects in the dist folder
IF (NOT DIST_ARCH)
    SET(DIST_ARCH x86_64)
ENDIF()

IF (NOT DIST_DIR)
    SET(DIST_DIR "${DIST_ARCH}")
ENDIF()

# ===============================================
# Functions
# ===============================================
FUNCTION(LIST_LICENSES OUTPUT MODULES)
    FILE(WRITE ${OUTPUT} "R\"CHROMA__(\n")
    SET(_MODULES ${MODULES} ${ARGN})
    FOREACH(module ${_MODULES})
        SET(license_path "${EXTERNAL}/${module}/LICENSE")
        GET_FILENAME_COMPONENT(fullname "${license_path}" ABSOLUTE)
        FILE(APPEND ${OUTPUT} "License and copyrights for ${module}:\n\n")
        FILE(READ ${license_path} license)
        FILE(APPEND ${OUTPUT} ${license})
        FILE(APPEND ${OUTPUT} "\n\n")
    ENDFOREACH()
    FILE(APPEND ${OUTPUT} ")CHROMA__\"\n")
ENDFUNCTION(LIST_LICENSES)

# ===============================================
# Sub-projects
# ===============================================
# Common to all platforms
ADD_SUBDIRECTORY(${EXTERNAL}/libz/chroma)
ADD_SUBDIRECTORY(${EXTERNAL}/libgtest/chroma)
ADD_SUBDIRECTORY(${EXTERNAL}/benchmark/chroma)
ADD_SUBDIRECTORY(${EXTERNAL}/glad/chroma)
ADD_SUBDIRECTORY(${EXTERNAL}/getopt)
ADD_SUBDIRECTORY(${TOOLS}/resc)
ADD_SUBDIRECTORY(${COMPONENTS}/math)
ADD_SUBDIRECTORY(${COMPONENTS}/utils)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/src)
IF(BUILD_DEMO)
    ADD_SUBDIRECTORY(${EXTERNAL}/glfw/chroma)
    ADD_SUBDIRECTORY(${COMPONENTS}/display)
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/demo/viewer)
ENDIF()
