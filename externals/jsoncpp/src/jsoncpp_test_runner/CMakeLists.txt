FIND_PACKAGE(PythonInterp 2.6)

ADD_EXECUTABLE(jsoncpp_test_runner
               main.cpp
               )
SET_TARGET_PROPERTIES(jsoncpp_test_runner PROPERTIES FOLDER "Test")

IF(BUILD_SHARED_LIBS)
    ADD_DEFINITIONS( -DJSON_DLL )
    TARGET_LINK_LIBRARIES(jsoncpp_test_runner jsoncpp)
ELSE(BUILD_SHARED_LIBS)
    TARGET_LINK_LIBRARIES(jsoncpp_test_runner jsoncpp)
ENDIF()

SET_TARGET_PROPERTIES(jsoncpp_test_runner PROPERTIES OUTPUT_NAME jsoncpp_test_runner)

IF(PYTHONINTERP_FOUND)
    # Run end to end parser/writer tests
    SET(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../test)
    SET(RUNJSONTESTS_PATH ${TEST_DIR}/runjsontests.py)
    ADD_CUSTOM_TARGET(jsoncpp_readerwriter_tests
                      "${PYTHON_EXECUTABLE}" -B "${RUNJSONTESTS_PATH}" $<TARGET_FILE:jsoncpp_test_runner> "${TEST_DIR}/data"
                      DEPENDS jsoncpp_test_runner jsoncpp_test
                      )
    ADD_CUSTOM_TARGET(jsoncpp_check DEPENDS jsoncpp_readerwriter_tests)
    
    SET_TARGET_PROPERTIES(jsoncpp_readerwriter_tests PROPERTIES FOLDER "Test")
    SET_TARGET_PROPERTIES(jsoncpp_check PROPERTIES FOLDER "Test")
ENDIF()
