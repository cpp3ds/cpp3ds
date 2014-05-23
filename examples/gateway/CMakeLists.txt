cmake_minimum_required(VERSION 2.8)

# project name
set(PROJECT "cpp3ds-gateway")
project(${PROJECT})

find_package(Qt5Widgets REQUIRED)

include_directories(
	"${PROJECT_SOURCE_DIR}/../../include"
    "${PROJECT_SOURCE_DIR}/../../../tween-engine/include"
    "${PROJECT_SOURCE_DIR}/../../../ctrulib/libctru/include"
)

link_directories(
	"${PROJECT_SOURCE_DIR}/../../lib"
    "${PROJECT_SOURCE_DIR}/../../../tween-engine/lib"
    "${PROJECT_SOURCE_DIR}/../../../ctrulib/libctru/lib"
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

set(SOURCE_FILES
    src/gateway.cpp
)

add_executable(${PROJECT} ${SOURCE_FILES})
qt5_use_modules(${PROJECT} Core Gui Widgets)

target_link_libraries(${PROJECT} cpp3ds-sim tween-test sfml-graphics sfml-window sfml-system)

set_target_properties(${PROJECT} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -O3")
set_target_properties(${PROJECT} PROPERTIES COMPILE_DEFINITIONS "SIMULATION")
#set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_SIM_FLAGS}")