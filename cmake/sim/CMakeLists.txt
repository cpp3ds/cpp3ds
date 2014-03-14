
add_library(cpp3ds-sim STATIC
    ${SRC_SIM}
    ${RESOURCE_SOURCE}
    ${RESOURCE_HEADER}
)
set_target_properties(cpp3ds-sim PROPERTIES COMPILE_DEFINITIONS "SIMULATION")
set_target_properties(cpp3ds-sim PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_SIM_FLAGS}")
#set_target_properties(cpp3ds-sim PROPERTIES LINK_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_SIM_FLAGS}")
