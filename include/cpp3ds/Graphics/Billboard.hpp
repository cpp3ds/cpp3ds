#ifndef CPP3DS_BILLBOARD_HPP
#define CPP3DS_BILLBOARD_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/Sprite.hpp>


namespace cpp3ds
{
class Camera;

////////////////////////////////////////////////////////////
/// \brief A sprite which automatically rotates to
///        face the camera.
///
////////////////////////////////////////////////////////////
class Billboard : public Sprite
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty billboard with no source texture.
    ///
    ////////////////////////////////////////////////////////////
    Billboard();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the billboard from a source texture
    ///
    /// \param texture Source texture
    ///
    /// \see setTexture
    ///
    ////////////////////////////////////////////////////////////
    explicit Billboard(const Texture& texture);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the billboard from a sub-rectangle of a source texture
    ///
    /// \param texture   Source texture
    /// \param rectangle Sub-rectangle of the texture to assign to the billboard
    ///
    /// \see setTexture, setTextureRect
    ///
    ////////////////////////////////////////////////////////////
    Billboard(const Texture& texture, const IntRect& rectangle);

    ////////////////////////////////////////////////////////////
    /// \brief Set the camera the billboard should track
    ///
    /// The \a camera argument refers to a camera that must
    /// exist as long as the billboard uses it. Indeed, the billboard
    /// doesn't store its own copy of the camera, but rather keeps
    /// a pointer to the one that you passed to this function.
    /// If the camera is destroyed and the billboard tries to
    /// use it, the behaviour is undefined.
    ///
    /// \param camera Camera the billboard should track
    ///
    /// \see getCamera
    ///
    ////////////////////////////////////////////////////////////
    void setCamera(const Camera& camera);

    ////////////////////////////////////////////////////////////
    /// \brief Get the camera the billboard should track
    ///
    /// If the billboard has no camera, a NULL pointer is returned.
    /// The returned pointer is const, which means that you can't
    /// modify the camera when you retrieve it with this function.
    ///
    /// \return Pointer to the billboard's camera
    ///
    /// \see setCamera
    ///
    ////////////////////////////////////////////////////////////
    const Camera* getCamera() const;

private :

    ////////////////////////////////////////////////////////////
    /// \brief Draw the billboard to a render target
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(RenderTarget& target, RenderStates states) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    const Camera* m_camera; ///< Camera the billboard should track
};

} // namespace cpp3ds


#endif // cpp3ds_BILLBOARD_HPP


////////////////////////////////////////////////////////////
/// \class cpp3ds::Billboard
/// \ingroup graphics
///
/// cpp3ds::Billboard is an cpp3ds::Sprite that rotates to always
/// face a specific camera.
///
/// It inherits all the functions from cpp3ds::Sprite:
/// position, rotation, scale, origin as well as the sprite-specific
/// properties such as the texture to use, the part of it to display,
/// and the convenience functions to change the overall color of the
/// sprite, or to get its bounding rectangle.
///
/// For the cpp3ds::Billboard to automatically face a camera whenever
/// it is drawn, a camera needs to be specified using setCamera.
/// When initially constructed, no camera is tracked and the billboard
/// will not automatically rotate.
///
/// It is important to note that as with cpp3ds::Sprite the cpp3ds::Billboard
/// instance doesn't copy the texture that it uses, it only keeps a
/// reference to it. Thus, a cpp3ds::Texture must not be destroyed while
/// it is used by a cpp3ds::Billboard (i.e. never write a function that
/// uses a local cpp3ds::Texture instance for creating a billboard).
///
/// The same applies for the camera. It is not copied, but instead
/// a reference to it is stored.
///
/// See also the note on coordinates and undistorted rendering in cpp3ds::Transformable.
///
/// Usage example:
/// \code
/// // Declare and use a camera
/// cpp3ds::Camera camera;
/// window.setView(camera);
///
/// // Declare and load a texture
/// cpp3ds::Texture texture;
/// texture.loadFromFile("texture.png");
///
/// // Create a billboard
/// cpp3ds::Billboard billboard;
/// billboard.setTexture(texture);
/// billboard.setTextureRect(cpp3ds::IntRect(10, 10, 50, 30));
/// billboard.setColor(cpp3ds::Color(255, 255, 255, 200));
/// billboard.setCamera(camera);
/// billboard.setPosition(100, 25, 50);
///
/// // Draw it
/// window.draw(billboard);
/// \endcode
///
/// \see cpp3ds::Sprite, cpp3ds::Texture, cpp3ds::Transformable
///
////////////////////////////////////////////////////////////
