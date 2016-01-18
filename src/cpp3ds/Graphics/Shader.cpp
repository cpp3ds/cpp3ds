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
#include <cpp3ds/OpenGL/GLExtensions.hpp>
#include <cpp3ds/System/InputStream.hpp>
#include <cpp3ds/System/Err.hpp>
#include <fstream>
#include <vector>
#include <3ds/gpu/shbin.h>
#include "CitroHelpers.hpp"


namespace
{
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
}


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Shader::CurrentTextureType Shader::CurrentTexture;
Shader Shader::Default;


////////////////////////////////////////////////////////////
Shader::Shader() :
m_shaderProgram (NULL),
m_currentTexture(-1),
m_textures      (),
m_params        ()
{
}


////////////////////////////////////////////////////////////
Shader::~Shader()
{
    if (m_shaderProgram)
        shaderProgramFree(m_shaderProgram);
    if (m_dvlb)
        DVLB_Free(m_dvlb);
}


////////////////////////////////////////////////////////////
bool Shader::loadFromFile(const std::string& filename, Type type)
{
	std::string shaderfile;
	shaderfile = filename + ".shbin";

	// Read the file
	if (!getFileContents(shaderfile, m_shaderData))
	{
		err() << "Failed to open shader file \"" << shaderfile << "\"" << std::endl;
		return false;
	}

	return loadBinary(reinterpret_cast<Uint8*>(&m_shaderData[0]), m_shaderData.size(), type);
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
        // Get parameter location and assign it new values
        int location = getParamLocation(name);
        if (location != -1)
        {
            C3D_FVUnifSet(GPU_VERTEX_SHADER, location, x, 0, 0, 0);
        }
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y)
{
    if (m_shaderProgram)
    {
        // Get parameter location and assign it new values
        int location = getParamLocation(name);
        if (location != -1)
        {
            C3D_FVUnifSet(GPU_VERTEX_SHADER, location, x, y, 0, 0);
        }
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y, float z)
{
    if (m_shaderProgram)
    {
        // Get parameter location and assign it new values
        int location = getParamLocation(name);
        if (location != -1)
        {
            C3D_FVUnifSet(GPU_VERTEX_SHADER, location, x, y, z, 0);
        }
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y, float z, float w)
{
    if (m_shaderProgram)
    {
        // Get parameter location and assign it new values
        int location = getParamLocation(name);
        if (location != -1)
        {
            C3D_FVUnifSet(GPU_VERTEX_SHADER, location, x, y, z, w);
        }
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
        // Get parameter location and assign it new values
        int location = getParamLocation(name);
        if (location != -1)
        {
            C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, location, reinterpret_cast<const C3D_Mtx*>(transform.getMatrix()));
        }
    }
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Texture& texture)
{
    if (m_shaderProgram)
    {
        err() << "Setting texture as uniform isn't supported." << std::endl;
        return;
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
shaderProgram_s* Shader::getNativeHandle() const
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
        C3D_BindProgram(shader->m_shaderProgram);
        CitroBindUniforms(shader->m_shaderProgram);
    }
    else
    {
        // Bind default shader
        C3D_BindProgram(Default.m_shaderProgram);
        CitroBindUniforms(Default.m_shaderProgram);
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
    err() << "Failed to compile shader: not yet supported" << std::endl;
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::loadBinary(const Uint8* data, const Uint32 size, Type type)
{
    // Reset the internal state
    m_currentTexture = -1;
    m_textures.clear();
    m_params.clear();

    if (!m_shaderProgram)
        m_shaderProgram = (shaderProgram_s*)malloc(sizeof(shaderProgram_s));

    m_dvlb = DVLB_ParseFile((u32*)data, size);
    if (R_FAILED(shaderProgramInit(m_shaderProgram)))
        return false;
    if (type == Vertex)
        shaderProgramSetVsh(m_shaderProgram, &m_dvlb->DVLE[0]);
    else
        shaderProgramSetGsh(m_shaderProgram, &m_dvlb->DVLE[0], 0);
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
        int location = shaderInstanceGetUniformLocation(m_shaderProgram->vertexShader, name.c_str());
        m_params.insert(std::make_pair(name, location));

        if (location == -1)
            err() << "Parameter \"" << name << "\" not found in shader" << std::endl;

        return location;
    }
}

} // namespace cpp3ds

