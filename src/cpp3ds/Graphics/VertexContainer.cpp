////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/VertexContainer.hpp>
#include <cpp3ds/Graphics/VertexArray.hpp>
#include <cpp3ds/Graphics/VertexBuffer.hpp>
#include <cpp3ds/Graphics/RenderTarget.hpp>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
VertexContainer::VertexContainer() :
m_impl(NULL)
{
    if (VertexBuffer::isAvailable())
    {
        m_impl = new VertexBuffer;
    }
    else
    {
        m_impl = new VertexArray;
    }
}


////////////////////////////////////////////////////////////
VertexContainer::VertexContainer(PrimitiveType type, unsigned int vertexCount) :
m_impl(NULL)
{
    if (VertexBuffer::isAvailable())
    {
        m_impl = new VertexBuffer(type, vertexCount);
    }
    else
    {
        m_impl = new VertexArray(type, vertexCount);
    }
}


////////////////////////////////////////////////////////////
VertexContainer::VertexContainer(int) :
m_impl(NULL)
{
}


////////////////////////////////////////////////////////////
VertexContainer::VertexContainer(const VertexContainer& copy) :
m_impl(NULL)
{
    if (copy.m_impl)
    {
        // copy is a VertexContainer
        if (VertexBuffer::isAvailable())
        {
            m_impl = new VertexBuffer(*static_cast<const VertexBuffer*>(copy.m_impl));
        }
        else
        {
            m_impl = new VertexArray(*static_cast<const VertexArray*>(copy.m_impl));
        }
    }

    // If copy is not a VertexContainer, derived classes will handle copying
}


////////////////////////////////////////////////////////////
VertexContainer::~VertexContainer()
{
    delete m_impl;
}


////////////////////////////////////////////////////////////
unsigned int VertexContainer::getVertexCount() const
{
    return m_impl->getVertexCount();
}


////////////////////////////////////////////////////////////
Vertex& VertexContainer::operator [](unsigned int index)
{
    return (*m_impl)[index];
}


////////////////////////////////////////////////////////////
const Vertex& VertexContainer::operator [](unsigned int index) const
{
    return (*m_impl)[index];
}


////////////////////////////////////////////////////////////
void VertexContainer::clear()
{
    m_impl->clear();
}


////////////////////////////////////////////////////////////
void VertexContainer::resize(unsigned int vertexCount)
{
    m_impl->resize(vertexCount);
}


////////////////////////////////////////////////////////////
void VertexContainer::append(const Vertex& vertex)
{
    m_impl->append(vertex);
}


////////////////////////////////////////////////////////////
void VertexContainer::setPrimitiveType(PrimitiveType type)
{
    m_impl->setPrimitiveType(type);
}


////////////////////////////////////////////////////////////
PrimitiveType VertexContainer::getPrimitiveType() const
{
    return m_impl->getPrimitiveType();
}


////////////////////////////////////////////////////////////
FloatBox VertexContainer::getBounds() const
{
    return m_impl->getBounds();
}


////////////////////////////////////////////////////////////
VertexContainer& VertexContainer::operator =(const VertexContainer& right)
{
    VertexContainer temp(right);

    std::swap(m_impl, temp.m_impl);

    return *this;
}


////////////////////////////////////////////////////////////
void VertexContainer::draw(RenderTarget& target, RenderStates states) const
{
    if (VertexBuffer::isAvailable())
    {
        target.draw(*static_cast<const VertexBuffer*>(m_impl), states);
    }
    else
    {
        m_impl->draw(target, states);
    }
}

} // namespace cpp3ds
