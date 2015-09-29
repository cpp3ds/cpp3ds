set(SRCROOT ${PROJECT_SOURCE_DIR}/src/cpp3ds/Network)

set(SRC
    ${SRCROOT}/Ftp.cpp
    ${SRCROOT}/Http.cpp
    ${SRCROOT}/IpAddress.cpp
    ${SRCROOT}/Packet.cpp
    ${SRCROOT}/Socket.cpp
    ${SRCROOT}/SocketImpl.cpp
    ${SRCROOT}/SocketSelector.cpp
    ${SRCROOT}/TcpListener.cpp
    ${SRCROOT}/TcpSocket.cpp
    ${SRCROOT}/UdpSocket.cpp
)

add_cpp3ds_library(cpp3ds-network
                   SOURCES ${SRC}
                   DEPENDS cpp3ds-system)
