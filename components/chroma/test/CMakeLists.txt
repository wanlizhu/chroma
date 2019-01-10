CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(test-${TARGET})

# ===============================================
# Unit tests
# ===============================================
ADD_EXECUTABLE(test_${TARGET} ${TARGET}_test.cpp)
SET_TARGET_PROPERTIES(test_${TARGET} PROPERTIES FOLDER TestAndBenchmark)
TARGET_LINK_LIBRARIES(test_${TARGET} PRIVATE ${TARGET} gtest)
TARGET_COMPILE_OPTIONS(test_${TARGET} PRIVATE ${COMPILER_FLAGS})

ADD_EXECUTABLE(test_${TARGET}_exposure ${TARGET}_test_exposure.cpp)
SET_TARGET_PROPERTIES(test_${TARGET}_exposure PROPERTIES FOLDER TestAndBenchmark)
TARGET_LINK_LIBRARIES(test_${TARGET}_exposure PRIVATE ${TARGET} gtest)
TARGET_COMPILE_OPTIONS(test_${TARGET}_exposure PRIVATE ${COMPILER_FLAGS})

ADD_EXECUTABLE(test_depth depth_test.cpp)
SET_TARGET_PROPERTIES(test_depth PROPERTIES FOLDER TestAndBenchmark)

