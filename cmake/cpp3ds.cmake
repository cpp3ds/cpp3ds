# DevkitPro Paths are broken on windows, so we have to fix those
macro(msys_to_cmake_path MsysPath ResultingPath)
    string(REGEX REPLACE "^/([a-zA-Z])/" "\\1:/" ${ResultingPath} "${MsysPath}")
endmacro()

msys_to_cmake_path("$ENV{CPP3DS}" CPP3DS)
msys_to_cmake_path("$ENV{GL3DS}" GL3DS)

msys_to_cmake_path("$ENV{DEVKITPRO}" DEVKITPRO)
if(NOT IS_DIRECTORY ${DEVKITPRO})
    message(FATAL_ERROR "Please set DEVKITPRO in your environment")
endif()

msys_to_cmake_path("$ENV{DEVKITARM}" DEVKITARM)
if(NOT IS_DIRECTORY ${DEVKITARM})
    message(FATAL_ERROR "Please set DEVKITARM in your environment")
endif()

include(FindCTRULIB)
find_package(CTRULIB)


##############
## 3DSXTOOL ##
##############
if(NOT 3DSXTOOL)
    message(STATUS "Looking for 3dsxtool...")
    find_program(3DSXTOOL 3dsxtool ${DEVKITARM}/bin)
    if(3DSXTOOL)
        message(STATUS "3dsxtool: ${3DSXTOOL} - found")
    else()
        message(WARNING "3dsxtool - not found")
    endif()
endif()


##############
## SMDHTOOL ##
##############
if(NOT SMDHTOOL)
    message(STATUS "Looking for smdhtool...")
    find_program(SMDHTOOL smdhtool ${DEVKITARM}/bin)
    if(SMDHTOOL)
        message(STATUS "smdhtool: ${SMDHTOOL} - found")
    else()
        message(WARNING "smdhtool - not found")
    endif()
endif()


#############
## PICASSO ##
#############
if(NOT PICASSO_EXE)
    message(STATUS "Looking for Picasso...")
    find_program(PICASSO_EXE picasso ${DEVKITARM}/bin)
    if(PICASSO_EXE)
        message(STATUS "Picasso: ${PICASSO_EXE} - found")
        set(SHADER_AS picasso CACHE STRING "The shader assembler to be used. Allowed values are 'picasso' or 'nihstro'")
    else()
        message(WARNING "Picasso - not found")
    endif()
endif()


#############
## NIHSTRO ##
#############

if(NOT NIHSTRO_AS)
    message(STATUS "Looking for nihstro...")
    find_program(NIHSTRO_AS nihstro-assemble ${DEVKITARM}/bin)
    if(NIHSTRO_AS)
        message(STATUS "nihstro: ${NIHSTRO_AS} - found")
        set(SHADER_AS nihstro CACHE STRING "The shader assembler to be used. Allowed values are 'picasso' or 'nihstro'")
    else()
        message(STATUS "nihstro - not found")
    endif()
endif()


function(compile_shaders output directory)
	string(REGEX REPLACE "/+$" "" directory "${directory}") # Remove trailing slash
	if(SHADER_AS)
		foreach(shader ${ARGN})
			get_filename_component(filename ${shader} NAME)
			get_filename_component(dir ${shader} DIRECTORY)
			string(REPLACE ${directory} "" newdir ${dir})
			file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}${newdir})
			list(APPEND ${output} "${CMAKE_CURRENT_BINARY_DIR}${newdir}/${filename}")
			add_custom_command(
				OUTPUT ${CMAKE_CURRENT_BINARY_DIR}${newdir}/${filename}
				COMMAND $ENV{NIHSTRO}/nihstro-assemble ${shader} -o ${CMAKE_CURRENT_BINARY_DIR}${newdir}/${filename}
				DEPENDS ${shader}
				COMMENT "Compiling shader ${filename}"
			)
		endforeach(shader)
	else()
		message(ERROR "Called compile_shaders() without any compiler detected.")
	endif()
	set(${output} ${${output}} PARENT_SCOPE)
endfunction()


macro(compile_resources name directory output)
	add_custom_command(
		OUTPUT ${output}
		COMMAND python ${CPP3DS}/scripts/res_compile.py -o ${output} -d ${directory} -d ${CMAKE_CURRENT_BINARY_DIR} -n ${name} ${ARGN}
		DEPENDS ${ARGN}
		COMMENT "Generating ${name}"
	)
endmacro()
