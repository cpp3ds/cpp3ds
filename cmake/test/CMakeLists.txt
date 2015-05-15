set(CMAKE_AUTOMOC ON)
qt5_wrap_cpp(tst_hdr_moc ${CMAKE_CURRENT_SOURCE_DIR}/../../include/cpp3ds/Emulator/Emulator.hpp)
qt5_add_resources(RESOURCE_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/../../res/emu/emulator.qrc)

add_library(cpp3ds-test STATIC
    ${SRC_TEST}
    ${tst_hdr_moc}
    ${RESOURCE_HEADER}
)
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../../include
	${CMAKE_CURRENT_SOURCE_DIR}/../../extlibs/include
	${CMAKE_CURRENT_SOURCE_DIR}/../../../tween-engine/include
)
set_target_properties(cpp3ds-test PROPERTIES COMPILE_DEFINITIONS "TESTING")
qt5_use_modules(cpp3ds-test Core Gui Widgets)
