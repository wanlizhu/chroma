CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(math)

SET(TARGET math)
SET(PUBLIC_HDR_DIR include)

IF (MSVC)
    SET(OPTIMIZATION_FLAGS /fp:fast)
ELSE()
    SET(OPTIMIZATION_FLAGS -ffast-math -ffp-contract=fast)
ENDIF()

# ===============================================
# Sources and headers
# ===============================================
FILE(GLOB_RECURSE PUBLIC_HEADERS ${PUBLIC_HDR_DIR}/${TARGET}/*.h)
SET(SRCS dummy.cpp)

# ===============================================
# Include and target definitions
# ===============================================
INCLUDE_DIRECTORIES(${PUBLIC_HDR_DIR})
ADD_LIBRARY(${TARGET} STATIC ${PUBLIC_HEADERS} ${SRCS})

TARGET_INCLUDE_DIRECTORIES(${TARGET} PUBLIC ${PUBLIC_HDR_DIR})

# ===============================================
# Installation
# ===============================================
INSTALL(DIRECTORY ${PUBLIC_HDR_DIR}/math DESTINATION include)

# ===============================================
# Test executables
# ===============================================
FILE(GLOB_RECURSE TEST_SRCS test/*.cpp)
ADD_EXECUTABLE(test_${TARGET} ${TEST_SRCS})
SET_TARGET_PROPERTIES(test_${TARGET} PROPERTIES FOLDER TestAndBenchmark)
TARGET_LINK_LIBRARIES(test_${TARGET} PRIVATE math gtest)

# ===============================================
# Benchmarks
# ===============================================
SET(BENCHMARK_SRCS benchmark/benchmark_fast.cpp)
ADD_EXECUTABLE(benchmark_${TARGET} ${BENCHMARK_SRCS})
SET_TARGET_PROPERTIES(benchmark_${TARGET} PROPERTIES FOLDER TestAndBenchmark)
TARGET_COMPILE_OPTIONS(benchmark_${TARGET} PRIVATE ${OPTIMIZATION_FLAGS})
TARGET_LINK_LIBRARIES(benchmark_${TARGET} PRIVATE benchmark_main utils math)
