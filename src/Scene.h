#pragma once
#include <iostream>
#include <vector>
#include "Cube.h"
#include "Floor.h"
#include "Light.h"
#include "Shader.h"
#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

class Scene {
public:
	Scene();
	~Scene();

	void run();

private:
	unsigned int cubeVAO, cubeVBO, floorVAO, floorVBO;
	unsigned int cubeShader, floorShader, floorTexBuf;
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	physx::PxDefaultAllocator gDefaultAllocatorCallback;
	physx::PxDefaultErrorCallback gDefaultErrorCallback;
	physx::PxPhysics* gPhysics = NULL;						// Physics class to create actors
	physx::PxFoundation* gFoundation = NULL;				// foundation of the physics engine
	physx::PxCooking* gCooking = NULL;						// only needed if convex mesh is in used. Currently unnecessary
	physx::PxDefaultCpuDispatcher* gDispatcher = NULL;		// for multithreading
	physx::PxScene* gScene = NULL;							// main scene for all actors to interact
	physx::PxTolerancesScale gToleranceScale;				// defines the scale like units, gravity in the scene
	std::vector<Cube> cubeRender;							// pointer to cubes spawned for rendering
	std::vector<PxRigidDynamic*> cubeActor;					// stores the incumbent dynamic cube actors
	void setupCubeVertexBuffer(int layout);					// Setup the VAO and VBO for the cube object
	void setupFloorVertexBuffer(int layout);				// Setup the VAO and VBO for the floor object
	void floorTextureSetup(const char* texturePath);		// Setup the texture for the floor object
	void updateViewSettings(Camera camera);					// Update the projection and view matrix together with the camera position in the Shader Program
	void drawFloor(Floor floor);							// Draw a floor object
	void drawCube(Cube cube);								// Draw a cube object
	void initPhysics();										// initialize foundation, physics, cooking and scene (gravity)
	void releasePhysx();									// Release all physx class
	void destroyAll(GLFWwindow* window);					// Terminate window, release buffer memory
	void process_input(GLFWwindow* window);					// execute certain processes
	void spawnCube(Camera camera);
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos);						// able to orient the cursor around the screen with the mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);				// able to change the field of view by scrolling the mouse
void framebuffer_size_callback(GLFWwindow* window, int width, int height);				// able to change the screen size while the program is running
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);		// main function to spawn cubes