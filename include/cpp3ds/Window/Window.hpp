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
class Window : public RenderTarget, GlResource
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
	/// \brief Create (or recreate) the window
	///
	/// If the window was already created, it closes it first.
	///
	/// The parameter is an optional structure specifying
	/// advanced OpenGL context settings such as antialiasing,
	/// depth-buffer bits, etc.
	///
	/// \param settings Additional settings for the underlying OpenGL context
	///
	////////////////////////////////////////////////////////////
	void create(const ContextSettings& settings = ContextSettings());

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
	/// \brief Get the settings of the OpenGL context of the window
	///
	/// Note that these settings may be different from what was
	/// passed to the constructor or the create() function,
	/// if one or more settings were not supported. In this case,
	/// SFML chose the closest match.
	///
	/// \return Structure containing the OpenGL context settings
	///
	////////////////////////////////////////////////////////////
	const ContextSettings& getSettings() const;

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
    virtual Vector2u getSize() const;

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

	////////////////////////////////////////////////////////////
	/// \brief Copy the current contents of the window to an image
	///
	/// This is a slow operation, whose main purpose is to make
	/// screenshots of the application. If you want to update an
	/// image with the contents of the window and then use it for
	/// drawing, you should rather use a sf::Texture and its
	/// update(Window&) function.
	/// You can also draw things directly to a texture with the
	/// sf::RenderTexture class.
	///
	/// \return Image containing the captured contents
	///
	////////////////////////////////////////////////////////////
	Image capture() const;

private:

	////////////////////////////////////////////////////////////
	/// \brief Activate the target for rendering
	///
	/// \param active True to make the target active, false to deactivate it
	///
	/// \return True if the function succeeded
	///
	////////////////////////////////////////////////////////////
	virtual bool activate(bool active);

protected:

    ////////////////////////////////////////////////////////////
    /// \brief Perform some common internal initializations
    ///
    ////////////////////////////////////////////////////////////
    void initialize();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
	priv::GlContext*  m_context;        ///< Platform-specific implementation of the OpenGL context
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
