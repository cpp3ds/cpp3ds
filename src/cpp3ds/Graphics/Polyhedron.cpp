////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/Polyhedron.hpp>
#include <cpp3ds/Graphics/RenderTarget.hpp>
#include <cpp3ds/Graphics/Texture.hpp>
#include <cpp3ds/System/Err.hpp>
#include <cmath>


namespace
{
    // Compute the normal of a face given 2 of its edges
    cpp3ds::Vector3f computeNormal(const cpp3ds::Vector3f& v1, const cpp3ds::Vector3f& v2)
    {
        cpp3ds::Vector3f normal(v1.y * v2.z - v1.z * v2.y,
                            v1.z * v2.x - v1.x * v2.z,
                            v1.x * v2.y - v1.y * v2.x);
        float length = std::sqrt(normal.x * normal.x +
                                 normal.y * normal.y +
                                 normal.z * normal.z);
        if (length != 0.f)
            normal /= length;
        return normal;
    }
}


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Polyhedron::~Polyhedron()
{
}


////////////////////////////////////////////////////////////
void Polyhedron::setColor(const Color& color)
{
    m_color = color;

    updateColors();
}


////////////////////////////////////////////////////////////
const Color& Polyhedron::getColor() const
{
    return m_color;
}


////////////////////////////////////////////////////////////
void Polyhedron::setTexture(const Texture* texture)
{
    m_texture = texture;
}


////////////////////////////////////////////////////////////
const Texture* Polyhedron::getTexture() const
{
    return m_texture;
}


////////////////////////////////////////////////////////////
FloatBox Polyhedron::getLocalBounds() const
{
    return m_insideBounds;
}


////////////////////////////////////////////////////////////
FloatBox Polyhedron::getGlobalBounds() const
{
    return getTransform().transformBox(getLocalBounds());
}


////////////////////////////////////////////////////////////
Polyhedron::Polyhedron() :
m_texture     (NULL),
m_color       (Color::White),
m_vertices    (Triangles),
m_insideBounds()
{
}


////////////////////////////////////////////////////////////
void Polyhedron::update() const
{
    // Get the total number of faces of the polyhedron
    unsigned int count = getFaceCount();
    if (!count)
    {
        m_vertices.resize(0);
        return;
    }

    m_vertices.resize(count * 3);

    // Vertices
    for (unsigned int i = 0; i < count; ++i)
    {
        Face face = getFace(i);

        m_vertices[i * 3 + 0] = face.v0;
        m_vertices[i * 3 + 1] = face.v1;
        m_vertices[i * 3 + 2] = face.v2;
    }

    // Update the bounding rectangle
    m_insideBounds = m_vertices.getBounds();
}


////////////////////////////////////////////////////////////
void Polyhedron::draw(RenderTarget& target, RenderStates states) const
{
    states.transform *= getTransform();

    // Render the inside
    states.texture = m_texture;
    target.draw(m_vertices, states);
}


////////////////////////////////////////////////////////////
void Polyhedron::generateNormals()
{
    // Get the total number of faces of the polyhedron
    unsigned int count = m_vertices.getVertexCount() / 3;

    // Vertices
    for (unsigned int i = 0; i < count; ++i)
    {
        Vector3f normal = computeNormal(m_vertices[i * 3 + 2].position - m_vertices[i * 3 + 1].position,
                                        m_vertices[i * 3 + 0].position - m_vertices[i * 3 + 1].position);

        m_vertices[i * 3 + 0].normal = normal;
        m_vertices[i * 3 + 1].normal = normal;
        m_vertices[i * 3 + 2].normal = normal;
    }
}


////////////////////////////////////////////////////////////
void Polyhedron::updateColors()
{
    for (unsigned int i = 0; i < m_vertices.getVertexCount(); ++i)
        m_vertices[i].color = m_color;
}

} // namespace cpp3ds
