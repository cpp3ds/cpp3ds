include(gresource)

gresource(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../res/sim
  simulator.gresource.xml
  ${CMAKE_CURRENT_BINARY_DIR}/../../src/sim3ds/Simulator
  ${CMAKE_CURRENT_BINARY_DIR}/../../include/cpp3ds/Simulator
  RESOURCE_SOURCE
  RESOURCE_HEADER
)

include_directories( ${GTKMM_INCLUDE_DIRS} )
link_directories( ${GTKMM_LIBRARY_DIRS} )


add_library(cpp3ds-sim STATIC
    ${SRC_SIM}
    ${RESOURCE_SOURCE}
    ${RESOURCE_HEADER}
)
set_target_properties(cpp3ds-sim PROPERTIES COMPILE_DEFINITIONS "SIMULATION")
set_target_properties(cpp3ds-sim PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_SIM_FLAGS}")
#set_target_properties(cpp3ds-sim PROPERTIES LINK_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_SIM_FLAGS}")

target_link_libraries(cpp3ds-sim ${GTKMM_LIBRARIES})
