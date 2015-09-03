////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/Cuboid.hpp>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Cuboid::Cuboid(const Vector3f& size)
{
    setSize(size);
}


////////////////////////////////////////////////////////////
void Cuboid::setSize(const Vector3f& size)
{
    m_size = size;
    update();
    generateNormals();
}


////////////////////////////////////////////////////////////
const Vector3f& Cuboid::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
unsigned int Cuboid::getFaceCount() const
{
    return 12;
}


////////////////////////////////////////////////////////////
Polyhedron::Face Cuboid::getFace(unsigned int index) const
{
    float left   = m_size.x / -2.f;
    float top    = m_size.y /  2.f;
    float front  = m_size.z /  2.f;
    float right  = m_size.x /  2.f;
    float bottom = m_size.y / -2.f;
    float back   = m_size.z / -2.f;

    const Color& color = getColor();

    switch (index)
    {
        default:
        // Front 1
        case 0:  { Face face = {Vertex(Vector3f(left, top, front), color), Vertex(Vector3f(left, bottom, front), color), Vertex(Vector3f(right, bottom, front), color)}; return face; }
        // Front 2
        case 1:  { Face face = {Vertex(Vector3f(left, top, front), color), Vertex(Vector3f(right, bottom, front), color), Vertex(Vector3f(right, top, front), color)}; return face; }
        // Right 1
        case 2:  { Face face = {Vertex(Vector3f(right, top, front), color), Vertex(Vector3f(right, bottom, front), color), Vertex(Vector3f(right, bottom, back), color)}; return face; }
        // Right 2
        case 3:  { Face face = {Vertex(Vector3f(right, top, front), color), Vertex(Vector3f(right, bottom, back), color), Vertex(Vector3f(right, top, back), color)}; return face; }
        // Back 1
        case 4:  { Face face = {Vertex(Vector3f(right, top, back), color), Vertex(Vector3f(right, bottom, back), color), Vertex(Vector3f(left, bottom, back), color)}; return face; }
        // Back 2
        case 5:  { Face face = {Vertex(Vector3f(right, top, back), color), Vertex(Vector3f(left, bottom, back), color), Vertex(Vector3f(left, top, back), color)}; return face; }
        // Left 1
        case 6:  { Face face = {Vertex(Vector3f(left, top, back), color), Vertex(Vector3f(left, bottom, back), color), Vertex(Vector3f(left, bottom, front), color)}; return face; }
        // Left 2
        case 7:  { Face face = {Vertex(Vector3f(left, top, back), color), Vertex(Vector3f(left, bottom, front), color), Vertex(Vector3f(left, top, front), color)}; return face; }
        // Top 1
        case 8:  { Face face = {Vertex(Vector3f(left, top, back), color), Vertex(Vector3f(left, top, front), color), Vertex(Vector3f(right, top, front), color)}; return face; }
        // Top 2
        case 9:  { Face face = {Vertex(Vector3f(left, top, back), color), Vertex(Vector3f(right, top, front), color), Vertex(Vector3f(right, top, back), color)}; return face; }
        // Bottom 1
        case 10: { Face face = {Vertex(Vector3f(left, bottom, front), color), Vertex(Vector3f(left, bottom, back), color), Vertex(Vector3f(right, bottom, back), color)}; return face; }
        // Bottom 2
        case 11: { Face face = {Vertex(Vector3f(left, bottom, front), color), Vertex(Vector3f(right, bottom, back), color), Vertex(Vector3f(right, bottom, front), color)}; return face; }
    }
}

} // namespace cpp3ds
