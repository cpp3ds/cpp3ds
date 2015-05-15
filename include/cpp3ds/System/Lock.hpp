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

#ifndef CPP3DS_LOCK_HPP
#define CPP3DS_LOCK_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/System/NonCopyable.hpp>


namespace cpp3ds
{
class Mutex;

////////////////////////////////////////////////////////////
/// \brief Automatic wrapper for locking and unlocking mutexes
///
////////////////////////////////////////////////////////////
class Lock : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Construct the lock with a target mutex
    ///
    /// The mutex passed to cpp3ds::Lock is automatically locked.
    ///
    /// \param mutex Mutex to lock
    ///
    ////////////////////////////////////////////////////////////
    explicit Lock(Mutex& mutex);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// The destructor of cpp3ds::Lock automatically unlocks its mutex.
    ///
    ////////////////////////////////////////////////////////////
    ~Lock();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Mutex& m_mutex; ///< Mutex to lock / unlock
};

} // namespace cpp3ds


#endif


////////////////////////////////////////////////////////////
/// \class cpp3ds::Lock
/// \ingroup system
///
/// cpp3ds::Lock is a RAII wrapper for cpp3ds::Mutex. By unlocking
/// it in its destructor, it ensures that the mutex will
/// always be released when the current scope (most likely
/// a function) ends.
/// This is even more important when an exception or an early
/// return statement can interrupt the execution flow of the
/// function.
///
/// For maximum robustness, cpp3ds::Lock should always be used
/// to lock/unlock a mutex.
///
/// Usage example:
/// \code
/// cpp3ds::Mutex mutex;
/// 
/// void function()
/// {
///     cpp3ds::Lock lock(mutex); // mutex is now locked
/// 
///     functionThatMayThrowAnException(); // mutex is unlocked if this function throws
/// 
///     if (someCondition)
///         return; // mutex is unlocked
/// 
/// } // mutex is unlocked
/// \endcode
///
/// Because the mutex is not explicitely unlocked in the code,
/// it may remain locked longer than needed. If the region
/// of the code that needs to be protected by the mutex is
/// not the entire function, a good practice is to create a
/// smaller, inner scope so that the lock is limited to this
/// part of the code.
///
/// \code
/// cpp3ds::Mutex mutex;
/// 
/// void function()
/// {
///     {
///       cpp3ds::Lock lock(mutex);
///       codeThatRequiresProtection();
/// 
///     } // mutex is unlocked here
/// 
///     codeThatDoesntCareAboutTheMutex();
/// }
/// \endcode
///
/// Having a mutex locked longer than required is a bad practice
/// which can lead to bad performances. Don't forget that when
/// a mutex is locked, other threads may be waiting doing nothing
/// until it is released.
///
/// \see cpp3ds::Mutex
///
////////////////////////////////////////////////////////////
