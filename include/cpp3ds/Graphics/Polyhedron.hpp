#ifndef CPP3DS_POLYHEDRON_HPP
#define CPP3DS_POLYHEDRON_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/Drawable.hpp>
#include <cpp3ds/Graphics/Transformable.hpp>
#include <cpp3ds/Graphics/VertexContainer.hpp>
#include <cpp3ds/Graphics/Box.hpp>
#include <cpp3ds/System/Vector3.hpp>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
/// \brief Base class for textured polyhedra
///
////////////////////////////////////////////////////////////
class Polyhedron : public Drawable, public Transformable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Holds the 3 vertices that make up a face
    ///
    ////////////////////////////////////////////////////////////
    struct Face
    {
        Vertex v0; ///< The first vertex of the face
        Vertex v1; ///< The second vertex of the face
        Vertex v2; ///< The third vertex of the face
    };

    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Polyhedron();

    ////////////////////////////////////////////////////////////
    /// \brief Set the color of the polyhedron
    ///
    /// This color is modulated (multiplied) with the polyhedron's
    /// texture if any. It can be used to colorize the polyhedron,
    /// or change its global opacity.
    /// By default, the polyhedron's color is opaque white.
    ///
    /// \param color New color of the polyhedron
    ///
    /// \see getColor
    ///
    ////////////////////////////////////////////////////////////
    void setColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Get the color of the polyhedron
    ///
    /// \return Color of the polyhedron
    ///
    /// \see setColor
    ///
    ////////////////////////////////////////////////////////////
    const Color& getColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the source texture of the polyhedron
    ///
    /// The \a texture argument refers to a texture that must
    /// exist as long as the polyhedron uses it. Indeed, the polyhedron
    /// doesn't store its own copy of the texture, but rather keeps
    /// a pointer to the one that you passed to this function.
    /// If the source texture is destroyed and the polyhedron tries to
    /// use it, the behaviour is undefined.
    /// \a texture can be NULL to disable texturing.
    ///
    /// \param texture New texture
    ///
    /// \see getTexture
    ///
    ////////////////////////////////////////////////////////////
    void setTexture(const Texture* texture);

    ////////////////////////////////////////////////////////////
    /// \brief Get the source texture of the polyhedron
    ///
    /// If the polyhedron has no source texture, a NULL pointer is returned.
    /// The returned pointer is const, which means that you can't
    /// modify the texture when you retrieve it with this function.
    ///
    /// \return Pointer to the polyhedron's texture
    ///
    /// \see setTexture
    ///
    ////////////////////////////////////////////////////////////
    const Texture* getTexture() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the total number of faces of the polyhedron
    ///
    /// \return Number of faces of the polyhedron
    ///
    /// \see getFace
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int getFaceCount() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get a face of the polyhedron
    ///
    /// The result is undefined if \a index is out of the valid range.
    ///
    /// \param index Index of the face to get, in range [0 .. getFaceCount() - 1]
    ///
    /// \return Index-th face of the polyhedron
    ///
    /// \see getFaceCount
    ///
    ////////////////////////////////////////////////////////////
    virtual Face getFace(unsigned int index) const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the local bounding box of the entity
    ///
    /// The returned box is in local coordinates, which means
    /// that it ignores the transformations (translation, rotation,
    /// scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// entity in the entity's coordinate system.
    ///
    /// \return Local bounding box of the entity
    ///
    ////////////////////////////////////////////////////////////
    FloatBox getLocalBounds() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the global bounding box of the entity
    ///
    /// The returned box is in global coordinates, which means
    /// that it takes in account the transformations (translation,
    /// rotation, scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// object in the global 3D world's coordinate system.
    ///
    /// \return Global bounding box of the entity
    ///
    ////////////////////////////////////////////////////////////
    FloatBox getGlobalBounds() const;

    ////////////////////////////////////////////////////////////
    /// \brief Generate normals using face data
    ///
    /// In the case that the input data doesn't contain
    /// vertex normals, this method will generate them
    /// through a simple cross-product using 2 edges of a face.
    ///
    /// This will generate the same normal for all 3 vertices
    /// of a face, so if smooth lighting or per-pixel lighting
    /// is required, you still need to specify your own normal
    /// data through other means.
    ///
    ////////////////////////////////////////////////////////////
    virtual void generateNormals();

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Polyhedron();

    ////////////////////////////////////////////////////////////
    /// \brief Recompute the internal geometry of the polyhedron
    ///
    /// This function must be called by the derived class everytime
    /// the polyhedron's faces change (ie. the result of either
    /// getFaceCount or getFace is different or the vertex data
    /// has been modified).
    ///
    ////////////////////////////////////////////////////////////
    void update() const;

private :

    ////////////////////////////////////////////////////////////
    /// \brief Draw the polyhedron to a render target
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(RenderTarget& target, RenderStates states) const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the vertices' color
    ///
    ////////////////////////////////////////////////////////////
    void updateColors();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    const Texture*          m_texture;      ///< Texture of the polyhedron
    Color                   m_color;        ///< Color
    mutable VertexContainer m_vertices;     ///< Vertex array containing the geometry
    mutable FloatBox        m_insideBounds; ///< Bounding rectangle of the inside (fill)
};

} // namespace cpp3ds


#endif // CPP3DS_POLYHEDRON_HPP


////////////////////////////////////////////////////////////
/// \class cpp3ds::Polyhedron
/// \ingroup graphics
///
/// cpp3ds::Polyhedron is a drawable class that allows to define and
/// display a custom polyhedron on a render target.
/// It's only an abstract base, it needs to be specialized for
/// concrete types of polyhedra (sphere, cuboid, convex polyhedron, ...).
///
/// In addition to the attributes provided by the specialized
/// polyhedron classes, a shape always has the following attributes:
/// \li a texture
/// \li a color
///
/// Each feature is optional, and can be disabled easily:
/// \li the texture can be null
/// \li the color can be cpp3ds::Color::Transparent
///
/// You can write your own derived polyhedron class, there are only
/// two virtual functions to override:
/// \li getFaceCount must return the number of faces in the polyhedron
/// \li getFace must return the faces of the polyhedron
///
/// \see cpp3ds::Cuboid, cpp3ds::SphericalPolyhedron, cpp3ds::ConvexPolyhedron, cpp3ds::Transformable
///
////////////////////////////////////////////////////////////
