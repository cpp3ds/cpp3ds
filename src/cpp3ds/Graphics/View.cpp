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
#include <cpp3ds/Graphics/View.hpp>
#include <cmath>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
View::View() :
m_transformUpdated       (false),
m_invTransformUpdated    (false),
m_viewTransformUpdated   (false),
m_invViewTransformUpdated(false),
m_position               (),
m_size                   (),
m_rotation               (0),
m_viewport               (0, 0, 1, 1)
{
    reset(FloatRect(0, 0, 1000, 1000));
}


////////////////////////////////////////////////////////////
View::View(const FloatRect& rectangle) :
m_transformUpdated       (false),
m_invTransformUpdated    (false),
m_viewTransformUpdated   (false),
m_invViewTransformUpdated(false),
m_position               (),
m_size                   (),
m_rotation               (0),
m_viewport               (0, 0, 1, 1)
{
    reset(rectangle);
}


////////////////////////////////////////////////////////////
View::View(const Vector3f& center, const Vector2f& size) :
m_transformUpdated       (false),
m_invTransformUpdated    (false),
m_viewTransformUpdated   (false),
m_invViewTransformUpdated(false),
m_position               (center),
m_size                   (size),
m_rotation               (0),
m_viewport               (0, 0, 1, 1)
{

}


////////////////////////////////////////////////////////////
View::~View()
{

}


////////////////////////////////////////////////////////////
void View::setCenter(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	m_viewTransformUpdated    = false;
	m_invViewTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void View::setCenter(const Vector3f& center)
{
	setCenter(center.x, center.y, center.z);
}


////////////////////////////////////////////////////////////
void View::setSize(float width, float height)
{
    m_size.x = width;
    m_size.y = height;

    m_transformUpdated        = false;
    m_invTransformUpdated     = false;
    m_viewTransformUpdated    = false;
    m_invViewTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void View::setSize(const Vector2f& size)
{
    setSize(size.x, size.y);
}


////////////////////////////////////////////////////////////
void View::setRotation(float angle)
{
    m_rotation = static_cast<float>(fmod(angle, 360));
    if (m_rotation < 0)
        m_rotation += 360.f;

    m_viewTransformUpdated    = false;
    m_invViewTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void View::setViewport(const FloatRect& viewport)
{
    m_viewport = viewport;
}


////////////////////////////////////////////////////////////
void View::reset(const FloatRect& rectangle)
{
    m_position.x = rectangle.left + rectangle.width / 2.f;
    m_position.y = rectangle.top + rectangle.height / 2.f;
    m_size.x   = rectangle.width;
    m_size.y   = rectangle.height;
    m_rotation = 0;

    m_transformUpdated        = false;
    m_invTransformUpdated     = false;
    m_viewTransformUpdated    = false;
    m_invViewTransformUpdated = false;
}


////////////////////////////////////////////////////////////
const Vector3f& View::getCenter() const
{
    return m_position;
}


////////////////////////////////////////////////////////////
const Vector2f& View::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
float View::getRotation() const
{
    return m_rotation;
}


////////////////////////////////////////////////////////////
const FloatRect& View::getViewport() const
{
    return m_viewport;
}


////////////////////////////////////////////////////////////
void View::move(float offsetX, float offsetY, float offsetZ)
{
    setCenter(m_position.x + offsetX, m_position.y + offsetY, m_position.z + offsetZ);
}


////////////////////////////////////////////////////////////
void View::move(const Vector3f& offset)
{
    setCenter(m_position + offset);
}


////////////////////////////////////////////////////////////
void View::rotate(float angle)
{
    setRotation(m_rotation + angle);
}


////////////////////////////////////////////////////////////
void View::zoom(float factor)
{
    setSize(m_size.x * factor, m_size.y * factor);
}


////////////////////////////////////////////////////////////
const Transform& View::getTransform() const
{
	// Recompute the matrix if needed
	if (!m_transformUpdated)
	{
		// Projection components
		float a =  2.f / m_size.x;
		float b = -2.f / m_size.y;

		// Rebuild the projection matrix
		m_transform = Transform(a,   0.f,  0.f, -1.f,
								0.f, b,    0.f,  1.f,
								0.f, 0.f, -2.f, -1.f,
								0.f, 0.f,  0.f,  1.f);

		m_transformUpdated = true;
	}

	return m_transform;
}


////////////////////////////////////////////////////////////
const Transform& View::getInverseTransform() const
{
    // Recompute the matrix if needed
    if (!m_invTransformUpdated)
    {
        m_inverseTransform = getTransform().getInverse();
        m_invTransformUpdated = true;
    }

    return m_inverseTransform;
}


////////////////////////////////////////////////////////////
const Transform& View::getViewTransform() const
{
    // Recompute the view matrix if needed
    if (!m_viewTransformUpdated)
    {
        // Rotation components
        float angle  = m_rotation * 3.141592654f / 180.f;
        float cosine = static_cast<float>(std::cos(angle));
        float sine   = static_cast<float>(std::sin(angle));

        // Translation components
        float x = m_size.x / 2.f - m_position.x;
        float y = m_size.y / 2.f - m_position.y;

        m_viewTransform = Transform( cosine, sine,   0.f, -x * cosine - y * sine,
                                    -sine,   cosine, 0.f,  x * sine - y * cosine,
                                     0.f,    0.f,    1.f,  0.f,
                                     0.f,    0.f,    0.f,  1.f);

        m_viewTransformUpdated = true;
    }

    return m_viewTransform;
}


////////////////////////////////////////////////////////////
const Transform& View::getInverseViewTransform() const
{
    // Recompute the matrix if needed
    if (!m_invViewTransformUpdated)
    {
        m_inverseViewTransform = getViewTransform().getInverse();
        m_invViewTransformUpdated = true;
    }

    return m_inverseViewTransform;
}


////////////////////////////////////////////////////////////
const Vector3f& View::getPosition() const
{
    return getCenter();
}

} // namespace sf
