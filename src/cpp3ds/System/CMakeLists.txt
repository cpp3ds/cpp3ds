set(SRCROOT ${PROJECT_SOURCE_DIR}/src/cpp3ds/System)

set(SRC
    ${SRCROOT}/Clock.cpp
    ${SRCROOT}/Err.cpp
    ${SRCROOT}/FileInputStream.cpp
    ${SRCROOT}/FileSystem.cpp
    ${SRCROOT}/I18n.cpp
    ${SRCROOT}/Lock.cpp
    ${SRCROOT}/MemoryInputStream.cpp
    ${SRCROOT}/Mutex.cpp
    ${SRCROOT}/Service.cpp
    ${SRCROOT}/Sleep.cpp
    ${SRCROOT}/String.cpp
    ${SRCROOT}/Thread.cpp
    ${SRCROOT}/ThreadLocal.cpp
    ${SRCROOT}/Time.cpp
)

add_cpp3ds_library(cpp3ds-system
                   SOURCES ${SRC})
