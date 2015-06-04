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
#include <cpp3ds/System/Clock.hpp>
#include <3ds.h>

// TODO: Update for New 3DS?
#define TICKS_PER_SEC 268123480

namespace cpp3ds
{
namespace priv {

	static Time getCurrentTime()
	{
		return seconds(static_cast<float>(svcGetSystemTick())/TICKS_PER_SEC);
	}
}

////////////////////////////////////////////////////////////
Clock::Clock():
m_startTime(priv::getCurrentTime())
{
}


////////////////////////////////////////////////////////////
Time Clock::getElapsedTime() const
{
    return priv::getCurrentTime() - m_startTime;
}


////////////////////////////////////////////////////////////
Time Clock::restart()
{
    Time now = priv::getCurrentTime();
    Time elapsed = now - m_startTime;
    m_startTime = now;

    return elapsed;
}

} // namespace cpp3ds
