find_package(Qt5Widgets REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(
	${PROJECT_SOURCE_DIR}/include
	${CPP3DS}/include
	${EMU_INCLUDE_DIRECTORIES}
)

link_directories(
	${CPP3DS}/lib
	${EMU_LINK_DIRECTORIES}
)

add_executable(${PROJECT_NAME}-emu ${SOURCE_FILES})

qt5_use_modules(${PROJECT_NAME}-emu Core Gui Widgets)

target_link_libraries(${PROJECT_NAME}-emu ${CPP3DS_EMU_LIBS})

set_target_properties(${PROJECT_NAME}-emu PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_EMU_FLAGS} -std=c++11")
set_target_properties(${PROJECT_NAME}-emu PROPERTIES COMPILE_DEFINITIONS "EMULATION")
set_target_properties(${PROJECT_NAME}-emu PROPERTIES LINK_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_EMU_FLAGS} -Wl,--wrap=main")
