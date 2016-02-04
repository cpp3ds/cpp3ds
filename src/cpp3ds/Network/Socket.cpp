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
#include <mbedtls/certs.h>
#include <stdlib.h>


namespace
{
    static int entropy_func(void *data, unsigned char *output, size_t len)
    {
        for (int i = 0; i < len; ++i)
            output[i] = rand() % 256;
        return 0;
    }
}


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Socket::Socket(Type type, bool secure) :
m_type      (type),
m_socket    (priv::SocketImpl::invalidSocket()),
m_isBlocking(true),
m_isSecure  (secure)
{
    m_secureData.socket.fd = priv::SocketImpl::invalidSocket();
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
    if (m_isSecure)
    {
        if (m_secureData.socket.fd != priv::SocketImpl::invalidSocket())
        {
            if (blocking)
                mbedtls_net_set_block(&m_secureData.socket);
            else
                mbedtls_net_set_nonblock(&m_secureData.socket);
        }
    }
    else
    {
        if (m_socket != priv::SocketImpl::invalidSocket())
            priv::SocketImpl::setBlocking(m_socket, blocking);
    }

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

    if ((secure && !oldSecure && m_socket != priv::SocketImpl::invalidSocket()) ||
        (!secure && oldSecure && m_secureData.socket.fd != priv::SocketImpl::invalidSocket()))
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
    return m_isSecure ? m_secureData.socket.fd : m_socket;
}


////////////////////////////////////////////////////////////
Socket::SecureData& Socket::getSecureData() const
{
    return m_secureData;
}


////////////////////////////////////////////////////////////
void Socket::create()
{
    // Don't create the socket if it already exists
    if (m_isSecure)
    {
        if (m_secureData.socket.fd == priv::SocketImpl::invalidSocket())
        {
            int ret;
            mbedtls_net_init(&m_secureData.socket);
            mbedtls_ssl_init(&m_secureData.ssl);
            mbedtls_ssl_config_init(&m_secureData.conf);
            mbedtls_x509_crt_init(&m_secureData.cacert);
            mbedtls_ctr_drbg_init(&m_secureData.ctr_drbg);
            mbedtls_entropy_init( &m_secureData.entropy );

            srand(time(nullptr));
            if (ret = mbedtls_ctr_drbg_seed(&m_secureData.ctr_drbg, entropy_func, &m_secureData.entropy, NULL, 0 ))
                err() << "mbedtls_ctr_drbg_seed failed: " << ret << std::endl;
            if ((ret = mbedtls_x509_crt_parse(&m_secureData.cacert, (const unsigned char *) mbedtls_test_cas_pem, mbedtls_test_cas_pem_len)) < 0)
                err() << "mbedtls_x509_crt_parse failed: -0x" << std::hex << ret << std::endl;

            // use mbedtls_ssl_conf_endpoint to set socket as server
            if ((ret = mbedtls_ssl_config_defaults(&m_secureData.conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
                err() << "mbedtls_ssl_config_defaults failed: " << ret << std::endl;
            mbedtls_ssl_conf_authmode(&m_secureData.conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
            mbedtls_ssl_conf_ca_chain(&m_secureData.conf, &m_secureData.cacert, NULL);
            mbedtls_ssl_conf_rng(&m_secureData.conf, mbedtls_ctr_drbg_random, &m_secureData.ctr_drbg);
            if ((ret = mbedtls_ssl_setup(&m_secureData.ssl, &m_secureData.conf)) != 0)
                err() << "mbedtls_ssl_setup failed: " << ret << std::endl;
            if ((ret = mbedtls_ssl_set_hostname(&m_secureData.ssl, "mbed TLS Server")) != 0)
                err() << "mbedtls_ssl_set_hostname failed: " << ret << std::endl;
            mbedtls_ssl_set_bio(&m_secureData.ssl, &m_secureData.socket, mbedtls_net_send, mbedtls_net_recv, NULL);

            setBlocking(m_isBlocking);
        }
    }
    else
    {
        if (m_socket == priv::SocketImpl::invalidSocket())
        {
            SocketHandle handle = socket(PF_INET, m_type == Tcp ? SOCK_STREAM : SOCK_DGRAM, IPPROTO_IP);
            create(handle);
        }
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

        if (m_type == Tcp)
        {
			/*
            // Disable the Nagle algorithm (i.e. removes buffering of TCP packets)
            int yes = 1;
            if (setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
            {
                err() << "Failed to set socket option \"TCP_NODELAY\" ; "
                      << "all your TCP packets will be buffered" << std::endl;
            }

            // On Mac OS X, disable the SIGPIPE signal on disconnection
            #ifdef SFML_SYSTEM_MACOS
                if (setsockopt(m_socket, SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
                {
                    err() << "Failed to set socket option \"SO_NOSIGPIPE\"" << std::endl;
                }
            #endif
            */
        }
        else
        {
            // Enable broadcast by default for UDP sockets
            int yes = 1;
//            if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
//            {
//                err() << "Failed to enable broadcast on UDP socket" << std::endl;
//            }
        }
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
    }

    // Close the secure socket
    if (m_secureData.socket.fd != priv::SocketImpl::invalidSocket())
    {
        mbedtls_net_free(&m_secureData.socket);
        mbedtls_ssl_free(&m_secureData.ssl);
        mbedtls_ssl_config_free(&m_secureData.conf);
        mbedtls_x509_crt_free(&m_secureData.cacert);
        mbedtls_ctr_drbg_free(&m_secureData.ctr_drbg);
        mbedtls_entropy_free(&m_secureData.entropy);
        m_secureData.socket.fd = priv::SocketImpl::invalidSocket();
    }
}

} // namespace cpp3ds
