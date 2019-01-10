#ifndef CHROMA_MINIGL_H
#define CHROMA_MINIGL_H

#define GL_GLEXT_PROTOTYPES 1

#include <GL/glcorearb.h>
#include <GL/glext.h>

#if defined(WIN32) || defined(_WIN32)

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#ifdef far
#undef far
#endif

#ifdef near
#undef near
#endif

#ifdef ERROR
#undef ERROR
#endif

#ifdef OPAQUE
#undef OPAQUE
#endif

#ifdef TRANSPARENT
#undef TRANSPARENT
#endif

#ifdef PURE
#undef PURE
#endif

#endif

namespace minigl {

/**
 * Looks up and binds all available OpenGL Core functions.
 * Every call to this function will increase an internal reference
 * counter that can be decreased by calling unbind().
 *
 * @return 0 on success or -1 if an error occurred.
 */
int bind();

/**
 * Unbinds all available OpenGL Core functions.
 * Every call to this function will decrease an internal reference
 * counter and unbind all OpenGL functions when the counter reaches 0.
 * As such you should assume that no OpenGL calls can be made after
 * calling this function.
 */
void unbind();

} 

#endif
