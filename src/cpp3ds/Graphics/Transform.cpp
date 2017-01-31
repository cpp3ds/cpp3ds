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
#include <cpp3ds/Graphics/Transform.hpp>
#include <cmath>
#include "CitroHelpers.hpp"


namespace cpp3ds
{
////////////////////////////////////////////////////////////
const Transform Transform::Identity;


////////////////////////////////////////////////////////////
Transform::Transform()
{
    // Identity matrix
    Mtx_Identity(&m_matrix);
}


////////////////////////////////////////////////////////////
Transform::Transform(float a00, float a01, float a02,
                     float a10, float a11, float a12,
                     float a20, float a21, float a22)
{
    m_matrix.m[3] = a00; m_matrix.m[2] = a01; m_matrix.m[1]  = 0.f; m_matrix.m[0] = a02;
    m_matrix.m[7] = a10; m_matrix.m[6] = a11; m_matrix.m[5]  = 0.f; m_matrix.m[4] = a12;
    m_matrix.m[11] = 0.f; m_matrix.m[10] = 0.f; m_matrix.m[9] = 1.f; m_matrix.m[8] = 0.f;
    m_matrix.m[15] = a20; m_matrix.m[14] = a21; m_matrix.m[13] = 0.f; m_matrix.m[12] = a22;
}


void Transform::apply3dsFix()
{
    // Fix depth range to [-1, 0]
    C3D_Mtx mp2, mp3;
    Mtx_Identity(&mp2);
    mp2.r[2].z = 0.5;
    mp2.r[2].w = -0.5;
    Mtx_Multiply(&mp3, &mp2, &m_matrix);

    // Fix the 3DS screens' orientation by swapping the X and Y axis
    Mtx_Identity(&mp2);
    mp2.r[0].x = 0.0;
    mp2.r[0].y = 1.0;
    mp2.r[1].x = -1.0; // flipped
    mp2.r[1].y = 0.0;
    Mtx_Multiply(&m_matrix, &mp2, &mp3);
}


////////////////////////////////////////////////////////////
#ifdef EMULATION
const float* Transform::getMatrix() const
{
    return m_matrix;
}
#else
const C3D_Mtx* Transform::getMatrix() const
{
  return &m_matrix;
}
#endif

////////////////////////////////////////////////////////////
Transform Transform::getInverse() const
{
    // Compute the determinant
    float det = m_matrix.m[3] * (m_matrix.m[12] * m_matrix.m[6] - m_matrix.m[14] * m_matrix.m[4]) -
                m_matrix.m[7] * (m_matrix.m[12] * m_matrix.m[2] - m_matrix.m[14] * m_matrix.m[0]) +
                m_matrix.m[15] * (m_matrix.m[4] * m_matrix.m[2] - m_matrix.m[6] * m_matrix.m[0]);

    // Compute the inverse if the determinant is not zero
    // (don't use an epsilon because the determinant may *really* be tiny)
    if (det != 0.f)
    {
        return Transform( (m_matrix.m[12] * m_matrix.m[6] - m_matrix.m[14] * m_matrix.m[4]) / det,
                         -(m_matrix.m[12] * m_matrix.m[2] - m_matrix.m[14] * m_matrix.m[0]) / det,
                          (m_matrix.m[4] * m_matrix.m[2] - m_matrix.m[6] * m_matrix.m[0]) / det,
                         -(m_matrix.m[12] * m_matrix.m[7] - m_matrix.m[15] * m_matrix.m[4]) / det,
                          (m_matrix.m[12] * m_matrix.m[3] - m_matrix.m[15] * m_matrix.m[0]) / det,
                         -(m_matrix.m[4] * m_matrix.m[3] - m_matrix.m[7] * m_matrix.m[0]) / det,
                          (m_matrix.m[14]  * m_matrix.m[7] - m_matrix.m[15] * m_matrix.m[6])  / det,
                         -(m_matrix.m[14]  * m_matrix.m[3] - m_matrix.m[15] * m_matrix.m[2])  / det,
                          (m_matrix.m[6]  * m_matrix.m[3] - m_matrix.m[7] * m_matrix.m[2])  / det);
    }
    else
    {
        return Identity;
    }
}


////////////////////////////////////////////////////////////
Vector2f Transform::transformPoint(float x, float y) const
{
    return Vector2f(m_matrix.m[3] * x + m_matrix.m[2] * y + m_matrix.m[0],
                    m_matrix.m[7] * x + m_matrix.m[6] * y + m_matrix.m[4]);
}


////////////////////////////////////////////////////////////
Vector2f Transform::transformPoint(const Vector2f& point) const
{
    return transformPoint(point.x, point.y);
}


////////////////////////////////////////////////////////////
FloatRect Transform::transformRect(const FloatRect& rectangle) const
{
    // Transform the 4 corners of the rectangle
    const Vector2f points[] =
    {
        transformPoint(rectangle.left, rectangle.top),
        transformPoint(rectangle.left, rectangle.top + rectangle.height),
        transformPoint(rectangle.left + rectangle.width, rectangle.top),
        transformPoint(rectangle.left + rectangle.width, rectangle.top + rectangle.height)
    };

    // Compute the bounding rectangle of the transformed points
    float left = points[0].x;
    float top = points[0].y;
    float right = points[0].x;
    float bottom = points[0].y;
    for (int i = 1; i < 4; ++i)
    {
        if      (points[i].x < left)   left = points[i].x;
        else if (points[i].x > right)  right = points[i].x;
        if      (points[i].y < top)    top = points[i].y;
        else if (points[i].y > bottom) bottom = points[i].y;
    }

    return FloatRect(left, top, right - left, bottom - top);
}


////////////////////////////////////////////////////////////
Transform& Transform::combine(const Transform& transform)
{
    const float* a = m_matrix.m;
    const float* b = transform.m_matrix.m;

    *this = Transform(a[3] * b[3]  + a[2] * b[7]  + a[0] * b[15],
                      a[3] * b[2]  + a[2] * b[6]  + a[0] * b[14],
                      a[3] * b[0] + a[2] * b[4] + a[0] * b[12],
                      a[7] * b[3]  + a[6] * b[7]  + a[4] * b[15],
                      a[7] * b[2]  + a[6] * b[6]  + a[4] * b[14],
                      a[7] * b[0] + a[6] * b[4] + a[4] * b[12],
                      a[15] * b[3]  + a[14] * b[7]  + a[12] * b[15],
                      a[15] * b[2]  + a[14] * b[6]  + a[12] * b[14],
                      a[15] * b[0] + a[14] * b[4] + a[12] * b[12]);

    return *this;
}


////////////////////////////////////////////////////////////
Transform& Transform::translate(float x, float y)
{
    Transform translation(1, 0, x,
                          0, 1, y,
                          0, 0, 1);

    return combine(translation);
}


////////////////////////////////////////////////////////////
Transform& Transform::translate(const Vector2f& offset)
{
    return translate(offset.x, offset.y);
}


////////////////////////////////////////////////////////////
Transform& Transform::rotate(float angle)
{
    float rad = angle * 3.141592654f / 180.f;
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    Transform rotation(cos, -sin, 0,
                       sin,  cos, 0,
                       0,    0,   1);

    return combine(rotation);
}


////////////////////////////////////////////////////////////
Transform& Transform::rotate(float angle, float centerX, float centerY)
{
    float rad = angle * 3.141592654f / 180.f;
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    Transform rotation(cos, -sin, centerX * (1 - cos) + centerY * sin,
                       sin,  cos, centerY * (1 - cos) - centerX * sin,
                       0,    0,   1);

    return combine(rotation);
}


////////////////////////////////////////////////////////////
Transform& Transform::rotate(float angle, const Vector2f& center)
{
    return rotate(angle, center.x, center.y);
}


////////////////////////////////////////////////////////////
Transform& Transform::scale(float scaleX, float scaleY)
{
    Transform scaling(scaleX, 0,      0,
                      0,      scaleY, 0,
                      0,      0,      1);

    return combine(scaling);
}


////////////////////////////////////////////////////////////
Transform& Transform::scale(float scaleX, float scaleY, float centerX, float centerY)
{
    Transform scaling(scaleX, 0,      centerX * (1 - scaleX),
                      0,      scaleY, centerY * (1 - scaleY),
                      0,      0,      1);

    return combine(scaling);
}


////////////////////////////////////////////////////////////
Transform& Transform::scale(const Vector2f& factors)
{
    return scale(factors.x, factors.y);
}


////////////////////////////////////////////////////////////
Transform& Transform::scale(const Vector2f& factors, const Vector2f& center)
{
    return scale(factors.x, factors.y, center.x, center.y);
}


////////////////////////////////////////////////////////////
Transform operator *(const Transform& left, const Transform& right)
{
    return Transform(left).combine(right);
}


////////////////////////////////////////////////////////////
Transform& operator *=(Transform& left, const Transform& right)
{
    return left.combine(right);
}


////////////////////////////////////////////////////////////
Vector2f operator *(const Transform& left, const Vector2f& right)
{
    return left.transformPoint(right);
}

}
