////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/Billboard.hpp>
#include <cpp3ds/Graphics/Camera.hpp>
#include <cmath>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Billboard::Billboard() :
m_camera(NULL)
{
}


////////////////////////////////////////////////////////////
Billboard::Billboard(const Texture& texture) :
Sprite  (texture),
m_camera(NULL)
{
}


////////////////////////////////////////////////////////////
Billboard::Billboard(const Texture& texture, const IntRect& rectangle) :
Sprite  (texture, rectangle),
m_camera(NULL)
{
}


////////////////////////////////////////////////////////////
void Billboard::draw(RenderTarget& target, RenderStates states) const
{
    if (m_camera)
    {
        Vector3f pos = getPosition();

        // Construct the basis of our rotation matrix
        Vector3f zAxis = m_camera->getPosition() - pos;
        float zAxisNorm = std::sqrt(zAxis.x * zAxis.x +
                                    zAxis.y * zAxis.y +
                                    zAxis.z * zAxis.z);
        zAxis /= zAxisNorm;

        Vector3f xAxis(zAxis.z, 0.f, -zAxis.x);
        float xAxisNorm = std::sqrt(xAxis.x * xAxis.x +
                                    xAxis.y * xAxis.y +
                                    xAxis.z * xAxis.z);
        xAxis /= xAxisNorm;

        Vector3f yAxis(zAxis.y * xAxis.z - zAxis.z * xAxis.y,
                       zAxis.z * xAxis.x - zAxis.x * xAxis.z,
                       zAxis.x * xAxis.y - zAxis.y * xAxis.x);

        // No need to normalize y axis, x and z orthogonal

        // Combine a translation, rotation and translation into 1 transform
        pos -= xAxis * pos.x + yAxis * pos.y + zAxis * pos.z;

        Transform billboardTransform(xAxis.x, yAxis.x, zAxis.x, pos.x,
                                     xAxis.y, yAxis.y, zAxis.y, pos.y,
                                     xAxis.z, yAxis.z, zAxis.z, pos.z,
                                     0.f,     0.f,     0.f,     1.f);

        states.transform *= billboardTransform;
    }

    Sprite::draw(target, states);
}


////////////////////////////////////////////////////////////
void Billboard::setCamera(const Camera& camera)
{
    m_camera = &camera;
}


////////////////////////////////////////////////////////////
const Camera* Billboard::getCamera() const
{
    return m_camera;
}

} // namespace cpp3ds
