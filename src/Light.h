#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Light {
public:
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);	// position of the light source/caster, default at origin
	glm::vec3 color = glm::vec3(1.0f);				// default white light, RGB
	float ambient = 0.1f;							// ambient factor, ambient * color
	float diffuse = 1.0f;							// diffuse amount, diffuse * color
	glm::vec3 specular = glm::vec3(1.0f);			// specular light spot, default at white color

	Light();
	~Light();

};

class Spotlight: public Light {
public:
	glm::vec3 spotlightDIR = glm::vec3(0.0f, 0.0f, 0.0f);	// light direction
	float outerCutOff = cos(glm::radians(20.0f));			// outer boundary of the light spot, larger radius than inner cut off
	float innerCutOff = cos(glm::radians(12.0f));			// start of the fading boundary, smaller radius than outer cut off
	glm::vec3 attenuation = glm::vec3(1.0f, 0.09f, 0.032f); // light attenuation increase with distance, arg: constant, linear, quadratic coef

	Spotlight();
	~Spotlight();
};