include(arm-toolchain)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${DEVKITPRO}/portlibs/armv6k/include
        ${DEVKITPRO}/portlibs/3ds/include
        ${LIBCTRU_INCLUDE_DIRS}
)

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "-std=c++11")

add_subdirectory(Audio)
add_subdirectory(Graphics)
add_subdirectory(Network)
add_subdirectory(System)
add_subdirectory(Window)
