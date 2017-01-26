# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Generic)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER ${DEVKITARM}/bin/arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER ${DEVKITARM}/bin/arm-none-eabi-g++)
SET(CMAKE_SIZE ${DEVKITARM}/bin/arm-none-eabi-size)
SET(CMAKE_OBJCOPY ${DEVKITARM}/bin/arm-none-eabi-objcopy)
SET(CMAKE_AR ${DEVKITARM}/bin/arm-none-eabi-ar)
SET(CMAKE_RANLIB ${DEVKITARM}/bin/arm-none-eabi-ranlib)

set(CMAKE_FIND_ROOT_PATH ${DEVKITARM} ${DEVKITPRO} ${DEVKITPRO}/portlibs/3ds ${DEVKITPRO}/portlibs/armv6k ${DEVKITPRO}/portlibs/3ds/lib ${DEVKITPRO}/portlibs/armv6k/lib)
# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
