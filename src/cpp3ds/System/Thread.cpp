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
#include <cpp3ds/System/Thread.hpp>
#include <malloc.h>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
void Thread::initialize()
{
	threadStack = (u32*)memalign(32, 4 * 1024);
}


////////////////////////////////////////////////////////////
Thread::~Thread()
{
    wait();
	free(threadStack);
    delete m_entryPoint;
}


////////////////////////////////////////////////////////////
void Thread::launch()
{
    wait();
	svcCreateThread(&m_thread, &Thread::entryPoint, (u32)this, &threadStack[1024], 0x3F, 0);
}


////////////////////////////////////////////////////////////
void Thread::wait()
{
	// TODO: check that this isn't current thread asking to wait for itself
    if (m_thread != NULL)
	{
		svcWaitSynchronization(m_thread, U64_MAX);
		svcCloseHandle(m_thread);
		m_thread = NULL;
	}
}


////////////////////////////////////////////////////////////
void Thread::terminate()
{
    if (m_thread != NULL)
    {
		// TODO: implement this
    }
}


////////////////////////////////////////////////////////////
void Thread::run()
{
    m_entryPoint->run();
}


////////////////////////////////////////////////////////////
void Thread::entryPoint(void* userData)
{
	// The Thread instance is stored in the user data
	Thread* owner = static_cast<Thread*>(userData);

	// Forward to the owner
	owner->run();

	svcExitThread();
}

} // namespace cpp3ds
