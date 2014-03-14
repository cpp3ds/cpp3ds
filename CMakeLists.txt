cmake_minimum_required(VERSION 2.8)

# project name
project(CPP3DS)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)
pkg_check_modules(GMODULE gmodule-2.0)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/cpp3ds)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

# add the SFML header path
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${GTKMM_INCLUDE_DIRS}
    "../tween-engine/include"
)

link_directories(
	${GTKMM_LIBRARY_DIRS}
    "../tween-engine/lib"
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
FILE(GLOB_RECURSE SRC_SIM ${SRCROOT}/sim3ds/*.cpp)
set(ARM_SRC ${SRC} ${SRCROOT}/cpp3ds/cpp3ds-crt0.S)
set(SRC_TEST ${SRC})

# Remove duplicate files for simulator
foreach(test1 ${SRC_SIM})
    get_filename_component(filename1 ${test1} NAME)
    foreach(test2 ${SRC})
        get_filename_component(filename2 ${test2} NAME)
        if(${filename1} STREQUAL ${filename2})
            list(REMOVE_ITEM SRC ${test2})
        endif()
    endforeach(test2)
endforeach(test1)
list(APPEND SRC_SIM ${SRC})
list(REMOV ${SRCROOT}/cpp3ds/syscalls.cpp)

#source_group("" FILES ${SRC})

#MESSAGE(STATUS "\nFound following source files:\n ${SRC}\n")



include(gresource)

gresource(
  ${CMAKE_CURRENT_SOURCE_DIR}/res/sim
  simulator.gresource.xml
  ${CMAKE_CURRENT_BINARY_DIR}/src/sim3ds/Simulator
  ${CMAKE_CURRENT_BINARY_DIR}/include/cpp3ds/Simulator
  RESOURCE_SOURCE
  RESOURCE_HEADER
)


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

set(CPP3DS_ARM_FLAGS "-g -Wall -pedantic -march=armv5te -fno-rtti -fno-exceptions")
set(CPP3DS_TEST_FLAGS "-g -Wall -pedantic -fno-rtti -fno-exceptions")
set(CPP3DS_SIM_FLAGS "-g -Wall -pedantic")

add_library(cpp3ds-test STATIC
    ${SRC_TEST}
)
set_target_properties(cpp3ds-sim PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_ARM_FLAGS}")

add_library(cpp3ds-sim STATIC
    ${SRC_SIM}
    ${RESOURCE_SOURCE}
    ${RESOURCE_HEADER}
)
set_target_properties(cpp3ds-sim PROPERTIES COMPILE_DEFINITIONS "SIMULATION")
set_target_properties(cpp3ds-sim PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_SIM_FLAGS}")
#set_target_properties(cpp3ds-sim PROPERTIES LINK_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_SIM_FLAGS}")

add_library(cpp3ds-test STATIC
    ${SRC_TEST}
)
set_target_properties(cpp3ds-test PROPERTIES COMPILE_DEFINITIONS "TESTING")


if(CPP3DS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
