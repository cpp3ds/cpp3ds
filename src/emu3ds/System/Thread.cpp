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


namespace cpp3ds
{
////////////////////////////////////////////////////////////
void Thread::initialize()
{

}


////////////////////////////////////////////////////////////
Thread::~Thread()
{
    delete m_thread;
    delete m_entryPoint;
}


////////////////////////////////////////////////////////////
void Thread::launch()
{
    m_thread->launch();
}


////////////////////////////////////////////////////////////
void Thread::wait()
{
    m_thread->wait();
}


////////////////////////////////////////////////////////////
void Thread::terminate()
{
    m_thread->terminate();
}


////////////////////////////////////////////////////////////
void Thread::run()
{
}


////////////////////////////////////////////////////////////
void Thread::entryPoint(void* userData)
{
}

void Thread::setStackSize(size_t stacksize)
{
    m_stackSize = stacksize;
}

void Thread::setPriority(int priority)
{
    m_priority = priority;
}

void Thread::setRelativePriority(int priority)
{
    m_priority = priority;
}

void Thread::setAffinity(int affinity)
{
    m_affinity = affinity;
}

} // namespace cpp3ds
