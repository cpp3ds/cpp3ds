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

#ifndef CPP3DS_VERTEX_HPP
#define CPP3DS_VERTEX_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/Color.hpp>
#include <cpp3ds/System/Vector2.hpp>

namespace cpp3ds
{
////////////////////////////////////////////////////////////
/// \brief Define a point with color and texture coordinates
///
////////////////////////////////////////////////////////////
class Vertex
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Vertex();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the vertex from its position
    ///
    /// The vertex color is white and texture coordinates are (0, 0).
    ///
    /// \param thePosition Vertex position
    ///
    ////////////////////////////////////////////////////////////
    Vertex(const Vector2f& thePosition);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the vertex from its position and color
    ///
    /// The texture coordinates are (0, 0).
    ///
    /// \param thePosition Vertex position
    /// \param theColor    Vertex color
    ///
    ////////////////////////////////////////////////////////////
    Vertex(const Vector2f& thePosition, const Color& theColor);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the vertex from its position and texture coordinates
    ///
    /// The vertex color is white.
    ///
    /// \param thePosition  Vertex position
    /// \param theTexCoords Vertex texture coordinates
    ///
    ////////////////////////////////////////////////////////////
    Vertex(const Vector2f& thePosition, const Vector2f& theTexCoords);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the vertex from its position, color and texture coordinates
    ///
    /// \param thePosition  Vertex position
    /// \param theColor     Vertex color
    /// \param theTexCoords Vertex texture coordinates
    ///
    ////////////////////////////////////////////////////////////
    Vertex(const Vector2f& thePosition, const Color& theColor, const Vector2f& theTexCoords);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2f  position;  ///< 2D position of the vertex
    Color     color;     ///< Color of the vertex
    Vector2f  texCoords; ///< Coordinates of the texture's pixel to map to the vertex
};

}


#endif


////////////////////////////////////////////////////////////
/// \class cpp3ds::Vertex
/// \ingroup graphics
///
/// A vertex is an improved point. It has a position and other
/// extra attributes that will be used for drawing: in SFML,
/// vertices also have a color and a pair of texture coordinates.
///
/// The vertex is the building block of drawing. Everything which
/// is visible on screen is made of vertices. They are grouped
/// as 2D primitives (triangles, quads, ...), and these primitives
/// are grouped to create even more complex 2D entities such as
/// sprites, texts, etc.
///
/// If you use the graphical entities of SFML (sprite, text, shape)
/// you won't have to deal with vertices directly. But if you want
/// to define your own 2D entities, such as tiled maps or particle
/// systems, using vertices will allow you to get maximum performances.
///
/// Example:
/// \code
/// // define a 100x100 square, red, with a 10x10 texture mapped on it
/// cpp3ds::Vertex vertices[] =
/// {
///     cpp3ds::Vertex(cpp3ds::Vector2f(  0,   0), cpp3ds::Color::Red, cpp3ds::Vector2f( 0,  0)),
///     cpp3ds::Vertex(cpp3ds::Vector2f(  0, 100), cpp3ds::Color::Red, cpp3ds::Vector2f( 0, 10)),
///     cpp3ds::Vertex(cpp3ds::Vector2f(100, 100), cpp3ds::Color::Red, cpp3ds::Vector2f(10, 10)),
///     cpp3ds::Vertex(cpp3ds::Vector2f(100,   0), cpp3ds::Color::Red, cpp3ds::Vector2f(10,  0))
/// };
///
/// // draw it
/// window.draw(vertices, 4, cpp3ds::Quads);
/// \endcode
///
/// Note: although texture coordinates are supposed to be an integer
/// amount of pixels, their type is float because of some buggy graphics
/// drivers that are not able to process integer coordinates correctly.
///
/// \see cpp3ds::VertexArray
///
////////////////////////////////////////////////////////////
