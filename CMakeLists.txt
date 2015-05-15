cmake_minimum_required(VERSION 2.8)

project(CPP3DS)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(Qt5Widgets REQUIRED)
find_package(Freetype REQUIRED)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/cpp3ds)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

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
FILE(GLOB_RECURSE CFILES ${SRCROOT}/cpp3ds/*.[cS]) # Put asm .S files with C files
FILE(GLOB_RECURSE CPPFILES ${SRCROOT}/cpp3ds/*.cpp)
FILE(GLOB_RECURSE SRC_EMU ${SRCROOT}/emu3ds/*.cpp)
set(SRC_ARM ${CPPFILES})

# Remove duplicate (override) files for emulator
foreach(emufile ${SRC_EMU})
    get_filename_component(emufilename ${emufile} NAME)
    foreach(file ${CPPFILES})
        get_filename_component(filename ${file} NAME)
        if(${emufilename} STREQUAL ${filename})
            list(REMOVE_ITEM CPPFILES ${file})
        endif()
    endforeach(file)
endforeach(emufile)
list(APPEND SRC_EMU ${CPPFILES})
#list(REMOVE_ITEM SRC_EMU ${SRCROOT}/cpp3ds/syscalls.cpp)
#list(REMOVE_ITEM SRC_EMU ${SRCROOT}/cpp3ds/cpp3ds.cpp)
#list(REMOVE_ITEM CPPFILES_TEST ${SRCROOT}/cpp3ds/cpp3ds.cpp)
set(SRC_TEST ${SRC_EMU})

#MESSAGE(STATUS "\nFound following source files:\n ${SRC}\n")

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

#set_source_files_properties(${ASMFILES} PROPERTIES COMPILE_FLAGS "-g ${ARCH}")
#set_source_files_properties(${CFILES} PROPERTIES COMPILE_FLAGS "")
#set_source_files_properties(${SRC_ARM} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_ARM_FLAGS}")
#set_source_files_properties(${SRC_EMU} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_EMU_FLAGS}")

#if(CPP3DS_BUILD_EXAMPLES)
    add_subdirectory(examples/cube)
#endif()

add_subdirectory(cmake/arm)
add_subdirectory(cmake/emu)
#add_subdirectory(cmake/test)
