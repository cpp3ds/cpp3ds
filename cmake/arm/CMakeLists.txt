
include(arm-toolchain)

set(ASM_SRC ${SRCROOT}/cpp3ds/cpp3ds-crt0.S)
set_source_files_properties(${ASM_SRC} PROPERTIES LANGUAGE C)

add_library(cpp3ds STATIC
    ${SRC_ARM}
    ${ASM_SRC}
)
set_target_properties(cpp3ds PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_ARM_FLAGS}")
