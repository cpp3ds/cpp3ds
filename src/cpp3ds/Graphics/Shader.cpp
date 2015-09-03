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
#include <cpp3ds/Graphics/VertexBuffer.hpp>
#include <cpp3ds/Resources.hpp>
#include <cpp3ds/OpenGL.hpp>
#include <cpp3ds/System/InputStream.hpp>
#include <cpp3ds/System/Mutex.hpp>
#include <cpp3ds/System/Lock.hpp>
#include <cpp3ds/System/Err.hpp>
#include <3ds.h>
#include <fstream>
#include <vector>


namespace
{
    // Thread-safe unique identifier generator,
    // is used for id
    cpp3ds::Uint64 getUniqueId()
    {
        static cpp3ds::Uint64 id = 1; // start at 1, zero is "no program"
        static cpp3ds::Mutex mutex;

        cpp3ds::Lock lock(mutex);
        return id++;
    }

    // Retrieve the maximum number of texture units available
    GLint getMaxTextureUnits()
    {
        GLint maxUnits;
        glCheck(glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &maxUnits));
        return maxUnits;
    }

    // Read the contents of a file into an array of char
    bool getFileContents(const std::string& filename, std::vector<char>& buffer)
    {
        std::ifstream file(filename.c_str(), std::ios_base::binary);
        if (file)
        {
            file.seekg(0, std::ios_base::end);
            std::streamsize size = file.tellg();
            if (size > 0)
            {
                file.seekg(0, std::ios_base::beg);
                buffer.resize(static_cast<std::size_t>(size));
                file.read(&buffer[0], size);
            }
            buffer.push_back('\0');
            return true;
        }
        else
        {
            return false;
        }
    }

    // Read the contents of a stream into an array of char
    bool getStreamContents(cpp3ds::InputStream& stream, std::vector<char>& buffer)
    {
        bool success = true;
        cpp3ds::Int64 size = stream.getSize();
        if (size > 0)
        {
            buffer.resize(static_cast<std::size_t>(size));
            stream.seek(0);
            cpp3ds::Int64 read = stream.read(&buffer[0], size);
            success = (read == size);
        }
        buffer.push_back('\0');
        return success;
    }
}


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Shader::CurrentTextureType Shader::CurrentTexture;


////////////////////////////////////////////////////////////
Shader::Shader() :
m_shaderProgram (0),
m_currentTexture(-1),
m_textures      (),
m_params        (),
m_attributes    (),
m_blockBindings (),
m_warnMissing   (true),
m_id            (0),
m_parameterBlock(false),
m_blockProgram  (0)
{
}


////////////////////////////////////////////////////////////
Shader::~Shader()
{
    // Destroy effect program
    if (m_shaderProgram)
        glDeleteProgram(m_shaderProgram);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::string& filename, Type type)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename, const std::string& geometryShaderFilename)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& shader, Type type)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
{
	return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadFromResource(const std::string& shader, Type type, bool compiled)
{
    if (compiled) {
        return loadBinary(priv::resources[shader].data, priv::resources[shader].size, type);
    } else {
        return compile(shader.c_str(), nullptr, nullptr);
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
bool Shader::loadFromStream(InputStream& vertexShaderStream, InputStream& fragmentShaderStream, InputStream& geometryShaderStream)
{
    return false;
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, int x) const
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
            glCheck(glUniform1i(location, x));
        }

        // Disable program
        glCheck(glUseProgram(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, int x, int y) const
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
            glCheck(glUniform2i(location, x, y));
        }

        // Disable program
        glCheck(glUseProgram(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, int x, int y, int z) const
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
            glCheck(glUniform3i(location, x, y, z));
        }

        // Disable program
        glCheck(glUseProgram(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, int x, int y, int z, int w) const
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
            glCheck(glUniform4i(location, x, y, z, w));
        }

        // Disable program
        glCheck(glUseProgram(program));
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector2i& v) const
{
    setParameter(name, v.x, v.y);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector3i& v) const
{
    setParameter(name, v.x, v.y, v.z);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x) const
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
void Shader::setParameter(const std::string& name, float x, float y) const
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
void Shader::setParameter(const std::string& name, float x, float y, float z) const
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
void Shader::setParameter(const std::string& name, float x, float y, float z, float w) const
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
void Shader::setParameter(const std::string& name, const Vector2f& v) const
{
    setParameter(name, v.x, v.y);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector3f& v) const
{
    setParameter(name, v.x, v.y, v.z);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Color& color) const
{
    setParameter(name, color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const cpp3ds::Transform& transform) const
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
void Shader::setParameter(const std::string& name, const Texture& texture) const
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
void Shader::setParameter(const std::string& name, CurrentTextureType) const
{
    if (m_shaderProgram)
    {
        // Find the location of the variable in the shader
        m_currentTexture = getParamLocation(name);
    }
}


////////////////////////////////////////////////////////////
int Shader::getVertexAttributeLocation(const std::string& name) const
{
	// Check the cache
	LocationTable::const_iterator it = m_attributes.find(name);
	if (it != m_attributes.end())
	{
		// Already in cache, return it
		return it->second;
	}
	else
	{
		// Not in cache, request the location from OpenGL
		int location = -1;
//		int location = glGetAttribLocationARB(m_shaderProgram, name.c_str());
		if (location == -1)
		{
			// Error: location not found
			if (m_warnMissing)
				err() << "Vertex attribute \"" << name << "\" not found in shader" << std::endl;
		}

		m_attributes.insert(std::make_pair(name, location));

		return location;
	}
}


////////////////////////////////////////////////////////////
bool Shader::warnMissing(bool warn) const
{
	bool previousWarn = m_warnMissing;
	m_warnMissing = warn;
	return previousWarn;
}


//////////////////////////////////////////////////////////////
//void Shader::beginParameterBlock() const
//{
//	m_parameterBlock = true;
//
//	m_blockProgram = static_cast<unsigned int>(glGetHandleARB(GL_PROGRAM_OBJECT_ARB));
//
//	if (m_blockProgram != m_shaderProgram)
//	glCheck(glUseProgramObjectARB(m_shaderProgram));
//}
//
//
//////////////////////////////////////////////////////////////
//void Shader::endParameterBlock() const
//{
//	m_parameterBlock = false;
//
//	if (m_blockProgram != m_shaderProgram)
//	glCheck(glUseProgramObjectARB(m_blockProgram));
//}


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
        if (shader->m_currentTexture != -1)
            glCheck(glUniform1i(shader->m_currentTexture, 0));
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
bool Shader::compile(const char* vertexShaderCode, const char* fragmentShaderCode, const char* geometryShaderCode)
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadBinary(const Uint8* data, const Uint32 size, Type type)
{
	if (!m_shaderProgram)
		m_shaderProgram = glCreateProgram();

    // Reset the internal state
    m_currentTexture = -1;
    m_textures.clear();
    m_params.clear();

	if (type == Vertex)
    	glProgramBinary(m_shaderProgram, GL_VERTEX_SHADER_BINARY, data, (GLsizei)size);
	else if (type == Geometry)
		glProgramBinary(m_shaderProgram, GL_GEOMETRY_SHADER_BINARY, data, (GLsizei)size);

    return true;
}


////////////////////////////////////////////////////////////
bool Shader::isGeometryShaderAvailable()
{
	return isAvailable();
//	return isAvailable() && GLEW_VERSION_3_2;
}


////////////////////////////////////////////////////////////
std::string Shader::getSupportedVersion()
{
	return "";
}


////////////////////////////////////////////////////////////
unsigned int Shader::getMaximumUniformComponents()
{
	if (!isAvailable())
		return 0;

	GLint maxVertexUniformComponents = 10;
//	GLint maxVertexUniformComponents = 0;
//	glCheck(glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB, &maxVertexUniformComponents));
	return maxVertexUniformComponents;
}


////////////////////////////////////////////////////////////
void Shader::bindTextures() const
{
	TextureTable::const_iterator it = m_textures.begin();
	for (std::size_t i = 0; i < m_textures.size(); ++i)
	{
		GLint index = static_cast<GLsizei>(i + 1);
		glCheck(glUniform1i(it->first, index));
		glCheck(GLEXT_glActiveTexture(GLEXT_GL_TEXTURE0 + index));
		Texture::bind(it->second);
		++it;
	}

	// Make sure that the texture unit which is left active is the number 0
	glCheck(GLEXT_glActiveTexture(GLEXT_GL_TEXTURE0));
}

////////////////////////////////////////////////////////////
int Shader::getParamLocation(const std::string& name) const
{
    // Check the cache
    LocationTable::const_iterator it = m_params.find(name);
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

