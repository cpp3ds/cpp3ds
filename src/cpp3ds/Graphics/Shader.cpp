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


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/Shader.hpp>
#include <cpp3ds/Graphics/Texture.hpp>
#include <cpp3ds/Resources.hpp>
#include <cpp3ds/OpenGL.hpp>
#include <cpp3ds/System/InputStream.hpp>
#include <cpp3ds/System/Err.hpp>
#include <3ds.h>
#include <fstream>
#include <vector>


// OpenGL ES 1 does't support GLSL shaders at all, we have to provide an empty implementation

namespace cpp3ds
{
////////////////////////////////////////////////////////////
Shader::CurrentTextureType Shader::CurrentTexture;


////////////////////////////////////////////////////////////
Shader::Shader() :
m_shaderProgram (0),
m_currentTexture(-1),
m_textures      (),
m_params        ()
{
}


////////////////////////////////////////////////////////////
Shader::~Shader()
{
    if (m_shaderProgram)
        glDeleteProgram(m_shaderProgram);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::string& filename, Type type)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& shader, Type type)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromResource(const std::string& vertexShader, Type type, bool compiled)
{
    if (compiled) {
        return loadProgram(priv::resources[vertexShader].data, priv::resources[vertexShader].size);
    } else {
        return compile(vertexShader.c_str(), nullptr);
    }
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& stream, Type type)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromStream(InputStream& vertexShaderStream, InputStream& fragmentShaderStream)
{
    return false;
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x)
{
    if (m_shaderProgram)
    {
        // Enable program
        GLint program;
        glCheck(glGetIntegerv(GL_CURRENT_PROGRAM, &program));
        glCheck(glUseProgram(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = getParamLocation(name);
        if (location != -1)
        {
            glCheck(glUniform1f(location, x));
        }

        // Disable program
        glCheck(glUseProgram(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y)
{
    if (m_shaderProgram)
    {
        // Enable program
        GLint program;
        glCheck(glGetIntegerv(GL_CURRENT_PROGRAM, &program));
        glCheck(glUseProgram(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = getParamLocation(name);
        if (location != -1)
        {
            glCheck(glUniform2f(location, x, y));
        }

        // Disable program
        glCheck(glUseProgram(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y, float z)
{
    if (m_shaderProgram)
    {
        // Enable program
        GLint program;
        glCheck(glGetIntegerv(GL_CURRENT_PROGRAM, &program));
        glCheck(glUseProgram(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = getParamLocation(name);
        if (location != -1)
        {
            glCheck(glUniform3f(location, x, y, z));
        }

        // Disable program
        glCheck(glUseProgram(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y, float z, float w)
{
    if (m_shaderProgram)
    {
        // Enable program
        GLint program;
        glCheck(glGetIntegerv(GL_CURRENT_PROGRAM, &program));
        glCheck(glUseProgram(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = getParamLocation(name);
        if (location != -1)
        {
            glCheck(glUniform4f(location, x, y, z, w));
        }

        // Disable program
        glCheck(glUseProgram(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector2f& v)
{
    setParameter(name, v.x, v.y);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector3f& v)
{
    setParameter(name, v.x, v.y, v.z);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Color& color)
{
    setParameter(name, color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const cpp3ds::Transform& transform)
{
    if (m_shaderProgram)
    {
        // Enable program
        GLint program;
        glCheck(glGetIntegerv(GL_CURRENT_PROGRAM, &program));
        glCheck(glUseProgram(m_shaderProgram));

        // Get parameter location and assign it new values
        GLint location = getParamLocation(name);
        if (location != -1)
        {
            glCheck(glUniformMatrix4fv(location, 1, GL_FALSE, transform.getMatrix()));
        }

        // Disable program
        glCheck(glUseProgram(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Texture& texture)
{
    if (m_shaderProgram)
    {
        // Find the location of the variable in the shader
        int location = getParamLocation(name);
        if (location != -1)
        {
            // Store the location -> texture mapping
            TextureTable::iterator it = m_textures.find(location);
            if (it == m_textures.end())
            {
                // New entry, make sure there are enough texture units
                GLint maxUnits = 0;
                glCheck(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits));

                if (m_textures.size() + 1 >= static_cast<std::size_t>(maxUnits))
                {
                    err() << "Impossible to use texture \"" << name << "\" for shader: all available texture units are used" << std::endl;
                    return;
                }

                m_textures[location] = &texture;
            }
            else
            {
                // Location already used, just replace the texture
                it->second = &texture;
            }
        }
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, CurrentTextureType)
{
    if (m_shaderProgram)
    {
        // Find the location of the variable in the shader
        m_currentTexture = getParamLocation(name);
    }
}



////////////////////////////////////////////////////////////
unsigned int Shader::getNativeHandle() const
{
    return m_shaderProgram;
}


////////////////////////////////////////////////////////////
void Shader::bind(const Shader* shader)
{
    // Make sure that we can use shaders
    if (!isAvailable())
    {
        err() << "Failed to bind or unbind shader: your system doesn't support shaders "
        << "(you should test Shader::isAvailable() before trying to use the Shader class)" << std::endl;
        return;
    }

    if (shader && shader->m_shaderProgram)
    {
        // Enable the program
//        glCheck(GLEXT_glUseProgramObject(castToGlHandle(shader->m_shaderProgram)));
        glCheck(glUseProgram(shader->m_shaderProgram));

        // Bind the textures
        shader->bindTextures();

        // Bind the current texture
//        if (shader->m_currentTexture != -1)
//            glCheck(GLEXT_glUniform1i(shader->m_currentTexture, 0));
    }
    else
    {
        // Bind no shader
        glCheck(glUseProgram(0));
    }
}


////////////////////////////////////////////////////////////
bool Shader::isAvailable()
{
    return true;
}


////////////////////////////////////////////////////////////
bool Shader::compile(const char* vertexShaderCode, const char* fragmentShaderCode)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadProgram(const Uint8* data, const Uint32 size)
{
    if (m_shaderProgram) {
        glCheck(glDeleteProgram(m_shaderProgram));
        m_shaderProgram = 0;
    }

    // Reset the internal state
    m_currentTexture = -1;
    m_textures.clear();
    m_params.clear();

    m_shaderProgram = glCreateProgram();
    glProgramBinary(m_shaderProgram, GL_VERTEX_SHADER_BINARY, data, (GLsizei)size);
//    shaderProgramInit(&shader);
//
//    dvlb = DVLB_ParseFile((u32*)data, size);
//    shaderProgramSetVsh(&shader, &dvlb->DVLE[0]);
    return true;
}


////////////////////////////////////////////////////////////
void Shader::bindTextures() const
{
}

////////////////////////////////////////////////////////////
int Shader::getParamLocation(const std::string& name)
{
    // Check the cache
    ParamTable::const_iterator it = m_params.find(name);
    if (it != m_params.end()) {
        // Already in cache, return it
        return it->second;
    }
    else {
        // Not in cache, request the location from OpenGL
        int location = glGetUniformLocation(m_shaderProgram, name.c_str());
        m_params.insert(std::make_pair(name, location));

        if (location == -1)
            err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

        return location;
    }
}

} // namespace cpp3ds

