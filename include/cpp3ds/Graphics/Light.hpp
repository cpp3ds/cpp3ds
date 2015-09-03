#ifndef CPP3DS_LIGHT_HPP
#define CPP3DS_LIGHT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Graphics/Color.hpp>
#include <cpp3ds/Window/GlResource.hpp>
#include <cpp3ds/System/Vector3.hpp>
#include <string>
#include <set>


namespace cpp3ds
{
class Shader;

////////////////////////////////////////////////////////////
/// \brief Light source, either positional or directional
///
////////////////////////////////////////////////////////////
class Light : GlResource
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates a default positional light source with
    /// position (0, 0, 0), white color, 0.f ambient intensity,
    /// 1.f diffuse intensity and 1.f specular intensity.
    ///
    ////////////////////////////////////////////////////////////
    Light();

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy instance to copy
    ///
    ////////////////////////////////////////////////////////////
    Light(const Light& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Light();

    ////////////////////////////////////////////////////////////
    /// \brief Set this light to a directional light or positional light
    ///
    /// Positional light sources have a position within the scene
    /// to be rendered. They emit light uniformly in all directions.
    /// Directional lights can be considered as if they were positioned
    /// infinitely far away from the scene, thus the rays they emit
    /// all arrive parallel to one another.
    ///
    /// \param directional True to set this light source as a directional light, false to set it as a positional light
    ///
    /// \see isDirectional
    ///
    ////////////////////////////////////////////////////////////
    void setDirectional(bool directional);

    ////////////////////////////////////////////////////////////
    /// \brief Set the position of this light source
    ///
    /// This function completely overwrites the previous position.
    /// See the move function to apply an offset based on the previous position instead.
    /// The default position of a light source is (0, 0, 0).
    ///
    /// Keep in mind that this takes into account the current view matrix.
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
    /// \brief Set the position of this light source
    ///
    /// This function completely overwrites the previous position.
    /// See the move function to apply an offset based on the previous position instead.
    /// The default position of a light source is (0, 0, 0).
    ///
    /// Keep in mind that this takes into account the current view matrix.
    ///
    /// \param position The new position of this light source
    ///
    /// \see move, getPosition
    ///
    ////////////////////////////////////////////////////////////
    void setPosition(const Vector3f& position);

    ////////////////////////////////////////////////////////////
    /// \brief Get the position of this light source
    ///
    /// Warning: If this is a directional light source, this will
    /// return its direction as they are the same.
    ///
    /// \return The position of this light source
    ///
    /// \see setPosition
    ///
    ////////////////////////////////////////////////////////////
    const Vector3f& getPosition() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the direction of this light source
    ///
    /// This function completely overwrites the previous direction.
    ///
    /// \param x X coordinate of the new direction
    /// \param y Y coordinate of the new direction
    /// \param z Z coordinate of the new direction
    ///
    /// \see getDirection
    ///
    ////////////////////////////////////////////////////////////
    void setDirection(float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// \brief Set the direction of this light source
    ///
    /// This function completely overwrites the previous direction.
    ///
    /// \param direction The new direction of this light source
    ///
    /// \see getDirection
    ///
    ////////////////////////////////////////////////////////////
    void setDirection(const Vector3f& direction);

    ////////////////////////////////////////////////////////////
    /// \brief Get the direction of this light source
    ///
    /// Warning: If this is a positional light source, this will
    /// return its position as they are the same.
    ///
    /// \return The direction of this light source
    ///
    /// \see setDirection
    ///
    ////////////////////////////////////////////////////////////
    const Vector3f& getDirection() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the color of the light produced by this light source
    ///
    /// The default color of the light produced by a
    /// light source is cpp3ds::Color::White.
    ///
    /// \param color The new color of the light produced by this light source
    ///
    /// \see getColor
    ///
    ////////////////////////////////////////////////////////////
    void setColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Get the color of the light produced by this light source
    ///
    /// \return The color of the light produced by this light source
    ///
    /// \see setColor
    ///
    ////////////////////////////////////////////////////////////
    const Color& getColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the ambient intensity of this light source
    ///
    /// The default ambient intensity of a light source is 0.f
    /// meaning that when a surface is not illuminated by the light,
    /// it will appear black.
    ///
    /// \param intensity The new ambient intensity of this light source
    ///
    /// \see getAmbientIntensity
    ///
    ////////////////////////////////////////////////////////////
    void setAmbientIntensity(float intensity);

    ////////////////////////////////////////////////////////////
    /// \brief Get the ambient intensity of this light source
    ///
    /// \return The ambient intensity of this light source
    ///
    /// \see setAmbientIntensity
    ///
    ////////////////////////////////////////////////////////////
    float getAmbientIntensity() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the diffuse intensity of this light source
    ///
    /// The default diffuse intensity of a light source is 1.f
    /// meaning that when a surface is fully illuminated by the light,
    /// it will appear as the color/texture it would have if it
    /// was not lit at all.
    ///
    /// \param intensity The new diffuse intensity of this light source
    ///
    /// \see getDiffuseIntensity
    ///
    ////////////////////////////////////////////////////////////
    void setDiffuseIntensity(float intensity);

    ////////////////////////////////////////////////////////////
    /// \brief Get the diffuse intensity of this light source
    ///
    /// \return The diffuse intensity of this light source
    ///
    /// \see setDiffuseIntensity
    ///
    ////////////////////////////////////////////////////////////
    float getDiffuseIntensity() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the specular intensity of this light source
    ///
    /// The default specular intensity of a light source is 1.f
    /// meaning that when a surface is viewed from the same direction
    /// as the incident light ray (the light is shining at the surface
    /// in the same direction as you are looking at the surface),
    /// a specular highlight will appear on the surface.
    ///
    /// \param intensity The new specular intensity of this light source
    ///
    /// \see getSpecularIntensity
    ///
    ////////////////////////////////////////////////////////////
    void setSpecularIntensity(float intensity);

    ////////////////////////////////////////////////////////////
    /// \brief Get the specular intensity of this light source
    ///
    /// \return The specular intensity of this light source
    ///
    /// \see setSpecularIntensity
    ///
    ////////////////////////////////////////////////////////////
    float getSpecularIntensity() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the constant attenuation of this light source
    ///
    /// The constant attenuation factor of a light source is always
    /// applied during lighting computations disregarding the distance
    /// of a light source to a vertex. As such it is the same for all
    /// vertices.
    ///
    /// The default constant attenuation is 1, which means no attenuation.
    ///
    /// Attenuation factor: 1.f / (constant + linear * d + quadratic * d * d)
    ///
    /// \param attenuation The new constant attenuation factor of this light source
    ///
    /// \see getConstantAttenuation
    ///
    ////////////////////////////////////////////////////////////
    void setConstantAttenuation(float attenuation);

    ////////////////////////////////////////////////////////////
    /// \brief Get the constant attenuation of this light source
    ///
    /// \return The constant attenuation of this light source
    ///
    /// \see setConstantAttenuation
    ///
    ////////////////////////////////////////////////////////////
    float getConstantAttenuation() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the linear attenuation of this light source
    ///
    /// The linear attenuation factor of a light source is applied
    /// during lighting computations taking into account the distance
    /// of a light source to a vertex.
    ///
    /// The default linear attenuation is 0, which means no attenuation.
    ///
    /// Attenuation factor: 1.f / (constant + linear * d + quadratic * d * d)
    ///
    /// \param attenuation The new linear attenuation factor of this light source
    ///
    /// \see getLinearAttenuation
    ///
    ////////////////////////////////////////////////////////////
    void setLinearAttenuation(float attenuation);

    ////////////////////////////////////////////////////////////
    /// \brief Get the linear attenuation of this light source
    ///
    /// \return The linear attenuation of this light source
    ///
    /// \see setLinearAttenuation
    ///
    ////////////////////////////////////////////////////////////
    float getLinearAttenuation() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the quadratic attenuation of this light source
    ///
    /// The quadratic attenuation factor of a light source is applied
    /// during lighting computations taking into account the square of
    /// the distance of a light source to a vertex.
    ///
    /// The default quadratic attenuation is 0, which means no attenuation.
    ///
    /// Attenuation factor: 1.f / (constant + linear * d + quadratic * d * d)
    ///
    /// \param attenuation The new quadratic attenuation factor of this light source
    ///
    /// \see getQuadraticAttenuation
    ///
    ////////////////////////////////////////////////////////////
    void setQuadraticAttenuation(float attenuation);

    ////////////////////////////////////////////////////////////
    /// \brief Get the quadratic attenuation of this light source
    ///
    /// \return The quadratic attenuation of this light source
    ///
    /// \see setQuadraticAttenuation
    ///
    ////////////////////////////////////////////////////////////
    float getQuadraticAttenuation() const;

    ////////////////////////////////////////////////////////////
    /// \brief Move this light source by a given offset
    ///
    /// This function adds to the current position of the light source,
    /// unlike setPosition which overwrites it.
    /// Thus, it is equivalent to the following code:
    /// \code
    /// cpp3ds::Vector3f pos = light.getPosition();
    /// light.setPosition(pos.x + offsetX, pos.y + offsetY, pos.z + offsetZ);
    /// \endcode
    ///
    /// \param offsetX X offset
    /// \param offsetY Y offset
    /// \param offsetZ Z offset
    ///
    /// \see setPosition
    ///
    ////////////////////////////////////////////////////////////
    void move(float offsetX, float offsetY, float offsetZ);

    ////////////////////////////////////////////////////////////
    /// \brief Move this light source by a given offset
    ///
    /// This function adds to the current position of the light source,
    /// unlike setPosition which overwrites it.
    /// Thus, it is equivalent to the following code:
    /// \code
    /// light.setPosition(light.getPosition() + offset);
    /// \endcode
    ///
    /// \param offset Offset
    ///
    /// \see setPosition
    ///
    ////////////////////////////////////////////////////////////
    void move(const Vector3f& offset);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether this is a directional light or not
    ///
    /// \return True if this is a directional light, false if it is a positional light
    ///
    /// \see setDirectional
    ///
    ////////////////////////////////////////////////////////////
    bool isDirectional() const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    Light& operator =(const Light& right);

    ////////////////////////////////////////////////////////////
    /// \brief Enable this light during rendering
    ///
    /// Enable this light so it will be factored in when objects
    /// are rendered in the future.
    ///
    /// \code
    /// cpp3ds::Light l1, l2;
    /// ...
    /// l1.enable();
    /// // draw stuff taking l1 into account...
    /// l2.enable();
    /// // draw stuff taking l2 into account as well...
    /// l1.disable();
    /// // draw stuff taking only l2 into account...
    /// l2.disable();
    /// // draw stuff taking no lights into account (very dark)...
    /// \endcode
    ///
    /// \see disable
    ///
    ////////////////////////////////////////////////////////////
    void enable();

    ////////////////////////////////////////////////////////////
    /// \brief Disable this light during rendering
    ///
    /// Disable this light so it is no longer taken into account
    /// when rendering objects in the future.
    ///
    /// \see enable
    ///
    ////////////////////////////////////////////////////////////
    void disable();

    ////////////////////////////////////////////////////////////
    /// \brief Check whether this light is enabled
    ///
    /// \return true if this light is enabled
    ///
    /// \see enable, disable
    ///
    ////////////////////////////////////////////////////////////
    bool isEnabled();

    ////////////////////////////////////////////////////////////
    /// \brief Get the maximum number of lights supported
    ///
    /// This maximum number of lights supported is defined by
    /// the graphics driver. A common value on old as well as new
    /// hardware is 8.
    /// If you require more light sources than is supported
    /// by the fixed function pipeline, you will need to resort to
    /// using shaders.
    ///
    /// \return Maximum number of lights supported
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int getMaximumLights();

    ////////////////////////////////////////////////////////////
    /// \brief Enable lighting for objects that are going to be rendered
    ///
    /// \see disableLighting
    ///
    ////////////////////////////////////////////////////////////
    static void enableLighting();

    ////////////////////////////////////////////////////////////
    /// \brief Disable lighting for objects that are going to be rendered
    ///
    /// \see enableLighting
    ///
    ////////////////////////////////////////////////////////////
    static void disableLighting();

    ////////////////////////////////////////////////////////////
    /// \brief Check whether lighting is enabled
    ///
    /// \return true if lighting is enabled
    ///
    /// \see enableLighting, disableLighting
    ///
    ////////////////////////////////////////////////////////////
    static bool isLightingEnabled();

    ////////////////////////////////////////////////////////////
    /// \brief Check whether shader lighting is supported
    ///
    /// \return true if shader lighting is supported
    ///
    ////////////////////////////////////////////////////////////
    static bool hasShaderLighting();

    ////////////////////////////////////////////////////////////
    /// \brief Increase the lighting reference count
    ///
    /// This function is meant for internal use only.
    ///
    ////////////////////////////////////////////////////////////
    static void increaseLightReferences();

    ////////////////////////////////////////////////////////////
    /// \brief Decrease the lighting reference count
    ///
    /// This function is meant for internal use only.
    ///
    ////////////////////////////////////////////////////////////
    static void decreaseLightReferences();

    ////////////////////////////////////////////////////////////
    /// \brief Add lighting data to the given shader
    ///
    /// \param shader Shader to add the lighting data to
    ///
    ////////////////////////////////////////////////////////////
    static void addLightsToShader(const Shader& shader);

private :

    ////////////////////////////////////////////////////////////
    /// \brief Get a free identifier for this light
    ///
    ////////////////////////////////////////////////////////////
    void getId();

    ////////////////////////////////////////////////////////////
    /// \brief Set that the lighting data needs to be re-uploaded
    ///
    ////////////////////////////////////////////////////////////
    static void setNeedUniformUpload();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    int                 m_light;                ///< Internal light identifier
    Vector3f            m_position;             ///< Position/direction of the light
    bool                m_directional;          ///< Whether the light is a directional light
    Color               m_color;                ///< Color of the light
    float               m_ambientIntensity;     ///< Ambient intensity of the light
    float               m_diffuseIntensity;     ///< Diffuse intensity of the light
    float               m_specularIntensity;    ///< Specular intensity of the light
    float               m_constantAttenuation;  ///< Constant attenuation used during lighting computations
    float               m_linearAttenuation;    ///< Linear attenuation used during lighting computations
    float               m_quadraticAttenuation; ///< Quadratic attenuation used during lighting computations
    bool                m_enabled;              ///< Whether the light is enabled
    mutable std::string m_shaderElement;        ///< Cached string containing the element used to access lighting data in a shader
};

} // namespace cpp3ds


#endif // CPP3DS_LIGHT_HPP

////////////////////////////////////////////////////////////
/// \class cpp3ds::Light
/// \ingroup graphics
///
/// cpp3ds::Light defines a light source in the 3D scene.
/// Without lighting, 3D objects will not be interpreted as
/// such because they lack shading, the difference in color
/// we perceive because of different light intensities.
///
/// cpp3ds::Light supports 2 different types of lights, positional
/// i.e. point lights and directional lights.
///
/// Positional light sources have a fixed position within a 3D
/// scene. They emit light in all directions in a sphere outwards
/// from their position. Because of this, the virtual light rays
/// do not travel parallel to each other. This is important to
/// remember when using shadowing techniques. An example of a
/// positional light source would be a simple light bulb.
///
/// Directional light sources <em>do not</em> have a position.
/// They are defined by the \e direction in which they emit light
/// within the scene, hence their name. They can be seen as positioned
/// infinitely far away from the scene. In contrast to positional
/// light sources, the virtual light rays they emit are all
/// parallel to one another. The best example of a directional
/// light source would be the sun. Although it is a point light
/// at larger scales, once its emitted rays arrive on earth
/// they can be considered to be parallel. The most common use
/// of directional light sources is for outdoor lighting for
/// exactly this reason.
///
/// As with lights in real life, an cpp3ds::Light emits light of a
/// certain color and with certain properties. The "color" of
/// the light which is emitted is known as its diffuse color.
/// This determines what color an object will be rendered with
/// taking into consideration its own color and the amount
/// of light it is receiving. In the case an object is not
/// illuminated at all, it is still illuminated by a kind of
/// "global" light source known as the ambient light source.
/// The ambient color determines what the object will look like
/// when it is not illuminated by the light source. If the
/// object should not be visible in this case, the ambient
/// color can be set to black. The specular color of a light
/// source determines what highlights on an object will look
/// like. Take a shiny ball as an example: when a high intensity
/// light is shined directly at it, a spot (usually the color
/// of the light itself) can be seen at a certain point on
/// the ball where all of the incident light rays are reflected
/// into the eye. The specular color is normally set to the
/// same hue as the diffuse color although with a much higher
/// intensity depending on the effect to be achieved.
///
/// As with light in real life, the intensity of the light
/// decreases with increasing distance from the source. This
/// can be modelled in cpp3ds::Light as well using the provided
/// attenuation factors.
/// When the graphics hardware lights a vertex, it determines
/// how much of the source intensity arrives at the vertex
/// using the following formula:
///
/// factor = 1.f / (constant + linear * d + quadratic * d * d)
///
/// \li \e d is the distance between the light source and the vertex
/// \li \e constant is the constant attenuation factor
/// \li \e linear is the linear attenuation factor
/// \li \e quadratic is the quadratic attenuation factor
///
/// Using that factor it can modify the incoming light color
/// to simulate attenuation.
///
/// To use lights, lighting has to be enabled globally and
/// each light to be used has to be enabled. Objects that are
/// drawn are only affected by enabled lights and only when
/// lighting is enabled. To selectively light certain objects
/// with certain lights, just enable and disable them accordingly.
///
/// All objects to be lit need to have vertex normals available.
/// If the normals are not set properly, lighting will not function
/// as intended. The various cpp3ds::Polyhedron classes have methods
/// of specifying/generating the required normals. 2D objects have
/// their normals automatically set to point in the correct direction.
///
/// It is important to note that the position of the light is
/// affected by the current transform on the modelview matrix
/// stack <em>when it is set</em>. This means that when using
/// a moving camera and moving light source, the camera should
/// be updated using setView \e before setting the position of the
/// light to its new value every frame. Failure to do so will
/// result in wrong light positions.
///
/// A technical detail to keep in mind is that cpp3ds::Light makes
/// use of the fixed-function OpenGL lighting functionality.
/// This means that:
/// \li The number of light sources you can create with cpp3ds::Light is limited
/// \li Lighting is calculated <em>per vertex</em> and not per fragment
/// \li Anything other than the Blinn-Phong shading model is not supported
/// If you want to overcome these limitations, you will have to
/// resort to using shaders to perform your own lighting computation.
///
/// Usage example:
/// \code
/// cpp3ds::RenderWindow window;
///
/// // Declare a light
/// cpp3ds::Light light;
///
/// Set the light's properties
/// light.setDirectional(false);
/// light.setAmbientColor(cpp3ds::Color(50, 50, 50));
/// light.setLinearAttenuation(0.002f);
///
/// // Update the camera with its new position if needed
/// window.setView(camera);
///
/// // Set the light's position
/// light.setPosition(lightPosition);
///
/// // Enable lighting globally
/// cpp3ds::Light::enableLighting();
///
/// // Enable the light for the objects which should be lit by the light
/// light.enable();
///
/// // Render lit stuff
/// window.draw(litModel);
///
/// // Disable the light for the objects which should not be lit the light
/// light.disable();
///
/// // Render unlit stuff
/// window.draw(unlitModel);
///
/// // Disable lighting globally for certain objects e.g. text, GUIs
/// // They will be drawn using their full color intensity
/// cpp3ds::Light::disableLighting();
///
/// // Render GUI
/// window.draw(theGUI);
/// \endcode
///
////////////////////////////////////////////////////////////
