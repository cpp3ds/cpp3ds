#ifndef CPP3DS_SPHERICALPOLYHEDRON_HPP
#define CPP3DS_SPHERICALPOLYHEDRON_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/Polyhedron.hpp>
#include <vector>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
/// \brief Specialized polyhedron representing a spherical polyhedron
///
////////////////////////////////////////////////////////////
class SphericalPolyhedron : public Polyhedron
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates a spherical polyhedron positioned at (0, 0, 0)
    /// with radius 0 and 5 subdivisions.
    ///
    /// \param radius       Radius of the spherical polyhedron
    /// \param subdivisions Number of times to subdivide faces of the base geometry
    ///
    ////////////////////////////////////////////////////////////
    explicit SphericalPolyhedron(float radius = 0, unsigned int subdivisions = 5);

    ////////////////////////////////////////////////////////////
    /// \brief Set the radius of the spherical polyhedron
    ///
    /// \param radius New radius of the spherical polyhedron
    ///
    /// \see getRadius
    ///
    ////////////////////////////////////////////////////////////
    void setRadius(float radius);

    ////////////////////////////////////////////////////////////
    /// \brief Get the radius of the spherical polyhedron
    ///
    /// \return Radius of the spherical polyhedron
    ///
    /// \see setRadius
    ///
    ////////////////////////////////////////////////////////////
    float getRadius() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the number of times the base polyhedron is subdivided
    ///
    /// \param subdivisions Number of times the base polyhedron is subdivided
    ///
    /// \see getSubdivisions
    ///
    ////////////////////////////////////////////////////////////
    void setSubdivisions(unsigned int subdivisions);

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of times the base polyhedron is subdivided
    ///
    /// \return The number of times the base polyhedron is subdivided
    ///
    /// \see setSubdivisions
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getSubdivisions() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of faces of the polyhedron
    ///
    /// \return Number of faces of the polyhedron
    ///
    /// \see setFaceCount
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int getFaceCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a face of the polyhedron
    ///
    /// The result is undefined if \a index is out of the valid range.
    ///
    /// \param index Index of the face to get, in range [0 .. getFaceCount() - 1]
    ///
    /// \return Index-th face of the polyhedron
    ///
    ////////////////////////////////////////////////////////////
    virtual Face getFace(unsigned int index) const;

private :

    ////////////////////////////////////////////////////////////
    /// \brief Construct the geometry from the base polyhedron and given subdivisions
    ///
    ////////////////////////////////////////////////////////////
    void construct() const;

    ////////////////////////////////////////////////////////////
    /// \brief Subdivide a face of the current geometry
    ///
    /// \param a First corner position
    /// \param b Second corner position
    /// \param c Third corner position
    /// \param s Current subdivision
    ///
    ////////////////////////////////////////////////////////////
    void subdivide(const Vector3f& a, const Vector3f& b, const Vector3f& c, unsigned int s) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    float        m_radius;       ///< Radius of the spherical polyhedron
    unsigned int m_subdivisions; ///< Radius of the spherical polyhedron

    mutable std::vector<Vertex> m_geometry; ///< Constructed geometry
};

} // namespace cpp3ds


#endif // CPP3DS_SPHERICALPOLYHEDRON_HPP


////////////////////////////////////////////////////////////
/// \class cpp3ds::SphericalPolyhedron
/// \ingroup graphics
///
/// This class inherits all the functions of cpp3ds::Transformable
/// (position, rotation, scale, bounds, ...) as well as the
/// functions of cpp3ds::Polyhedron (color, texture, ...).
///
/// Usage example:
/// \code
/// cpp3ds::SphericalPolyhedron sphere;
/// sphere.setRadius(150);
/// sphere.setColor(cpp3ds::Color::Red);
/// sphere.setPosition(10, 20, 30);
/// ...
/// window.draw(sphere);
/// \endcode
///
/// Since the graphics card can't draw perfect spheres, we have to
/// fake them through tessellation of a base icosahedron. The
/// "subdivisions" property of cpp3ds::SphericalPolyhedron defines how many
/// subdivisions to perform on the faces of the base primitive,
/// and therefore defines the quality of the sphere.
///
/// \see cpp3ds::Polyhedron, cpp3ds::Cuboid, cpp3ds::ConvexPolyhedron
///
////////////////////////////////////////////////////////////
