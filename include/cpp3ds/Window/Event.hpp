#ifndef CPP3DS_EVENT_HPP
#define CPP3DS_EVENT_HPP

#include <cpp3ds/Config.hpp>
#include <cpp3ds/Window/Keyboard.hpp>
#include <cpp3ds/Window/Sensor.hpp>


namespace cpp3ds {
////////////////////////////////////////////////////////////
/// \brief Defines a system event and its parameters
///
////////////////////////////////////////////////////////////
class Event {
public :

    ////////////////////////////////////////////////////////////
    /// \brief Keyboard event parameters (KeyPressed, KeyReleased)
    ///
    ////////////////////////////////////////////////////////////
    struct KeyEvent {
        Keyboard::Key code;    ///< Code of the key(s) that has been pressed
    };

    ////////////////////////////////////////////////////////////
    /// \brief Joystick axis move event parameters (JoystickMoved)
    ///
    ////////////////////////////////////////////////////////////
    struct JoystickMoveEvent {
        int x;   ///< New position on the x-axis (in range [-100 .. 100])
		int y;   ///< New position on the y-axis (in range [-100 .. 100])
    };

    ////////////////////////////////////////////////////////////
    /// \brief Touch events parameters (TouchBegan, TouchMoved, TouchEnded)
    ///
    ////////////////////////////////////////////////////////////
    struct TouchEvent {
        int x;               ///< X position of the touch, relative to the left of the owner window
        int y;               ///< Y position of the touch, relative to the top of the owner window
    };

    ////////////////////////////////////////////////////////////
    /// \brief Sensor event parameters (SensorChanged)
    ///
    ////////////////////////////////////////////////////////////
    struct SensorEvent {
		Sensor::Type type; ///< Type of the sensor
		float x;           ///< Current value of the sensor on X axis
		float y;           ///< Current value of the sensor on Y axis
		float z;           ///< Current value of the sensor on Z axis
    };

	////////////////////////////////////////////////////////////
	/// \brief Slider event parameters (Slider3DChanged, SliderVolumeChanged)
	///
	////////////////////////////////////////////////////////////
	struct SliderEvent {
		float value;       ///< Current value of the slider [0-1] range
	};

    ////////////////////////////////////////////////////////////
    /// \brief Enumeration of the different types of events
    ///
    ////////////////////////////////////////////////////////////
    enum EventType {
        KeyPressed,             ///< A key was pressed (data in event.key)
        KeyReleased,            ///< A key was released (data in event.key)
        JoystickMoved,          ///< The joystick moved along an axis (data in event.joystickMove)
        TouchBegan,             ///< A touch event began (data in event.touch)
        TouchMoved,             ///< A touch moved (data in event.touch)
        TouchEnded,             ///< A touch event ended (data in event.touch)
        TouchSwiped,
        SensorChanged,          ///< A sensor value changed (data in event.sensor)
		Slider3DChanged,
		SliderVolumeChanged,

        Count                   ///< Keep last -- the total number of event types
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    EventType type; ///< Type of the event

    union {
        KeyEvent             key;             ///< Key event parameters (Event::KeyPressed, Event::KeyReleased)
        JoystickMoveEvent    joystickMove;    ///< Joystick move event parameters (Event::JoystickMoved)
        TouchEvent           touch;           ///< Touch events parameters (Event::TouchBegan, Event::TouchMoved, Event::TouchEnded)
        SensorEvent          sensor;          ///< Sensor event parameters (Event::SensorChanged)
    	SliderEvent          slider;
	};
};

}


#endif


////////////////////////////////////////////////////////////
/// \class cpp3ds::Event
/// \ingroup window
///
/// cpp3ds::Event holds all the informations about a system event
/// that just happened. Events are retrieved using the
/// cpp3ds::Window::pollEvent and cpp3ds::Window::waitEvent functions.
///
/// A cpp3ds::Event instance contains the type of the event
/// (mouse moved, key pressed, window closed, ...) as well
/// as the details about this particular event. Please note that
/// the event parameters are defined in a union, which means that
/// only the member matching the type of the event will be properly
/// filled; all other members will have undefined values and must not
/// be read if the type of the event doesn't match. For example,
/// if you received a KeyPressed event, then you must read the
/// event.key member, all other members such as event.MouseMove
/// or event.text will have undefined values.
///
/// Usage example:
/// \code
/// cpp3ds::Event event;
/// while (window.pollEvent(event))
/// {
///     // Request for closing the window
///     if (event.type == cpp3ds::Event::Closed)
///         window.close();
///
///     // The escape key was pressed
///     if ((event.type == cpp3ds::Event::KeyPressed) && (event.key.code == cpp3ds::Keyboard::Escape))
///         window.close();
///
///     // The window was resized
///     if (event.type == cpp3ds::Event::Resized)
///         doSomethingWithTheNewSize(event.size.width, event.size.height);
///
///     // etc ...
/// }
/// \endcode
///
////////////////////////////////////////////////////////////
