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
#include <cpp3ds/Window/GlContext.hpp>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Window::Window() :
m_frameTimeLimit(Time::Zero),
m_size          (0, 0)
{
	// Perform common initializations
	initialize();
}


////////////////////////////////////////////////////////////
Window::~Window()
{
    close();
}


////////////////////////////////////////////////////////////
void Window::create(const ContextSettings& settings)
{
	// Destroy the previous window implementation
	close();

	unsigned int width  = settings.screen == TopScreen ? 400 : 400;
	unsigned int height = 240;

	m_size.x = width;
	m_size.y = height;

	// Recreate the context
	m_context = priv::GlContext::create(settings, width, height);

	// Perform common initializations
	initialize();
}


////////////////////////////////////////////////////////////
void Window::close()
{
}


////////////////////////////////////////////////////////////
bool Window::isOpen() const
{
    return m_context != NULL;
//	return true;
}


////////////////////////////////////////////////////////////
const ContextSettings& Window::getSettings() const
{
	static const ContextSettings empty(TopScreen, 0, 0, 0);

	return m_context ? m_context->getSettings() : empty;
}


////////////////////////////////////////////////////////////
bool Window::activate(bool active)
{
	return setActive(active);
}


////////////////////////////////////////////////////////////
bool Window::setActive(bool active) const
{
	if (m_context)
	{
		if (m_context->setActive(active))
		{
			return true;
		}
		else
		{
			err() << "Failed to activate the window's context" << std::endl;
			return false;
		}
	}
	else
	{
		return false;
	}
}


////////////////////////////////////////////////////////////
Vector2u Window::getSize() const
{
	return m_size;
}


////////////////////////////////////////////////////////////
Image Window::capture() const
{
	Image image;
	if (setActive())
	{
		int width = static_cast<int>(getSize().x);
		int height = static_cast<int>(getSize().y);

		// copy rows one by one and flip them (OpenGL's origin is bottom while SFML's origin is top)
		std::vector<Uint8> pixels(width * height * 4);
		for (int i = 0; i < height; ++i)
		{
			Uint8* ptr = &pixels[i * width * 4];
			// TODO: implement glReadPixels in gl3ds
//			glCheck(glReadPixels(0, height - i - 1, width, 1, GL_RGBA, GL_UNSIGNED_BYTE, ptr));
		}

		image.create(width, height, &pixels[0]);
	}

	return image;
}


////////////////////////////////////////////////////////////
void Window::setVerticalSyncEnabled(bool enabled)
{
//    if (setActive())
//        m_context->setVerticalSyncEnabled(enabled);
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
void Window::display()
{
	// Display the backbuffer on screen
	if (setActive())
		m_context->display();

    // Limit the framerate if needed
    if (m_frameTimeLimit != Time::Zero)
    {
        sleep(m_frameTimeLimit - m_clock.getElapsedTime());
        m_clock.restart();
    }
}


////////////////////////////////////////////////////////////
void Window::initialize()
{
    // Setup default behaviours (to get a consistent behaviour across different implementations)
    setVerticalSyncEnabled(false);
    setFramerateLimit(0);

    // Reset frame time
    m_clock.restart();

	// Just initialize the render target part
	RenderTarget::initialize();
}

} // namespace cpp3ds
