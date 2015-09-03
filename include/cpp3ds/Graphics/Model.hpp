#ifndef CPP3DS_MODEL_HPP
#define CPP3DS_MODEL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/Polyhedron.hpp>
#include <vector>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
/// \brief Base class for 3D models
///
////////////////////////////////////////////////////////////
class Model : public Polyhedron
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Model();

    ////////////////////////////////////////////////////////////
    /// \brief Get the total number of faces of the model
    ///
    /// \return Number of faces of the model
    ///
    /// \see getFace, addFace, clearFaces
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int getFaceCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a face of the model
    ///
    /// The result is undefined if \a index is out of the valid range.
    ///
    /// \param index Index of the face to get, in range [0 .. getFaceCount() - 1]
    ///
    /// \return Index-th face of the model
    ///
    /// \see getFaceCount, addFace, clearFaces
    ///
    ////////////////////////////////////////////////////////////
    virtual Face getFace(unsigned int index) const;

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Model();

    ////////////////////////////////////////////////////////////
    /// \brief Add a vertex to the model
    ///
    /// Add a vertex to the model that you can use later
    /// during face specification. Vertices are referenced
    /// by their index, so make sure to add them in the
    /// correct order.
    ///
    /// \param vertex Vertex to add
    ///
    /// \see setVertex, getVertex, getVertexCount
    ///
    ////////////////////////////////////////////////////////////
    void addVertex(const Vertex& vertex);

    ////////////////////////////////////////////////////////////
    /// \brief Set a vertex at the given index to a new value
    ///
    /// Set the vertex at the given index to a new value.
    /// This can be useful when the geometry of the model
    /// changes e.g. when animating it or applying other
    /// techniques that do not change the face topology.
    ///
    /// The result is undefined if \a index is out of the valid range.
    ///
    /// \param index  Index of the vertex to set
    /// \param vertex New vertex data
    ///
    /// \see addVertex, getVertex, getVertexCount
    ///
    ////////////////////////////////////////////////////////////
    void setVertex(unsigned int index, const Vertex& vertex);

    ////////////////////////////////////////////////////////////
    /// \brief Get a vertex at the given index
    ///
    /// Get the vertex at the given index.
    ///
    /// The result is undefined if \a index is out of the valid range.
    ///
    /// \param index Index of the vertex to get
    ///
    /// \return Index-th vertex of the model
    ///
    /// \see addVertex, setVertex, getVertexCount
    ///
    ////////////////////////////////////////////////////////////
    const Vertex& getVertex(unsigned int index) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of vertices in the model
    ///
    /// \return Number of vertices in the model
    ///
    /// \see addVertex, setVertex, getVertex
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getVertexCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Add a face to the model
    ///
    /// Add a triangular face to the model. The vertex indices
    /// to be provided are based on the order in which the
    /// vertices were added using addVertex. By default
    /// back-facing polygons are culled by the graphics
    /// hardware, and as such, specify all front-facing faces
    /// with counter-clockwise winding.
    ///
    /// The result is undefined if any indices are out of the valid range.
    ///
    /// \param index0 Index of the first vertex
    /// \param index1 Index of the second vertex
    /// \param index2 Index of the third vertex
    ///
    /// \see getFace, getFaceCount, clearFaces
    ///
    ////////////////////////////////////////////////////////////
    void addFace(unsigned int index0, unsigned int index1, unsigned int index2);

    ////////////////////////////////////////////////////////////
    /// \brief Clear the face data
    ///
    /// \see getFace, getFaceCount, addFace
    ///
    ////////////////////////////////////////////////////////////
    void clearFaces();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Struct containing the indices that belong to a face
    ///
    ////////////////////////////////////////////////////////////
    struct FaceIndices
    {
        unsigned int index0; ///< First vertex index
        unsigned int index1; ///< Second vertex index
        unsigned int index2; ///< Third vertex index
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<Vertex>      m_vertices; ///< Vertex data
    std::vector<FaceIndices> m_faces;    ///< Face data
};

} // namespace cpp3ds


#endif // CPP3DS_MODEL_HPP


////////////////////////////////////////////////////////////
/// \class cpp3ds::Model
/// \ingroup graphics
///
/// cpp3ds::Model is a drawable class that acts as an interface
/// between a user-defined class handling the application-specific
/// model data and cpp3ds. Loading of the model data is handled
/// by user code and geometry specification is done using
/// the methods provided by cpp3ds::Model.
/// Once the model is loaded, it can be displayed on a
/// render target like any other drawable.
///
/// This class inherits all the functions of cpp3ds::Transformable
/// (position, rotation, scale, bounds, ...) as well as the
/// functions of cpp3ds::Polyhedron (color, texture, ...).
///
/// It is important to keep in mind that while specifying faces,
/// faces with clockwise winding (vertices specified in clockwise
/// order from the perspective of the viewer) are culled by default.
/// If you want a face to be facing the "outside" of the polyhedron,
/// specify its vertices in counter-clockwise order.
///
/// If you want to light your scene, you will either need to
/// specify the vertex normal data yourself, or if you want to
/// automatically generate per-face normals you can call
/// generateNormals() after you are done specifying the faces.
///
/// After loading the model or modifying geometry data in any
/// way, call the update method to synchronize the internal
/// data structures with the data you specified.
///
/// \see cpp3ds::Polyhedron
///
////////////////////////////////////////////////////////////
