# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(ui_header ${CMAKE_CURRENT_SOURCE_DIR}/../../include/cpp3ds/Emulator/ui_emulator.h)

add_custom_command(
	OUTPUT ${ui_header}
	COMMAND uic ${CMAKE_CURRENT_SOURCE_DIR}/../../res/emu/emulator.ui -o ${ui_header}
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../../res/emu/emulator.ui
	COMMENT "Generating ui_emulator.h"
)
             
qt5_wrap_cpp(tst_hdr_moc ${CMAKE_CURRENT_SOURCE_DIR}/../../include/cpp3ds/Emulator/Emulator.hpp)
#qt5_wrap_ui(tst_form_hdr ${CMAKE_CURRENT_SOURCE_DIR}/../../res/emu/emulator.ui)
qt5_add_resources(RESOURCE_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/../../res/emu/emulator.qrc)

#include_directories( ${GTKMM_INCLUDE_DIRS} )
#link_directories( ${GTKMM_LIBRARY_DIRS} )


add_library(cpp3ds-emu STATIC
    ${SRC_EMU}
    ${tst_hdr_moc}
    ${tst_form_hdr}
    ${RESOURCE_HEADER}
)
set_target_properties(cpp3ds-emu PROPERTIES COMPILE_DEFINITIONS "EMULATION")
set_target_properties(cpp3ds-emu PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_EMU_FLAGS}")
#set_target_properties(cpp3ds-emu PROPERTIES LINK_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_EMU_FLAGS}")

add_custom_target(ui ALL DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/../../include/cpp3ds/Emulator/ui_emulator.h
)
add_dependencies(cpp3ds-emu ui)
qt5_use_modules(cpp3ds-emu Core Gui Widgets)
#target_link_libraries(cpp3ds-emu ${GTKMM_LIBRARIES})
