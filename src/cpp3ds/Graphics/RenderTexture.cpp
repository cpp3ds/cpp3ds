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
#include <cpp3ds/Graphics/RenderTexture.hpp>
#include <cpp3ds/OpenGL.hpp>
#include <cpp3ds/System/Err.hpp>
#include <cpp3ds/Graphics/TextureSaver.hpp>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
RenderTexture::RenderTexture():
m_width  (0),
m_height (0)
{

}


////////////////////////////////////////////////////////////
RenderTexture::~RenderTexture()
{
	delete m_context;
}


////////////////////////////////////////////////////////////
bool RenderTexture::create(unsigned int width, unsigned int height, bool depthBuffer)
{
    // Create the texture
    if (!m_texture.create(width, height))
    {
        err() << "Impossible to create render texture (failed to create the target texture)" << std::endl;
        return false;
    }

    // We disable smoothing by default for render textures
    setSmooth(false);

    // Store the dimensions
    m_width = width;
    m_height = height;

    // Create the in-memory OpenGL context
    m_context = new Context(ContextSettings(TopScreen, depthBuffer ? 32 : 0), width, height);

    // We can now initialize the render target part
    RenderTarget::initialize();

    return true;
}


////////////////////////////////////////////////////////////
void RenderTexture::setSmooth(bool smooth)
{
    m_texture.setSmooth(smooth);
}


////////////////////////////////////////////////////////////
bool RenderTexture::isSmooth() const
{
    return m_texture.isSmooth();
}


////////////////////////////////////////////////////////////
void RenderTexture::setRepeated(bool repeated)
{
    m_texture.setRepeated(repeated);
}


////////////////////////////////////////////////////////////
bool RenderTexture::isRepeated() const
{
    return m_texture.isRepeated();
}


////////////////////////////////////////////////////////////
bool RenderTexture::setActive(bool active)
{
	return m_context->setActive(active);
}


////////////////////////////////////////////////////////////
void RenderTexture::display()
{
    // Update the target texture
    if (setActive(true))
    {
        // Make sure that the current texture binding will be preserved
        priv::TextureSaver save;

        // Copy the rendered pixels to the texture
        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture.m_texture));
        glCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_width, m_height));
        m_texture.m_pixelsFlipped = true;
    }
}


////////////////////////////////////////////////////////////
Vector2u RenderTexture::getSize() const
{
    return m_texture.getSize();
}


////////////////////////////////////////////////////////////
const Texture& RenderTexture::getTexture() const
{
    return m_texture;
}


////////////////////////////////////////////////////////////
bool RenderTexture::activate(bool active)
{
    return setActive(active);
}

} // namespace cpp3ds
