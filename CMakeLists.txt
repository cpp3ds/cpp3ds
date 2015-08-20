cmake_minimum_required(VERSION 2.8)

project(cpp3ds)

set(CPP3DS ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH "${CPP3DS}/cmake" ${CMAKE_MODULE_PATH})
include(cpp3ds)

find_package(Qt5Widgets REQUIRED)
find_package(Freetype REQUIRED)

include_directories(${FREETYPE_INCLUDE_DIRS})

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

# Fetch all source files.
FILE(GLOB_RECURSE CFILES ${PROJECT_SOURCE_DIR}/src/cpp3ds/*.[cS]) # Put asm .S files with C files
FILE(GLOB_RECURSE CPPFILES ${PROJECT_SOURCE_DIR}/src/cpp3ds/*.cpp)
FILE(GLOB_RECURSE CPP3DS_SOURCE_EMU ${PROJECT_SOURCE_DIR}/src/emu3ds/*.cpp)
set(CPP3DS_SOURCE_3DS ${CPPFILES})

# Remove duplicate (override) files for emulator
foreach(emufile ${CPP3DS_SOURCE_EMU})
    get_filename_component(emufilename ${emufile} NAME)
    foreach(file ${CPPFILES})
        get_filename_component(filename ${file} NAME)
        if(${emufilename} STREQUAL ${filename})
            list(REMOVE_ITEM CPPFILES ${file})
        endif()
    endforeach(file)
endforeach(emufile)
list(APPEND CPP3DS_SOURCE_EMU ${CPPFILES})
set(CPP3DS_SOURCE_TEST ${CPP3DS_SOURCE_EMU})

# Fetch all core_resources bundled with all software using cpp3ds.
# Shaders are defined separately because they need compiling first.
set(SHADER_FILES ${PROJECT_SOURCE_DIR}/res/core_resource/default_shader.vsh)
set(RESOURCE_FILES ${PROJECT_SOURCE_DIR}/res/core_resource/opensans.ttf)
set(RESOURCE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/core_resource.cpp)

compile_shaders(SHADER_OUTPUT ${PROJECT_SOURCE_DIR}/res/core_resource/ ${SHADER_FILES})

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
#set(CMAKE_SKIP_BUILD_RPATH TRUE)

# set the output directory for cpp3ds libraries and binaries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# compile flags
set(ARCH "-march=armv6k -mtune=mpcore -mfloat-abi=hard")
set(CPP3DS_ARM_FLAGS "-Wall -pedantic -O0 ${ARCH} -fno-rtti -fno-exceptions")
set(CPP3DS_TEST_FLAGS "-g -Wall -pedantic -O0")
set(CPP3DS_EMU_FLAGS "-g -Wall -pedantic -O0")

#if(CPP3DS_BUILD_EXAMPLES)
    add_subdirectory(examples)
#endif()

add_subdirectory(cmake/arm)
add_subdirectory(cmake/emu)
#add_subdirectory(cmake/test)
