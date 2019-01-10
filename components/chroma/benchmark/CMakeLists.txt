CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(benchmark-${TARGET})

# ===============================================
# Benchmarks
# ===============================================
SET(BENCHMARK_SRCS benchmark_${TARGET}.cpp)
ADD_EXECUTABLE(benchmark_${TARGET} ${BENCHMARK_SRCS})
SET_TARGET_PROPERTIES(benchmark_${TARGET} PROPERTIES FOLDER TestAndBenchmark)
TARGET_LINK_LIBRARIES(benchmark_${TARGET} PRIVATE benchmark_main utils math ${TARGET})
