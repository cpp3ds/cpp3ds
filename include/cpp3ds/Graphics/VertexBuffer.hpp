#ifndef CPP3DS_VERTEXBUFFER_HPP
#define CPP3DS_VERTEXBUFFER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/Vertex.hpp>
#include <cpp3ds/Graphics/PrimitiveType.hpp>
#include <cpp3ds/Graphics/Rect.hpp>
#include <cpp3ds/Graphics/Box.hpp>
#include <cpp3ds/Graphics/VertexContainer.hpp>
#include <cpp3ds/Window/GlResource.hpp>
#ifndef EMULATION
#include <cpp3ds/System/LinearAllocator.hpp>
#endif
#include <vector>
#include <map>


namespace cpp3ds
{
class Shader;

////////////////////////////////////////////////////////////
/// \brief Define a set of one or more primitives
///
////////////////////////////////////////////////////////////
class VertexBuffer : public VertexContainer, GlResource
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty vertex buffer.
    ///
    ////////////////////////////////////////////////////////////
    VertexBuffer();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the vertex buffer with a type and an initial number of vertices
    ///
    /// \param type        Type of primitives
    /// \param vertexCount Initial number of vertices in the buffer
    ///
    ////////////////////////////////////////////////////////////
    explicit VertexBuffer(PrimitiveType type, unsigned int vertexCount = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy instance to copy
    ///
    ////////////////////////////////////////////////////////////
    VertexBuffer(const VertexBuffer& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~VertexBuffer();

    ////////////////////////////////////////////////////////////
    /// \brief Create the vertex buffer
    ///
    /// If this function fails, the vertex buffer is left unchanged.
    ///
    /// \return True if creation was successful
    ///
    ////////////////////////////////////////////////////////////
    bool create();

    ////////////////////////////////////////////////////////////
    /// \brief Return the vertex count
    ///
    /// \return Number of vertices in the buffer
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getVertexCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a read-write access to a vertex by its index
    ///
    /// This function doesn't check \a index, it must be in range
    /// [0, getVertexCount() - 1]. The behaviour is undefined
    /// otherwise.
    ///
    /// \param index Index of the vertex to get
    ///
    /// \return Reference to the index-th vertex
    ///
    /// \see getVertexCount
    ///
    ////////////////////////////////////////////////////////////
    Vertex& operator [](unsigned int index);

    ////////////////////////////////////////////////////////////
    /// \brief Get a read-only access to a vertex by its index
    ///
    /// This function doesn't check \a index, it must be in range
    /// [0, getVertexCount() - 1]. The behaviour is undefined
    /// otherwise.
    ///
    /// \param index Index of the vertex to get
    ///
    /// \return Const reference to the index-th vertex
    ///
    /// \see getVertexCount
    ///
    ////////////////////////////////////////////////////////////
    const Vertex& operator [](unsigned int index) const;

    ////////////////////////////////////////////////////////////
    /// \brief Clear the vertex buffer
    ///
    /// This function removes all the vertices from the buffer.
    /// It doesn't deallocate the corresponding memory, so that
    /// adding new vertices after clearing doesn't involve
    /// reallocating all the memory.
    ///
    ////////////////////////////////////////////////////////////
    void clear();

    ////////////////////////////////////////////////////////////
    /// \brief Resize the vertex buffer
    ///
    /// If \a vertexCount is greater than the current size, the previous
    /// vertices are kept and new (default-constructed) vertices are
    /// added.
    /// If \a vertexCount is less than the current size, existing vertices
    /// are removed from the buffer.
    ///
    /// \param vertexCount New size of the buffer (number of vertices)
    ///
    ////////////////////////////////////////////////////////////
    void resize(unsigned int vertexCount);

    ////////////////////////////////////////////////////////////
    /// \brief Add a vertex to the buffer
    ///
    /// \param vertex Vertex to add
    ///
    ////////////////////////////////////////////////////////////
    void append(const Vertex& vertex);

    ////////////////////////////////////////////////////////////
    /// \brief Set the type of primitives to draw
    ///
    /// This function defines how the vertices must be interpreted
    /// when it's time to draw them:
    /// \li As points
    /// \li As lines
    /// \li As triangles
    /// \li As quads
    /// The default primitive type is cpp3ds::Points.
    ///
    /// \param type Type of primitive
    ///
    ////////////////////////////////////////////////////////////
    void setPrimitiveType(PrimitiveType type);

    ////////////////////////////////////////////////////////////
    /// \brief Get the type of primitives drawn by the vertex buffer
    ///
    /// \return Primitive type
    ///
    ////////////////////////////////////////////////////////////
    PrimitiveType getPrimitiveType() const;

    ////////////////////////////////////////////////////////////
    /// \brief Compute the bounding box of the vertex buffer
    ///
    /// This function returns the axis-aligned box that
    /// contains all the vertices of the buffer.
    ///
    /// \return Bounding box of the vertex buffer
    ///
    ////////////////////////////////////////////////////////////
    FloatBox getBounds() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a pointer (non-const) to the data
    ///
    /// This function returns a non-const pointer to the data
    /// block allocated by this VertexBuffer. It can be used
    /// for transferring arbitrary data between host and graphics
    /// memory. The number of bytes available can be computed
    /// with getVertexCount() * sizeof(cpp3ds::Vertex).
    ///
    /// \return Non-const pointer to the data
    ///
    ////////////////////////////////////////////////////////////
    void* getPointer();

    ////////////////////////////////////////////////////////////
    /// \brief Get a pointer (const) to the data
    ///
    /// This function returns a const pointer to the data
    /// block allocated by this VertexBuffer. The number
    /// of bytes available can be computed with
    /// getVertexCount() * sizeof(cpp3ds::Vertex).
    ///
    /// \return Const pointer to the data
    ///
    ////////////////////////////////////////////////////////////
    const void* getPointer() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the name of the underlying buffer object
    ///
    /// This function returns the name of the underlying
    /// OpenGL buffer object, i.e. the identifier returned
    /// by glGenBuffers.
    ///
    /// \return Name of the underlying buffer object
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getBufferObjectName() const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    VertexBuffer& operator =(const VertexBuffer& right);

    ////////////////////////////////////////////////////////////
    /// \brief Bind a vertex buffer for rendering
    ///
    /// This function is not part of the graphics API, it mustn't be
    /// used when drawing cpp3ds entities. It must be used only if you
    /// mix cpp3ds::VertexBuffer with OpenGL code.
    ///
    /// \code
    /// cpp3ds::VertexBuffer buffer1, buffer2;
    /// ...
    /// cpp3ds::VertexBuffer::bind(&buffer1);
    /// // draw OpenGL stuff that use buffer1...
    /// cpp3ds::VertexBuffer::bind(&buffer2);
    /// // draw OpenGL stuff that use buffer2...
    /// cpp3ds::VertexBuffer::bind(NULL);
    /// // draw OpenGL stuff that use no vertex buffer...
    /// \endcode
    ///
    /// \param buffer Pointer to the vertex buffer to bind, can be null to use no vertex buffer
    ///
    ////////////////////////////////////////////////////////////
    static void bind(const VertexBuffer* buffer);

    ////////////////////////////////////////////////////////////
    /// \brief Bind a vertex buffer to a specific target
    ///
    /// This function is not part of the graphics API, it mustn't be
    /// used when drawing cpp3ds entities. It must be used only if you
    /// mix cpp3ds::VertexBuffer with OpenGL code.
    ///
    /// \code
    /// cpp3ds::VertexBuffer buffer1, buffer2;
    /// ...
    /// cpp3ds::VertexBuffer::bind(&buffer1, GL_ELEMENT_ARRAY_ARB);
    /// // draw OpenGL stuff that use buffer1 as an index buffer...
    /// cpp3ds::VertexBuffer::bind(&buffer2, GL_ELEMENT_ARRAY_ARB);
    /// // draw OpenGL stuff that use buffer2 as an index buffer...
    /// cpp3ds::VertexBuffer::bind(NULL, GL_ELEMENT_ARRAY_ARB);
    /// // draw OpenGL stuff that use no index buffer...
    /// \endcode
    ///
    /// \param buffer Pointer to the vertex buffer to bind, can be null to use no vertex buffer
    /// \param target Target to bind to
    ///
    ////////////////////////////////////////////////////////////
    static void bind(const VertexBuffer* buffer, unsigned int target);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the system supports vertex buffers
    ///
    /// This function should always be called before using
    /// the vertex buffer features. If it returns false, then
    /// any attempt to use cpp3ds::VertexBuffer will fail.
    ///
    /// \return True if vertex buffers are supported, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isAvailable();

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the system supports vertex array objects
    ///
    /// This function should always be called before using
    /// any vertex array object features.
    ///
    /// \return True if vertex array objects are supported, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool hasVertexArrayObjects();

private :

    friend class RenderTarget;
    friend class Shader;

    ////////////////////////////////////////////////////////////
    /// \brief Draw the vertex buffer to a render target
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(RenderTarget& target, RenderStates states) const;

    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    typedef std::map<std::pair<Uint64, Uint64>, unsigned int> ArrayObjects;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    #ifdef EMULATION
	std::vector<Vertex>  m_vertices;      ///< Vertices contained in the buffer
	#else
    std::vector<Vertex, LinearAllocator<Vertex>>  m_vertices;
	#endif
    PrimitiveType        m_primitiveType; ///< Type of primitives to draw
    unsigned int         m_bufferObject;  ///< OpenGL identifier for the buffer object
    Uint64               m_cacheId;       ///< Unique number that identifies the vertex buffer to the render target's cache
    mutable bool         m_needUpload;    ///< Whether the buffer data needs to be re-uploaded
    mutable ArrayObjects m_arrayObjects;  ///< Map of (render target, shader) pairs to array object identifiers
};

} // namespace cpp3ds


#endif // CPP3DS_VERTEXBUFFER_HPP


////////////////////////////////////////////////////////////
/// \class cpp3ds::VertexBuffer
/// \ingroup graphics
///
/// cpp3ds::VertexBuffer is a very simple wrapper around a dynamic
/// buffer of vertices and a primitives type.
///
/// It inherits cpp3ds::Drawable, but unlike other drawables it
/// is not transformable.
///
/// An cpp3ds::VertexBuffer functions exactly like an cpp3ds::VertexArray
/// except that vertex data is stored in GPU memory and only
/// resynchronized with system memory when necessary. This is
/// analog to cpp3ds::Image and cpp3ds::Texture.
///
/// Be aware of the order when specifying vertices. By default,
/// outward facing faces have counter-clockwise winding and as
/// such any faces specified in clockwise order might not be
/// displayed.
///
/// Example:
/// \code
/// cpp3ds::VertexBuffer lines(cpp3ds::LinesStrip, 4);
/// lines[0].position = cpp3ds::Vector3f(10, 0, 0);
/// lines[1].position = cpp3ds::Vector3f(20, 0, 0);
/// lines[2].position = cpp3ds::Vector3f(30, 5, 0);
/// lines[3].position = cpp3ds::Vector3f(40, 2, 0);
///
/// window.draw(lines);
/// \endcode
///
/// \see cpp3ds::Vertex, cpp3ds::VertexArray, cpp3ds::VertexContainer
///
////////////////////////////////////////////////////////////