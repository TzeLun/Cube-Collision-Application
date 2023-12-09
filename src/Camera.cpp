#include "Camera.h"

Camera::Camera() {
	
}

Camera::~Camera() {

}


void Camera::strafe(GLFWwindow* window) {

	Camera::camera_pos, Camera::camera_front, Camera::camera_up;
	Camera::deltaTime;
	const float camera_speed = 20.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Camera::camera_pos += camera_speed * Camera::camera_front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Camera::camera_pos -= camera_speed * Camera::camera_front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Camera::camera_pos -= camera_speed * glm::normalize(glm::cross(Camera::camera_front, Camera::camera_up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Camera::camera_pos += camera_speed * glm::normalize(glm::cross(Camera::camera_front, Camera::camera_up));
	}
}

void Camera::setPos(glm::vec3 pos) {
	Camera::camera_pos = pos;
}

void Camera::setFront(glm::vec3 front) {
	Camera::camera_front = front;
}

void Camera::setUp(glm::vec3 up) {
	Camera::camera_up = up;
}

void Camera::setDt(float dt) {
	Camera::deltaTime = dt;
}

glm::vec3 Camera::pos() {
	return Camera::camera_pos;
}

glm::vec3 Camera::front() {
	return Camera::camera_front;
}

glm::vec3 Camera::up() {
	return Camera::camera_up;
}

float Camera::dt() {
	return Camera::deltaTime;
}