////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/SphericalPolyhedron.hpp>
#include <cmath>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
SphericalPolyhedron::SphericalPolyhedron(float radius, unsigned int subdivisions) :
m_radius      (radius),
m_subdivisions(subdivisions)
{
    update();
}


////////////////////////////////////////////////////////////
void SphericalPolyhedron::setRadius(float radius)
{
    m_radius = radius;

    std::vector<Vertex>().swap(m_geometry);

    update();
}


////////////////////////////////////////////////////////////
float SphericalPolyhedron::getRadius() const
{
    return m_radius;
}


////////////////////////////////////////////////////////////
void SphericalPolyhedron::setSubdivisions(unsigned int subdivisions)
{
    m_subdivisions = subdivisions;

    std::vector<Vertex>().swap(m_geometry);

    update();
}


////////////////////////////////////////////////////////////
unsigned int SphericalPolyhedron::getSubdivisions() const
{
    return m_subdivisions;
}

////////////////////////////////////////////////////////////
unsigned int SphericalPolyhedron::getFaceCount() const
{
    if (m_geometry.empty())
        construct();

    return static_cast<unsigned int>(m_geometry.size()) / 3;
}


////////////////////////////////////////////////////////////
Polyhedron::Face SphericalPolyhedron::getFace(unsigned int index) const
{
    if (m_geometry.empty())
        construct();

    Face face = {m_geometry[index * 3 + 0],
                 m_geometry[index * 3 + 1],
                 m_geometry[index * 3 + 2]};

    // This is probably the last time we will need the
    // geometry for a while so clear it to save memory.
    if (index == (m_geometry.size() / 3 - 1))
        std::vector<Vertex>().swap(m_geometry);

    return face;
}


////////////////////////////////////////////////////////////
void SphericalPolyhedron::construct() const
{
    // Icosahedron radii
    static const float a = 0.525731112119133606f;
    static const float b = 0.850650808352039932f;

    static Vector3f vertices[] =
    {
        Vector3f( a,  0, -b),
        Vector3f(-a,  0, -b),
        Vector3f( a,  0,  b),
        Vector3f(-a,  0,  b),
        Vector3f( 0, -b, -a),
        Vector3f( 0, -b,  a),
        Vector3f( 0,  b, -a),
        Vector3f( 0,  b,  a),
        Vector3f(-b, -a,  0),
        Vector3f( b, -a,  0),
        Vector3f(-b,  a,  0),
        Vector3f( b,  a,  0)
    };

    static unsigned int indices[] =
    {
        0,  1,  6,
        0,  4,  1,
        0,  9,  4,
        2,  7,  3,
        4,  5,  8,
        4,  8,  1,
        5,  2,  3,
        5,  3,  8,
        6,  1,  10,
        7,  2,  11,
        7,  6,  10,
        7,  10, 3,
        7,  11, 6,
        8,  3,  10,
        8,  10, 1,
        9,  0,  11,
        9,  2,  5,
        9,  5,  4,
        9,  11, 2,
        11, 0,  6
    };

    m_geometry.clear();
    m_geometry.reserve(60 * (1 << (m_subdivisions * 2)));

    for (int i = 0; i < 20; ++i)
        subdivide(vertices[indices[i * 3 + 0]], vertices[indices[i * 3 + 1]], vertices[indices[i * 3 + 2]], m_subdivisions);
}


////////////////////////////////////////////////////////////
void SphericalPolyhedron::subdivide(const Vector3f& a, const Vector3f& b, const Vector3f& c, unsigned int s) const
{
    if (!s)
    {
        Vertex va(a * m_radius);
        va.color = getColor();
        va.normal = a;
        Vertex vb(b * m_radius);
        vb.color = getColor();
        vb.normal = b;
        Vertex vc(c * m_radius);
        vc.color = getColor();
        vc.normal = c;

        m_geometry.push_back(va);
        m_geometry.push_back(vb);
        m_geometry.push_back(vc);
    }
    else
    {
        Vector3f ab((a + b) / 2.f);
        Vector3f bc((b + c) / 2.f);
        Vector3f ca((c + a) / 2.f);

        // Normalize all positions
        ab /= std::sqrt(ab.x * ab.x + ab.y * ab.y + ab.z * ab.z);
        bc /= std::sqrt(bc.x * bc.x + bc.y * bc.y + bc.z * bc.z);
        ca /= std::sqrt(ca.x * ca.x + ca.y * ca.y + ca.z * ca.z);

        subdivide(a,  ab, ca, s - 1);
        subdivide(b,  bc, ab, s - 1);
        subdivide(c,  ca, bc, s - 1);
        subdivide(ab, bc, ca, s - 1);
    }
}

} // namespace cpp3ds
