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
#include <cpp3ds/Graphics/Transformable.hpp>
#include <cmath>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Transformable::Transformable() :
m_origin                    (0, 0, 0),
m_position                  (0, 0, 0),
m_rotation                  (0),
m_scale                     (1, 1, 1),
m_rotation_transform        (),
m_transform                 (),
m_transformNeedUpdate       (true),
m_inverseTransform          (),
m_inverseTransformNeedUpdate(true)
{
}


////////////////////////////////////////////////////////////
Transformable::~Transformable()
{
}


////////////////////////////////////////////////////////////
void Transformable::setPosition(float x, float y, float z)
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::setPosition(const Vector3f& position)
{
    setPosition(position.x, position.y, position.z);
}


////////////////////////////////////////////////////////////
void Transformable::setRotation(float angle)
{
    m_rotation = static_cast<float>(fmod(angle, 360));
    if (m_rotation < 0)
        m_rotation += 360.f;

    m_rotation_transform = Transform::Identity;
    m_rotation_transform.rotate(m_rotation, Vector3f(0.f, 0.f, 1.f));

    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::setRotation(float angle, const Vector3f& axis)
{
	m_rotation = static_cast<float>(fmod(angle, 360));
	if (m_rotation < 0)
		m_rotation += 360.f;

	m_rotation_transform = Transform::Identity;
	m_rotation_transform.rotate(m_rotation, axis);

	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::setScale(float factorX, float factorY, float factorZ)
{
    m_scale.x = factorX;
    m_scale.y = factorY;
    m_scale.z = factorZ;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::setScale(const Vector3f& factors)
{
    setScale(factors.x, factors.y, factors.z);
}


////////////////////////////////////////////////////////////
void Transformable::setOrigin(float x, float y, float z)
{
    m_origin.x = x;
    m_origin.y = y;
    m_origin.z = z;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::setOrigin(const Vector3f& origin)
{
    setOrigin(origin.x, origin.y, origin.z);
}


////////////////////////////////////////////////////////////
const Vector3f& Transformable::getPosition() const
{
    return m_position;
}


////////////////////////////////////////////////////////////
float Transformable::getRotation() const
{
    return m_rotation;
}


////////////////////////////////////////////////////////////
const Vector3f& Transformable::getScale() const
{
    return m_scale;
}


////////////////////////////////////////////////////////////
const Vector3f& Transformable::getOrigin() const
{
    return m_origin;
}


////////////////////////////////////////////////////////////
void Transformable::move(float offsetX, float offsetY, float offsetZ)
{
    setPosition(m_position.x + offsetX, m_position.y + offsetY, m_position.z + offsetZ);
}


////////////////////////////////////////////////////////////
void Transformable::move(const Vector3f& offset)
{
    setPosition(m_position.x + offset.x, m_position.y + offset.y, m_position.z + offset.z);
}


////////////////////////////////////////////////////////////
void Transformable::rotate(float angle)
{
    setRotation(m_rotation + angle);
}


////////////////////////////////////////////////////////////
void Transformable::rotate(float angle, const Vector3f& axis)
{
	Transform new_rotation = Transform::Identity;
	new_rotation.rotate(angle, axis);
	m_rotation_transform = new_rotation * m_rotation_transform;

	m_transformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::scale(float factorX, float factorY, float factorZ)
{
    setScale(m_scale.x * factorX, m_scale.y * factorY, m_scale.z * factorZ);
}


////////////////////////////////////////////////////////////
void Transformable::scale(const Vector3f& factor)
{
    setScale(m_scale.x * factor.x, m_scale.y * factor.y, m_scale.z * factor.z);
}


////////////////////////////////////////////////////////////
const Transform& Transformable::getTransform() const
{
    // Recompute the combined transform if needed
    if (m_transformNeedUpdate)
    {
        float angle  = -m_rotation * 3.141592654f / 180.f;
        float cosine = static_cast<float>(std::cos(angle));
        float sine   = static_cast<float>(std::sin(angle));
        float sxc    = m_scale.x * cosine;
        float syc    = m_scale.y * cosine;
        float sxs    = m_scale.x * sine;
        float sys    = m_scale.y * sine;
        float sz     = m_scale.z;
        float tx     = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
        float ty     =  m_origin.x * sxs - m_origin.y * syc + m_position.y;
        float tz     = -m_origin.z                          + m_position.z;

        // Translation into origin
        Transform origin_transform = Transform::Identity;
        origin_transform.translate(-m_origin);

        // Scale
        Transform scale_transform = Transform::Identity;
        scale_transform.scale(m_scale);

        // Translate to position
        Transform position_transform = Transform::Identity;
        position_transform.translate(m_position);

        m_transform = position_transform * scale_transform * m_rotation_transform * origin_transform;
        m_transformNeedUpdate = false;
    }

    return m_transform;
}


////////////////////////////////////////////////////////////
const Transform& Transformable::getInverseTransform() const
{
    // Recompute the inverse transform if needed
    if (m_inverseTransformNeedUpdate)
    {
        m_inverseTransform = getTransform().getInverse();
        m_inverseTransformNeedUpdate = false;
    }

    return m_inverseTransform;
}

} // namespace cpp3ds
