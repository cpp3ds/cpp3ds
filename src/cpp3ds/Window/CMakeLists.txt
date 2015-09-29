set(SRCROOT ${PROJECT_SOURCE_DIR}/src/cpp3ds/Window)

set(SRC
    ${SRCROOT}/Context.cpp
    ${SRCROOT}/EventManager.cpp
    ${SRCROOT}/Game.cpp
    ${SRCROOT}/GlContext.cpp
    ${SRCROOT}/GlResource.cpp
    ${SRCROOT}/Keyboard.cpp
    ${SRCROOT}/Sensor.cpp
    ${SRCROOT}/Window.cpp
)

add_cpp3ds_library(cpp3ds-window
                   SOURCES ${SRC}
                   DEPENDS cpp3ds-system)
