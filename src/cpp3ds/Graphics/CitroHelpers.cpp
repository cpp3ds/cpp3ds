#include "CitroHelpers.hpp"

namespace
{
	C3D_MtxStack projectionMatrix, modelviewMatrix, textureMatrix;
}

void CitroInit()
{
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	// Configure attributes for use with the vertex shader
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 2); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_UNSIGNED_BYTE, 4); // v1=color
	AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 2); // v2=texcoord

	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR, 0, 0);
	C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

	C3D_DepthTest(false, GPU_GEQUAL, GPU_WRITE_ALL);
	C3D_CullFace(GPU_CULL_NONE);
}

void CitroDestroy()
{
	C3D_Fini();
}

void CitroBindUniforms(shaderProgram_s* program)
{
	int location;
	location = shaderInstanceGetUniformLocation(program->vertexShader, "projection");
	MtxStack_Bind(&projectionMatrix, GPU_VERTEX_SHADER, location, 4);
	location = shaderInstanceGetUniformLocation(program->vertexShader, "modelview");
	MtxStack_Bind(&modelviewMatrix, GPU_VERTEX_SHADER, location, 4);
	location = shaderInstanceGetUniformLocation(program->vertexShader, "texture");
	MtxStack_Bind(&textureMatrix, GPU_VERTEX_SHADER, location, 4);
}

void CitroUpdateMatrixStacks()
{
	MtxStack_Update(&projectionMatrix);
	MtxStack_Update(&modelviewMatrix);
	MtxStack_Update(&textureMatrix);
}

C3D_MtxStack* CitroGetProjectionMatrix()
{
	return &projectionMatrix;
}

C3D_MtxStack* CitroGetModelviewMatrix()
{
	return &modelviewMatrix;
}

C3D_MtxStack* CitroGetTextureMatrix()
{
	return &textureMatrix;
}
