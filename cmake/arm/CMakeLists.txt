include(arm-toolchain)

set_source_files_properties(${CFILES} PROPERTIES LANGUAGE C)
set_source_files_properties(${CPP3DS_SOURCE_3DS} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_ARM_FLAGS}")
set_source_files_properties(${RESOURCE_OUTPUT} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CPP3DS_ARM_FLAGS}")
set_source_files_properties(${CFILES} PROPERTIES COMPILE_FLAGS "-g ${ARCH}")
set_source_files_properties(${RESOURCE_OUTPUT} PROPERTIES GENERATED TRUE)

include_directories(
	${PROJECT_SOURCE_DIR}/include
	${DEVKITPRO}/portlibs/armv6k/include
	${LIBCTRU_INCLUDE_DIRS}
	${GL3DS}/include
)

add_library(cpp3ds STATIC
	${CPP3DS_SOURCE_3DS}
	${CFILES}
	${RESOURCE_OUTPUT}
)

add_dependencies(cpp3ds cpp3ds-res)
