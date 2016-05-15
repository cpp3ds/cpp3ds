include(arm-toolchain)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include_directories(
	${PROJECT_SOURCE_DIR}/include
	${CPP3DS}/include
	${CPP3DS}/extlibs/include
	${DEVKITPRO}/portlibs/3ds/include
	${DEVKITPRO}/portlibs/armv6k/include
	${GL3DS}/include
    ${CTRULIB}/include
    ${ARM_INCLUDE_DIRECTORIES}
)
link_directories(
	${CPP3DS}/lib
    ${CTRULIB}/lib
    ${DEVKITPRO}/portlibs/3ds/lib
    ${DEVKITPRO}/portlibs/armv6k/lib
    ${GL3DS}/lib
    ${ARM_LINK_DIRECTORIES}
)

compile_shaders(SHADER_OUTPUT ${SHADER_FILES})
set_source_files_properties(${SHADER_OUTPUT} PROPERTIES GENERATED TRUE)
file(GLOB_RECURSE ROMFS_FILES ${PROJECT_SOURCE_DIR}/res/romfs/*)

add_executable(${PROJECT_NAME}.elf ${SOURCE_FILES} ${ARM_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}.elf ${CPP3DS_ARM_LIBS})
set_target_properties(${PROJECT_NAME}.elf PROPERTIES COMPILE_FLAGS "${CPP3DS_ARM_FLAGS}")
set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-specs=3dsx.specs -march=armv6k -mtune=mpcore -mfloat-abi=hard -gc-sections")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

add_3dsx_target(${PROJECT_NAME}.elf)
add_cia_target(${PROJECT_NAME}.elf ${CIA_CONFIG} ${BANNER_IMAGE} ${BANNER_SOUND})
