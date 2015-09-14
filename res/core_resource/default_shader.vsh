// This shader provides the basics needed for getting started with cpp3ds
// (and getting started with most non-cpp3ds homebrew too).
// It's enabled by default in cpp3ds::Game unless you specify a shader yourself.

.alias v_position  v0
.alias v_color     v1
.alias v_texcoord  v2
.alias v_normal    v3

// setup constants
.alias myconst      c32 as (2.0, 0.0, 0.5, 1.0)
.alias def_normquat c90 as (1.0, 0.0, 0.0, 0.0)
.alias negative_one c94 as (-1.0, -1.0, -1.0, -1.0)
.alias zero         c93 as (0.0, 0.0, 0.0, 0.0)
.alias half_tau     c92 as (180.0, 180.0, 0.0, 0.0)
.alias one          c91 as (1.0, 1.0, 1.0, 1.0)

// setup output map
.alias outpos  o0      as position
.alias outcol  o1      as color
.alias outtex0 o2.xyzw as texcoord0 // Would like to use .xy instead, but this is not supported by ctrulib currently
.alias outtex1 o3.xyzw as texcoord1
.alias outtex2 o4.xyzw as texcoord2
.alias outquat o5      as quaternion
.alias outview o6.xyzw as view

// setup uniform map, for use with SHDR_GetUniformRegister
.alias projection     c0
.alias modelview      c4
.alias texture        c8-c11


main:

mdvl:  // tempreg = modelview * v_position
	dp4 r0.x,  modelview[0],  v_position
	dp4 r0.y,  modelview[1],  v_position
	dp4 r0.z,  modelview[2],  v_position
	dp4 r0.w,  modelview[3],  v_position

proj:  // outpos = projection * tempreg
	dp4 outpos.x,  projection[0],  r0
	dp4 outpos.y,  projection[1],  r0
	dp4 outpos.z,  projection[2],  r0
	dp4 outpos.w,  projection[3],  r0

view: // view = -(modelView * in_position);
	mul outview, r0, negative_one

tex:  // outtex0 = texture * v_texcoord
	dp4 outtex0.x, texture[0], v_texcoord
	dp4 outtex0.y, texture[1], v_texcoord
	mov outtex0.zw, myconst.zw
	mov outtex1,  myconst.yyyw
	mov outtex2,  myconst.yyyw

color: // result.color = in.color
	mov outcol, v_color

quat:
	// r1 = normalize(v_normal)
	mov r2, v_normal
	mov r2.w, zero.w
	dp3 r1, r2.xyz, r2.xyz
	rsq r1, r1
	mul r1, r1, v_normal

	// r0.z = (v_normal.z + 1)/2;
	add r0.z, v_normal.z, one.z
	mul r0.z, r0.z, myconst.z

	cmp r0.zz, zero.zz, >=, >=
	if cc.x  // r0.z >= 0
		mov r4.x, r0.z  // TODO: fix nihstro rsq bug
		rsq r1.z, r4.x
		mul r3.xy, v_normal.xy, myconst.zz
		mul outquat.xy, r3.xy, r1.zz
		//mul outquat.x, r3.x, r1.z
		//mul outquat.y, r3.y, r1.z

		mov r4.x, r1.z  // TODO: fix nihstro rcp bug
		rcp r1.z, r4.x
		mov outquat.z, r1.z
		mov outquat.w, zero.w
	else
		mov outquat, def_normquat
	endif

	nop
	end
endmain:
