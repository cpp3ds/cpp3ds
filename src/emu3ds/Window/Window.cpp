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
#include <cpp3ds/Emulator.hpp>
#include <cpp3ds/Window/Window.hpp>
#include <cpp3ds/Graphics/Sprite.hpp>
#include <cpp3ds/System/Sleep.hpp>
#include <cpp3ds/System/Err.hpp>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Window::Window() :
m_frameTimeLimit(Time::Zero)
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
	unsigned int width  = settings.screen == TopScreen ? 400 : 320;
	unsigned int height = 240;

	m_size.x = width;
	m_size.y = height;

	// Perform common initializations
	initialize();
}


////////////////////////////////////////////////////////////
void Window::close()
{
#ifndef TEST
	_emulator->stop();
#endif
}


////////////////////////////////////////////////////////////
bool Window::isOpen() const {
#ifndef TEST
	return _emulator->getState() != EMU_STOPPED;
#endif
}


////////////////////////////////////////////////////////////
const ContextSettings& Window::getSettings() const
{
	static const ContextSettings empty(TopScreen, 0, 0, 0);
	// TODO: store settings or fetch from emu window?
	return empty;
}


////////////////////////////////////////////////////////////
Vector2u Window::getSize() const
{
	return m_size;
}


////////////////////////////////////////////////////////////
void Window::setVerticalSyncEnabled(bool enabled)
{
#ifndef TEST
	_emulator->screen->setVerticalSyncEnabled(enabled);
#endif
}


////////////////////////////////////////////////////////////
void Window::setFramerateLimit(unsigned int limit)
{
#ifndef TEST
	_emulator->screen->setFramerateLimit(limit);
#endif
}


////////////////////////////////////////////////////////////
bool Window::activate(bool active)
{
	return setActive(active);
}


////////////////////////////////////////////////////////////
bool Window::setActive(bool active) const
{
	return true;
}


////////////////////////////////////////////////////////////
void Window::display()
{

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
