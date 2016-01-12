cmake_minimum_required(VERSION 2.8)

project(cpp3ds)

set(CPP3DS ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH "${CPP3DS}/cmake" ${CMAKE_MODULE_PATH})
include(cpp3ds)

option(BUILD_EMULATOR "Build cpp3ds emulator (Qt5 required)" ON)
option(BUILD_EXAMPLES "Build all cpp3ds example projects" ON)
option(BUILD_DOCS "Build doxygen documentation" OFF)
option(ENABLE_OGG "Include OGG encoder/decoder classes" ON)
option(ENABLE_FLAC "Include FLAC encoder/decoder classes" OFF)
option(ENABLE_MP3 "Include MP3 decoder class" OFF)

if(ENABLE_OGG)
	add_definitions(-DCPP3DS_ENABLE_OGG)
endif()
if(ENABLE_FLAC)
	add_definitions(-DCPP3DS_ENABLE_FLAC)
endif()
if(ENABLE_MP3)
	add_definitions(-DCPP3DS_ENABLE_MP3)
endif()

# C++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Fetch all core_resources bundled with all software using cpp3ds.
# Shaders are defined separately because they need compiling first.
set(SHADER_FILES ${PROJECT_SOURCE_DIR}/res/core_resource/default_shader.vsh)
set(RESOURCE_FILES ${PROJECT_SOURCE_DIR}/res/core_resource/opensans.ttf)
set(RESOURCE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/core_resource.cpp)

set(SHADER_AS nihstro)
compile_core_shaders(SHADER_OUTPUT ${PROJECT_SOURCE_DIR}/res/core_resource/ ${SHADER_FILES})

compile_resources(
	"core_resources"
	"${PROJECT_SOURCE_DIR}/res/core_resource"
	${RESOURCE_OUTPUT}
	${RESOURCE_FILES} ${SHADER_OUTPUT}
)

add_custom_target(cpp3ds-res ALL DEPENDS ${RESOURCE_OUTPUT} ${SHADER_OUTPUT})

# uninstall target
configure_file(
    "cmake/cmake_uninstall.cmake.in"
    "cmake/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake)

# disable the rpath stuff
set(CMAKE_SKIP_BUILD_RPATH TRUE)

# set the output directory for cpp3ds libraries and binaries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# compile flags
set(ARCH "-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft")
set(CPP3DS_ARM_FLAGS "-g -O2 ${ARCH} -ffunction-sections -fdata-sections")
set(CPP3DS_TEST_FLAGS "-g -O2")
set(CPP3DS_EMU_FLAGS "-g -O2")

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(BUILD_DOCS)
	add_subdirectory(doc)
endif()

add_subdirectory(src)
