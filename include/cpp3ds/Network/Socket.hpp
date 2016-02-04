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

#ifndef CPP3DS_SOCKET_HPP
#define CPP3DS_SOCKET_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp3ds/Network/SocketHandle.hpp>
#include <cpp3ds/System/NonCopyable.hpp>
#include <vector>
#include <mbedtls/net.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>


namespace cpp3ds
{
class SocketSelector;

////////////////////////////////////////////////////////////
/// \brief Base class for all the socket types
///
////////////////////////////////////////////////////////////
class Socket : NonCopyable
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Status codes that may be returned by socket functions
    ///
    ////////////////////////////////////////////////////////////
    enum Status
    {
        Done,         ///< The socket has sent / received the data
        NotReady,     ///< The socket is not ready to send / receive data yet
        Partial,      ///< The socket sent a part of the data
        Disconnected, ///< The TCP socket has been disconnected
        Error         ///< An unexpected error happened
    };

    ////////////////////////////////////////////////////////////
    /// \brief Some special values used by sockets
    ///
    ////////////////////////////////////////////////////////////
    enum
    {
        AnyPort = 0 ///< Special value that tells the system to pick any available port
    };

    struct SecureData
    {
        mbedtls_net_context socket;
        mbedtls_entropy_context entropy;
        mbedtls_ctr_drbg_context ctr_drbg;
        mbedtls_ssl_context ssl;
        mbedtls_ssl_config conf;
        mbedtls_x509_crt cacert;
    };

public:

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Socket();

    ////////////////////////////////////////////////////////////
    /// \brief Set the blocking state of the socket
    ///
    /// In blocking mode, calls will not return until they have
    /// completed their task. For example, a call to Receive in
    /// blocking mode won't return until some data was actually
    /// received.
    /// In non-blocking mode, calls will always return immediately,
    /// using the return code to signal whether there was data
    /// available or not.
    /// By default, all sockets are blocking.
    ///
    /// \param blocking True to set the socket as blocking, false for non-blocking
    ///
    /// \see isBlocking
    ///
    ////////////////////////////////////////////////////////////
    void setBlocking(bool blocking);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether the socket is in blocking or non-blocking mode
    ///
    /// \return True if the socket is blocking, false otherwise
    ///
    /// \see setBlocking
    ///
    ////////////////////////////////////////////////////////////
    bool isBlocking() const;

    void setSecure(bool secure);
    bool isSecure() const;

protected:

    ////////////////////////////////////////////////////////////
    /// \brief Types of protocols that the socket can use
    ///
    ////////////////////////////////////////////////////////////
    enum Type
    {
        Tcp, ///< TCP protocol
        Udp  ///< UDP protocol
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor can only be accessed by derived classes.
    ///
    /// \param type Type of the socket (TCP or UDP)
    ///
    ////////////////////////////////////////////////////////////
    Socket(Type type, bool secure);

    ////////////////////////////////////////////////////////////
    /// \brief Return the internal handle of the socket
    ///
    /// The returned handle may be invalid if the socket
    /// was not created yet (or already destroyed).
    /// This function can only be accessed by derived classes.
    ///
    /// \return The internal (OS-specific) handle of the socket
    ///
    ////////////////////////////////////////////////////////////
    SocketHandle getHandle() const;

    SecureData& getSecureData() const;

    ////////////////////////////////////////////////////////////
    /// \brief Create the internal representation of the socket
    ///
    /// This function can only be accessed by derived classes.
    ///
    ////////////////////////////////////////////////////////////
    void create();

    ////////////////////////////////////////////////////////////
    /// \brief Create the internal representation of the socket
    ///        from a socket handle
    ///
    /// This function can only be accessed by derived classes.
    ///
    /// \param handle OS-specific handle of the socket to wrap
    ///
    ////////////////////////////////////////////////////////////
    void create(SocketHandle handle);

    ////////////////////////////////////////////////////////////
    /// \brief Close the socket gracefully
    ///
    /// This function can only be accessed by derived classes.
    ///
    ////////////////////////////////////////////////////////////
    void close();

private:

    friend class SocketSelector;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Type         m_type;       ///< Type of the socket (TCP or UDP)
    bool         m_isBlocking; ///< Current blocking mode of the socket
    bool         m_isSecure;   ///< Socket using SSL
    SocketHandle m_socket;     ///< Socket descriptor
    mutable SecureData m_secureData; ///< Data needed for secure socket
};

} // namespace cpp3ds


#endif // CPP3DS_SOCKET_HPP


////////////////////////////////////////////////////////////
/// \class cpp3ds::Socket
/// \ingroup network
///
/// This class mainly defines internal stuff to be used by
/// derived classes.
///
/// The only public features that it defines, and which
/// is therefore common to all the socket classes, is the
/// blocking state. All sockets can be set as blocking or
/// non-blocking.
///
/// In blocking mode, socket functions will hang until
/// the operation completes, which means that the entire
/// program (well, in fact the current thread if you use
/// multiple ones) will be stuck waiting for your socket
/// operation to complete.
///
/// In non-blocking mode, all the socket functions will
/// return immediately. If the socket is not ready to complete
/// the requested operation, the function simply returns
/// the proper status code (Socket::NotReady).
///
/// The default mode, which is blocking, is the one that is
/// generally used, in combination with threads or selectors.
/// The non-blocking mode is rather used in real-time
/// applications that run an endless loop that can poll
/// the socket often enough, and cannot afford blocking
/// this loop.
///
/// \see cpp3ds::TcpListener, cpp3ds::TcpSocket, cpp3ds::UdpSocket
///
////////////////////////////////////////////////////////////
