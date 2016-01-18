// This shader provides the basics needed for getting started with cpp3ds
// (and getting started with most non-cpp3ds orthogonal 2d homebrew too).
// It's enabled by default in cpp3ds::Game unless you specify a shader yourself.

// setup constants
.alias myconst c32 as (1.0, 0.0, 0.5, 1.0)
// x = 1/255
.alias colorfix c33 as (0.0039215686, 0, 0, 0)

// setup output map
.alias outpos  o0      as position
.alias outcol  o1      as color
.alias outtex0 o2.xyzw as texcoord0 // Would like to use .xy instead, but this is not supported by ctrulib currently
.alias outtex1 o3.xyzw as texcoord1
.alias outtex2 o4.xyzw as texcoord2

// setup uniform map, for use with SHDR_GetUniformRegister
.alias projection     c0-c3
.alias modelview      c4-c7
.alias texture        c8-c11

main:
	mov r1.xy,  v0.xy
	mov r1.zw,  myconst.yw

mdvl:  // tempreg = mdlvMtx * in.pos
	dp4 r0.x,  modelview[0],  r1
	dp4 r0.y,  modelview[1],  r1
	dp4 r0.z,  modelview[2],  r1
	mov r0.w,  myconst.w

proj:  // result.pos = projMtx * tempreg
	dp4 outpos.x,  projection[0],  r0
	dp4 outpos.y,  projection[1],  r0
	dp4 outpos.z,  projection[2],  r0
	dp4 outpos.w,  projection[3],  r0

tex:  // result.texcoord = in.texcoord
	dp4 outtex0.x, texture[0], v2
	dp4 outtex0.y, texture[1], v2
	mov outtex0.zw, myconst.zw
	mov outtex1,  myconst.yyyw
	mov outtex2,  myconst.yyyw

color: // result.color = in.color
	mul outcol, v1.xyzw, colorfix.xxxx

	nop
	end

endmain:
