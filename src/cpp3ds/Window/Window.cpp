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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Window/Window.hpp>
#include <cpp3ds/System/Sleep.hpp>
#include <cpp3ds/System/Err.hpp>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Window::Window() :
m_frameTimeLimit(Time::Zero),
m_size          (400, 240)
{
	// Perform common initializations
	initialize();
	RenderTarget::initialize();
}


////////////////////////////////////////////////////////////
Window::~Window()
{
    close();
}


////////////////////////////////////////////////////////////
void Window::close()
{
}


////////////////////////////////////////////////////////////
bool Window::isOpen() const
{
//    return m_impl != NULL;
	return true;
}


////////////////////////////////////////////////////////////
bool Window::pollEvent(Event& event)
{
//    if (m_impl && m_impl->popEvent(event, false))
//    {
//        return filterEvent(event);
//    }
//    else
//    {
        return false;
//    }
}


////////////////////////////////////////////////////////////
bool Window::waitEvent(Event& event)
{
//    if (m_impl && m_impl->popEvent(event, true))
//    {
//        return filterEvent(event);
//    }
//    else
//    {
        return false;
//    }
}


////////////////////////////////////////////////////////////
Vector2u Window::getSize() const
{
	return m_size;
//    return RenderTexture::getSize();
}


////////////////////////////////////////////////////////////
void Window::setVerticalSyncEnabled(bool enabled)
{
//    if (setActive())
//        m_context->setVerticalSyncEnabled(enabled);
}


////////////////////////////////////////////////////////////
void Window::setMouseCursorVisible(bool visible)
{
//    if (m_impl)
//        m_impl->setMouseCursorVisible(visible);
}


////////////////////////////////////////////////////////////
void Window::setKeyRepeatEnabled(bool enabled)
{
//    if (m_impl)
//        m_impl->setKeyRepeatEnabled(enabled);
}


////////////////////////////////////////////////////////////
void Window::setFramerateLimit(unsigned int limit)
{
    if (limit > 0)
        m_frameTimeLimit = seconds(1.f / limit);
    else
        m_frameTimeLimit = Time::Zero;
}


////////////////////////////////////////////////////////////
void Window::setJoystickThreshold(float threshold)
{
//    if (m_impl)
//        m_impl->setJoystickThreshold(threshold);
}


////////////////////////////////////////////////////////////
void Window::display()
{
    // Display the backbuffer on screen
    RenderTexture::display();

    // Limit the framerate if needed
    if (m_frameTimeLimit != Time::Zero)
    {
        sleep(m_frameTimeLimit - m_clock.getElapsedTime());
        m_clock.restart();
    }
}


////////////////////////////////////////////////////////////
bool Window::filterEvent(const Event& event)
{
//    // Notify resize events to the derived class
//    if (event.type == Event::Resized)
//    {
//        // Cache the new size
//        m_size.x = event.size.width;
//        m_size.y = event.size.height;
//
//        // Notify the derived class
//        onResize();
//    }

    return true;
}


////////////////////////////////////////////////////////////
void Window::initialize()
{
    // Setup default behaviours (to get a consistent behaviour across different implementations)
    setMouseCursorVisible(true);
    setVerticalSyncEnabled(false);
    setKeyRepeatEnabled(false);
    setFramerateLimit(0);

    // Get and cache the initial size of the window
//    m_size = m_impl->getSize();

    // Reset frame time
    m_clock.restart();
}

} // namespace cpp3ds
