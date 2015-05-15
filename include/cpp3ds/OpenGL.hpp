////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef CPP3DS_OPENGL_HPP
#define CPP3DS_OPENGL_HPP

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Config.hpp>
#include <cpp3ds/Graphics/GLCheck.hpp>
//#include <cpp3ds/OpenGL/gl.h>

#ifdef EMULATION

	#ifdef _MSC_VER
        #include <windows.h>
    #endif
    #include <GL/gl.h>
    #include <GL/glu.h>

//	#define glEnable ::glEnable

#else

//	#include <GLES/gl.h>
//	#include <GLES/glext.h>
    #include <cpp3ds/OpenGL/gl.h>

//	#define glEnable ::cpp3ds::glEnable

#endif
/*
//    void glActiveTexture (GLenum texture);
//    void glAlphaFunc (GLenum func, GLclampf ref);
//    void glBindBuffer (GLenum target, GLuint buffer);
//    void glBindTexture (GLenum target, GLuint texture);
//    void glBlendFunc (GLenum sfactor, GLenum dfactor);
    void glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
    void glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
    void glClear (GLbitfield mask);
    void glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
//    void glClearDepth (GLclampf depth);
//    void glClearStencil (GLint s);
//    void glClientActiveTexture (GLenum texture);
//    void glClipPlane (GLenum plane, const GLfloat *equation);
//    void glColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
//    void glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
//    void glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
//    void glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
//    void glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
//    void glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
//    void glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
//    void glCullFace (GLenum mode);
//    void glDeleteBuffers (GLsizei n, const GLuint *buffers);
//    void glDeleteTextures (GLsizei n, const GLuint *textures);
//    void glDepthFunc (GLenum func);
//    void glDepthMask (GLboolean flag);
//    void glDepthRange (GLclampf zNear, GLclampf zFar);
//    void glDisable (GLenum cap);
//    void glDisableClientState (GLenum array);
//    void glDrawArrays (GLenum mode, GLint first, GLsizei count);
//    void glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
//    void glEnable (GLenum cap);
//    void glEnableClientState (GLenum array);
//    void glFinish (void);
//    void glFlush (void);
//    void glFog (GLenum pname, GLfloat param);
//    void glFogv (GLenum pname, const GLfloat *params);
//    void glFrontFace (GLenum mode);
//    void glFrustum (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
//    void glGenBuffers (GLsizei n, GLuint *buffers);
//    void glGenTextures (GLsizei n, GLuint *textures);
//
//    void glGetBooleanv (GLenum pname, GLboolean *params);
//    void glGetFixedv (GLenum pname, GLfixed *params);
//    void glGetFloatv (GLenum pname, GLfloat *params);
//    void glGetIntegerv (GLenum pname, GLint *params);
//
//    void glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params);
//    void glGetClipPlane (GLenum pname, GLfloat eqn[4]);
//    GLenum glGetError (void);
//    void glGetLight (GLenum light, GLenum pname, GLfloat *params);
//    void glGetMaterial (GLenum face, GLenum pname, GLfloat *params);
//    void glGetPointerv (GLenum pname, GLvoid **params);
//    const GLubyte * glGetString (GLenum name);
//    void glGetTexEnv (GLenum env, GLenum pname, GLint *params);
//    void glGetTexParameter (GLenum target, GLenum pname, GLfloat *params);
//    void glHint (GLenum target, GLenum mode);
//    GLboolean glIsBuffer (GLuint buffer);
//    GLboolean glIsEnabled (GLenum cap);
//    GLboolean glIsTexture (GLuint texture);
//    void glLightModel (GLenum pname, GLfloat param);
//    void glLightModelv (GLenum pname, const GLfloat *params);
//    void glLight (GLenum light, GLenum pname, GLfloat param);
//    void glLightv (GLenum light, GLenum pname, const GLfloat *params);
//    void glLineWidth (GLfloat width);
//    void glLoadIdentity (void);
//    void glLoadMatrix (const GLfloat *m);
//    void glLogicOp (GLenum opcode);
//    void glMaterial (GLenum face, GLenum pname, GLfloat param);
//    void glMaterialv (GLenum face, GLenum pname, const GLfloat *params);
//    void glMatrixMode (GLenum mode);
//    void glMultMatrix (const GLfloat *m);
//    void glMultiTexCoord (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
//    void glNormal (GLfloat nx, GLfloat ny, GLfloat nz);
//    void glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
//    void glOrtho (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
//    void glPixelStorei (GLenum pname, GLint param);
//    void glPointParameter (GLenum pname, GLfloat param);
//    void glPointParameterv (GLenum pname, const GLfloat *params);
//    void glPointSize (GLfloat size);
//    void glPolygonOffset (GLfloat factor, GLfloat units);
//    void glPopMatrix (void);
//    void glPushMatrix (void);
//    void glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
//    void glRotate (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
//    void glSampleCoverage (GLclampf value, GLboolean invert);
//    void glScale (GLfloat x, GLfloat y, GLfloat z);
//    void glScissor (GLint x, GLint y, GLsizei width, GLsizei height);
//    void glShadeModel (GLenum mode);
//    void glStencilFunc (GLenum func, GLint ref, GLuint mask);
//    void glStencilMask (GLuint mask);
//    void glStencilOp (GLenum fail, GLenum zfail, GLenum zpass);
//    void glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
//    void glTexEnvi (GLenum target, GLenum pname, GLint param);
//    void glTexEnviv (GLenum target, GLenum pname, const GLint *params);
//    void glTexEnvf (GLenum target, GLenum pname, GLfloat param);
//    void glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params);
//    void glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
//    void glTexParameter (GLenum target, GLenum pname, GLfloat param);
//    void glTexParameterv (GLenum target, GLenum pname, const GLfloat *params);
//    void glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
//    void glTranslate (GLfloat x, GLfloat y, GLfloat z);
//    void glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
//    void glViewport (GLint x, GLint y, GLsizei width, GLsizei height);
*/

#endif
