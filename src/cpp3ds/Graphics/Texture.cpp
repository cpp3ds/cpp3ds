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
#include <cpp3ds/Graphics/Texture.hpp>
#include <cpp3ds/Graphics/Image.hpp>
#include <cpp3ds/OpenGL/GLExtensions.hpp>
#include <cpp3ds/Window/Window.hpp>
#include <cpp3ds/System/Mutex.hpp>
#include <cpp3ds/System/Lock.hpp>
#include <cpp3ds/System/Err.hpp>
#include <cpp3ds/OpenGL.hpp>
#include <cassert>
#include <cstring>
#include <iostream>
#include <c3d/texture.h>
#include "CitroHelpers.hpp"

// Note: vertical flip flag set so 0,0 is top left of texture
#define TEXTURE_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

namespace
{
	cpp3ds::Mutex mutex;

    // Thread-safe unique identifier generator,
    // is used for states cache (see RenderTarget)
	cpp3ds::Uint64 getUniqueId()
	{
		cpp3ds::Lock lock(mutex);

		static cpp3ds::Uint64 id = 1; // start at 1, zero is "no texture"

		return id++;
	}
}


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Texture::Texture() :
m_size         (0, 0),
m_actualSize   (0, 0),
m_texture      (nullptr),
m_isSmooth     (false),
m_isRepeated   (false),
m_pixelsFlipped(false),
m_cacheId      (getUniqueId())
{

}


////////////////////////////////////////////////////////////
Texture::Texture(const Texture& copy) :
m_size         (0, 0),
m_actualSize   (0, 0),
m_texture      (nullptr),
m_isSmooth     (copy.m_isSmooth),
m_isRepeated   (copy.m_isRepeated),
m_pixelsFlipped(false),
m_cacheId      (getUniqueId())
{
    if (copy.m_texture)
        loadFromImage(copy.copyToImage());
}


////////////////////////////////////////////////////////////
Texture::~Texture()
{
    // Destroy the OpenGL texture
    if (m_texture)
    {
        C3D_TexDelete(m_texture);
        delete m_texture;
    }
}


////////////////////////////////////////////////////////////
bool Texture::create(unsigned int width, unsigned int height)
{
    // Check if texture parameters are valid before creating it
    if ((width == 0) || (height == 0))
    {
        err() << "Failed to create texture, invalid size (" << width << "x" << height << ")" << std::endl;
        return false;
    }

    // Compute the internal texture dimensions depending on NPOT textures support
    Vector2u actualSize(getValidSize(width), getValidSize(height));

    // Check the maximum texture size
    unsigned int maxSize = getMaximumSize();
    if ((actualSize.x > maxSize) || (actualSize.y > maxSize))
    {
        err() << "Failed to create texture, its internal size is too high "
              << "(" << actualSize.x << "x" << actualSize.y << ", "
              << "maximum is " << maxSize << "x" << maxSize << ")"
              << std::endl;
        return false;
    }
    if (actualSize.x < 64) actualSize.x = 64;
    if (actualSize.y < 64) actualSize.y = 64;

    // All the validity checks passed, we can store the new texture settings
    m_size.x        = width;
    m_size.y        = height;
    m_actualSize    = actualSize;
    m_pixelsFlipped = false;

	ensureGlContext();

    // Create the citro3d texture, or delete if already created
    if (!m_texture)
        m_texture = new C3D_Tex();
    else
        C3D_TexDelete(m_texture);

    if (!m_texture)
        return false;
    if (!C3D_TexInit(m_texture, m_actualSize.x, m_actualSize.y, GPU_RGBA8))
        return false;

    C3D_TexSetWrap(m_texture,
                   m_isRepeated ? GPU_REPEAT : GPU_CLAMP_TO_EDGE,
                   m_isRepeated ? GPU_REPEAT : GPU_CLAMP_TO_EDGE);
    C3D_TexSetFilter(m_texture,
                     m_isSmooth ? GPU_LINEAR : GPU_NEAREST,
                     m_isSmooth ? GPU_LINEAR : GPU_NEAREST);

    m_cacheId = getUniqueId();

    return true;
}


////////////////////////////////////////////////////////////
bool Texture::loadFromFile(const std::string& filename, const IntRect& area)
{
    Image image;
    return image.loadFromFile(filename) && loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromMemory(const void* data, std::size_t size, const IntRect& area)
{
    Image image;
    return image.loadFromMemory(data, size) && loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromStream(InputStream& stream, const IntRect& area)
{
    Image image;
    return image.loadFromStream(stream) && loadFromImage(image, area);
}

////////////////////////////////////////////////////////////
bool Texture::loadFromImage(const Image& image, const IntRect& area)
{
    // Retrieve the image size
    int width = static_cast<int>(image.getSize().x);
    int height = static_cast<int>(image.getSize().y);

    // Load the entire image if the source area is either empty or contains the whole image
    if (area.width == 0 || (area.height == 0) ||
       ((area.left <= 0) && (area.top <= 0) && (area.width >= width) && (area.height >= height)))
    {
        // Load the entire image
        if (create(image.getSize().x, image.getSize().y))
        {
            update(image);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // Load a sub-area of the image

        // Adjust the rectangle to the size of the image
        IntRect rectangle = area;
        if (rectangle.left   < 0) rectangle.left = 0;
        if (rectangle.top    < 0) rectangle.top  = 0;
        if (rectangle.left + rectangle.width > width)  rectangle.width  = width - rectangle.left;
        if (rectangle.top + rectangle.height > height) rectangle.height = height - rectangle.top;

        // Create the texture and upload the pixels
        if (create(rectangle.width, rectangle.height))
        {
            err() << "Unsupported texture operation!" << std::endl;
            // Copy the pixels to the texture, row by row
            const Uint8* pixels = image.getPixelsPtr() + 4 * (rectangle.left + (width * rectangle.top));
//            glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
            for (int i = 0; i < rectangle.height; ++i)
            {
                // TODO: implement GX_TextureCopy
//                glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, rectangle.width, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
                pixels += 4 * width;
            }

            return true;
        }
        else
        {
            return false;
        }
    }
}


////////////////////////////////////////////////////////////
Vector2u Texture::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
Image Texture::copyToImage() const
{
    // Easy case: empty texture
    if (!m_texture)
        return Image();

    // Create an array of pixels
    std::vector<Uint8> pixels(m_size.x * m_size.y * 4);

    u32 *data = (u32*)linearAlloc(m_texture->size);
    u32 dim = GX_BUFFER_DIM(m_texture->width, m_texture->height);
    GX_DisplayTransfer((u32*)m_texture->data, dim, data, dim, TEXTURE_TRANSFER_FLAGS);
    gspWaitForPPF();
    GSPGPU_FlushDataCache(data, m_texture->size);

    for (int i = 0; i < m_texture->width * m_texture->height; ++i)
        data[i] = __builtin_bswap32(data[i]);

	if ((m_size == m_actualSize) && !m_pixelsFlipped)
	{
		// Texture is not padded nor flipped, we can use a direct copy
        std::memcpy(&pixels[0], data, pixels.size());
	}
	else
	{
		// Texture is either padded or flipped, we have to use a slower algorithm

		// All the pixels will first be copied to a temporary array
		std::vector<Uint8> allPixels(m_actualSize.x * m_actualSize.y * 4);
        std::memcpy(&allPixels[0], data, allPixels.size());

		// Then we copy the useful pixels from the temporary array to the final one
		const Uint8* src = &allPixels[0];
		Uint8* dst = &pixels[0];
		int srcPitch = m_actualSize.x * 4;
		int dstPitch = m_size.x * 4;

		// Handle the case where source pixels are flipped vertically
		if (m_pixelsFlipped)
		{
			src += srcPitch * (m_size.y - 1);
			srcPitch = -srcPitch;
		}

		for (unsigned int i = 0; i < m_size.y; ++i)
		{
			std::memcpy(dst, src, dstPitch);
			src += srcPitch;
			dst += dstPitch;
		}
	}

    // Create the image
    Image image;
    image.create(m_size.x, m_size.y, &pixels[0]);

    linearFree(data);

    return image;
}


////////////////////////////////////////////////////////////
void Texture::update(const Uint8* pixels)
{
    // Update the whole texture
    update(pixels, m_size.x, m_size.y, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Uint8* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
    assert(x + width <= m_size.x);
    assert(y + height <= m_size.y);

    if (pixels && m_texture)
    {
        // TODO: use GX_TextureCopy

        const u32 *pixels32 = reinterpret_cast<const u32*>(pixels);
        u32 *data = (u32*)linearAlloc(m_texture->size);
        u32 dim = GX_BUFFER_DIM(m_texture->width, m_texture->height);

        GX_DisplayTransfer((u32*)m_texture->data, dim, data, dim, TEXTURE_TRANSFER_FLAGS);
        gspWaitForPPF();

        for (int h = 0; h < height; ++h)
        {
            for (int w = 0; w < width; ++w)
            {
                data[(y+h)*m_texture->width+x+w] = __builtin_bswap32(pixels32[(h*width) + w]);
            }
        }
        GSPGPU_FlushDataCache(data, m_texture->size);

        GX_DisplayTransfer(data, dim, (u32*)m_texture->data, dim, TEXTURE_TRANSFER_FLAGS | GX_TRANSFER_OUT_TILED(1));
        gspWaitForPPF();

        linearFree(data);

        m_pixelsFlipped = false;
        m_cacheId = getUniqueId();
    }
}


////////////////////////////////////////////////////////////
void Texture::update(const Image& image)
{
    // Update the whole texture
    update(image, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Image& image, unsigned int x, unsigned int y)
{
	update(image.getPixelsPtr(), image.getSize().x, image.getSize().y, x, y);
}


////////////////////////////////////////////////////////////
void Texture::update(const Window& window)
{
    update(window, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Window& window, unsigned int x, unsigned int y)
{
    assert(x + window.getSize().x <= m_size.x);
    assert(y + window.getSize().y <= m_size.y);

    if (m_texture)
    {
        err() << "Function not yet implemented" << std::endl;

        m_pixelsFlipped = true;
        m_cacheId = getUniqueId();
    }
}


////////////////////////////////////////////////////////////
void Texture::setSmooth(bool smooth)
{
    if (smooth != m_isSmooth)
    {
        m_isSmooth = smooth;

        if (m_texture)
        {
            C3D_TexSetFilter(m_texture,
                             m_isSmooth ? GPU_LINEAR : GPU_NEAREST,
                             m_isSmooth ? GPU_LINEAR : GPU_NEAREST);
        }
    }
}


////////////////////////////////////////////////////////////
bool Texture::isSmooth() const
{
    return m_isSmooth;
}


////////////////////////////////////////////////////////////
void Texture::setRepeated(bool repeated)
{
    if (repeated != m_isRepeated)
    {
        m_isRepeated = repeated;

        if (m_texture)
        {
            C3D_TexSetWrap(m_texture,
                           m_isRepeated ? GPU_REPEAT : GPU_CLAMP_TO_EDGE,
                           m_isRepeated ? GPU_REPEAT : GPU_CLAMP_TO_EDGE);
        }
    }
}


////////////////////////////////////////////////////////////
bool Texture::isRepeated() const
{
    return m_isRepeated;
}


////////////////////////////////////////////////////////////
void Texture::bind(const Texture* texture, CoordinateType coordinateType)
{
    if (texture && texture->m_texture)
    {
        // Bind the texture
        C3D_TexBind(0, texture->m_texture);

        C3D_TexEnv* env = C3D_GetTexEnv(0);
        C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, 0);
        C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
        C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);

        // Check if we need to define a special texture matrix
        if ((coordinateType == Pixels) || texture->m_pixelsFlipped)
        {
            float matrix[16] = {0.f, 0.f, 0.f, 1.f,
                                0.f, 0.f, 1.f, 0.f,
                                0.f, 1.f, 0.f, 0.f,
                                1.f, 0.f, 0.f, 0.f};

            // If non-normalized coordinates (= pixels) are requested, we need to
            // setup scale factors that convert the range [0 .. size] to [0 .. 1]
            if (coordinateType == Pixels)
            {
                matrix[3] = 1.f / texture->m_actualSize.x;
                matrix[6] = 1.f / texture->m_actualSize.y;
            }

            // If pixels are flipped we must invert the Y axis
            if (texture->m_pixelsFlipped)
            {
                matrix[6] = -matrix[6];
                matrix[9] = static_cast<float>(texture->m_size.y) / texture->m_actualSize.y;
            }

            // Load the matrix
            memcpy(MtxStack_Cur(CitroGetTextureMatrix())->m, matrix, sizeof(C3D_Mtx));
        }
    }
    else
    {
        // Bind no texture
        C3D_TexBind(0, NULL);

        C3D_TexEnv* env = C3D_GetTexEnv(0);
        C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR, 0, 0);
        C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
        C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

        // Reset the texture matrix
        Mtx_Identity(MtxStack_Cur(CitroGetTextureMatrix()));
    }
}


////////////////////////////////////////////////////////////
unsigned int Texture::getMaximumSize()
{
	return 1024;
}


////////////////////////////////////////////////////////////
Texture& Texture::operator =(const Texture& right)
{
    Texture temp(right);

    std::swap(m_size,          temp.m_size);
    std::swap(m_actualSize,    temp.m_actualSize);
    std::swap(m_texture,       temp.m_texture);
    std::swap(m_isSmooth,      temp.m_isSmooth);
    std::swap(m_isRepeated,    temp.m_isRepeated);
    std::swap(m_pixelsFlipped, temp.m_pixelsFlipped);
    m_cacheId = getUniqueId();

    return *this;
}


////////////////////////////////////////////////////////////
unsigned int Texture::getValidSize(unsigned int size)
{
    // If hardware doesn't support NPOT textures, we calculate the nearest power of two
    unsigned int powerOfTwo = 1;
    while (powerOfTwo < size)
        powerOfTwo *= 2;

    return powerOfTwo;
}

} // namespace cpp3ds
