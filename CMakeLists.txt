cmake_minimum_required(VERSION 2.8)

# project name
project(CPP3DS)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(Qt5Widgets)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/cpp3ds)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

# add the SFML header path
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
    "../tween-engine/include"
    "../ctrulib/libctru/include"
)

link_directories(
    "../tween-engine/lib"
    "../ctrulib/libctru/lib"
)

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




# Fetch all source files
FILE(GLOB_RECURSE SRC ${SRCROOT}/cpp3ds/*.cpp)
FILE(GLOB_RECURSE SRC_EMU ${SRCROOT}/emu3ds/*.cpp)
set(SRC_ARM ${SRC})
set(SRC_TEST ${SRC})

# Remove duplicate files for emulator
foreach(test1 ${SRC_EMU})
    get_filename_component(filename1 ${test1} NAME)
    foreach(test2 ${SRC})
        get_filename_component(filename2 ${test2} NAME)
        if(${filename1} STREQUAL ${filename2})
            list(REMOVE_ITEM SRC ${test2})
        endif()
    endforeach(test2)
endforeach(test1)
list(APPEND SRC_EMU ${SRC})
list(REMOVE_ITEM SRC_EMU ${SRCROOT}/cpp3ds/syscalls.cpp)
list(REMOVE_ITEM SRC_EMU ${SRCROOT}/cpp3ds/cpp3ds.cpp)
list(REMOVE_ITEM SRC_TEST ${SRCROOT}/cpp3ds/cpp3ds.cpp)

#source_group("" FILES ${SRC})

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

# set the output directory for cpp3ds libraries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

set(CPP3DS_ARM_FLAGS "-Wall -pedantic -O3 -march=armv6 -fdata-sections -fno-rtti -fno-exceptions")
set(CPP3DS_TEST_FLAGS "-g -Wall -pedantic -O0 -fno-rtti -fno-exceptions")
set(CPP3DS_EMU_FLAGS "-g -Wall -pedantic -O0")


if(CPP3DS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

add_subdirectory(cmake/arm)
add_subdirectory(cmake/emu)
add_subdirectory(cmake/test)
