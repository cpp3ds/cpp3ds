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
#include <iostream>

namespace cpp3ds
{
////////////////////////////////////////////////////////////
void Thread::initialize()
{
	m_stackSize = 32 * 1024;
	m_affinity = -2;
	setRelativePriority(1);
}


////////////////////////////////////////////////////////////
Thread::~Thread()
{
    wait();
    delete m_entryPoint;
}


////////////////////////////////////////////////////////////
void Thread::launch()
{
    wait();
	m_thread = threadCreate(&Thread::entryPoint, this, m_stackSize, m_priority, m_affinity, false);
}


////////////////////////////////////////////////////////////
void Thread::wait()
{
	// TODO: check that this isn't current thread asking to wait for itself
	if (m_thread != nullptr) {
		threadJoin(m_thread, U64_MAX);
		threadFree(m_thread);
		m_thread = nullptr;
	}
}


////////////////////////////////////////////////////////////
void Thread::terminate()
{
    if (m_thread != nullptr)
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

}

void Thread::setStackSize(size_t stacksize)
{
	m_stackSize = stacksize;
}

void Thread::setPriority(int priority)
{
	m_priority = priority;
}

void Thread::setRelativePriority(int relPriority)
{
	s32 priority;
	svcGetThreadPriority(&priority, CUR_THREAD_HANDLE);
	priority += relPriority;
	if (priority < 0x18) priority = 0x18;
	if (priority > 0x3F) priority = 0x3F;
	m_priority = priority;
}

void Thread::setAffinity(int affinity)
{
	m_affinity = affinity;
}

} // namespace cpp3ds
