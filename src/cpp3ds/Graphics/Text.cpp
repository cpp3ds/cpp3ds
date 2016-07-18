////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
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
#include <cpp3ds/Graphics/Text.hpp>
#include <cpp3ds/Graphics/Texture.hpp>
#include <cpp3ds/Graphics/RenderTarget.hpp>
#include <cpp3ds/Resources.hpp>
#include <cmath>
#include <iostream>
#ifndef EMULATION
#include "CitroHelpers.hpp"
#include <citro3d.h>
#endif


namespace
{
// Add an underline or strikethrough line to the vertex array
void addLine(cpp3ds::VertexArray& vertices, float lineLength, float lineTop, const cpp3ds::Color& color, float offset, float thickness, float outlineThickness = 0)
{
    float top = std::floor(lineTop + offset - (thickness / 2) + 0.5f);
    float bottom = top + std::floor(thickness + 0.5f);

    vertices.append(cpp3ds::Vertex(cpp3ds::Vector2f(-outlineThickness,             top    - outlineThickness), color, cpp3ds::Vector2f(1, 1)));
    vertices.append(cpp3ds::Vertex(cpp3ds::Vector2f(lineLength + outlineThickness, top    - outlineThickness), color, cpp3ds::Vector2f(1, 1)));
    vertices.append(cpp3ds::Vertex(cpp3ds::Vector2f(-outlineThickness,             bottom + outlineThickness), color, cpp3ds::Vector2f(1, 1)));
    vertices.append(cpp3ds::Vertex(cpp3ds::Vector2f(-outlineThickness,             bottom + outlineThickness), color, cpp3ds::Vector2f(1, 1)));
    vertices.append(cpp3ds::Vertex(cpp3ds::Vector2f(lineLength + outlineThickness, top    - outlineThickness), color, cpp3ds::Vector2f(1, 1)));
    vertices.append(cpp3ds::Vertex(cpp3ds::Vector2f(lineLength + outlineThickness, bottom + outlineThickness), color, cpp3ds::Vector2f(1, 1)));
}

// Add a glyph quad to the vertex array
void addGlyphQuad(cpp3ds::VertexArray& vertices, cpp3ds::Vector2f position, const cpp3ds::Color& color, const cpp3ds::Glyph& glyph, float italic, float outlineThickness = 0)
{
    float left   = glyph.bounds.left;
    float top    = glyph.bounds.top;
    float right  = glyph.bounds.left + glyph.bounds.width;
    float bottom = glyph.bounds.top  + glyph.bounds.height;

    float u1 = static_cast<float>(glyph.textureRect.left);
    float v1 = static_cast<float>(glyph.textureRect.top);
    float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width);
    float v2 = static_cast<float>(glyph.textureRect.top  + glyph.textureRect.height);

    vertices.append(cpp3ds::Vertex(cpp3ds::Vector2f(position.x + left  - italic * top    - outlineThickness, position.y + top    - outlineThickness), color, cpp3ds::Vector2f(u1, v1)));
    vertices.append(cpp3ds::Vertex(cpp3ds::Vector2f(position.x + right - italic * top    - outlineThickness, position.y + top    - outlineThickness), color, cpp3ds::Vector2f(u2, v1)));
    vertices.append(cpp3ds::Vertex(cpp3ds::Vector2f(position.x + left  - italic * bottom - outlineThickness, position.y + bottom - outlineThickness), color, cpp3ds::Vector2f(u1, v2)));
    vertices.append(cpp3ds::Vertex(cpp3ds::Vector2f(position.x + left  - italic * bottom - outlineThickness, position.y + bottom - outlineThickness), color, cpp3ds::Vector2f(u1, v2)));
    vertices.append(cpp3ds::Vertex(cpp3ds::Vector2f(position.x + right - italic * top    - outlineThickness, position.y + top    - outlineThickness), color, cpp3ds::Vector2f(u2, v1)));
    vertices.append(cpp3ds::Vertex(cpp3ds::Vector2f(position.x + right - italic * bottom - outlineThickness, position.y + bottom - outlineThickness), color, cpp3ds::Vector2f(u2, v2)));
}
}


namespace cpp3ds
{

#ifndef EMULATION
extern cpp3ds::Texture *system_font_textures;
#endif

namespace priv
{
	// Default font for Text objects for user convenience
	Font system_font;
	static bool system_font_loaded;
}

////////////////////////////////////////////////////////////
Text::Text() :
        m_string            (),
        m_font              (&priv::system_font),
        m_characterSize     (30),
        m_style             (Regular),
        m_fillColor         (255, 255, 255),
        m_outlineColor      (0, 0, 0),
        m_outlineThickness  (0),
        m_vertices          (Triangles),
        m_outlineVertices   (Triangles),
        m_bounds            (),
        m_geometryNeedUpdate(false),
        m_useSystemFont     (false)
{

}


////////////////////////////////////////////////////////////
Text::Text(const String& string, const Font& font, unsigned int characterSize) :
        m_string            (string),
        m_font              (&font),
        m_characterSize     (characterSize),
        m_style             (Regular),
        m_fillColor         (255, 255, 255),
        m_outlineColor      (0, 0, 0),
        m_outlineThickness  (0),
        m_vertices          (Triangles),
        m_outlineVertices   (Triangles),
        m_bounds            (),
        m_geometryNeedUpdate(true),
        m_useSystemFont     (false)
{

}


////////////////////////////////////////////////////////////
void Text::setString(const String& string)
{
    if (m_string != string)
    {
        m_string = string;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setFont(const Font& font)
{
    if (m_font != &font)
    {
        m_font = &font;
        m_geometryNeedUpdate = true;
        m_useSystemFont = false;
    }
}


////////////////////////////////////////////////////////////
void Text::useSystemFont()
{
#ifndef EMULATION
    m_geometryNeedUpdate = true;
    m_useSystemFont = true;
#endif
}


////////////////////////////////////////////////////////////
void Text::setCharacterSize(unsigned int size)
{
    if (m_characterSize != size)
    {
        m_characterSize = size;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setStyle(Uint32 style)
{
    if (m_style != style)
    {
        m_style = style;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void Text::setFillColor(const Color& color)
{
    if (color != m_fillColor)
    {
        m_fillColor = color;

        // Change vertex colors directly, no need to update whole geometry
        // (if geometry is updated anyway, we can skip this step)
        if (!m_geometryNeedUpdate)
        {
            for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i)
                m_vertices[i].color = m_fillColor;
        }
    }
}


////////////////////////////////////////////////////////////
void Text::setOutlineColor(const Color& color)
{
    if (color != m_outlineColor)
    {
        m_outlineColor = color;

        // Change vertex colors directly, no need to update whole geometry
        // (if geometry is updated anyway, we can skip this step)
        if (!m_geometryNeedUpdate)
        {
            for (std::size_t i = 0; i < m_outlineVertices.getVertexCount(); ++i)
                m_outlineVertices[i].color = m_outlineColor;
        }
    }
}


////////////////////////////////////////////////////////////
void Text::setOutlineThickness(float thickness)
{
    if (thickness != m_outlineThickness)
    {
        m_outlineThickness = thickness;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
const String& Text::getString() const
{
    return m_string;
}


////////////////////////////////////////////////////////////
const Font* Text::getFont() const
{
    return m_font;
}


////////////////////////////////////////////////////////////
unsigned int Text::getCharacterSize() const
{
    return m_characterSize;
}


////////////////////////////////////////////////////////////
Uint32 Text::getStyle() const
{
    return m_style;
}


////////////////////////////////////////////////////////////
const Color& Text::getFillColor() const
{
    return m_fillColor;
}


////////////////////////////////////////////////////////////
const Color& Text::getOutlineColor() const
{
    return m_outlineColor;
}


////////////////////////////////////////////////////////////
float Text::getOutlineThickness() const
{
    return m_outlineThickness;
}


////////////////////////////////////////////////////////////
Vector2f Text::findCharacterPosSystemFont(std::size_t index) const
{
    return Vector2f();
}


////////////////////////////////////////////////////////////
Vector2f Text::findCharacterPos(std::size_t index) const
{
    if (m_useSystemFont)
        return findCharacterPosSystemFont(index);

    // Make sure that we have a valid font
    if (!m_font)
        return Vector2f();

    // Adjust the index if it's out of range
    if (index > m_string.getSize())
        index = m_string.getSize();

    // Precompute the variables needed by the algorithm
    bool  bold   = (m_style & Bold) != 0;
    float hspace = static_cast<float>(m_font->getGlyph(L' ', m_characterSize, bold).advance);
    float vspace = static_cast<float>(m_font->getLineSpacing(m_characterSize));

    // Compute the position
    Vector2f position;
    Uint32 prevChar = 0;
    for (std::size_t i = 0; i < index; ++i)
    {
        Uint32 curChar = m_string[i];

        // Apply the kerning offset
        position.x += static_cast<float>(m_font->getKerning(prevChar, curChar, m_characterSize));
        prevChar = curChar;

        // Handle special characters
        switch (curChar)
        {
            case ' ':  position.x += hspace;                 continue;
            case '\t': position.x += hspace * 4;             continue;
            case '\n': position.y += vspace; position.x = 0; continue;
        }

        // For regular characters, add the advance offset of the glyph
        position.x += static_cast<float>(m_font->getGlyph(curChar, m_characterSize, bold).advance);
    }

    // Transform the position to global coordinates
    position = getTransform().transformPoint(position);

    return position;
}


////////////////////////////////////////////////////////////
FloatRect Text::getLocalBounds() const
{
    ensureGeometryUpdate();

    return m_bounds;
}


////////////////////////////////////////////////////////////
FloatRect Text::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


////////////////////////////////////////////////////////////
void Text::drawSystemFont(RenderTarget& target, RenderStates states) const
{
    ensureGeometryUpdate();
    states.transform *= getTransform();
#ifdef _3DS
    if (target.m_cache.viewChanged)
        target.applyCurrentView();
    if (states.blendMode != target.m_cache.lastBlendMode)
        target.applyBlendMode(states.blendMode);
    if (states.shader)
        target.applyShader(states.shader);

    target.applyTransform(states.transform);
    Mtx_Identity(MtxStack_Cur(CitroGetTextureMatrix()));
    CitroUpdateMatrixStacks();

    C3D_BufInfo* bufInfo = C3D_GetBufInfo();
    BufInfo_Init(bufInfo);
    BufInfo_Add(bufInfo, &m_vertices[0], sizeof(Vertex), 3, 0x210);

    C3D_TexEnv* env = C3D_GetTexEnv(0);
    C3D_TexEnvSrc(env, C3D_RGB, GPU_PRIMARY_COLOR, 0, 0);
    C3D_TexEnvSrc(env, C3D_Alpha, GPU_TEXTURE0, GPU_PRIMARY_COLOR, 0);
    C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
    C3D_TexEnvFunc(env, C3D_RGB, GPU_REPLACE);
    C3D_TexEnvFunc(env, C3D_Alpha, GPU_MODULATE);

    int vertexIndex = 0;
    int lastTextureIndex = -1;
    for (Uint16 textureIndex : m_systemGlyphTextures)
    {
        if (lastTextureIndex != textureIndex) {
            lastTextureIndex = textureIndex;
            C3D_TexBind(0, system_font_textures[textureIndex].getNativeTexture());
        }
        C3D_DrawArrays(GPU_TRIANGLE_STRIP, vertexIndex, 4);
        vertexIndex += 4;
    }
#endif
    target.applyTexture(NULL);
}


////////////////////////////////////////////////////////////
void Text::draw(RenderTarget& target, RenderStates states) const
{
    if (m_useSystemFont)
    {
        drawSystemFont(target, states);
    }
    else if (m_font)
    {
        ensureGeometryUpdate();

        states.transform *= getTransform();
        states.texture = &m_font->getTexture(m_characterSize);

        // Only draw the outline if there is something to draw
        if (m_outlineThickness != 0)
            target.draw(m_outlineVertices, states);

        target.draw(m_vertices, states);
    }
}


////////////////////////////////////////////////////////////
void Text::ensureGeometryUpdateSystemFont() const
{
#ifndef EMULATION
    m_systemGlyphTextures.clear();

    float maxX = 0.f;
    float x = 0.f;
    float y = 0.f;
    float scaleX = static_cast<float>(m_characterSize) / 25.f;
    float scaleY = scaleX;
    bool baseline = false;

    ssize_t  units;
    uint32_t code;

    auto str = m_string.toUtf8();
    const uint8_t* p = str.c_str();
    float firstX = x;
    int lastSheet = -1;
    int vertexIndex = 0;
    do
    {
        if (!*p) break;
        units = decode_utf8(&code, p);
        if (units == -1)
            break;
        p += units;
        if (code == '\n')
        {
            x = firstX;
            y += scaleY*fontGetInfo()->lineFeed;
        }
        else if (code > 0)
        {
            int glyphIdx = fontGlyphIndexFromCodePoint(code);
            fontGlyphPos_s data;
            fontCalcGlyphPos(&data, glyphIdx, GLYPH_POS_CALC_VTXCOORD, scaleX, scaleY);

            m_systemGlyphTextures.push_back(data.sheetIndex);

            m_vertices.append(Vertex(Vector2f(x+data.vtxcoord.left,  y+data.vtxcoord.bottom), m_fillColor, Vector2f(data.texcoord.left,  data.texcoord.bottom)));
            m_vertices.append(Vertex(Vector2f(x+data.vtxcoord.right, y+data.vtxcoord.bottom), m_fillColor, Vector2f(data.texcoord.right, data.texcoord.bottom)));
            m_vertices.append(Vertex(Vector2f(x+data.vtxcoord.left,  y+data.vtxcoord.top), m_fillColor, Vector2f(data.texcoord.left,  data.texcoord.top)));
            m_vertices.append(Vertex(Vector2f(x+data.vtxcoord.right, y+data.vtxcoord.top), m_fillColor, Vector2f(data.texcoord.right, data.texcoord.top)));

            x += data.xAdvance;
            if (x > maxX)
                maxX = x;
        }
    } while (code > 0);

    m_bounds.left = 0;
    m_bounds.top = 0;
    m_bounds.width = maxX;
    m_bounds.height = y + scaleY * fontGetInfo()->lineFeed;
#endif
}


////////////////////////////////////////////////////////////
void Text::ensureGeometryUpdate() const
{
	// Load system's opensans.tff if user attempts drawing without font
	if (!m_useSystemFont && m_font == &priv::system_font && !priv::system_font_loaded)
	{
		priv::system_font_loaded = true;
		priv::ResourceInfo font = priv::core_resources["opensans.ttf"];
		priv::system_font.loadFromMemory(font.data, font.size);
	}

    // Do nothing, if geometry has not changed
    if (!m_geometryNeedUpdate)
        return;

    // Mark geometry as updated
    m_geometryNeedUpdate = false;

    // Clear the previous geometry
    m_vertices.clear();
    m_outlineVertices.clear();
    m_bounds = FloatRect();

    // No font or text: nothing to draw
    if (!m_font || m_string.isEmpty())
        return;

    if (m_useSystemFont)
    {
        ensureGeometryUpdateSystemFont();
        return;
    }

    // Compute values related to the text style
    bool  bold               = (m_style & Bold) != 0;
    bool  underlined         = (m_style & Underlined) != 0;
    bool  strikeThrough      = (m_style & StrikeThrough) != 0;
    float italic             = (m_style & Italic) ? 0.208f : 0.f; // 12 degrees
    float underlineOffset    = m_font->getUnderlinePosition(m_characterSize);
    float underlineThickness = m_font->getUnderlineThickness(m_characterSize);

    // Compute the location of the strike through dynamically
    // We use the center point of the lowercase 'x' glyph as the reference
    // We reuse the underline thickness as the thickness of the strike through as well
    FloatRect xBounds = m_font->getGlyph(L'x', m_characterSize, bold).bounds;
    float strikeThroughOffset = xBounds.top + xBounds.height / 2.f;

    // Precompute the variables needed by the algorithm
    float hspace = static_cast<float>(m_font->getGlyph(L' ', m_characterSize, bold).advance);
    float vspace = static_cast<float>(m_font->getLineSpacing(m_characterSize));
    float x      = 0.f;
    float y      = static_cast<float>(m_characterSize);

    // Create one quad for each character
    float minX = static_cast<float>(m_characterSize);
    float minY = static_cast<float>(m_characterSize);
    float maxX = 0.f;
    float maxY = 0.f;
    Uint32 prevChar = 0;
    for (std::size_t i = 0; i < m_string.getSize(); ++i)
    {
        Uint32 curChar = m_string[i];

        // Apply the kerning offset
        x += m_font->getKerning(prevChar, curChar, m_characterSize);
        prevChar = curChar;

        // If we're using the underlined style and there's a new line, draw a line
        if (underlined && (curChar == L'\n'))
        {
            addLine(m_vertices, x, y, m_fillColor, underlineOffset, underlineThickness);

            if (m_outlineThickness != 0)
                addLine(m_outlineVertices, x, y, m_outlineColor, underlineOffset, underlineThickness, m_outlineThickness);
        }

        // If we're using the strike through style and there's a new line, draw a line across all characters
        if (strikeThrough && (curChar == L'\n'))
        {
            addLine(m_vertices, x, y, m_fillColor, strikeThroughOffset, underlineThickness);

            if (m_outlineThickness != 0)
                addLine(m_outlineVertices, x, y, m_outlineColor, strikeThroughOffset, underlineThickness, m_outlineThickness);
        }

        // Handle special characters
        if ((curChar == ' ') || (curChar == '\t') || (curChar == '\n'))
        {
            // Update the current bounds (min coordinates)
            minX = std::min(minX, x);
            minY = std::min(minY, y);

            switch (curChar)
            {
                case ' ':  x += hspace;        break;
                case '\t': x += hspace * 4;    break;
                case '\n': y += vspace; x = 0; break;
            }

            // Update the current bounds (max coordinates)
            maxX = std::max(maxX, x);
            maxY = std::max(maxY, y);

            // Next glyph, no need to create a quad for whitespace
            continue;
        }


        // Apply the outline
        if (m_outlineThickness != 0)
        {
            const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, bold, m_outlineThickness);

            float left   = glyph.bounds.left;
            float top    = glyph.bounds.top;
            float right  = glyph.bounds.left + glyph.bounds.width;
            float bottom = glyph.bounds.top  + glyph.bounds.height;

            // Add the outline glyph to the vertices
            addGlyphQuad(m_outlineVertices, Vector2f(x, y), m_outlineColor, glyph, italic, m_outlineThickness);

            // Update the current bounds with the outlined glyph bounds
            minX = std::min(minX, x + left   - italic * bottom - m_outlineThickness);
            maxX = std::max(maxX, x + right  - italic * top    - m_outlineThickness);
            minY = std::min(minY, y + top    - m_outlineThickness);
            maxY = std::max(maxY, y + bottom - m_outlineThickness);
        }

        // Extract the current glyph's description
        const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, bold);

        // Add the glyph to the vertices
        addGlyphQuad(m_vertices, Vector2f(x, y), m_fillColor, glyph, italic);

        // Update the current bounds with the non outlined glyph bounds
        if (m_outlineThickness == 0)
        {
            float left   = glyph.bounds.left;
            float top    = glyph.bounds.top;
            float right  = glyph.bounds.left + glyph.bounds.width;
            float bottom = glyph.bounds.top  + glyph.bounds.height;

            minX = std::min(minX, x + left  - italic * bottom);
            maxX = std::max(maxX, x + right - italic * top);
            minY = std::min(minY, y + top);
            maxY = std::max(maxY, y + bottom);
        }

        // Advance to the next character
        x += glyph.advance;
    }

    // If we're using the underlined style, add the last line
    if (underlined && (x > 0))
    {
        addLine(m_vertices, x, y, m_fillColor, underlineOffset, underlineThickness);

        if (m_outlineThickness != 0)
            addLine(m_outlineVertices, x, y, m_outlineColor, underlineOffset, underlineThickness, m_outlineThickness);
    }

    // If we're using the strike through style, add the last line across all characters
    if (strikeThrough && (x > 0))
    {
        addLine(m_vertices, x, y, m_fillColor, strikeThroughOffset, underlineThickness);

        if (m_outlineThickness != 0)
            addLine(m_outlineVertices, x, y, m_outlineColor, strikeThroughOffset, underlineThickness, m_outlineThickness);
    }

    // Update the bounding rectangle
    m_bounds.left = minX;
    m_bounds.top = minY;
    m_bounds.width = maxX - minX;
    m_bounds.height = maxY - minY;
}

} // namespace cpp3ds