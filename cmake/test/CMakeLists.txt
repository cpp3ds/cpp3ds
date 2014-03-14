
add_library(cpp3ds-test STATIC
    ${SRC_TEST}
)
set_target_properties(cpp3ds-test PROPERTIES COMPILE_DEFINITIONS "TESTING")
