////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/VertexBuffer.hpp>
#include <cpp3ds/Graphics/RenderTarget.hpp>
#include <cpp3ds/OpenGL.hpp>
#include <cpp3ds/System/Mutex.hpp>
#include <cpp3ds/System/Lock.hpp>
#include <cpp3ds/System/Err.hpp>


namespace
{
    // Thread-safe unique identifier generator,
    // is used for states cache (see RenderTarget)
    cpp3ds::Uint64 getUniqueId()
    {
        static cpp3ds::Uint64 id = 1; // start at 1, zero is "no buffer"
        static cpp3ds::Mutex mutex;

        cpp3ds::Lock lock(mutex);
        return id++;
    }
}


namespace cpp3ds
{
////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer() :
VertexContainer(0),
m_vertices     (),
m_primitiveType(Triangles),
m_bufferObject (0),
m_cacheId      (getUniqueId()),
m_needUpload   (true)
{
    create();
}


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(PrimitiveType type, unsigned int vertexCount) :
VertexContainer(0),
m_vertices     (vertexCount),
m_primitiveType(type),
m_bufferObject (0),
m_cacheId      (getUniqueId()),
m_needUpload   (true)
{
    create();
}


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(const VertexBuffer& copy) :
VertexContainer(0),
m_vertices     (copy.m_vertices),
m_primitiveType(copy.m_primitiveType),
m_bufferObject (0),
m_cacheId      (getUniqueId()),
m_needUpload   (true)
{
    create();
}


////////////////////////////////////////////////////////////
VertexBuffer::~VertexBuffer()
{
    // Destroy buffer object
    if (m_bufferObject)
    {
        ensureGlContext();

        GLuint bufferObject = static_cast<GLuint>(m_bufferObject);
        glCheck(glDeleteBuffers(1, &bufferObject));
    }
}


////////////////////////////////////////////////////////////
bool VertexBuffer::create()
{
    // First make sure that we can use vertex buffers
    if (!isAvailable())
    {
        err() << "Failed to create a vertex buffer: your system doesn't support vertex buffers "
              << "(you should test VertexBuffer::isAvailable() before trying to create a VertexBuffer object)" << std::endl;
        return false;
    }

    // Create the OpenGL buffer object if it doesn't exist yet
    if (!m_bufferObject)
    {
        GLuint bufferObject;
        glCheck(glGenBuffers(1, &bufferObject));
        m_bufferObject = static_cast<unsigned int>(bufferObject);
    }

    m_needUpload = true;

    return true;
}


////////////////////////////////////////////////////////////
unsigned int VertexBuffer::getVertexCount() const
{
    return static_cast<unsigned int>(m_vertices.size());
}


////////////////////////////////////////////////////////////
Vertex& VertexBuffer::operator [](unsigned int index)
{
    m_needUpload = true;

    return m_vertices[index];
}


////////////////////////////////////////////////////////////
const Vertex& VertexBuffer::operator [](unsigned int index) const
{
    return m_vertices[index];
}


////////////////////////////////////////////////////////////
void VertexBuffer::clear()
{
    if (!m_vertices.empty())
        m_needUpload = true;

    m_vertices.clear();
}


////////////////////////////////////////////////////////////
void VertexBuffer::resize(unsigned int vertexCount)
{
    if (m_vertices.size() != vertexCount)
        m_needUpload = true;

    m_vertices.resize(vertexCount);
}


////////////////////////////////////////////////////////////
void VertexBuffer::append(const Vertex& vertex)
{
    m_needUpload = true;

    m_vertices.push_back(vertex);
}


////////////////////////////////////////////////////////////
void VertexBuffer::setPrimitiveType(PrimitiveType type)
{
    m_primitiveType = type;
}


////////////////////////////////////////////////////////////
PrimitiveType VertexBuffer::getPrimitiveType() const
{
    return m_primitiveType;
}


////////////////////////////////////////////////////////////
FloatBox VertexBuffer::getBounds() const
{
    if (!m_vertices.empty())
    {
        float left   = m_vertices[0].position.x;
        float top    = m_vertices[0].position.y;
        float front  = m_vertices[0].position.z;
        float right  = m_vertices[0].position.x;
        float bottom = m_vertices[0].position.y;
        float back   = m_vertices[0].position.z;

        for (std::size_t i = 1; i < m_vertices.size(); ++i)
        {
            Vector3f position = m_vertices[i].position;

            // Update left and right
            if (position.x < left)
                left = position.x;
            else if (position.x > right)
                right = position.x;

            // Update top and bottom
            if (position.y < top)
                top = position.y;
            else if (position.y > bottom)
                bottom = position.y;

            // Update front and back
            if (position.z < front)
                front = position.z;
            else if (position.z > back)
                back = position.z;
        }

        return FloatBox(left, top, front, right - left, bottom - top, back - front);
    }
    else
    {
        // Buffer is empty
        return FloatBox();
    }
}


////////////////////////////////////////////////////////////
void* VertexBuffer::getPointer()
{
    m_needUpload = true;

    return &m_vertices[0];
}


////////////////////////////////////////////////////////////
const void* VertexBuffer::getPointer() const
{
    return &m_vertices[0];
}


////////////////////////////////////////////////////////////
unsigned int VertexBuffer::getBufferObjectName() const
{
    return m_bufferObject;
}


////////////////////////////////////////////////////////////
VertexBuffer& VertexBuffer::operator =(const VertexBuffer& right)
{
    VertexBuffer temp(right);

    std::swap(m_vertices,      temp.m_vertices);
    std::swap(m_primitiveType, temp.m_primitiveType);
    m_cacheId = getUniqueId();

    m_needUpload = true;

    return *this;
}


////////////////////////////////////////////////////////////
void VertexBuffer::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(*this, states);
}


////////////////////////////////////////////////////////////
void VertexBuffer::bind(const VertexBuffer* buffer)
{
    bind(buffer, GL_ARRAY_BUFFER);
}


////////////////////////////////////////////////////////////
void VertexBuffer::bind(const VertexBuffer* buffer, unsigned int target)
{
    ensureGlContext();

    if (buffer && buffer->m_bufferObject)
    {
        // Bind the buffer
        glCheck(glBindBuffer(target, buffer->m_bufferObject));

        if (buffer->m_needUpload)
        {
            // Orphan the buffer first to give the driver an opportunity to optimize streaming
            glCheck(glBufferData(target, buffer->m_vertices.size() * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW));
            glCheck(glBufferData(target, buffer->m_vertices.size() * sizeof(Vertex), &(buffer->m_vertices[0]), GL_DYNAMIC_DRAW));
            buffer->m_needUpload = false;
        }
    }
    else
    {
        // Bind no buffer
        glCheck(glBindBuffer(target, 0));
    }
}


////////////////////////////////////////////////////////////
bool VertexBuffer::isAvailable()
{
    static bool checked = false;
    static bool bufferObjectsSupported = false;
    if (!checked)
    {
        checked = true;

        ensureGlContext();

        // Make sure that GLEW is initialized
//        priv::ensureGlewInit();

        bufferObjectsSupported = true; //(GLEW_ARB_vertex_buffer_object != 0);
    }

    return bufferObjectsSupported;
}


////////////////////////////////////////////////////////////
bool VertexBuffer::hasVertexArrayObjects()
{
    static bool checked = false;
    static bool vertexArrayObjectsSupported = false;
    if (!checked)
    {
        checked = true;

        ensureGlContext();

        // Make sure that GLEW is initialized
//        priv::ensureGlewInit();

        vertexArrayObjectsSupported = true; //(GLEW_ARB_vertex_array_object != 0);
    }

    return vertexArrayObjectsSupported;
}

} // namespace cpp3ds
