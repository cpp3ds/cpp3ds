////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
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
#include <cpp3ds/Network/Socket.hpp>
#include <cpp3ds/Network/SocketImpl.hpp>
#include <cpp3ds/System/Err.hpp>
#include <stdlib.h>
#include <3ds/services/sslc.h>
#include <cpp3ds/System/Service.hpp>


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Socket::Socket(Type type, bool secure) :
m_type      (type),
m_socket    (priv::SocketImpl::invalidSocket()),
m_isBlocking(true),
m_isSecure  (secure)
{
}


////////////////////////////////////////////////////////////
Socket::~Socket()
{
    // Close the socket before it gets destructed
    close();
}


////////////////////////////////////////////////////////////
void Socket::setBlocking(bool blocking)
{
    // Apply if the socket is already created
    if (m_socket != priv::SocketImpl::invalidSocket())
        priv::SocketImpl::setBlocking(m_socket, blocking);

    m_isBlocking = blocking;
}


////////////////////////////////////////////////////////////
bool Socket::isBlocking() const
{
    return m_isBlocking;
}


////////////////////////////////////////////////////////////
void Socket::setSecure(bool secure)
{
    bool oldSecure = m_isSecure;
    m_isSecure = secure;

    if (secure)
        Service::enable(SSL);

    if (secure ^ oldSecure && m_socket != priv::SocketImpl::invalidSocket())
    {
        close();
        create();
    }
}


////////////////////////////////////////////////////////////
bool Socket::isSecure() const
{
    return m_isSecure;
}


////////////////////////////////////////////////////////////
SocketHandle Socket::getHandle() const
{
    return m_socket;
}


////////////////////////////////////////////////////////////
Socket::SecureData& Socket::getSecureData() const
{
    return m_secureData;
}


////////////////////////////////////////////////////////////
void Socket::create()
{
    if (m_socket == priv::SocketImpl::invalidSocket())
    {
        SocketHandle handle = socket(PF_INET, m_type == Tcp ? SOCK_STREAM : SOCK_DGRAM, IPPROTO_IP);
        create(handle);
    }
}


////////////////////////////////////////////////////////////
void Socket::create(SocketHandle handle)
{
    // Don't create the socket if it already exists
    if (m_socket == priv::SocketImpl::invalidSocket())
    {
        // Assign the new handle
        m_socket = handle;

        // Set the current blocking state
        setBlocking(m_isBlocking);
    }
}


////////////////////////////////////////////////////////////
void Socket::close()
{
    // Close the socket
    if (m_socket != priv::SocketImpl::invalidSocket())
    {
        priv::SocketImpl::close(m_socket);
        m_socket = priv::SocketImpl::invalidSocket();

        if (isSecure())
        {
            sslcDestroyContext(&m_secureData.sslContext);
            sslcCloseClientCertContext(m_secureData.clientCertContext);
            sslcDestroyRootCertChain(m_secureData.rootCertChain);
        }
    }
}

} // namespace cpp3ds
