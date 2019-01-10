#include <gtest/gtest.h>
#include <minigl/minigl.h>
#include "opengl_support.hpp"

namespace minigl {

TEST(MiniGLTest, Initialization) {
    EXPECT_EQ(0, bind());
    unbind();
}

TEST(MiniGLTest, GetVersion) {
    EXPECT_EQ(0, bind());
    gl::OpenGLContext context = gl::createOpenGLContext();
    gl::setCurrentOpenGLContext(context);

    GLint major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    printf("OpenGL v%d.%d\n", major, minor);
    gl::destroyOpenGLContext(context);
    unbind();
}

TEST(MiniGLTest, DoubleInitialization) {
    int result = bind();
    EXPECT_EQ(result, bind());
    unbind();
    unbind();
}

TEST(MiniGLTest, CallOpenGL) {
    EXPECT_EQ(0, bind());

    gl::OpenGLContext context = gl::createOpenGLContext();
    EXPECT_TRUE(context != nullptr);

    gl::setCurrentOpenGLContext(context);
    EXPECT_TRUE(glGetString(GL_VENDOR) != nullptr);

    gl::destroyOpenGLContext(context);
    unbind();
}

TEST(MiniGLTest, CallOpenGLDoubleInit) {
    int result = bind();
    EXPECT_EQ(result, bind());
    unbind();

    gl::OpenGLContext context = gl::createOpenGLContext();
    EXPECT_TRUE(context != nullptr);

    gl::setCurrentOpenGLContext(context);
    EXPECT_TRUE(glGetString(GL_VENDOR) != nullptr);

    gl::destroyOpenGLContext(context);
    unbind();
}

TEST(MiniGLTest, CallBindAndGetBound) {
    EXPECT_EQ(0, bind());
    gl::OpenGLContext context = gl::createOpenGLContext();
    gl::setCurrentOpenGLContext(context);

    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    GLint boundTexId = -1; // different from texId, just to be sure
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexId);
    ASSERT_EQ(texId, boundTexId);

    gl::destroyOpenGLContext(context);
    unbind();
}

}; // namespace minigl

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
