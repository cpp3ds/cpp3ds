#ifndef CPP3DS_TOUCH_HPP
#define CPP3DS_TOUCH_HPP

#include <cpp3ds/System/Vector2.hpp>

namespace cpp3ds {
class Window;

////////////////////////////////////////////////////////////
/// \brief Give access to the real-time state of the touches
///
////////////////////////////////////////////////////////////
class Touch {
public :

    ////////////////////////////////////////////////////////////
    /// \brief Check if a touch event is currently down
    ///
    /// \return True if \a finger is currently touching the screen, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isDown();

    ////////////////////////////////////////////////////////////
    /// \brief Get the current position of a touch in desktop coordinates
    ///
    /// This function returns the current touch position.
    ///
    /// \return Current position of \a finger, or undefined if it's not down
    ///
    ////////////////////////////////////////////////////////////
    static Vector2i getPosition();
};

}

#endif


////////////////////////////////////////////////////////////
/// \class cpp3ds::Touch
/// \ingroup window
///
/// cpp3ds::Touch provides an interface to the state of the
/// touches. It only contains static functions, so it's not
/// meant to be instanciated.
///
/// This class allows users to query the touches state at any
/// time and directly, without having to deal with a window and
/// its events. Compared to the TouchBegan, TouchMoved
/// and TouchEnded events, cpp3ds::Touch can retrieve the
/// state of the touches at any time (you don't need to store and
/// update a boolean on your side in order to know if a touch is down),
/// and you always get the real state of the touches, even if they
/// happen when your window is out of focus and no event is triggered.
///
/// The getPosition function can be used to retrieve the current
/// position of a touch. There are two versions: one that operates
/// in global coordinates (relative to the desktop) and one that
/// operates in window coordinates (relative to a specific window).
///
/// Touches are identified by an index (the "finger"), so that in
/// multi-touch events, individual touches can be tracked correctly.
/// As long as a finger touches the screen, it will keep the same index
/// even if other fingers start or stop touching the screen in the
/// meantime. As a consequence, active touch indices may not always be
/// sequential (i.e. touch number 0 may be released while touch number 1
/// is still down).
///
/// Usage example:
/// \code
/// if (cpp3ds::Touch::isDown())
/// {
///     // screen is being touched
/// }
///
/// // get global position of touch
/// cpp3ds::Vector2i globalPos = cpp3ds::Touch::getPosition();
/// \endcode
///
/// \see cpp3ds::Joystick, cpp3ds::Keyboard, cpp3ds::Sensor
///
////////////////////////////////////////////////////////////
