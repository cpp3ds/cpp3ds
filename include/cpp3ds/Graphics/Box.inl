

////////////////////////////////////////////////////////////
template <typename T>
Box<T>::Box() :
left  (0),
top   (0),
front (0),
width (0),
height(0),
depth (0)
{

}


////////////////////////////////////////////////////////////
template <typename T>
Box<T>::Box(T boxLeft, T boxTop, T boxFront, T boxWidth, T boxHeight, T boxDepth) :
left  (boxLeft),
top   (boxTop),
front (boxFront),
width (boxWidth),
height(boxHeight),
depth (boxDepth)
{

}


////////////////////////////////////////////////////////////
template <typename T>
Box<T>::Box(const Vector3<T>& position, const Vector3<T>& size) :
left  (position.x),
top   (position.y),
front (position.z),
width (size.x),
height(size.y),
depth (size.z)
{

}


////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
Box<T>::Box(const Box<U>& box) :
left  (static_cast<T>(box.left)),
top   (static_cast<T>(box.top)),
front (static_cast<T>(box.front)),
width (static_cast<T>(box.width)),
height(static_cast<T>(box.height)),
depth (static_cast<T>(box.depth))
{
}


////////////////////////////////////////////////////////////
template <typename T>
bool Box<T>::contains(T x, T y, T z) const
{
    // Boxes with negative dimensions are allowed, so we must handle them correctly

    // Compute the real min and max of the box on all axes
    T minX = std::min(left, static_cast<T>(left + width));
    T maxX = std::max(left, static_cast<T>(left + width));
    T minY = std::min(top, static_cast<T>(top + height));
    T maxY = std::max(top, static_cast<T>(top + height));
    T minZ = std::min(front, static_cast<T>(front + depth));
    T maxZ = std::max(front, static_cast<T>(front + depth));

    return (x >= minX) && (x < maxX) && (y >= minY) && (y < maxY) && (z >= minZ) && (z < maxZ);
}


////////////////////////////////////////////////////////////
template <typename T>
bool Box<T>::contains(const Vector3<T>& point) const
{
    return contains(point.x, point.y, point.z);
}


////////////////////////////////////////////////////////////
template <typename T>
bool Box<T>::intersects(const Box<T>& box) const
{
    Box<T> intersection;
    return intersects(box, intersection);
}


////////////////////////////////////////////////////////////
template <typename T>
bool Box<T>::intersects(const Box<T>& box, Box<T>& intersection) const
{
    // Boxes with negative dimensions are allowed, so we must handle them correctly

    // Compute the min and max of the first box on all axes
    T b1MinX = std::min(left, static_cast<T>(left + width));
    T b1MaxX = std::max(left, static_cast<T>(left + width));
    T b1MinY = std::min(top, static_cast<T>(top + height));
    T b1MaxY = std::max(top, static_cast<T>(top + height));
    T b1MinZ = std::min(front, static_cast<T>(front + depth));
    T b1MaxZ = std::max(front, static_cast<T>(front + depth));

    // Compute the min and max of the second box on all axes
    T b2MinX = std::min(box.left, static_cast<T>(box.left + box.width));
    T b2MaxX = std::max(box.left, static_cast<T>(box.left + box.width));
    T b2MinY = std::min(box.top, static_cast<T>(box.top + box.height));
    T b2MaxY = std::max(box.top, static_cast<T>(box.top + box.height));
    T b2MinZ = std::min(box.front, static_cast<T>(box.front + box.depth));
    T b2MaxZ = std::max(box.front, static_cast<T>(box.front + box.depth));

    // Compute the intersection boundaries
    T interLeft   = std::max(b1MinX, b2MinX);
    T interTop    = std::max(b1MinY, b2MinY);
    T interFront  = std::max(b1MinZ, b2MinZ);
    T interRight  = std::min(b1MaxX, b2MaxX);
    T interBottom = std::min(b1MaxY, b2MaxY);
    T interBack   = std::min(b1MaxZ, b2MaxZ);

    // If the intersection is valid (positive non zero area), then there is an intersection
    if ((interLeft < interRight) && (interTop < interBottom) && (interFront < interBack))
    {
        intersection = Box<T>(interLeft, interTop, interFront, interRight - interLeft, interBottom - interTop, interBack - interFront);
        return true;
    }
    else
    {
        intersection = Box<T>(0, 0, 0, 0, 0, 0);
        return false;
    }
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator ==(const Box<T>& left, const Box<T>& right)
{
    return (left.left == right.left) && (left.width == right.width) &&
           (left.top == right.top) && (left.height == right.height) &&
           (left.front == right.front) && (left.depth == right.depth);
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator !=(const Box<T>& left, const Box<T>& right)
{
    return !(left == right);
}
