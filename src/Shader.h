#include "Cube.h"
#include "Floor.h"
#include "Light.h"

#pragma once

namespace shader {
	void makeShaderProgram(unsigned int* shaderprogram, const char* vertexShaderPath, const char* fragmentShaderPath);
	void setCubeMaterial(unsigned int shaderprogram, Cube cube);
	void setFloorMaterial(unsigned int* shaderprogram, Floor floor);
	void setLighting(unsigned int* shaderprogram, Spotlight light);
}
