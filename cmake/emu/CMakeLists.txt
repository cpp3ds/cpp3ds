# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(ui_header ${PROJECT_SOURCE_DIR}/include/cpp3ds/Emulator/ui_emulator.h)

add_custom_command(
	OUTPUT ${ui_header}
	COMMAND uic ${PROJECT_SOURCE_DIR}/res/emu/emulator.ui -o ${ui_header}
	DEPENDS ${PROJECT_SOURCE_DIR}/res/emu/emulator.ui
	COMMENT "Generating ui_emulator.h"
)
             
qt5_wrap_cpp(tst_hdr_moc ${PROJECT_SOURCE_DIR}/include/cpp3ds/Emulator/Emulator.hpp)
qt5_add_resources(RESOURCE_HEADER ${PROJECT_SOURCE_DIR}/res/emu/emulator.qrc)
set_source_files_properties(${RESOURCE_OUTPUT} PROPERTIES GENERATED TRUE)

include_directories(
	${PROJECT_SOURCE_DIR}/include
)

add_library(cpp3ds-emu STATIC
    ${CPP3DS_SOURCE_EMU}
    ${tst_hdr_moc}
    ${RESOURCE_HEADER}
    ${RESOURCE_OUTPUT}
)
set_target_properties(cpp3ds-emu PROPERTIES COMPILE_DEFINITIONS "EMULATION")
set_source_files_properties(${CPP3DS_SOURCE_EMU} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_EMU_FLAGS}")

add_custom_target(ui ALL DEPENDS ${ui_header})
add_dependencies(cpp3ds-emu ui)
qt5_use_modules(cpp3ds-emu Core Gui Widgets)
