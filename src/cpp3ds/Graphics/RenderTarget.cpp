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
#include <cpp3ds/Graphics/RenderTarget.hpp>
#include <cpp3ds/Graphics/Drawable.hpp>
#include <cpp3ds/Graphics/Shader.hpp>
#include <cpp3ds/Graphics/Texture.hpp>
#include <cpp3ds/Graphics/VertexArray.hpp>
#include <cpp3ds/OpenGL.hpp>
#include <cpp3ds/System/Err.hpp>


namespace
{
    // Convert an cpp3ds::BlendMode::Factor constant to the corresponding OpenGL constant.
    cpp3ds::Uint32 factorToGlConstant(cpp3ds::BlendMode::Factor blendFactor)
    {
        switch (blendFactor)
        {
            default:
            case cpp3ds::BlendMode::Zero:             return GL_ZERO;
            case cpp3ds::BlendMode::One:              return GL_ONE;
            case cpp3ds::BlendMode::SrcColor:         return GL_SRC_COLOR;
            case cpp3ds::BlendMode::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
            case cpp3ds::BlendMode::DstColor:         return GL_DST_COLOR;
            case cpp3ds::BlendMode::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
            case cpp3ds::BlendMode::SrcAlpha:         return GL_SRC_ALPHA;
            case cpp3ds::BlendMode::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
            case cpp3ds::BlendMode::DstAlpha:         return GL_DST_ALPHA;
            case cpp3ds::BlendMode::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
        }
    }


    // Convert an cpp3ds::BlendMode::BlendEquation constant to the corresponding OpenGL constant.
    cpp3ds::Uint32 equationToGlConstant(cpp3ds::BlendMode::Equation blendEquation)
    {
        switch (blendEquation)
        {
            default:
            case cpp3ds::BlendMode::Add:             return GL_FUNC_ADD;
            case cpp3ds::BlendMode::Subtract:        return GL_FUNC_SUBTRACT;
        }
    }
}


namespace cpp3ds
{
////////////////////////////////////////////////////////////
RenderTarget::RenderTarget() :
m_defaultView(),
m_view       (),
m_cache      ()
{
	m_cache.vertexCache = new Vertex[StatesCache::VertexCacheSize];
	m_cache.glStatesSet = false;
}


////////////////////////////////////////////////////////////
RenderTarget::~RenderTarget()
{
	delete[] m_cache.vertexCache;
}


////////////////////////////////////////////////////////////
void RenderTarget::clear(const Color& color)
{
    if (activate(true))
    {
        // Unbind texture to fix RenderTexture preventing clear
        applyTexture(NULL);

        #ifdef EMULATION
            glCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
        #else
            // Use glClearColorIiEXT to avoid unnecessary float conversion
            glCheck(glClearColorIiEXT(color.r, color.g, color.b, color.a));
        #endif
        glCheck(glClear(GL_COLOR_BUFFER_BIT));
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::setView(const View& view)
{
    m_view = view;
    m_cache.viewChanged = true;
}


////////////////////////////////////////////////////////////
const View& RenderTarget::getView() const
{
    return m_view;
}


////////////////////////////////////////////////////////////
const View& RenderTarget::getDefaultView() const
{
    return m_defaultView;
}


////////////////////////////////////////////////////////////
IntRect RenderTarget::getViewport(const View& view) const
{
    float width  = static_cast<float>(getSize().x);
    float height = static_cast<float>(getSize().y);
    const FloatRect& viewport = view.getViewport();

    return IntRect(static_cast<int>(0.5f + width  * viewport.left),
                   static_cast<int>(0.5f + height * viewport.top),
                   static_cast<int>(0.5f + width  * viewport.width),
                   static_cast<int>(0.5f + height * viewport.height));
}


////////////////////////////////////////////////////////////
Vector2f RenderTarget::mapPixelToCoords(const Vector2i& point) const
{
    return mapPixelToCoords(point, getView());
}


////////////////////////////////////////////////////////////
Vector2f RenderTarget::mapPixelToCoords(const Vector2i& point, const View& view) const
{
    // First, convert from viewport coordinates to homogeneous coordinates
    Vector2f normalized;
    IntRect viewport = getViewport(view);
    normalized.x = -1.f + 2.f * (point.x - viewport.left) / viewport.width;
    normalized.y =  1.f - 2.f * (point.y - viewport.top)  / viewport.height;

    // Then transform by the inverse of the view matrix
    return view.getInverseTransform().transformPoint(normalized);
}


////////////////////////////////////////////////////////////
Vector2i RenderTarget::mapCoordsToPixel(const Vector2f& point) const
{
    return mapCoordsToPixel(point, getView());
}


////////////////////////////////////////////////////////////
Vector2i RenderTarget::mapCoordsToPixel(const Vector2f& point, const View& view) const
{
    // First, transform the point by the view matrix
    Vector2f normalized = view.getTransform().transformPoint(point);

    // Then convert to viewport coordinates
    Vector2i pixel;
    IntRect viewport = getViewport(view);
    pixel.x = static_cast<int>(( normalized.x + 1.f) / 2.f * viewport.width  + viewport.left);
    pixel.y = static_cast<int>((-normalized.y + 1.f) / 2.f * viewport.height + viewport.top);

    return pixel;
}


////////////////////////////////////////////////////////////
void RenderTarget::draw(const Drawable& drawable, const RenderStates& states)
{
    drawable.draw(*this, states);
}


////////////////////////////////////////////////////////////
void RenderTarget::draw(const Vertex* vertices, unsigned int vertexCount,
                        PrimitiveType type, const RenderStates& states)
{
    // Nothing to draw?
    if (!vertices || (vertexCount == 0))
        return;

	// Vertices allocated in the stack (common) can't be converted to physical address
	#ifndef EMULATION
	if (osConvertVirtToPhys((u32)vertices) == 0)
	{
		err() << "RenderTarget::draw() called with vertex array in inaccessible memory space." << std::endl;
		return;
	}
	#endif

	// GL_QUADS is unavailable on OpenGL ES
	if (type == Quads)
	{
		err() << "cpp3ds::Quads primitive type is not supported on OpenGL ES platforms, drawing skipped" << std::endl;
		return;
	}
	#define GL_QUADS 0

    if (activate(true))
    {
        // First set the persistent OpenGL states if it's the very first call
        if (!m_cache.glStatesSet)
            resetGLStates();

        // Check if the vertex count is low enough so that we can pre-transform them
        bool useVertexCache = (vertexCount <= StatesCache::VertexCacheSize);
        if (useVertexCache)
        {
            // Pre-transform the vertices and store them into the vertex cache
            for (unsigned int i = 0; i < vertexCount; ++i)
            {
                Vertex& vertex = m_cache.vertexCache[i];
                vertex.position = states.transform * vertices[i].position;
                vertex.color = vertices[i].color;
                vertex.texCoords = vertices[i].texCoords;
            }

            // Since vertices are transformed, we must use an identity transform to render them
            if (!m_cache.useVertexCache)
                applyTransform(Transform::Identity);
        }
        else
        {
            applyTransform(states.transform);
        }

        // Apply the view
        if (m_cache.viewChanged)
            applyCurrentView();

        // Apply the blend mode
        if (states.blendMode != m_cache.lastBlendMode)
            applyBlendMode(states.blendMode);

        // Apply the texture
        Uint64 textureId = states.texture ? states.texture->m_cacheId : 0;
        if (textureId != m_cache.lastTextureId)
            applyTexture(states.texture);

        // Apply the shader
        if (states.shader)
            applyShader(states.shader);

        // If we pre-transform the vertices, we must use our internal vertex cache
        if (useVertexCache)
        {
            // ... and if we already used it previously, we don't need to set the pointers again
            if (!m_cache.useVertexCache)
                vertices = m_cache.vertexCache;
            else
                vertices = NULL;
        }

        // Setup the pointers to the vertices' components
        if (vertices)
        {
            #ifdef EMULATION
                const char* data = reinterpret_cast<const char*>(vertices);
                glCheck(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), data + 0));
                glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), data + 8)); // 8 = sizeof(Vector2f)
                glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), data + 12)); // 12 = 8 + sizeof(Color)
            #else
				// Temorary workaround until gl3ds can get VAO gl*Pointer functions working
				u32 bufferOffsets[] = {0};
				u64 bufferPermutations[] = {0x210};
				u8 bufferAttribCounts[] = {3};
				GPU_SetAttributeBuffers(
					3, // number of attributes
					(u32*)osConvertVirtToPhys((u32)vertices),
					GPU_ATTRIBFMT(0, 2, GPU_FLOAT) | GPU_ATTRIBFMT(1, 4, GPU_UNSIGNED_BYTE) | GPU_ATTRIBFMT(2, 2, GPU_FLOAT),
					0xFF8, //0b1100
					0x210,
					1, //number of buffers
					bufferOffsets,
					bufferPermutations,
					bufferAttribCounts // number of attributes for each buffer
				);
            #endif
        }

        // Find the OpenGL primitive type
        static const GLenum modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
                                       GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
        GLenum mode = modes[type];

        // Draw the primitives
        glCheck(glDrawArrays(mode, 0, vertexCount));

        // Unbind the shader, if any
        if (states.shader)
            applyShader(NULL);

        // Update the cache
        m_cache.useVertexCache = useVertexCache;
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::pushGLStates()
{
	if (activate(true))
    {
        #ifdef CPP3DS_DEBUG
            // make sure that the user didn't leave an unchecked OpenGL error
            GLenum error = glGetError();
            if (error != GL_NO_ERROR)
            {
                err() << "OpenGL error (" << error << ") detected in user code, "
                      << "you should check for errors with glGetError()"
                      << std::endl;
            }
        #endif

		glCheck(glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS));
		glCheck(glPushAttrib(GL_ALL_ATTRIB_BITS));

		glCheck(glMatrixMode(GL_MODELVIEW));
		glCheck(glPushMatrix());
		glCheck(glMatrixMode(GL_PROJECTION));
		glCheck(glPushMatrix());
		glCheck(glMatrixMode(GL_TEXTURE));
		glCheck(glPushMatrix());
    }
    resetGLStates();
}


////////////////////////////////////////////////////////////
void RenderTarget::popGLStates()
{
    if (activate(true))
    {
		glCheck(glMatrixMode(GL_PROJECTION));
		glCheck(glPopMatrix());
		glCheck(glMatrixMode(GL_MODELVIEW));
		glCheck(glPopMatrix());
		glCheck(glMatrixMode(GL_TEXTURE));
		glCheck(glPopMatrix());

		glCheck(glPopClientAttrib());
		glCheck(glPopAttrib());
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::resetGLStates()
{
	// Check here to make sure a context change does not happen after activate(true)
    bool shaderAvailable = Shader::isAvailable();

    if (activate(true))
    {
        // Make sure that the texture unit which is active is the number 0
        if (GLEXT_multitexture)
        {
            glCheck(GLEXT_glClientActiveTexture(GLEXT_GL_TEXTURE0));
            glCheck(GLEXT_glActiveTexture(GLEXT_GL_TEXTURE0));
        }

        // Define the default OpenGL states
        glCheck(glDisable(GL_CULL_FACE));
        glCheck(glDisable(GL_DEPTH_TEST));
        glCheck(glDisable(GL_ALPHA_TEST));
        glCheck(glEnable(GL_TEXTURE_2D));
        glCheck(glEnable(GL_BLEND));
        glCheck(glMatrixMode(GL_MODELVIEW));
        #ifdef EMULATION
        	glCheck(glDisable(GL_LIGHTING));
		#endif
		glCheck(glEnableClientState(GL_VERTEX_ARRAY));
		glCheck(glEnableClientState(GL_COLOR_ARRAY));
		glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
        m_cache.glStatesSet = true;

        // Apply the default SFML states
        applyBlendMode(BlendAlpha);
        applyTransform(Transform::Identity);
        applyTexture(NULL);
        if (shaderAvailable)
            applyShader(NULL);

        m_cache.useVertexCache = false;

        // Set the default view
        setView(getView());
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::initialize()
{
    // Setup the default and current views
    m_defaultView.reset(FloatRect(0, 0, static_cast<float>(getSize().x), static_cast<float>(getSize().y)));
    m_view = m_defaultView;

    // Set GL states only on first draw, so that we don't pollute user's states
    m_cache.glStatesSet = false;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyCurrentView()
{
	// Set the viewport
    IntRect viewport = getViewport(m_view);
    int top = getSize().y - (viewport.top + viewport.height);
    glCheck(glViewport(viewport.left, top, viewport.width, viewport.height));

	// Set the projection matrix
    glCheck(glMatrixMode(GL_PROJECTION));
	glCheck(glLoadMatrixf(m_view.getTransform().getMatrix()));

    // Go back to model-view mode
    glCheck(glMatrixMode(GL_MODELVIEW));

    m_cache.viewChanged = false;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyBlendMode(const BlendMode& mode)
{
    // Apply the blend mode
	glCheck(glBlendFuncSeparate(
		factorToGlConstant(mode.colorSrcFactor), factorToGlConstant(mode.colorDstFactor),
		factorToGlConstant(mode.alphaSrcFactor), factorToGlConstant(mode.alphaDstFactor)));

	glCheck(glBlendEquationSeparate(
		equationToGlConstant(mode.colorEquation),
		equationToGlConstant(mode.alphaEquation)));

    m_cache.lastBlendMode = mode;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyTransform(const Transform& transform)
{
    // No need to call glMatrixMode(GL_MODELVIEW), it is always the
    // current mode (for optimization purpose, since it's the most used)
    glCheck(glLoadMatrixf(transform.getMatrix()));
}


////////////////////////////////////////////////////////////
void RenderTarget::applyTexture(const Texture* texture)
{
    Texture::bind(texture, Texture::Pixels);

    m_cache.lastTextureId = texture ? texture->m_cacheId : 0;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyShader(const Shader* shader)
{
    Shader::bind(shader);
}

} // namespace cpp3ds


////////////////////////////////////////////////////////////
// Render states caching strategies
//
// * View
//   If SetView was called since last draw, the projection
//   matrix is updated. We don't need more, the view doesn't
//   change frequently.
//
// * Transform
//   The transform matrix is usually expensive because each
//   entity will most likely use a different transform. This can
//   lead, in worst case, to changing it every 4 vertices.
//   To avoid that, when the vertex count is low enough, we
//   pre-transform them and therefore use an identity transform
//   to render them.
//
// * Blending mode
//   Since it overloads the == operator, we can easily check
//   whether any of the 6 blending components changed and,
//   thus, whether we need to update the blend mode.
//
// * Texture
//   Storing the pointer or OpenGL ID of the last used texture
//   is not enough; if the cpp3ds::Texture instance is destroyed,
//   both the pointer and the OpenGL ID might be recycled in
//   a new texture instance. We need to use our own unique
//   identifier system to ensure consistent caching.
//
// * Shader
//   Shaders are very hard to optimize, because they have
//   parameters that can be hard (if not impossible) to track,
//   like matrices or textures. The only optimization that we
//   do is that we avoid setting a null shader if there was
//   already none for the previous draw.
//
////////////////////////////////////////////////////////////
