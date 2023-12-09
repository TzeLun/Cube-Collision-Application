#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Shader.h"
#include <fstream>
#include <sstream>

void shader::makeShaderProgram(unsigned int* shaderprogram, const char* vertexShaderPath, const char* fragmentShaderPath) {
	
	// file loading, vertex file is in .vert whereas fragment file is .frag, as aligned with Khronos' GLSLANG
	std::string vertexStringCode;
	std::string fragmentStringCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);

		std::stringstream vertexShaderStream, fragmentShaderStream;

		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexStringCode = vertexShaderStream.str();
		fragmentStringCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE NOT SUCCESSFULLY READ" << std::endl;
	}
	const char* vertexShaderCode = vertexStringCode.c_str();
	const char* fragmentShaderCode = fragmentStringCode.c_str();

	// Create the shader program
	unsigned int vertex_shader;
	int success;
	char infoLog[512];
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION FAILED" << std::endl << infoLog << std::endl;
	}

	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION FAILED" << std::endl << infoLog << std::endl;
	}

	unsigned int shader_program;
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program); // link the shaders together
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING FAILED" << std::endl << infoLog << std::endl;
	}

	*shaderprogram = shader_program;

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

// update the color properties of the cube for lighting
void shader::setCubeMaterial(unsigned int shaderprogram, Cube cube) {
	glUseProgram(shaderprogram);
	glUniform3f(glGetUniformLocation(shaderprogram, "cube.diffuse"), cube.rMaterial.diffuse.x, cube.rMaterial.diffuse.y, cube.rMaterial.diffuse.z);
	glUniform1f(glGetUniformLocation(shaderprogram, "cube.shininess"), cube.rMaterial.shininess);
}

// bind the texture buffer to the texture attribute pointer
void shader::setFloorMaterial(unsigned int* shaderprogram, Floor floor) {
	glUseProgram(*shaderprogram);
	glUniform1i(glGetUniformLocation(*shaderprogram, "floor.diffuse"), 0); // set the sampler2D to texture0
	glUniform1f(glGetUniformLocation(*shaderprogram, "floor.shininess"), floor.rMaterial.shininess);
}

void shader::setLighting(unsigned int* shaderprogram, Spotlight light) {
	glUseProgram(*shaderprogram);
	glUniform3f(glGetUniformLocation(*shaderprogram, "slight.ambient"), (light.ambient * light.color).x, (light.ambient * light.color).y, (light.ambient * light.color).z);
	glUniform3f(glGetUniformLocation(*shaderprogram, "slight.diffuse"), (light.diffuse * light.color).x, (light.diffuse * light.color).y, (light.diffuse * light.color).z);
	glUniform3f(glGetUniformLocation(*shaderprogram, "slight.specular"), light.specular.x, light.specular.y, light.specular.z);
	glUniform3f(glGetUniformLocation(*shaderprogram, "slight.position"), light.pos.x, light.pos.y, light.pos.z);
	glUniform3f(glGetUniformLocation(*shaderprogram, "slight.spotlightDir"), light.spotlightDIR.x, light.spotlightDIR.y, light.spotlightDIR.z);
	glUniform1f(glGetUniformLocation(*shaderprogram, "slight.innerCutOff"), light.innerCutOff);
	glUniform1f(glGetUniformLocation(*shaderprogram, "slight.outerCutOff"), light.outerCutOff);
	glUniform1f(glGetUniformLocation(*shaderprogram, "slight.constant"), light.attenuation.x);
	glUniform1f(glGetUniformLocation(*shaderprogram, "slight.linear"), light.attenuation.y);
	glUniform1f(glGetUniformLocation(*shaderprogram, "slight.quadratic"), light.attenuation.z);
}
