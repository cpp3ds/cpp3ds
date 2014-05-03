#ifndef CPP3DS_JOYSTICK_HPP
#define CPP3DS_JOYSTICK_HPP

#include <cpp3ds/System/String.hpp>

namespace cpp3ds {
////////////////////////////////////////////////////////////
/// \brief Give access to the real-time state of the joysticks
///
////////////////////////////////////////////////////////////
class Joystick {
public :

    ////////////////////////////////////////////////////////////
    /// \brief Constants related to joysticks capabilities
    ///
    ////////////////////////////////////////////////////////////
    enum {
        Count       = 8,  ///< Maximum number of supported joysticks
        ButtonCount = 32, ///< Maximum number of supported buttons
        AxisCount   = 8   ///< Maximum number of supported axes
    };

    ////////////////////////////////////////////////////////////
    /// \brief Axes supported by SFML joysticks
    ///
    ////////////////////////////////////////////////////////////
    enum Axis {
        X,    ///< The X axis
        Y,    ///< The Y axis
        Z,    ///< The Z axis
        R,    ///< The R axis
        U,    ///< The U axis
        V,    ///< The V axis
        PovX, ///< The X axis of the point-of-view hat
        PovY  ///< The Y axis of the point-of-view hat
    };

    ////////////////////////////////////////////////////////////
    /// \brief Structure holding a joystick's identification
    ///
    ////////////////////////////////////////////////////////////
    struct Identification {
        Identification();

        cpp3ds::String   name;      ///< Name of the joystick
        unsigned int vendorId;  ///< Manufacturer identifier
        unsigned int productId; ///< Product identifier
    };

    ////////////////////////////////////////////////////////////
    /// \brief Check if a joystick is connected
    ///
    /// \param joystick Index of the joystick to check
    ///
    /// \return True if the joystick is connected, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isConnected(unsigned int joystick);

    ////////////////////////////////////////////////////////////
    /// \brief Return the number of buttons supported by a joystick
    ///
    /// If the joystick is not connected, this function returns 0.
    ///
    /// \param joystick Index of the joystick
    ///
    /// \return Number of buttons supported by the joystick
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int getButtonCount(unsigned int joystick);

    ////////////////////////////////////////////////////////////
    /// \brief Check if a joystick supports a given axis
    ///
    /// If the joystick is not connected, this function returns false.
    ///
    /// \param joystick Index of the joystick
    /// \param axis     Axis to check
    ///
    /// \return True if the joystick supports the axis, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool hasAxis(unsigned int joystick, Axis axis);

    ////////////////////////////////////////////////////////////
    /// \brief Check if a joystick button is pressed
    ///
    /// If the joystick is not connected, this function returns false.
    ///
    /// \param joystick Index of the joystick
    /// \param button   Button to check
    ///
    /// \return True if the button is pressed, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isButtonPressed(unsigned int joystick, unsigned int button);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current position of a joystick axis
    ///
    /// If the joystick is not connected, this function returns 0.
    ///
    /// \param joystick Index of the joystick
    /// \param axis     Axis to check
    ///
    /// \return Current position of the axis, in range [-100 .. 100]
    ///
    ////////////////////////////////////////////////////////////
    static float getAxisPosition(unsigned int joystick, Axis axis);

    ////////////////////////////////////////////////////////////
    /// \brief Get the joystick information
    ///
    /// \param joystick Index of the joystick
    ///
    /// \return Structure containing joystick information.
    ///
    ////////////////////////////////////////////////////////////
    static Identification getIdentification(unsigned int joystick);

    ////////////////////////////////////////////////////////////
    /// \brief Update the states of all joysticks
    ///
    /// This function is used internally by SFML, so you normally
    /// don't have to call it explicitely. However, you may need to
    /// call it if you have no window yet (or no window at all):
    /// in this case the joysticks states are not updated automatically.
    ///
    ////////////////////////////////////////////////////////////
    static void update();
};

}


#endif


////////////////////////////////////////////////////////////
/// \class cpp3ds::Joystick
/// \ingroup window
///
/// cpp3ds::Joystick provides an interface to the state of the
/// joysticks. It only contains static functions, so it's not
/// meant to be instantiated. Instead, each joystick is identified
/// by an index that is passed to the functions of this class.
///
/// This class allows users to query the state of joysticks at any
/// time and directly, without having to deal with a window and
/// its events. Compared to the JoystickMoved, JoystickButtonPressed
/// and JoystickButtonReleased events, cpp3ds::Joystick can retrieve the
/// state of axes and buttons of joysticks at any time
/// (you don't need to store and update a boolean on your side
/// in order to know if a button is pressed or released), and you
/// always get the real state of joysticks, even if they are
/// moved, pressed or released when your window is out of focus
/// and no event is triggered.
///
/// SFML supports:
/// \li 8 joysticks (cpp3ds::Joystick::Count)
/// \li 32 buttons per joystick (cpp3ds::Joystick::ButtonCount)
/// \li 8 axes per joystick (cpp3ds::Joystick::AxisCount)
///
/// Unlike the keyboard or mouse, the state of joysticks is sometimes
/// not directly available (depending on the OS), therefore an update()
/// function must be called in order to update the current state of
/// joysticks. When you have a window with event handling, this is done
/// automatically, you don't need to call anything. But if you have no
/// window, or if you want to check joysticks state before creating one,
/// you must call cpp3ds::Joystick::update explicitely.
///
/// Usage example:
/// \code
/// // Is joystick #0 connected?
/// bool connected = cpp3ds::Joystick::isConnected(0);
///
/// // How many buttons does joystick #0 support?
/// unsigned int buttons = cpp3ds::Joystick::getButtonCount(0);
///
/// // Does joystick #0 define a X axis?
/// bool hasX = cpp3ds::Joystick::hasAxis(0, cpp3ds::Joystick::X);
///
/// // Is button #2 pressed on joystick #0?
/// bool pressed = cpp3ds::Joystick::isButtonPressed(0, 2);
///
/// // What's the current position of the Y axis on joystick #0?
/// float position = cpp3ds::Joystick::getAxisPosition(0, cpp3ds::Joystick::Y);
/// \endcode
///
/// \see cpp3ds::Keyboard, cpp3ds::Touch
///
////////////////////////////////////////////////////////////
