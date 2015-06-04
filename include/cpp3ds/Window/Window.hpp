////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef CPP3DS_WINDOW_HPP
#define CPP3DS_WINDOW_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/System/Clock.hpp>
#include <cpp3ds/System/Vector2.hpp>
#include <cpp3ds/System/NonCopyable.hpp>
#include <cpp3ds/System/String.hpp>
#include <cpp3ds/Graphics/RenderTexture.hpp>


namespace cpp3ds
{

class Event;

////////////////////////////////////////////////////////////
/// \brief Window that serves as a target for OpenGL rendering
///
////////////////////////////////////////////////////////////
class Window : public RenderTexture
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor doesn't actually create the window,
    /// use the other constructors or call create() to do so.
    ///
    ////////////////////////////////////////////////////////////
    Window();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Closes the window and frees all the resources attached to it.
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Window();

    ////////////////////////////////////////////////////////////
	/// \brief Close the window and destroy all the attached resources
	///
	/// After calling this function, the sf::Window instance remains
	/// valid and you can call create() to recreate the window.
	/// All other functions such as pollEvent() or display() will
	/// still work (i.e. you don't have to test isOpen() every time),
	/// and will have no effect on closed windows.
	///
	////////////////////////////////////////////////////////////
	void close();

	////////////////////////////////////////////////////////////
	/// \brief Tell whether or not the window is open
	///
	/// This function returns whether or not the window exists.
	/// Note that a hidden window (setVisible(false)) is open
	/// (therefore this function would return true).
	///
	/// \return True if the window is open, false if it has been closed
	///
	////////////////////////////////////////////////////////////
	bool isOpen() const;


    ////////////////////////////////////////////////////////////
    /// \brief Pop the event on top of the event queue, if any, and return it
    ///
    /// This function is not blocking: if there's no pending event then
    /// it will return false and leave \a event unmodified.
    /// Note that more than one event may be present in the event queue,
    /// thus you should always call this function in a loop
    /// to make sure that you process every pending event.
    /// \code
    /// cpp3ds::Event event;
    /// while (window.pollEvent(event))
    /// {
    ///    // process event...
    /// }
    /// \endcode
    ///
    /// \param event Event to be returned
    ///
    /// \return True if an event was returned, or false if the event queue was empty
    ///
    /// \see waitEvent
    ///
    ////////////////////////////////////////////////////////////
    bool pollEvent(Event& event);

    ////////////////////////////////////////////////////////////
    /// \brief Wait for an event and return it
    ///
    /// This function is blocking: if there's no pending event then
    /// it will wait until an event is received.
    /// After this function returns (and no error occured),
    /// the \a event object is always valid and filled properly.
    /// This function is typically used when you have a thread that
    /// is dedicated to events handling: you want to make this thread
    /// sleep as long as no new event is received.
    /// \code
    /// cpp3ds::Event event;
    /// if (window.waitEvent(event))
    /// {
    ///    // process event...
    /// }
    /// \endcode
    ///
    /// \param event Event to be returned
    ///
    /// \return False if any error occured
    ///
    /// \see pollEvent
    ///
    ////////////////////////////////////////////////////////////
    bool waitEvent(Event& event);

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the rendering region of the window
    ///
    /// The size doesn't include the titlebar and borders
    /// of the window.
    ///
    /// \return Size in pixels
    ///
    /// \see setSize
    ///
    ////////////////////////////////////////////////////////////
    Vector2u getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable vertical synchronization
    ///
    /// Activating vertical synchronization will limit the number
    /// of frames displayed to the refresh rate of the monitor.
    /// This can avoid some visual artifacts, and limit the framerate
    /// to a good value (but not constant across different computers).
    ///
    /// Vertical synchronization is disabled by default.
    ///
    /// \param enabled True to enable v-sync, false to deactivate it
    ///
    ////////////////////////////////////////////////////////////
    void setVerticalSyncEnabled(bool enabled);

    ////////////////////////////////////////////////////////////
    /// \brief Show or hide the mouse cursor
    ///
    /// The mouse cursor is visible by default.
    ///
    /// \param visible True to show the mouse cursor, false to hide it
    ///
    ////////////////////////////////////////////////////////////
    void setMouseCursorVisible(bool visible);

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable automatic key-repeat
    ///
    /// If key repeat is enabled, you will receive repeated
    /// KeyPressed events while keeping a key pressed. If it is disabled,
    /// you will only get a single event when the key is pressed.
    ///
    /// Key repeat is enabled by default.
    ///
    /// \param enabled True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    void setKeyRepeatEnabled(bool enabled);

    ////////////////////////////////////////////////////////////
    /// \brief Limit the framerate to a maximum fixed frequency
    ///
    /// If a limit is set, the window will use a small delay after
    /// each call to display() to ensure that the current frame
    /// lasted long enough to match the framerate limit.
    /// SFML will try to match the given limit as much as it can,
    /// but since it internally uses cpp3ds::sleep, whose precision
    /// depends on the underlying OS, the results may be a little
    /// unprecise as well (for example, you can get 65 FPS when
    /// requesting 60).
    ///
    /// \param limit Framerate limit, in frames per seconds (use 0 to disable limit)
    ///
    ////////////////////////////////////////////////////////////
    void setFramerateLimit(unsigned int limit);

    ////////////////////////////////////////////////////////////
    /// \brief Change the joystick threshold
    ///
    /// The joystick threshold is the value below which
    /// no JoystickMoved event will be generated.
    ///
    /// The threshold value is 0.1 by default.
    ///
    /// \param threshold New threshold, in the range [0, 100]
    ///
    ////////////////////////////////////////////////////////////
    void setJoystickThreshold(float threshold);

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate the window as the current target
    ///        for OpenGL rendering
    ///
    /// A window is active only on the current thread, if you want to
    /// make it active on another thread you have to deactivate it
    /// on the previous thread first if it was active.
    /// Only one window can be active on a thread at a time, thus
    /// the window previously active (if any) automatically gets deactivated.
    ///
    /// \param active True to activate, false to deactivate
    ///
    /// \return True if operation was successful, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool setActive(bool active = true) const;

    ////////////////////////////////////////////////////////////
    /// \brief Display on screen what has been rendered to the window so far
    ///
    /// This function is typically called after all OpenGL rendering
    /// has been done for the current frame, in order to show
    /// it on screen.
    ///
    ////////////////////////////////////////////////////////////
    void display();

protected:

    ////////////////////////////////////////////////////////////
    /// \brief Processes an event before it is sent to the user
    ///
    /// This function is called every time an event is received
    /// from the internal window (through pollEvent or waitEvent).
    /// It filters out unwanted events, and performs whatever internal
    /// stuff the window needs before the event is returned to the
    /// user.
    ///
    /// \param event Event to filter
    ///
    ////////////////////////////////////////////////////////////
    bool filterEvent(const Event& event);

    ////////////////////////////////////////////////////////////
    /// \brief Perform some common internal initializations
    ///
    ////////////////////////////////////////////////////////////
    void initialize();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Clock             m_clock;          ///< Clock for measuring the elapsed time between frames
    Time              m_frameTimeLimit; ///< Current framerate limit
    Vector2u          m_size;           ///< Current size of the window
};

}


#endif


////////////////////////////////////////////////////////////
/// \class cpp3ds::Window
/// \ingroup window
///
/// cpp3ds::Window is the main class of the Window module. It defines
/// an OS window that is able to receive an OpenGL rendering.
///
/// A cpp3ds::Window can create its own new window, or be embedded into
/// an already existing control using the create(handle) function.
/// This can be useful for embedding an OpenGL rendering area into
/// a view which is part of a bigger GUI with existing windows,
/// controls, etc. It can also serve as embedding an OpenGL rendering
/// area into a window created by another (probably richer) GUI library
/// like Qt or wxWidgets.
///
/// The cpp3ds::Window class provides a simple interface for manipulating
/// the window: move, resize, show/hide, control mouse cursor, etc.
/// It also provides event handling through its pollEvent() and waitEvent()
/// functions.
///
/// Note that OpenGL experts can pass their own parameters (antialiasing
/// level, bits for the depth and stencil buffers, etc.) to the
/// OpenGL context attached to the window, with the cpp3ds::ContextSettings
/// structure which is passed as an optional argument when creating the
/// window.
///
/// Usage example:
/// \code
/// // Declare and create a new window
/// cpp3ds::Window window(cpp3ds::VideoMode(800, 600), "SFML window");
///
/// // Limit the framerate to 60 frames per second (this step is optional)
/// window.setFramerateLimit(60);
///
/// // The main loop - ends as soon as the window is closed
/// while (window.isOpen())
/// {
///    // Event processing
///    cpp3ds::Event event;
///    while (window.pollEvent(event))
///    {
///        // Request for closing the window
///        if (event.type == cpp3ds::Event::Closed)
///            window.close();
///    }
///
///    // Activate the window for OpenGL rendering
///    window.setActive();
///
///    // OpenGL drawing commands go here...
///
///    // End the current frame and display its contents on screen
///    window.display();
/// }
/// \endcode
///
////////////////////////////////////////////////////////////
