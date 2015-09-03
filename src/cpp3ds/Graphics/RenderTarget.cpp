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
#include <cpp3ds/Graphics/VertexBuffer.hpp>
#include <cpp3ds/Graphics/Light.hpp>
#include <cpp3ds/OpenGL.hpp>
#include <cpp3ds/System/Mutex.hpp>
#include <cpp3ds/System/Lock.hpp>
#include <cpp3ds/System/Err.hpp>
#include <sstream>
#include <cstddef>


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

	// Thread-safe unique identifier generator,
	// is used for id
	cpp3ds::Uint64 getUniqueId()
	{
		static cpp3ds::Uint64 id = 1;
		static cpp3ds::Mutex mutex;

		cpp3ds::Lock lock(mutex);
		return id++;
	}
}


namespace cpp3ds
{
////////////////////////////////////////////////////////////
RenderTarget::RenderTarget() :
m_defaultView           (),
m_view                  (NULL),
m_cache                 (),
m_depthTest             (false),
m_clearDepth            (false),
m_defaultShader         (NULL),
m_currentNonLegacyShader(NULL),
m_lastNonLegacyShader   (NULL),
m_id                    (getUniqueId()),
m_previousViewport      (-1, -1, -1, -1),
m_previousClearColor    (0, 0, 0, 0)
{
	m_cache.vertexCache = new Vertex[StatesCache::VertexCacheSize];
	m_cache.glStatesSet = false;
	Light::increaseLightReferences();
}


////////////////////////////////////////////////////////////
RenderTarget::~RenderTarget()
{
	Light::decreaseLightReferences();
	delete m_defaultShader;
	delete m_view;
	delete[] m_cache.vertexCache;
}


////////////////////////////////////////////////////////////
void RenderTarget::clear(const Color& color)
{
    if (activate(true))
    {
        // Unbind texture to fix RenderTexture preventing clear
        applyTexture(NULL);

		if (color != m_previousClearColor)
		{
        #ifdef EMULATION
            glCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
        #else
            // Use glClearColorIiEXT to avoid unnecessary float conversion
            glCheck(glClearColorIiEXT(color.r, color.g, color.b, color.a));
        #endif
			m_previousClearColor = color;
		}

		glCheck(glClear(GL_COLOR_BUFFER_BIT | (m_clearDepth ? GL_DEPTH_BUFFER_BIT : 0)));
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::enableDepthTest(bool enable)
{
	m_depthTest = enable;

	if(enable)
	{
		glCheck(glEnable(GL_DEPTH_TEST));
	}
	else
	glCheck(glDisable(GL_DEPTH_TEST));
}


////////////////////////////////////////////////////////////
const View& RenderTarget::getView() const
{
    return *m_view;
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
Vector2i RenderTarget::mapCoordsToPixel(const Vector3f& point) const
{
    return mapCoordsToPixel(point, getView());
}


////////////////////////////////////////////////////////////
Vector2i RenderTarget::mapCoordsToPixel(const Vector3f& point, const View& view) const
{
    // First, transform the point by the modelview and projection matrix
    Vector3f normalized = (view.getTransform() * view.getViewTransform()).transformPoint(point);

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
void RenderTarget::draw(const VertexBuffer& buffer, const RenderStates& states)
{
    // Nothing to draw?
    if (!buffer.getVertexCount())
        return;

	draw(&buffer.m_vertices[0], buffer.getVertexCount(), buffer.m_primitiveType, states);
	return;

    if (activate(true))
    {
        // First set the persistent OpenGL states if it's the very first call
        if (!m_cache.glStatesSet)
            resetGLStates();

        // Track if we need to set uniforms again for current shader
        bool shaderChanged = false;

        bool previousShaderWarnSetting = true;

//        if (m_defaultShader)
//        {
//            // Non-legacy rendering, need to set uniforms
//            if (states.shader)
//            {
//                m_currentNonLegacyShader = states.shader;
//                previousShaderWarnSetting = states.shader->warnMissing(false);
//            }
//            else
//                m_currentNonLegacyShader = m_defaultShader;
//
//            shaderChanged = (m_currentNonLegacyShader != m_lastNonLegacyShader);
//
//            m_currentNonLegacyShader->beginParameterBlock();
//        }

        applyTransform(states.transform);

        // Apply the view
        if (shaderChanged || m_cache.viewChanged)
            applyCurrentView();

        // Apply the blend mode
        if (states.blendMode != m_cache.lastBlendMode)
            applyBlendMode(states.blendMode);

        // Apply the texture
        Uint64 textureId = states.texture ? states.texture->m_cacheId : 0;
        if (shaderChanged || (textureId != m_cache.lastTextureId))
            applyTexture(states.texture);

        // Apply the shader
//        if (states.shader)
//            applyShader(states.shader);
//        else if (m_defaultShader)
//            applyShader(m_defaultShader);

        // Find the OpenGL primitive type
        static const GLenum modes[] = {GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN};
        GLenum mode = modes[buffer.getPrimitiveType()];

        // Setup the pointers to the vertices' components
        if (!m_defaultShader)
        {
#ifdef EMULATION
            // Apply the vertex buffer
            Uint64 vertexBufferId = buffer.m_cacheId;
            if (vertexBufferId != m_cache.lastVertexBufferId)
                applyVertexBuffer(&buffer);

            glCheck(glVertexPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position))));
            glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color))));
            glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords))));
            glCheck(glNormalPointer(GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal))));
#else
			// Temorary workaround until gl3ds can get VAO gl*Pointer functions working
			u32 bufferOffsets[] = {0};
			u64 bufferPermutations[] = {0x3210};
			u8 bufferAttribCounts[] = {4};
			GPU_SetAttributeBuffers(
					4, // number of attributes
					(u32 *) osConvertVirtToPhys((u32) &buffer.m_vertices[0]),
					GPU_ATTRIBFMT(0, 3, GPU_FLOAT) | GPU_ATTRIBFMT(1, 4, GPU_UNSIGNED_BYTE) |
					GPU_ATTRIBFMT(2, 2, GPU_FLOAT) | GPU_ATTRIBFMT(3, 3, GPU_FLOAT),
					0xFF8, //0b1100
					0x3210,
					1, //number of buffers
					bufferOffsets,
					bufferPermutations,
					bufferAttribCounts // number of attributes for each buffer
			);
#endif
            // Draw the primitives
            glCheck(glDrawArrays(mode, 0, buffer.getVertexCount()));
        }
        else
        {
            Light::addLightsToShader(*m_currentNonLegacyShader);

            unsigned int arrayObject = 0;
            bool newArray = true;
            bool needUpload = false;

            if (VertexBuffer::hasVertexArrayObjects())
            {
                // Lookup the current context (id, shader id) in the VertexBuffer
                std::pair<Uint64, Uint64> contextIdentifier(m_id, m_currentNonLegacyShader->m_id);
                VertexBuffer::ArrayObjects::iterator arrayObjectIter = buffer.m_arrayObjects.find(contextIdentifier);

                if (arrayObjectIter == buffer.m_arrayObjects.end())
                {
                    // VertexBuffer doesn't have a VAO in this context

                    // Create a new VAO
                    glCheck(glGenVertexArrays(1, &arrayObject));

                    // Register the VAO with the VertexBuffer
                    buffer.m_arrayObjects[contextIdentifier] = arrayObject;

                    // Mark the VAO age as 0
                    m_arrayAgeCount[arrayObject] = 0;
                }
                else
                {
                    // VertexBuffer has/had a VAO in this context

                    // Grab the VAO identifier from the VertexBuffer
                    arrayObject = arrayObjectIter->second;

                    // Still need to check if it still exists
                    ArrayAgeCount::iterator arrayAge = m_arrayAgeCount.find(arrayObject);

                    if (arrayAge != m_arrayAgeCount.end())
                    {
                        // VAO still exists in this context
                        newArray = false;

                        // Check if the VertexBuffer data needs to be re-uploaded
                        needUpload = buffer.m_needUpload;

                        // Mark the VAO age as 0
                        arrayAge->second = 0;
                    }
                    else
                    {
                        // VAO needs to be recreated in this context

                        // Create a new VAO
                        glCheck(glGenVertexArrays(1, &arrayObject));

                        // Register the VAO with the VertexBuffer
                        arrayObjectIter->second = arrayObject;

                        // Mark the VAO age as 0
                        m_arrayAgeCount[arrayObject] = 0;
                    }
                }

                glBindVertexArray(arrayObject);

                // Maximum array object age in draw calls before being purged
                // If an array object was not used to draw this many
                // calls, it will be considered expired and purged
                // from the context owned by this RenderTarget
                const static unsigned int maxArrayObjectAge = 10000;

                // Increment age counters and purge all expired VAOs
                for (ArrayAgeCount::iterator arrayAge = m_arrayAgeCount.begin(); arrayAge != m_arrayAgeCount.end();)
                {
                    arrayAge->second++;

                    if (arrayAge->second > maxArrayObjectAge)
                    {
                        glCheck(glDeleteVertexArrays(1, &(arrayAge->first)));
                        m_arrayAgeCount.erase(arrayAge++);
                        continue;
                    }

                    ++arrayAge;
                }
            }

            int vertexLocation = -1;
            int colorLocation = -1;
            int texCoordLocation = -1;
            int normalLocation = -1;

            // If we are creating a new array object or buffer data
            // needs to be re-uploaded, we need to rebind even if
            // it is still currently bound
            if (newArray || needUpload)
            {
                // Apply the vertex buffer
                Uint64 vertexBufferId = buffer.m_cacheId;
                applyVertexBuffer(&buffer);
            }

            if (newArray)
            {
                vertexLocation   = m_currentNonLegacyShader->getVertexAttributeLocation("sf_Vertex");
                colorLocation    = m_currentNonLegacyShader->getVertexAttributeLocation("sf_Color");
                texCoordLocation = m_currentNonLegacyShader->getVertexAttributeLocation("sf_MultiTexCoord0");
                normalLocation   = m_currentNonLegacyShader->getVertexAttributeLocation("sf_Normal");

                if (vertexLocation >= 0)
                {
                    glCheck(glEnableVertexAttribArray(vertexLocation));
                    glCheck(glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position))));
                }

                if (colorLocation >= 0)
                {
                    glCheck(glEnableVertexAttribArray(colorLocation));
                    glCheck(glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color))));
                }

                if (texCoordLocation >= 0)
                {
                    glCheck(glEnableVertexAttribArray(texCoordLocation));
                    glCheck(glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords))));
                }

                if (normalLocation >= 0)
                {
                    glCheck(glEnableVertexAttribArray(normalLocation));
                    glCheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal))));
                }
            }

            // Draw the primitives
            glCheck(glDrawArrays(mode, 0, buffer.getVertexCount()));

            if (arrayObject)
                glBindVertexArray(0);

            if (vertexLocation >= 0)
                glCheck(glDisableVertexAttribArray(vertexLocation));

            if (colorLocation >= 0)
                glCheck(glDisableVertexAttribArray(colorLocation));

            if (texCoordLocation >= 0)
                glCheck(glDisableVertexAttribArray(texCoordLocation));

            if (normalLocation >= 0)
                glCheck(glDisableVertexAttribArray(normalLocation));
        }

        // Unbind the shader, if any was bound in legacy mode
//        if (states.shader && !m_defaultShader)
//            applyShader(NULL);

//        if (m_defaultShader)
//        {
//            m_currentNonLegacyShader->endParameterBlock();
//
//            if (states.shader)
//                states.shader->warnMissing(previousShaderWarnSetting);
//
//            m_lastNonLegacyShader = m_currentNonLegacyShader;
//            m_currentNonLegacyShader = NULL;
//        }
    }
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

    if (activate(true))
    {
        // First set the persistent OpenGL states if it's the very first call
        if (!m_cache.glStatesSet)
            resetGLStates();

		// Track if we need to set uniforms again for current shader
		bool shaderChanged = false;

		bool previousShaderWarnSetting = true;

//		if (m_defaultShader)
//		{
//			// Non-legacy rendering, need to set uniforms
//			if (states.shader)
//			{
//				m_currentNonLegacyShader = states.shader;
//				previousShaderWarnSetting = states.shader->warnMissing(false);
//			}
//			else
//				m_currentNonLegacyShader = m_defaultShader;
//
//			shaderChanged = (m_currentNonLegacyShader != m_lastNonLegacyShader);
//
//			m_currentNonLegacyShader->beginParameterBlock();
//		}

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
        if (shaderChanged || m_cache.viewChanged)
            applyCurrentView();

        // Apply the blend mode
        if (states.blendMode != m_cache.lastBlendMode)
            applyBlendMode(states.blendMode);

        // Apply the texture
        Uint64 textureId = states.texture ? states.texture->m_cacheId : 0;
        if (shaderChanged || (textureId != m_cache.lastTextureId))
            applyTexture(states.texture);

        // Apply the shader
//        if (states.shader)
//            applyShader(states.shader);
//        else if (m_defaultShader)
//            applyShader(m_defaultShader);

		// Unbind any bound vertex buffer
		if (m_cache.lastVertexBufferId)
			applyVertexBuffer(NULL);

        // If we pre-transform the vertices, we must use our internal vertex cache
        if (useVertexCache)
        {
            // ... and if we already used it previously, we don't need to set the pointers again
            if (!m_cache.useVertexCache)
                vertices = m_cache.vertexCache;
            else
                vertices = NULL;
        }

		// Find the OpenGL primitive type
		static const GLenum modes[] = {GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN};
		GLenum mode = modes[type];

		// Setup the pointers to the vertices' components
		if (!m_defaultShader)
		{
			if (vertices) {
#ifdef EMULATION
                const char* data = reinterpret_cast<const char*>(vertices);
                glCheck(glVertexPointer(3, GL_FLOAT, sizeof(Vertex), data + offsetof(Vertex, position)));
                glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), data + offsetof(Vertex, color)));
                glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), data + offsetof(Vertex, texCoords)));
                glCheck(glNormalPointer(GL_FLOAT, sizeof(Vertex), data + offsetof(Vertex, normal)));
#else
				// Temorary workaround until gl3ds can get VAO gl*Pointer functions working
				u32 bufferOffsets[] = {0};
				u64 bufferPermutations[] = {0x3210};
				u8 bufferAttribCounts[] = {4};
				GPU_SetAttributeBuffers(
						4, // number of attributes
						(u32 *) osConvertVirtToPhys((u32) vertices),
						GPU_ATTRIBFMT(0, 3, GPU_FLOAT) | GPU_ATTRIBFMT(1, 4, GPU_UNSIGNED_BYTE) |
						GPU_ATTRIBFMT(2, 2, GPU_FLOAT) | GPU_ATTRIBFMT(3, 3, GPU_FLOAT),
						0xFF8, //0b1100
						0x3210,
						1, //number of buffers
						bufferOffsets,
						bufferPermutations,
						bufferAttribCounts // number of attributes for each buffer
				);
#endif
			}
			// Draw the primitives
			glCheck(glDrawArrays(mode, 0, vertexCount));

		} else {
			Light::addLightsToShader(*m_currentNonLegacyShader);

			int vertexLocation   = m_currentNonLegacyShader->getVertexAttributeLocation("sf_Vertex");
			int colorLocation    = m_currentNonLegacyShader->getVertexAttributeLocation("sf_Color");
			int texCoordLocation = m_currentNonLegacyShader->getVertexAttributeLocation("sf_MultiTexCoord0");
			int normalLocation   = m_currentNonLegacyShader->getVertexAttributeLocation("sf_Normal");

			const char* data = reinterpret_cast<const char*>(vertices);

			if (vertexLocation >= 0) {
				glCheck(glEnableVertexAttribArray(vertexLocation));
				glCheck(glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), data + offsetof(Vertex, position)));
			}

			if (colorLocation >= 0) {
				glCheck(glEnableVertexAttribArray(colorLocation));
				glCheck(glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), data + offsetof(Vertex, color)));
			}

			if (texCoordLocation >= 0) {
				glCheck(glEnableVertexAttribArray(texCoordLocation));
				glCheck(glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), data + offsetof(Vertex, texCoords)));
			}

			if (normalLocation >= 0) {
				glCheck(glEnableVertexAttribArray(normalLocation));
				glCheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), data + offsetof(Vertex, normal)));
			}

			// Draw the primitives
			glCheck(glDrawArrays(mode, 0, vertexCount));

			if (vertexLocation >= 0)
			glCheck(glDisableVertexAttribArray(vertexLocation));

			if (colorLocation >= 0)
			glCheck(glDisableVertexAttribArray(colorLocation));

			if (texCoordLocation >= 0)
			glCheck(glDisableVertexAttribArray(texCoordLocation));

			if (normalLocation >= 0)
			glCheck(glDisableVertexAttribArray(normalLocation));
		}


		// Unbind the shader, if any was bound in legacy mode
//		if (states.shader && !m_defaultShader)
//			applyShader(NULL);

//		if (m_defaultShader)
//		{
//			m_currentNonLegacyShader->endParameterBlock();
//
//			if (states.shader)
//				states.shader->warnMissing(previousShaderWarnSetting);
//
//			m_lastNonLegacyShader = m_currentNonLegacyShader;
//			m_currentNonLegacyShader = NULL;
//		}

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

        glCheck(glPushAttrib(GL_ALL_ATTRIB_BITS));

        if (!m_defaultShader)
        {
            glCheck(glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS));
            glCheck(glMatrixMode(GL_MODELVIEW));
            glCheck(glPushMatrix());
            glCheck(glMatrixMode(GL_PROJECTION));
            glCheck(glPushMatrix());
            glCheck(glMatrixMode(GL_TEXTURE));
            glCheck(glPushMatrix());
        }
    }

    resetGLStates();
}


////////////////////////////////////////////////////////////
void RenderTarget::popGLStates()
{
    if (activate(true))
    {
        if (m_defaultShader)
            applyShader(NULL);

        if (!m_defaultShader)
        {
            glCheck(glMatrixMode(GL_PROJECTION));
            glCheck(glPopMatrix());
            glCheck(glMatrixMode(GL_MODELVIEW));
            glCheck(glPopMatrix());
            glCheck(glMatrixMode(GL_TEXTURE));
            glCheck(glPopMatrix());
            glCheck(glPopClientAttrib());
        }

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
		glCheck(glDisable(GL_LIGHTING));
		if(!m_depthTest)
			glCheck(glDisable(GL_DEPTH_TEST));
        glCheck(glDisable(GL_ALPHA_TEST));
		glCheck(glEnable(GL_CULL_FACE));
		glCheck(glEnable(GL_BLEND));
		glCheck(glMatrixMode(GL_MODELVIEW));

		glCheck(glDepthFunc(GL_GEQUAL));
		glCheck(glClearDepth(0.f));
		glCheck(glDepthRangef(1.f, 0.f));

		if (!m_defaultShader)
		{
			glCheck(glEnable(GL_TEXTURE_2D));
			glCheck(glEnable(GL_COLOR_MATERIAL));
			glCheck(glEnable(GL_NORMALIZE));
			glCheck(glMatrixMode(GL_MODELVIEW));
			glCheck(glEnableClientState(GL_VERTEX_ARRAY));
			glCheck(glEnableClientState(GL_COLOR_ARRAY));
			glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
			glCheck(glEnableClientState(GL_NORMAL_ARRAY));
		}

		glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        m_cache.glStatesSet = true;

        // Apply the default SFML states
        applyBlendMode(BlendAlpha);
        applyTransform(Transform::Identity);
        applyTexture(NULL);

		if (shaderAvailable)
		{
			if (!m_defaultShader)
				applyShader(NULL);
			else
				applyShader(m_defaultShader);
		}

		if (VertexBuffer::isAvailable())
			applyVertexBuffer(NULL);

        m_cache.useVertexCache = false;

        // Set the default view
        setView(m_defaultView);
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::initialize()
{
    // Setup the default and current views
    m_defaultView.reset(FloatRect(0, 0, static_cast<float>(getSize().x), static_cast<float>(getSize().y)));

    delete m_view;
    m_view = new View(m_defaultView);

    // Set GL states only on first draw, so that we don't pollute user's states
    m_cache.glStatesSet = false;

    // Try to set up non-legacy pipeline if available
    setupNonLegacyPipeline();
}


////////////////////////////////////////////////////////////
void RenderTarget::applyCurrentView()
{
    // Set the viewport
    IntRect viewport = getViewport(*m_view);

    if (viewport != m_previousViewport)
    {
        int top = getSize().y - (viewport.top + viewport.height);
        glCheck(glViewport(viewport.left, top, viewport.width, viewport.height));
        m_previousViewport = viewport;
    }

    if (m_defaultShader)
    {
        const Shader* shader = NULL;

        if (m_currentNonLegacyShader)
            shader = m_currentNonLegacyShader;
        else
            shader = m_defaultShader;

        shader->setParameter("sf_ProjectionMatrix", m_view->getTransform());
        shader->setParameter("sf_ViewMatrix", m_view->getViewTransform());
        shader->setParameter("sf_ViewerPosition", m_view->getPosition());
    }
    else
    {
        // Set the projection matrix
        glCheck(glMatrixMode(GL_PROJECTION));
        glCheck(glLoadMatrixf(m_view->getTransform().getMatrix()));

        // Go back to model-view mode
        glCheck(glMatrixMode(GL_MODELVIEW));
    }

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
    if (m_defaultShader)
    {
        const Shader* shader = NULL;

        if (m_currentNonLegacyShader)
            shader = m_currentNonLegacyShader;
        else
            shader = m_defaultShader;

        shader->setParameter("sf_ModelMatrix", transform);

        const float* modelMatrix = transform.getMatrix();
        Transform normalMatrix(modelMatrix[0], modelMatrix[4], modelMatrix[8],  0.f,
                               modelMatrix[1], modelMatrix[5], modelMatrix[9],  0.f,
                               modelMatrix[2], modelMatrix[6], modelMatrix[10], 0.f,
                               0.f,            0.f,            0.f,             1.f);

        if (Light::isLightingEnabled())
            shader->setParameter("sf_NormalMatrix", normalMatrix.getInverse().getTranspose());
    }
    else
        // No need to call glMatrixMode(GL_MODELVIEW), it is always the
        // current mode (for optimization purpose, since it's the most used)
        glCheck(glLoadMatrixf((m_view->getViewTransform() * transform).getMatrix()));
}


////////////////////////////////////////////////////////////
void RenderTarget::applyViewTransform()
{
    if (!m_defaultShader)
        // No need to call glMatrixMode(GL_MODELVIEW), it is always the
        // current mode (for optimization purpose, since it's the most used)
        glCheck(glLoadMatrixf(m_view->getViewTransform().getMatrix()));
}


////////////////////////////////////////////////////////////
void RenderTarget::applyTexture(const Texture* texture)
{
    if (m_defaultShader)
    {
        const Shader* shader = NULL;

        if (m_currentNonLegacyShader)
            shader = m_currentNonLegacyShader;
        else
            shader = m_defaultShader;

        float xScale = 1.f;
        float yScale = 1.f;
        float yFlip  = 0.f;

        if (texture)
        {
            // Setup scale factors that convert the range [0 .. size] to [0 .. 1]
            xScale = 1.f / texture->m_actualSize.x;
            yScale = 1.f / texture->m_actualSize.y;

            // If pixels are flipped we must invert the Y axis
            if (texture->m_pixelsFlipped)
            {
                yScale = -yScale;
                yFlip = static_cast<float>(texture->m_size.y) / texture->m_actualSize.y;
            }

            Transform textureMatrix(xScale, 0.f,    0.f, 0.f,
                                    0.f,    yScale, 0.f, yFlip,
                                    0.f,    0.f,    1.f, 0.f,
                                    0.f,    0.f,    0.f, 1.f);

            shader->setParameter("sf_TextureMatrix", textureMatrix);
            shader->setParameter("sf_Texture0", *texture);
            shader->setParameter("sf_TextureEnabled", 1);
        }
        else
            shader->setParameter("sf_TextureEnabled", 0);
    }
    else
        Texture::bind(texture, Texture::Pixels);

    m_cache.lastTextureId = texture ? texture->m_cacheId : 0;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyShader(const Shader* shader)
{
    Shader::bind(shader);
}


////////////////////////////////////////////////////////////
void RenderTarget::applyVertexBuffer(const VertexBuffer* buffer)
{
	VertexBuffer::bind(buffer);

	m_cache.lastVertexBufferId = buffer ? buffer->m_cacheId : 0;
}


////////////////////////////////////////////////////////////
void RenderTarget::setupNonLegacyPipeline()
{
	// Setup the default shader if non-legacy rendering is supported
	delete m_defaultShader;
	m_defaultShader = NULL;

	// Check if our shader lighting implementation is supported
	if (!Light::hasShaderLighting())
		return;

	double versionNumber = 0.0;
	std::istringstream versionStringStream(Shader::getSupportedVersion());
	versionStringStream >> versionNumber;

// Disable non-legacy pipeline if requested
#if defined(SFML_LEGACY_GL)
versionNumber = 0.0;
#endif

	// This will only succeed if the supported version is not GLSL ES
	if (versionNumber > 1.29)
	{
		m_defaultShader = new Shader;

		std::stringstream vertexShaderSource;
		vertexShaderSource << "#version 130\n"
				"\n"
				"// Uniforms\n"
				"uniform mat4 sf_ModelMatrix;\n"
				"uniform mat4 sf_ViewMatrix;\n"
				"uniform mat4 sf_ProjectionMatrix;\n"
				"uniform mat4 sf_TextureMatrix;\n"
				"uniform int sf_TextureEnabled;\n"
				"uniform int sf_LightingEnabled;\n"
				"\n"
				"// Vertex attributes\n"
				"in vec3 sf_Vertex;\n"
				"in vec4 sf_Color;\n"
				"in vec2 sf_MultiTexCoord0;\n"
				"in vec3 sf_Normal;\n"
				"\n"
				"// Vertex shader outputs\n"
				"out vec4 sf_FrontColor;\n"
				"out vec2 sf_TexCoord0;\n"
				"out vec3 sf_FragWorldPosition;\n"
				"out vec3 sf_FragNormal;\n"
				"\n"
				"void main()\n"
				"{\n"
				"    // Vertex position\n"
				"    gl_Position = sf_ProjectionMatrix * sf_ViewMatrix * sf_ModelMatrix * vec4(sf_Vertex, 1.0);\n"
				"\n"
				"    // Vertex color\n"
				"    sf_FrontColor = sf_Color;\n"
				"\n"
				"    // Texture data\n"
				"    if (sf_TextureEnabled == 1)\n"
				"        sf_TexCoord0 = (sf_TextureMatrix * vec4(sf_MultiTexCoord0, 0.0, 1.0)).st;\n"
				"\n"
				"    // Lighting data\n"
				"    if (sf_LightingEnabled > 0)\n"
				"    {\n"
				"        sf_FragNormal = sf_Normal;\n"
				"        sf_FragWorldPosition = vec3(sf_ModelMatrix * vec4(sf_Vertex, 1.0));\n"
				"    }\n"
				"}\n";

		std::stringstream fragmentShaderSource;
		fragmentShaderSource << "#version 130\n";

//		if (Shader::isUniformBufferAvailable())
//			fragmentShaderSource << "#extension GL_ARB_uniform_buffer_object : enable\n";

		fragmentShaderSource << "\n"
				"// Light structure\n"
				"struct Light\n"
				"{\n"
				"    vec4 ambientColor;\n"
				"    vec4 diffuseColor;\n"
				"    vec4 specularColor;\n"
				"    vec4 positionDirection;\n"
				"    vec4 attenuation;\n"
				"};\n"
				"\n"
				"// Uniforms\n"
				"uniform mat4 sf_ModelMatrix;\n"
				"uniform mat4 sf_NormalMatrix;\n"
				"uniform sampler2D sf_Texture0;\n"
				"uniform int sf_TextureEnabled;\n"
				"uniform int sf_LightCount;\n"
				"uniform int sf_LightingEnabled;\n"
				"uniform vec3 sf_ViewerPosition;\n"
				"\n";

//		if (Shader::isUniformBufferAvailable())
//			fragmentShaderSource << "layout (std140) uniform Lights\n"
//					"{\n"
//					"    Light sf_Lights[" << Light::getMaximumLights() << "];\n"
//					"};\n";
//		else
			fragmentShaderSource << "uniform Light sf_Lights[" << Light::getMaximumLights() << "];\n";

		fragmentShaderSource << "\n"
				"// Fragment attributes\n"
				"in vec4 sf_FrontColor;\n"
				"in vec2 sf_TexCoord0;\n"
				"in vec3 sf_FragWorldPosition;\n"
				"in vec3 sf_FragNormal;\n"
				"\n"
				"// Fragment shader outputs\n"
				"out vec4 sf_FragColor;\n"
				"\n"
				"vec4 computeLighting()\n"
				"{\n"
				"    // Early return in case lighting disabled\n"
				"    if (sf_LightingEnabled == 0)\n"
				"        return vec4(1.0, 1.0, 1.0, 1.0);\n"
				"\n"
				"    // TODO: Implement way to manipulate materials\n"
				"    const float materialShininess = 1.0;\n"
				"    const vec4 materialSpecularColor = vec4(0.0001, 0.0001, 0.0001, 1.0);\n"
				"\n"
				"    vec3 fragmentNormal = normalize((sf_NormalMatrix * vec4(sf_FragNormal, 1.0)).xyz);\n"
				"    vec3 fragmentDistanceToViewer = normalize(sf_ViewerPosition - sf_FragWorldPosition);"
				"\n"
				"    vec4 totalIntensity = vec4(0.0, 0.0, 0.0, 0.0);\n"
				"\n"
				"    for (int index = 0; index < sf_LightCount; ++index)\n"
				"    {\n"
				"        vec3 rayDirection = normalize(sf_Lights[index].positionDirection.xyz);\n"
				"        float attenuationFactor = 1.0;"
				"\n"
				"        if (sf_Lights[index].positionDirection.w > 0.0)\n"
				"        {\n"
				"            rayDirection = normalize(sf_FragWorldPosition - sf_Lights[index].positionDirection.xyz);\n"
				"            float rayLength = length(sf_Lights[index].positionDirection.xyz - sf_FragWorldPosition);"
				"            vec4 attenuationCoefficients = vec4(1.0, rayLength, rayLength * rayLength, 0.0);"
				"            attenuationFactor = dot(sf_Lights[index].attenuation, attenuationCoefficients);\n"
				"        }\n"
				"\n"
				"        vec4 ambientIntensity = sf_Lights[index].ambientColor;\n"
				"\n"
				"        float diffuseCoefficient = max(0.0, dot(fragmentNormal, -rayDirection));\n"
				"        vec4 diffuseIntensity = sf_Lights[index].diffuseColor * diffuseCoefficient;\n"
				"\n"
				"        float specularCoefficient = 0.0;\n"
				"        if(diffuseCoefficient > 0.0)"
				"            specularCoefficient = pow(max(0.0, dot(fragmentDistanceToViewer, reflect(rayDirection, fragmentNormal))), materialShininess);"
				"        vec4 specularIntensity = specularCoefficient * materialSpecularColor * sf_Lights[index].specularColor;"
				"\n"
				"        totalIntensity += ambientIntensity + (diffuseIntensity + specularIntensity) / attenuationFactor;\n"
				"    }\n"
				"\n"
				"    return vec4(totalIntensity.rgb, 1.0);\n"
				"}\n"
				"\n"
				"vec4 computeTexture()\n"
				"{\n"
				"    if (sf_TextureEnabled == 0)\n"
				"        return vec4(1.0, 1.0, 1.0, 1.0);\n"
				"\n"
				"    return texture2D(sf_Texture0, sf_TexCoord0);\n"
				"}\n"
				"\n"
				"void main()\n"
				"{\n"
				"    // Fragment color\n"
				"    sf_FragColor = sf_FrontColor * computeTexture() * computeLighting();\n"
				"}\n";

		if (!m_defaultShader->loadFromMemory(vertexShaderSource.str(), fragmentShaderSource.str()))
		{
			err() << "Compiling default shader failed. Falling back to legacy pipeline..." << std::endl;
			delete m_defaultShader;
			m_defaultShader = NULL;
		}
	}
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
