find_package(Qt5Widgets REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

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

add_executable(${PROJECT_NAME}-emu ${SOURCE_FILES} ${EMU_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}-emu ${CPP3DS_EMU_LIBS} Qt5::Core Qt5::Gui Qt5::Widgets)

set_target_properties(${PROJECT_NAME}-emu PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_EMU_FLAGS} -std=c++11")
set_target_properties(${PROJECT_NAME}-emu PROPERTIES COMPILE_DEFINITIONS "EMULATION")
set_target_properties(${PROJECT_NAME}-emu PROPERTIES LINK_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_EMU_FLAGS} -Wl,--wrap=main")
