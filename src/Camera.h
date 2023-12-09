#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera {
public:

	Camera();
	~Camera();

	void strafe(GLFWwindow* window);	// strafe around with W (forward), A (left), S (backward), D (right)
	void setPos(glm::vec3 pos);				// set camera position 
	void setFront(glm::vec3 front);			// set camera front vector
	void setUp(glm::vec3 up);				// set up vector
	void setDt(float dt);					// update delta time
	glm::vec3 pos();						// return camera position
	glm::vec3 front();						// return camera front vector
	glm::vec3 up();							// return up vector
	float dt();								// retrieve deltaTime

private:
	glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f); // camera initial position
	glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f); // vector that dictates where the camera is facing, if z is negative, camera will face the front of object, positive means backside
	glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f); // up vector to calculate camera left/right (x-axis) vector.

	// Needed to calculate the rate of frame change. The frame change differs for everyone's PC based on the PC specs
	// This allows updates of the strafe to be consistent accross any PC
	float deltaTime = 0.0f;

};