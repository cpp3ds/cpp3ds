#pragma once
#include <citro3d.h>

void CitroInit(size_t commandBufferSize);
void CitroDestroy();
void CitroBindUniforms(shaderProgram_s* program);
void CitroUpdateMatrixStacks();
C3D_MtxStack* CitroGetProjectionMatrix();
C3D_MtxStack* CitroGetModelviewMatrix();
C3D_MtxStack* CitroGetTextureMatrix();
