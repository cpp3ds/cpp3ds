include(arm-toolchain)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
	${PROJECT_SOURCE_DIR}/include
	${CPP3DS}/include
	${CPP3DS}/extlibs/include
	${GL3DS}/include
    ${CTRULIB}/include
    ${ARM_INCLUDE_DIRECTORIES}
)
link_directories(
	${CPP3DS}/lib
    ${CTRULIB}/lib
    ${DEVKITPRO}/portlibs/lib
    ${DEVKITPRO}/portlibs/armv6k/lib
    ${GL3DS}/lib
    ${ARM_LINK_DIRECTORIES}
)
set_source_files_properties(${RESOURCE_OUTPUT} PROPERTIES GENERATED TRUE)

add_executable(${PROJECT_NAME}.elf
	${SOURCE_FILES}
	${RESOURCE_OUTPUT}
)

add_dependencies(${PROJECT_NAME}.elf ${PROJECT_NAME}-res)
target_link_libraries(${PROJECT_NAME}.elf ${CPP3DS_ARM_LIBS})
set_target_properties(${PROJECT_NAME}.elf PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_ARM_FLAGS}")
set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-specs=3dsx.specs -g -march=armv6k -mtune=mpcore -mfloat-abi=hard")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
