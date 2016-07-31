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
#include <cpp3ds/Network/Http.hpp>
#include <cpp3ds/System/Err.hpp>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <limits>
#include <3ds/services/httpc.h>
#include <cpp3ds/System/I18n.hpp>


namespace
{
    // Convert a string to lower case
    std::string toLower(std::string str)
    {
        for (std::string::iterator i = str.begin(); i != str.end(); ++i)
            *i = static_cast<char>(std::tolower(*i));
        return str;
    }
}


namespace cpp3ds
{
////////////////////////////////////////////////////////////
Http::Request::Request(const std::string& uri, Method method, const std::string& body)
{
    setMethod(method);
    setUri(uri);
    setHttpVersion(1, 0);
    setBody(body);
}


////////////////////////////////////////////////////////////
void Http::Request::setField(const std::string& field, const std::string& value)
{
    m_fields[toLower(field)] = value;
}


////////////////////////////////////////////////////////////
void Http::Request::setMethod(Http::Request::Method method)
{
    m_method = method;
}


////////////////////////////////////////////////////////////
void Http::Request::setUri(const std::string& uri)
{
    m_uri = uri;

    // Make sure it starts with a '/'
    if (m_uri.empty() || (m_uri[0] != '/'))
        m_uri.insert(0, "/");
}


////////////////////////////////////////////////////////////
void Http::Request::setHttpVersion(unsigned int major, unsigned int minor)
{
    m_majorVersion = major;
    m_minorVersion = minor;
}


////////////////////////////////////////////////////////////
void Http::Request::setBody(const std::string& body)
{
    m_body = body;
}


////////////////////////////////////////////////////////////
std::string Http::Request::prepare() const
{
    return "";
}


////////////////////////////////////////////////////////////
bool Http::Request::hasField(const std::string& field) const
{
    return m_fields.find(toLower(field)) != m_fields.end();
}


////////////////////////////////////////////////////////////
Http::Response::Response() :
m_status      (ConnectionFailed),
m_majorVersion(0),
m_minorVersion(0)
{

}


////////////////////////////////////////////////////////////
const std::string& Http::Response::getField(const std::string& field) const
{
    FieldTable::const_iterator it = m_fields.find(toLower(field));
    if (it != m_fields.end())
    {
        return it->second;
    }
    else if (m_context && m_context->httphandle)
    {
        char val[1024];
        httpcGetResponseHeader(m_context, field.c_str(), val, sizeof(val));
        m_fields[field] = val;
        return m_fields[field];
    }
}


////////////////////////////////////////////////////////////
Http::Response::Status Http::Response::getStatus() const
{
    return m_status;
}


////////////////////////////////////////////////////////////
unsigned int Http::Response::getMajorHttpVersion() const
{
    return m_majorVersion;
}


////////////////////////////////////////////////////////////
unsigned int Http::Response::getMinorHttpVersion() const
{
    return m_minorVersion;
}


////////////////////////////////////////////////////////////
const std::string& Http::Response::getBody() const
{
    return m_body;
}


////////////////////////////////////////////////////////////
void Http::Response::parse(httpcContext *context, Time timeout)
{
    Result ret;
    u32 statusCode = ConnectionFailed;

    m_context = context;
    if (R_FAILED(ret = httpcGetResponseStatusCodeTimeout(context, &statusCode, (u64)timeout.asMicroseconds() * 1000)))
    {
        err() << _("Failed to get HTTP status: 0x%08lX", ret).toAnsiString() << std::endl;
        return;
    }
    m_status = (Status)statusCode;
}


////////////////////////////////////////////////////////////
void Http::Response::parseFields(std::istream &in)
{

}


////////////////////////////////////////////////////////////
Http::Http() :
m_host(),
m_port(0)
{
    m_context.httphandle = 0;
}


////////////////////////////////////////////////////////////
Http::Http(const std::string& host, unsigned short port)
{
    setHost(host, port);
    m_context.httphandle = 0;
}


////////////////////////////////////////////////////////////
Http::~Http()
{
    close();
}


////////////////////////////////////////////////////////////
void Http::close()
{
    if (m_context.httphandle)
    {
        httpcCancelConnection(&m_context);
        httpcCloseContext(&m_context);
        m_context.httphandle = 0;
    }
}


////////////////////////////////////////////////////////////
void Http::setHost(const std::string& host, unsigned short port)
{
    m_hostUrl = host;

    // Check the protocol
    if (toLower(host.substr(0, 7)) == "http://")
    {
        // HTTP protocol
        m_hostName = host.substr(7);
        m_port     = (port != 0 ? port : 80);
    }
    else if (toLower(host.substr(0, 8)) == "https://")
    {
        // HTTPS protocol
        m_hostName = host.substr(8);
        m_port     = (port != 0 ? port : 443);
    }
    else
    {
        // Undefined protocol - use HTTP
        m_hostName = host;
        m_port     = (port != 0 ? port : 80);
    }

    // Remove any trailing '/' from the host name
    if (!m_hostName.empty() && (*m_hostName.rbegin() == '/'))
        m_hostName.erase(m_hostName.size() - 1);
    if (!m_hostUrl.empty() && (*m_hostUrl.rbegin() == '/'))
        m_hostUrl.erase(m_hostUrl.size() - 1);

    m_host = IpAddress(m_hostName);
}


////////////////////////////////////////////////////////////
Http::Response Http::sendRequest(const Http::Request& request, Time timeout, RequestCallback callback, size_t bufferSize)
{
    close();

    // Use 90 second default timeout for httpc
    if (timeout == Time::Zero)
        timeout = seconds(90);

    // First make sure that the request is valid -- add missing mandatory fields
    Request toSend(request);
    if (!toSend.hasField("User-Agent"))
    {
        toSend.setField("User-Agent", "Mozilla/5.0 (Nintendo 3DS; Mobile; rv:10.0) Gecko/20100101 libcpp3ds-network");
    }
    if (!toSend.hasField("Content-Length"))
    {
        std::ostringstream out;
        out << toSend.m_body.size();
        toSend.setField("Content-Length", out.str());
    }
    if ((toSend.m_method == Request::Post) && !toSend.hasField("Content-Type"))
    {
        toSend.setField("Content-Type", "application/x-www-form-urlencoded");
    }

    // Prepare the response
    Response received;

    std::string url = m_hostUrl + toSend.m_uri;
    HTTPC_RequestMethod method;
    switch (toSend.m_method) {
        case Request::Get:    method = HTTPC_METHOD_GET;    break;
        case Request::Post:   method = HTTPC_METHOD_POST;   break;
        case Request::Head:   method = HTTPC_METHOD_HEAD;   break;
        case Request::Put:    method = HTTPC_METHOD_PUT;    break;
        case Request::Delete: method = HTTPC_METHOD_DELETE; break;
    }

    // TODO: Handle return values / errors
    Result ret;
    u32 contentSize = 0;
    ret = httpcOpenContext(&m_context, method, url.c_str(), 1);
    ret = httpcSetClientCertDefault(&m_context, SSLC_DefaultClientCert_ClCertA);
    ret = httpcSetSSLOpt(&m_context, SSLCOPT_DisableVerify);

    // Add fields
    for (auto i = toSend.m_fields.begin(); i != toSend.m_fields.end(); ++i)
    {
        httpcAddRequestHeaderField(&m_context, i->first.c_str(), i->second.c_str());
    }

    if (!toSend.m_body.empty())
        httpcAddPostDataRaw(&m_context, (u32*)toSend.m_body.c_str(), toSend.m_body.size());

    ret = httpcBeginRequest(&m_context);

    received.parse(&m_context, timeout);
    if (received.getStatus() == Response::ConnectionFailed)
        return received;

    ret = httpcGetDownloadSizeState(&m_context, NULL, &contentSize);

    std::string receivedStr;
    u32 size = 0;
    u32 lastProcessed = 0;
    u32 processed = 0;
    u8 *buffer = new u8[bufferSize];
    char *charBuf = reinterpret_cast<char*>(buffer);
    Result dlret = HTTPC_RESULTCODE_DOWNLOADPENDING;

    while (dlret == HTTPC_RESULTCODE_DOWNLOADPENDING)
    {
        dlret = httpcReceiveDataTimeout(&m_context, buffer, bufferSize, (u64)timeout.asMicroseconds() * 1000);
        if (dlret != HTTPC_RESULTCODE_DOWNLOADPENDING && dlret != 0)
            err() << _("Failed to receieve HTTP data: 0x%08lX", dlret).toAnsiString() << std::endl;

        if (R_FAILED(ret = httpcGetDownloadSizeState(&m_context, &processed, NULL)))
            break;

        size = processed - lastProcessed;
        lastProcessed = processed;

        if (callback)
        {
            // Break if user interrupts the download
            if (!callback(buffer, size, processed, received))
                break;
        }
        else
        {
            receivedStr.append(charBuf, charBuf + size);
        }
    }

    delete[] buffer;
    received.m_body = receivedStr;
    return received;
}

} // namespace cpp3ds
