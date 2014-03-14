
include(arm-toolchain)

add_library(cpp3ds STATIC
    ${SRC_ARM}
)
set_target_properties(cpp3ds PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_ARM_FLAGS}")
