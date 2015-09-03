#ifndef CPP3DS_CAMERA_HPP
#define CPP3DS_CAMERA_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/View.hpp>
#include <cpp3ds/Graphics/Transformable.hpp>
#include <cpp3ds/System/Vector3.hpp>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
/// \brief 3D camera that defines what is shown on screen
///
////////////////////////////////////////////////////////////
class Camera : public View
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Construct the camera from its field of view and clipping planes
    ///
    /// This constructor creates a camera with the given
    /// field of view, near clipping plane distance and far
    /// clipping plane distance.
    ///
    /// The camera will be positioned at (0, 0, 0) with
    /// direction (0, 0, -1) and up (0, 1, 0) and will not
    /// be rotated or scaled by default.
    ///
    /// \param fov  Field of view of the camera in degrees
    /// \param near Near clipping plane distance of the camera
    /// \param far  Far clipping plane distance of the camera
    ///
    ////////////////////////////////////////////////////////////
    Camera(float fov, float near, float far);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Camera();

    ////////////////////////////////////////////////////////////
    /// \brief Set the position of the camera
    ///
    /// This function completely overwrites the previous position.
    /// See the move function to apply an offset based on the previous position instead.
    /// The default position of a camera is (0, 0, 0).
    ///
    /// \param x X coordinate of the new position
    /// \param y Y coordinate of the new position
    /// \param z Z coordinate of the new position
    ///
    /// \see move, getPosition
    ///
    ////////////////////////////////////////////////////////////
    void setPosition(float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// \brief Set the position of the camera
    ///
    /// This function completely overwrites the previous position.
    /// See the move function to apply an offset based on the previous position instead.
    /// The default position of a camera is (0, 0).
    ///
    /// \param position New position
    ///
    /// \see move, getPosition
    ///
    ////////////////////////////////////////////////////////////
    void setPosition(const Vector3f& position);

    ////////////////////////////////////////////////////////////
    /// \brief Get the position of the camera
    ///
    /// \return Current position
    ///
    /// \see setPosition
    ///
    ////////////////////////////////////////////////////////////
    virtual const Vector3f& getPosition() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the forward vector of the camera
    ///
    /// The direction (also called "at vector") is the vector
    /// pointing forward from the camera's perspective. Together
    /// with the up vector, it defines the 3D orientation of the
    /// camera in the scene. The direction vector doesn't
    /// have to be normalized.
    /// The default camera's direction is (0, 0, -1).
    ///
    /// \param x X coordinate of the camera's direction
    /// \param y Y coordinate of the camera's direction
    /// \param z Z coordinate of the camera's direction
    ///
    /// \see getDirection, setUpVector, setPosition
    ///
    ////////////////////////////////////////////////////////////
    void setDirection(float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// \brief Set the forward vector of the camera
    ///
    /// The direction (also called "at vector") is the vector
    /// pointing forward from the camera's perspective. Together
    /// with the up vector, it defines the 3D orientation of the
    /// camera in the scene. The direction vector doesn't
    /// have to be normalized.
    /// The default camera's direction is (0, 0, -1).
    ///
    /// \param direction New camera's direction
    ///
    /// \see getDirection, setUpVector, setPosition
    ///
    ////////////////////////////////////////////////////////////
    void setDirection(const Vector3f& direction);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current forward vector of the camera
    ///
    /// \return Camera's forward vector (not normalized)
    ///
    /// \see setDirection
    ///
    ////////////////////////////////////////////////////////////
    const Vector3f& getDirection() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the upward vector of the camera
    ///
    /// The up vector is the vector that points upward from the
    /// camera's perspective. Together with the direction, it
    /// defines the 3D orientation of the camera in the scene.
    /// The up vector doesn't have to be normalized.
    /// The default camera's up vector is (0, 1, 0).
    ///
    /// \param x X coordinate of the camera's up vector
    /// \param y Y coordinate of the camera's up vector
    /// \param z Z coordinate of the camera's up vector
    ///
    /// \see getUpVector, setDirection, setPosition
    ///
    ////////////////////////////////////////////////////////////
    void setUpVector(float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// \brief Set the upward vector of the camera
    ///
    /// The up vector is the vector that points upward from the
    /// camera's perspective. Together with the direction, it
    /// defines the 3D orientation of the camera in the scene.
    /// The up vector doesn't have to be normalized.
    /// The default camera's up vector is (0, 1, 0).
    ///
    /// \param upVector New camera's up vector
    ///
    /// \see getUpVector, setDirection, setPosition
    ///
    ////////////////////////////////////////////////////////////
    void setUpVector(const Vector3f& upVector);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current upward vector of the camera
    ///
    /// \return Camera's upward vector (not normalized)
    ///
    /// \see setUpVector
    ///
    ////////////////////////////////////////////////////////////
    const Vector3f& getUpVector() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the scale factors of the camera
    ///
    /// This function completely overwrites the previous scale.
    /// See the scale function to add a factor based on the previous scale instead.
    /// The default scale of a camera is (1, 1, 1).
    ///
    /// \param factorX New horizontal scale factor
    /// \param factorY New vertical scale factor
    /// \param factorZ New depth scale factor
    ///
    /// \see scale, getScale
    ///
    ////////////////////////////////////////////////////////////
    void setScale(float factorX, float factorY, float factorZ);

    ////////////////////////////////////////////////////////////
    /// \brief Set the scale factors of the camera
    ///
    /// This function completely overwrites the previous scale.
    /// See the scale function to add a factor based on the previous scale instead.
    /// The default scale of a transformable camera is (1, 1, 1).
    ///
    /// \param factors New scale factors
    ///
    /// \see scale, getScale
    ///
    ////////////////////////////////////////////////////////////
    void setScale(const Vector3f& factors);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current scale of the camera
    ///
    /// \return Current scale factors
    ///
    /// \see setScale
    ///
    ////////////////////////////////////////////////////////////
    const Vector3f& getScale() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the field of view of this camera
    ///
    /// The field of view of the camera specifies the opening
    /// angle of the frustum that is defined by this camera.
    /// Setting it to a larger value will make more geometry
    /// visible, whereas setting it to a smaller value will
    /// make less geometry visible.
    ///
    /// \param fov Field of view angle in degrees
    ///
    /// \see getFieldOfView
    ///
    ////////////////////////////////////////////////////////////
    void setFieldOfView(float fov);

    ////////////////////////////////////////////////////////////
    /// \brief Get the field of view of this camera
    ///
    /// \return Field of view angle in degrees
    ///
    /// \see setFieldOfView
    ///
    ////////////////////////////////////////////////////////////
    float getFieldOfView() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the distance to the near clipping plane
    ///
    /// The graphics hardware cannot draw objects that are
    /// infinitely far away from the viewer, hence it requires
    /// a frustum that defines the viewing volume in which
    /// all geometry must be to be rendered.
    /// The sides (left, right, top, bottom) are defined
    /// by the viewport and the field of view.
    /// The distance from the viewer to the "front" of the
    /// frustum is defined by the near clipping plane.
    /// Objects which are positioned closer to the viewer
    /// (or even behind the viewer) than the near clipping plane
    /// are clipped and thus not rendered.
    ///
    /// Because the frustum should always be "in front" of the
    /// viewer in order for objects to be rendered to the "screen"
    /// that you see, the distance to the near clipping plane
    /// must always be positive.
    ///
    /// Bear in mind that when using depth testing, the depth
    /// precision is affected by the distance between the near
    /// and far clipping planes.
    ///
    /// \param distance Distance to the near clipping plane
    ///
    /// \see getNearClippingPlane, setFarClippingPlane
    ///
    ////////////////////////////////////////////////////////////
    void setNearClippingPlane(float distance);

    ////////////////////////////////////////////////////////////
    /// \brief Get the distance to the near clipping plane
    ///
    /// \return The distance to the near clipping plane
    ///
    /// \see setNearClippingPlane, getFarClippingPlane
    ///
    ////////////////////////////////////////////////////////////
    float getNearClippingPlane() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the distance to the far clipping plane
    ///
    /// The graphics hardware cannot draw objects that are
    /// infinitely far away from the viewer, hence it requires
    /// a frustum that defines the viewing volume in which
    /// all geometry must be to be rendered.
    /// The sides (left, right, top, bottom) are defined
    /// by the viewport and the field of view.
    /// The distance from the viewer to the "back" of the
    /// frustum is defined by the far clipping plane.
    /// Objects which are positioned further away from the
    /// viewer than the far clipping plane are clipped
    /// and thus not rendered.
    ///
    /// Because the frustum should always be "in front" of the
    /// viewer in order for objects to be rendered to the "screen"
    /// that you see, the distance to the far clipping plane
    /// must always be positive.
    ///
    /// Bear in mind that when using depth testing, the depth
    /// precision is affected by the distance between the near
    /// and far clipping planes.
    ///
    /// \param distance Distance to the far clipping plane
    ///
    /// \see getFarClippingPlane, setNearClippingPlane
    ///
    ////////////////////////////////////////////////////////////
    void setFarClippingPlane(float distance);

    ////////////////////////////////////////////////////////////
    /// \brief Get the distance to the far clipping plane
    ///
    /// \return The distance to the far clipping plane
    ///
    /// \see setFarClippingPlane, getNearClippingPlane
    ///
    ////////////////////////////////////////////////////////////
    float getFarClippingPlane() const;

    ////////////////////////////////////////////////////////////
    /// \brief Scale the camera
    ///
    /// This function multiplies the current scale of the camera,
    /// unlike setScale which overwrites it.
    /// Thus, it is equivalent to the following code:
    /// \code
    /// cpp3ds::Vector3f scale = camera.getScale();
    /// camera.setScale(scale.x * factorX, scale.y * factorY, scale.z * factorZ);
    /// \endcode
    ///
    /// \param factorX Horizontal scale factor
    /// \param factorY Vertical scale factor
    /// \param factorZ Depth scale factor
    ///
    /// \see setScale
    ///
    ////////////////////////////////////////////////////////////
    void scale(float factorX, float factorY, float factorZ);

    ////////////////////////////////////////////////////////////
    /// \brief Scale the camera
    ///
    /// This function multiplies the current scale of the camera,
    /// unlike setScale which overwrites it.
    /// Thus, it is equivalent to the following code:
    /// \code
    /// cpp3ds::Vector3f scale = camera.getScale();
    /// camera.setScale(scale.x * factor.x, scale.y * factor.y, scale.z * factor.z);
    /// \endcode
    ///
    /// \param factor Scale factors
    ///
    /// \see setScale
    ///
    ////////////////////////////////////////////////////////////
    void scale(const Vector3f& factor);

    ////////////////////////////////////////////////////////////
    /// \brief Get the projection transform of the camera
    ///
    /// This function is meant for internal use only.
    ///
    /// \return Projection transform defining the camera
    ///
    /// \see getInverseTransform
    ///
    ////////////////////////////////////////////////////////////
    virtual const Transform& getTransform() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the view transform of the camera
    ///
    /// This function is meant for internal use only.
    ///
    /// \return View transform of the camera
    ///
    /// \see getTransform, getInverseTransform
    ///
    ////////////////////////////////////////////////////////////
    virtual const Transform& getViewTransform() const;

private:

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    float    m_fieldOfView; ///< Field of view of this camera, in degrees
    float    m_nearPlane;   ///< The distance to the near clipping plane
    float    m_farPlane;    ///< The distance to the far clipping plane
    Vector3f m_direction;   ///< The direction the camera is facing in
    Vector3f m_upVector;    ///< The up vector of the camera
    Vector3f m_scale;       ///< The scaling that is applied after the perspective transform
};

} // namespace cpp3ds


#endif // CPP3DS_VIEW_HPP


////////////////////////////////////////////////////////////
/// \class cpp3ds::Camera
/// \ingroup graphics
///
/// cpp3ds::Camera defines a camera in the 3D scene. This is a
/// very powerful concept: you can move, rotate or zoom
/// the entire scene without altering the way that your
/// drawable objects are drawn.
///
/// An cpp3ds::Camera is much like a camera in real life. It has
/// a position, a field of view (different camera lenses) a
/// direction in which it is pointed, a vector denoting
/// the direction that the top of the camera is pointing in
/// (think of a stick stuck to the top of the camera pointing
/// directly away from it) and a per-axis scale that is useful
/// for setting up the proper aspect ratio of the captured image
/// such as 4:3 and 16:9 cameras in real life.
///
/// Depending on where the camera is, where it is pointing,
/// where its "top" is facing, what its field of view is and
/// what its aspect ratio is, you will get a different final image.
///
/// Because cpp3ds::Camera inherits from cpp3ds::View, its viewport
/// allows to map the final image to a custom part of the
/// render target, and can be used for split-screen or for
/// creating a rear-view mirror, for example. If the source
/// image does not have the same size as the viewport, its
/// contents will be stretched or shrunk to fit in.
///
/// To apply a camera, you have to assign it to the render target.
/// Then, every object drawn to this render target will be
/// affected by the camera until you use another camera or view.
///
/// cpp3ds::Camera can be combined with cpp3ds::View to render a scene
/// using a 3D perspective and then to render 2D elements such as
/// text or a GUI using an cpp3ds::View. Just switch back and forth
/// depending on which you want to affect the following objects
/// to be drawn.
///
/// Usage example:
/// \code
/// cpp3ds::RenderWindow window;
///
/// // Construct the camera with a 90 degree field of view
/// // and about 1000 units of space between the clipping planes
/// cpp3ds::Camera camera(90.f, 0.001f, 1000.f);
///
/// // Set the camera aspect ratio as its scale
/// camera.scale(600.f / 800.f, 1.f, 1.f);
///
/// // Set its position, direction and up vector
/// camera.setPosition(10, 20, 30);
/// camera.setDirection(0, -1, -1);
/// camera.setUpVector(0, 1, 0);
///
/// // Set its target viewport to be half of the window
/// camera.setViewport(cpp3ds::FloatRect(0.f, 0.f, 1.f, 0.5f));
///
/// // Apply it
/// window.setView(camera);
///
/// // Render stuff
/// window.draw(someModel);
///
/// // Set to the default view for 2D rendering
/// window.setView(window.getDefaultView());
///
/// // Render 2D elements such as text or a GUI
/// window.draw(someText);
/// \endcode
///
/// See also the note on coordinates and undistorted rendering in cpp3ds::Transformable.
///
/// \see cpp3ds::View, cpp3ds::RenderWindow, cpp3ds::RenderTexture
///
////////////////////////////////////////////////////////////
