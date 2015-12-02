////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef CPP3DS_GLEXTENSIONS_HPP
#define CPP3DS_GLEXTENSIONS_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Config.hpp>
#include <cpp3ds/OpenGL.hpp>

#ifdef EMULATION

    #define GLEXT_blend_func_separate              GLEW_EXT_blend_func_separate
    #define GLEXT_blend_equation_separate          GLEW_EXT_blend_equation_separate
    #define GLEXT_glBlendFuncSeparate              glBlendFuncSeparateEXT
    #define GLEXT_glBlendEquationSeparate          glBlendEquationSeparateEXT
    #define GLEXT_framebuffer_object               GLEW_EXT_framebuffer_object
    #define GLEXT_glGenFramebuffers                glGenFramebuffersEXT
    #define GLEXT_glGenRenderbuffers               glGenRenderbuffersEXT
    #define GLEXT_glBindFramebuffer                glBindFramebufferEXT
    #define GLEXT_glBindRenderbuffer               glBindRenderbufferEXT
    #define GLEXT_glDeleteFramebuffers             glDeleteFramebuffersEXT
    #define GLEXT_glDeleteRenderbuffers            glDeleteRenderbuffersEXT
    #define GLEXT_glRenderbufferStorage            glRenderbufferStorageEXT
    #define GLEXT_glFramebufferRenderbuffer        glFramebufferRenderbufferEXT
    #define GLEXT_glFramebufferTexture2D           glFramebufferTexture2DEXT
    #define GLEXT_glCheckFramebufferStatus         glCheckFramebufferStatusEXT
    #define GLEXT_GL_FRAMEBUFFER                   GL_FRAMEBUFFER_EXT
    #define GLEXT_GL_FRAMEBUFFER_BINDING           GL_FRAMEBUFFER_BINDING_EXT
    #define GLEXT_GL_RENDERBUFFER                  GL_RENDERBUFFER_EXT
    #define GLEXT_GL_COLOR_ATTACHMENT0             GL_COLOR_ATTACHMENT0_EXT
    #define GLEXT_GL_DEPTH_ATTACHMENT              GL_DEPTH_ATTACHMENT_EXT
    #define GLEXT_GL_FRAMEBUFFER_COMPLETE          GL_FRAMEBUFFER_COMPLETE_EXT
    #define GLEXT_GL_DEPTH_COMPONENT               GL_DEPTH_COMPONENT
    #define GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION GL_INVALID_FRAMEBUFFER_OPERATION_EXT
    #define GLEXT_texture_non_power_of_two         GLEW_ARB_texture_non_power_of_two
    #define GLEXT_multitexture                     GLEW_ARB_multitexture
    #define GLEXT_glClientActiveTexture            glClientActiveTextureARB
    #define GLEXT_glActiveTexture                  glActiveTextureARB
    #define GLEXT_GL_TEXTURE0                      GL_TEXTURE0_ARB
    #define GLEXT_glBlendEquation                  glBlendEquation
    #define GLEXT_GL_FUNC_ADD                      GL_FUNC_ADD
    #define GLEXT_GL_FUNC_SUBTRACT                 GL_FUNC_SUBTRACT

#else

	#define GL_GLEXT_PROTOTYPES

    #define GLEXT_blend_func_separate              GL_OES_blend_func_separate
    #define GLEXT_blend_equation_separate          GL_OES_blend_equation_separate
    #define GLEXT_glBlendFuncSeparate              glBlendFuncSeparate
    #define GLEXT_glBlendEquationSeparate          glBlendEquationSeparate
    #define GLEXT_framebuffer_object               GL_OES_framebuffer_object
    #define GLEXT_glGenFramebuffers                glGenFramebuffers
    #define GLEXT_glGenRenderbuffers               glGenRenderbuffers
    #define GLEXT_glBindFramebuffer                glBindFramebuffer
    #define GLEXT_glBindRenderbuffer               glBindRenderbuffer
    #define GLEXT_glDeleteFramebuffers             glDeleteFramebuffers
    #define GLEXT_glDeleteRenderbuffers            glDeleteRenderbuffers
    #define GLEXT_glRenderbufferStorage            glRenderbufferStorage
    #define GLEXT_glFramebufferRenderbuffer        glFramebufferRenderbuffer
    #define GLEXT_glFramebufferTexture2D           glFramebufferTexture2D
    #define GLEXT_glCheckFramebufferStatus         glCheckFramebufferStatus
    #define GLEXT_GL_FRAMEBUFFER                   GL_FRAMEBUFFER
    #define GLEXT_GL_FRAMEBUFFER_BINDING           GL_FRAMEBUFFER_BINDING_EXT
    #define GLEXT_GL_RENDERBUFFER                  GL_RENDERBUFFER
    #define GLEXT_GL_COLOR_ATTACHMENT0             GL_COLOR_ATTACHMENT0_EXT
    #define GLEXT_GL_DEPTH_ATTACHMENT              GL_DEPTH_ATTACHMENT_EXT
    #define GLEXT_GL_FRAMEBUFFER_COMPLETE          GL_FRAMEBUFFER_COMPLETE_EXT
    #define GLEXT_GL_DEPTH_COMPONENT               GL_DEPTH_COMPONENT16_EXT
    #define GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION GL_INVALID_FRAMEBUFFER_OPERATION
    #define GLEXT_texture_non_power_of_two         false
    #define GLEXT_multitexture                     true
    #define GLEXT_glClientActiveTexture            glClientActiveTexture
    #define GLEXT_glActiveTexture                  glActiveTexture
    #define GLEXT_GL_TEXTURE0                      GL_TEXTURE0
    #define GLEXT_glBlendEquation                  glBlendEquation
    #define GLEXT_GL_FUNC_ADD                      GL_FUNC_ADD
    #define GLEXT_GL_FUNC_SUBTRACT                 GL_FUNC_SUBTRACT

#endif


namespace cpp3ds
{
namespace priv
{

////////////////////////////////////////////////////////////
/// \brief Make sure that extensions are initialized
///
////////////////////////////////////////////////////////////
void ensureExtensionsInit();

} // namespace priv

} // namespace cpp3ds


#endif
