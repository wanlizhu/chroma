#ifndef __MINI_GL_OPENGL_SUPPORT_HPP__
#define __MINI_GL_OPENGL_SUPPORT_HPP__

namespace minigl {
namespace gl {

typedef void* OpenGLContext;

/**
 * Creates a new OpenGL context.
 *
 * @return an OpenGLContext object that represents the newly created
 *         OpenGL context or nullptr if the context cannot be created
 */
OpenGLContext create_opengl_context();

/**
 * Makes the specified context the current OpenGL context on the
 * current thread.
 *
 * @param context the OpenGLContext to become current
 */
void set_current_opengl_context(OpenGLContext context);

/**
 * Destroys the specified OpenGL context.
 *
 * @param context the OpenGLContext to destroy
 */
void destroy_opengl_context(OpenGLContext context);

}; // namespace gl
}; // namespace minigl

#endif 
