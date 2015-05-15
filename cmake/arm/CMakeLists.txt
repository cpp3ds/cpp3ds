include(arm-toolchain)

set_source_files_properties(${CFILES} PROPERTIES LANGUAGE C)
set_source_files_properties(${SRC_ARM} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_ARM_FLAGS}")
set_source_files_properties(${CFILES} PROPERTIES COMPILE_FLAGS "-g ${ARCH}")

include_directories(
	${PROJECT_SOURCE_DIR}/include
	$ENV{DEVKITPRO}/portlibs/armv6k/include
    $ENV{CTRULIB}/include
)

add_library(cpp3ds STATIC
    ${SRC_ARM}
    ${CFILES}
)
