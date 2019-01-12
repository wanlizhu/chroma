CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(demo)

SET(TARGET demo)

# ===============================================
# Sources and headers
# ===============================================
FILE(GLOB_RECURSE HDRS src/*.h)
FILE(GLOB_RECURSE SRCS src/*.cpp)

# ===============================================
# Add target definition
# ===============================================
ADD_EXECUTABLE(${TARGET} ${HDRS} ${SRCS})
TARGET_LINK_LIBRARIES(${TARGET} math utils gui chroma)

# ===============================================
# Add post-build event
# ===============================================
IF(WIN32)
	ADD_CUSTOM_COMMAND(TARGET ${TARGET}
		POST_BUILD
		COMMAND del /s/q \"${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/assets/\" 
		COMMAND xcopy /y/e/q/f assets \"${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/assets/\" 
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Copy assets folder to demo destination.")
ELSE()
	ADD_CUSTOM_COMMAND(TARGET ${TARGET}
		POST_BUILD
		COMMAND rm -rf "${CMAKE_CURRENT_BINARY_DIR}/assets"
		COMMAND cp -r assets "${CMAKE_CURRENT_BINARY_DIR}"
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Copy assets folder to demo destination.")
ENDIF()