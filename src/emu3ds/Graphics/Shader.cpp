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
#include <cpp3ds/Resources.hpp>
#include <cpp3ds/Graphics/Shader.hpp>
#include <cpp3ds/Graphics/Texture.hpp>
#include <cpp3ds/System/InputStream.hpp>
#include <cpp3ds/System/Err.hpp>
#include <cpp3ds/OpenGL.hpp>
#include <fstream>
#include <vector>


// Shader not supported with emulator. Will need to perhaps
// make a conversion script to native GLSL, but not important

namespace cpp3ds
{
////////////////////////////////////////////////////////////
Shader::CurrentTextureType Shader::CurrentTexture;


////////////////////////////////////////////////////////////
Shader::Shader() :
m_shaderProgram (0),
m_currentTexture(-1)
{
}


////////////////////////////////////////////////////////////
Shader::~Shader()
{
}


////////////////////////////////////////////////////////////
bool Shader::loadFromResource(const std::string& filename, Type type, bool compiled)
{
//	priv::ResourceInfo res = priv::resources["glsl_" + filename];
    return false;
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
void Shader::setParameter(const std::string& name, int x) const
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x) const
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y) const
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y, float z) const
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, float x, float y, float z, float w) const
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector2f& v) const
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Vector3f& v) const
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Color& color) const
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const cpp3ds::Transform& transform) const
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, const Texture& texture) const
{
}


////////////////////////////////////////////////////////////
void Shader::setParameter(const std::string& name, CurrentTextureType) const
{
}


////////////////////////////////////////////////////////////
int Shader::getVertexAttributeLocation(const std::string& name) const
{
	return -1;
}


////////////////////////////////////////////////////////////
void Shader::bind(const Shader* shader)
{
}


////////////////////////////////////////////////////////////
bool Shader::isAvailable()
{
    return false;
}


////////////////////////////////////////////////////////////
bool Shader::compile(const char* vertexShaderCode, const char* fragmentShaderCode, const char* geometryShaderCode)
{
    return false;
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
	return 10;
}


////////////////////////////////////////////////////////////
void Shader::bindTextures() const
{
}

} // namespace cpp3ds

