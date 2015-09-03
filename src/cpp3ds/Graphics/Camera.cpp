////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/Camera.hpp>
#include <cmath>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Camera::Camera(float fov, float near, float far) :
m_fieldOfView(fov),
m_nearPlane  (near),
m_farPlane   (far),
m_direction  (0, 0, -1),
m_upVector   (0, 1, 0),
m_scale      (1, 1, 1)
{
    setPosition(0, 0, 0);
}


////////////////////////////////////////////////////////////
Camera::~Camera()
{

}


////////////////////////////////////////////////////////////
void Camera::setFieldOfView(float fov)
{
    m_fieldOfView = fov;
    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
float Camera::getFieldOfView() const
{
    return m_fieldOfView;
}


////////////////////////////////////////////////////////////
void Camera::setNearClippingPlane(float distance)
{
    m_nearPlane = distance;
    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
float Camera::getNearClippingPlane() const
{
    return m_nearPlane;
}


////////////////////////////////////////////////////////////
void Camera::setFarClippingPlane(float distance)
{
    m_farPlane = distance;
    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
float Camera::getFarClippingPlane() const
{
    return m_farPlane;
}


////////////////////////////////////////////////////////////
void Camera::setPosition(float x, float y, float z)
{
    setPosition(Vector3f(x, y, z));
}


////////////////////////////////////////////////////////////
void Camera::setPosition(const Vector3f& position)
{
    setCenter(position);
}


////////////////////////////////////////////////////////////
const Vector3f& Camera::getPosition() const
{
    return getCenter();
}


////////////////////////////////////////////////////////////
void Camera::setDirection(float x, float y, float z)
{
    setDirection(Vector3f(x, y, z));
}


////////////////////////////////////////////////////////////
void Camera::setDirection(const Vector3f& direction)
{
    m_direction = direction;
    m_viewTransformUpdated    = false;
    m_invViewTransformUpdated = false;
}


////////////////////////////////////////////////////////////
const Vector3f& Camera::getDirection() const
{
    return m_direction;
}


////////////////////////////////////////////////////////////
void Camera::setUpVector(float x, float y, float z)
{
    setUpVector(Vector3f(x, y, z));
}


////////////////////////////////////////////////////////////
void Camera::setUpVector(const Vector3f& upVector)
{
    m_upVector = upVector;
    m_viewTransformUpdated    = false;
    m_invViewTransformUpdated = false;
}


////////////////////////////////////////////////////////////
const Vector3f& Camera::getUpVector() const
{
    return m_upVector;
}


////////////////////////////////////////////////////////////
void Camera::setScale(float factorX, float factorY, float factorZ)
{
    setScale(Vector3f(factorX, factorY, factorZ));
}


////////////////////////////////////////////////////////////
void Camera::setScale(const Vector3f& factors)
{
    m_scale = factors;
    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
const Vector3f& Camera::getScale() const
{
    return m_scale;
}


////////////////////////////////////////////////////////////
void Camera::scale(float factorX, float factorY, float factorZ)
{
    m_scale.x *= factorX;
    m_scale.y *= factorY;
    m_scale.z *= factorZ;
    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void Camera::scale(const Vector3f& factor)
{
    scale(factor.x, factor.y, factor.z);
}


////////////////////////////////////////////////////////////
const Transform& Camera::getTransform() const
{
    // Recompute the perspective projection matrix if needed
    if (!m_transformUpdated)
    {
        float radians = m_fieldOfView / 2.f * 3.141592654f / 180.f;

        // Projection components
        float f = 1.f / std::tan(radians / 2.f);
        float g = (m_farPlane + m_nearPlane) / (m_nearPlane - m_farPlane);
        float h = 2.f * (m_farPlane * m_nearPlane) / (m_nearPlane - m_farPlane);

        // Rebuild the projection matrix
        m_transform = Transform(m_scale.x * f, 0.f,            0.f,           0.f,
                                0.f,           m_scale.y * f,  0.f,           0.f,
                                0.f,           0.f,            m_scale.z * g, h,
                                0.f,           0.f,           -m_scale.z,     0.f);

        m_transformUpdated = true;
    }

    return m_transform;
}


////////////////////////////////////////////////////////////
const Transform& Camera::getViewTransform() const
{
    // Recompute the view matrix if needed
    if (!m_viewTransformUpdated)
    {
        // View components
        float directionNorm = std::sqrt(m_direction.x * m_direction.x +
                                        m_direction.y * m_direction.y +
                                        m_direction.z * m_direction.z);
        Vector3f d = m_direction / directionNorm;

        float upNorm = std::sqrt(m_upVector.x * m_upVector.x +
                                 m_upVector.y * m_upVector.y +
                                 m_upVector.z * m_upVector.z);
        Vector3f un = m_upVector / upNorm;

        Vector3f s(d.y * un.z - d.z * un.y,
                   d.z * un.x - d.x * un.z,
                   d.x * un.y - d.y * un.x);

        float sNorm = std::sqrt(s.x * s.x +
                                s.y * s.y +
                                s.z * s.z);
        Vector3f sn = s / sNorm;

        Vector3f u(sn.y * d.z - sn.z * d.y,
                   sn.z * d.x - sn.x * d.z,
                   sn.x * d.y - sn.y * d.x);

        // Rebuild the view matrix
        m_viewTransform = Transform( s.x,  s.y,  s.z, 0.f,
                                     u.x,  u.y,  u.z, 0.f,
                                    -d.x, -d.y, -d.z, 0.f,
                                     0.f,  0.f,  0.f, 1.f);

        m_viewTransform.translate(-getCenter());

        m_viewTransformUpdated = true;
    }

    return m_viewTransform;
}

} // namespace cpp3ds
