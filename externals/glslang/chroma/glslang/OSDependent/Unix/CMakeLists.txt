ADD_LIBRARY(OSDependent STATIC 
    ${GLSLANG_DIR}/glslang/OSDependent/Unix/ossource.cpp 
    ${GLSLANG_DIR}/glslang/OSDependent/osinclude.h)
SET_PROPERTY(TARGET OSDependent PROPERTY FOLDER Externals/glslang)
SET_PROPERTY(TARGET OSDependent PROPERTY POSITION_INDEPENDENT_CODE ON)

