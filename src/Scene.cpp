#include "Scene.h"
#include <cstdlib>
#include <ctime>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

extern float cubeVertices[288]; // from Cube.cpp
extern float floorVertices[48]; // from Floor.cpp
float SCR_WIDTH = 800.0;								// screen width
float SCR_HEIGHT = 600.0;								// screen height
float yaw = -90.0f;										// if 0, camera facing along the x-axis, rotate clockwise 90 deg at the start (counter-clockwise is +ve). left right rotation
float pitch = 0.0f;										// changing this will rotate the camera up and down.
float lastxpos = SCR_WIDTH / 2.0f;						// both for mouse callback function
float lastypos = SCR_HEIGHT / 2.0f;
float fov = 45.0f;										// For scrolling callback function
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);	// for the mouse callback function
bool spawn = false;

Scene::Scene() {

}

Scene::~Scene() {
	std::cout << "PROGRAM TERMINATED" << std::endl;
}

void Scene::destroyAll(GLFWwindow* window) {
	std::cout << "DELETING BUFFERS AND SHADERS..." << std::endl;
	glfwDestroyWindow(window);
	glDeleteVertexArrays(1, &(Scene::cubeVAO));
	glDeleteBuffers(1, &(Scene::cubeVBO));
	glDeleteVertexArrays(1, &(Scene::floorVAO));
	glDeleteBuffers(1, &(Scene::floorVBO));
	glDeleteTextures(1, &(Scene::floorTexBuf));
	glDeleteProgram(Scene::cubeShader);
	glDeleteProgram(Scene::floorShader);
	std::cout << "TERMINATING GLFW..." << std::endl;
	glfwTerminate();
}

// Update projection, view and view position in the floor and cube shader
void Scene::updateViewSettings(Camera camera) {
	glUseProgram(Scene::floorShader);
	glUniformMatrix4fv(glGetUniformLocation(Scene::floorShader, "projection"), 1, GL_FALSE, glm::value_ptr(Scene::projection));
	glUniformMatrix4fv(glGetUniformLocation(Scene::floorShader, "view"), 1, GL_FALSE, glm::value_ptr(Scene::view));
	glUniform3f(glGetUniformLocation(Scene::floorShader, "viewPos"), camera.pos().x, camera.pos().y, camera.pos().z); // needed for specular lighting

	glUseProgram(Scene::cubeShader);
	glUniformMatrix4fv(glGetUniformLocation(Scene::cubeShader, "projection"), 1, GL_FALSE, glm::value_ptr(Scene::projection));
	glUniformMatrix4fv(glGetUniformLocation(Scene::cubeShader, "view"), 1, GL_FALSE, glm::value_ptr(Scene::view));
	glUniform3f(glGetUniformLocation(Scene::cubeShader, "viewPos"), camera.pos().x, camera.pos().y, camera.pos().z); // needed for specular lighting
}

// draw the floor
void Scene::drawFloor(Floor floor) {
	glUseProgram(Scene::floorShader);
	glBindVertexArray(Scene::floorVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Scene::floorTexBuf);
	glUniformMatrix4fv(glGetUniformLocation(Scene::floorShader, "model"), 1, GL_FALSE, glm::value_ptr(floor.getModel()));
	glm::mat3 normMat = glm::mat3(glm::transpose(glm::inverse(floor.getModel())));
	glUniformMatrix3fv(glGetUniformLocation(Scene::floorShader, "normMat"), 1, GL_FALSE, glm::value_ptr(normMat));
	glDrawArrays(GL_TRIANGLES, 0, 6); // floor has six vertices
	glBindVertexArray(0);
}

// draw a single cube
void Scene::drawCube(Cube cube) {
	glUseProgram(Scene::cubeShader);
	glBindVertexArray(Scene::cubeVAO);
	glUniform3f(glGetUniformLocation(Scene::cubeShader, "cube.diffuse"), cube.rMaterial.diffuse.x, cube.rMaterial.diffuse.y, cube.rMaterial.diffuse.z);
	glUniform1f(glGetUniformLocation(Scene::cubeShader, "cube.shininess"), cube.rMaterial.shininess);
	glUniform1f(glGetUniformLocation(Scene::cubeShader, "size"), cube.getSize());
	glUniformMatrix4fv(glGetUniformLocation(Scene::cubeShader, "model"), 1, GL_FALSE, glm::value_ptr(cube.getModel()));
	glm::mat3 normMat = glm::mat3(glm::transpose(glm::inverse(cube.getModel())));
	glUniformMatrix3fv(glGetUniformLocation(Scene::cubeShader, "normMat"), 1, GL_FALSE, glm::value_ptr(normMat));
	glDrawArrays(GL_TRIANGLES, 0, 36); // floor has 36 vertices
	glBindVertexArray(0);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	float xoffset, yoffset;
	xoffset = xpos - lastxpos;
	yoffset = lastypos - ypos; // reversed since y-axis starts from bottom (+ve) to top (-ve)
	lastxpos = xpos;
	lastypos = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// higher than 89 or lower than -89 will lead to lookAt flip
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	else if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	extern float fov;
	fov -= yoffset;
	if (fov < 1.0f) {
		fov = 1.0f;
	}
	else if (fov > 45.0) {
		fov = 45.0f;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Scene::process_input(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		spawn = true;
	}
}

void Scene::spawnCube(Camera camera) {
	float R = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
	float G = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
	float B = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
	float speed = 10.0f;
	glm::vec3 direction = camera.front();
	glm::vec3 velocity = direction * speed;
	Cube cube(0.2f);																				// cube of side length 0.2
	cube.setPos(camera.pos());																		// cube spawns at camera position (viewer's position)
	cube.updateModel();																				// update the model matrix with the new position
	cube.setGraphicMaterial(glm::vec3(1.0f), glm::vec3(R, G, B), glm::vec3(1.0f), 64.0f);			// cube has random colours
	cube.setPhysxMaterial(Scene::gPhysics, physx::PxVec3(0.3f, 0.47f, 0.2f), 1.0f);					// static 0.3f, dynamic 0.47f, restitution 0.2f, density 1.0f
	cube.makeDynamicActor(Scene::gPhysics);															// make cube into dynamic actor
	cube.getActor()->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z), true);	// initial linear velocity that shoots cube as projectile
	Scene::gScene->addActor(*cube.getActor());
	Scene::cubeRender.push_back(cube);																// add the cube's reference for rendering purposes
}

void Scene::setupCubeVertexBuffer(int layout) {
	glGenVertexArrays(1, &(Scene::cubeVAO));
	glGenBuffers(1, &(Scene::cubeVBO));
	glBindVertexArray(Scene::cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, Scene::cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(++layout, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(++layout, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(layout);
}

void Scene::setupFloorVertexBuffer(int layout) {
	glGenVertexArrays(1, &(Scene::floorVAO));
	glGenBuffers(1, &(Scene::floorVBO));
	glBindVertexArray(Scene::floorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, Scene::floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(++layout, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(++layout, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(layout);
}

void Scene::floorTextureSetup(const char* texturePath) {
	int width, height, nrChannels;
	GLenum format;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	if (data) {

		if (nrChannels == 4) {
			format = GL_RGBA;
		}
		else if (nrChannels == 3) {
			format = GL_RGB;
		}
		else if (nrChannels == 1) {
			format = GL_RED;
		}

		glGenTextures(1, &(Scene::floorTexBuf));
		glBindTexture(GL_TEXTURE_2D, Scene::floorTexBuf);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "ERROR::CODE FAILED TO CREATE IMAGE DATA" << std::endl;
		stbi_image_free(data);
	}
}

// Initialization of PhysX
void Scene::initPhysics() {
	std::cout << "INITIALIZING PHYSX..." << std::endl;
	// Create foundation (compulsory) :
	Scene::gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, Scene::gDefaultAllocatorCallback, Scene::gDefaultErrorCallback);
	if (!gFoundation) {
		std::cout << "ERROR::CODE FAILED TO CREATE PXFOUNDATION" << std::endl;
		std::exit(1);
	}

	// Create top-level PxPhysics object (compulsory) :
	bool recordMemoryAllocation = true;

	Scene::gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *(Scene::gFoundation), Scene::gToleranceScale, recordMemoryAllocation);
	if (!gPhysics) {
		std::cout << "ERROR::CODE FAILED TO CREATE PXPHYSICS" << std::endl;
		std::exit(1);
	}

	// Initialize cooking, needed for creating convex meshes :
	Scene::gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *(Scene::gFoundation), physx::PxCookingParams(Scene::gToleranceScale));
	if (!gCooking) {
		std::cout << "ERROR::CODE FAILED TO INITIALIZE PXCOOKING" << std::endl;
		std::exit(1);
	}

	// Initialize PxScene
	physx::PxSceneDesc sceneDesc(Scene::gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	Scene::gDispatcher = physx::PxDefaultCpuDispatcherCreate(0);						// create only one thread (no mutlithreading)
	if (!Scene::gDispatcher) {
		std::cout << "ERROR::CODE FAILED TO CREATE CPU DISPATCHER" << std::endl;
		std::exit(1);
	}
	sceneDesc.cpuDispatcher = Scene::gDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	Scene::gScene = Scene::gPhysics->createScene(sceneDesc);
	if (!Scene::gScene) {
		std::cout << "ERROR::CODE FAILED TO CREATE PXSCENE" << std::endl;
		std::exit(1);
	}
}

// release the physx classes once program is over
void Scene::releasePhysx() {
	// Once done, destroy the physics, foundation and cooking object
	std::cout << "RELEASING PHYSX OBJECTS..." << std::endl;
	Scene::gScene->release();
	Scene::gCooking->release();
	Scene::gPhysics->release();
	Scene::gFoundation->release();
}

// the main function to run the application
void Scene::run() {
	srand(static_cast <unsigned> (time(nullptr)));
	Scene::initPhysics();
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cube Collision Application", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// initialize camera
	Camera camera;
	camera.setPos(glm::vec3(0.0f, 3.0f, 15.0f));
	camera.setFront(glm::vec3(0.0f, 0.0f, -1.0f));
	camera.setUp(glm::vec3(0.0f, 1.0f, 0.0f));

	// set the mouse cursor callback
	// Mouse always position at the center of the window (never leave the window)
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);										// disable here is to hide the cursor
	glfwSetCursorPos(window, lastxpos, lastypos);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetMouseButtonCallback(window, mouse_button_callback);											// spawn cube when left clicked

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "ERROR::CODE FAILED TO INITIALIZE GLAD" << std::endl;
		return;
	}

	shader::makeShaderProgram(&(Scene::cubeShader), "D:/Nvidia PhysX Tinker/PhysX setup/shaders/cubeShader.vert", "D:/Nvidia PhysX Tinker/PhysX setup/shaders/cubeShader.frag");
	shader::makeShaderProgram(&(Scene::floorShader), "D:/Nvidia PhysX Tinker/PhysX setup/shaders/floorShader.vert", "D:/Nvidia PhysX Tinker/PhysX setup/shaders/floorShader.frag");

	Scene::setupCubeVertexBuffer(0);
	Scene::setupFloorVertexBuffer(3);

	Scene::floorTextureSetup("D:/Nvidia PhysX Tinker/PhysX setup/floorTexture/checkerboard.jpg");
	
	// Define the actors or objects
	// Floor:
	Floor floor;
	floor.rMaterial.shininess = 64.0f;
	shader::setFloorMaterial(&(Scene::floorShader), floor);												// Bind texture buffer and update the material shininess
	floor.setPhysxMaterial(Scene::gPhysics, physx::PxVec3(0.5, 0.5, 0.5));								// floor frition : (static 0.5, dynamic 0.5), restitution 0.5
	floor.makeStaticActor(Scene::gPhysics);																// make floor static actor
	Scene::gScene->addActor(*(floor.getActor()));														// Add floor actor to the scene

	// Central Cube:
	float centralCubeSize = 2.0f;
	Cube centerCube(centralCubeSize);
	centerCube.rMaterial.diffuse = glm::vec3(0.83, 0.686, 0.216);										// brass (golden) colour
	centerCube.rMaterial.shininess = 64.0f;
	centerCube.setPos(glm::vec3(0.0f, centralCubeSize, 0.0f));
	centerCube.updateModel();																			// not using orientation!
	centerCube.setPhysxMaterial(Scene::gPhysics, physx::PxVec3(0.51, 0.44, 0.54), 8.50f);				// centerCube treated as brass cube, set material and density
	centerCube.makeDynamicActor(Scene::gPhysics);														// make dynamic cube actor
	Scene::gScene->addActor(*centerCube.getActor());													// add center cube actor to the scene
	bool first_time = true;																				// render the center cube without getting its physics for the first time

	// Spotlight:
	Spotlight light;
	light.pos = glm::vec3(0.0f, 20.0f, -18.0f);															// position of light source
	light.ambient = 0.1f;																				// ambient strength
	light.spotlightDIR = glm::vec3(0.0f, 0.0f, -5.5f) - light.pos;										// shine onto the origin
	light.innerCutOff = cos(glm::radians(28.0f));
	light.outerCutOff = cos(glm::radians(30.0f));
	light.attenuation = glm::vec3(1.0f, 0.0f, 0.0f);													// attenuation not considered == 1.0

	float lastFrame = 0.0f;
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);																			// allow depth test
	std::cout << "START OF SIMULATION..." << std::endl;
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();																// ensure consistent fps for runs on different PC
		camera.setDt(currentFrame - lastFrame);
		lastFrame = currentFrame;
		camera.setFront(cameraFront);																	// update the camera front vector
		Scene::process_input(window);																	// check if left click is made, also execute other processes
		camera.strafe(window);
		Scene::projection = glm::perspective(glm::radians(fov), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);	// projection matrix for camera view
		Scene::view = glm::lookAt(camera.pos(), camera.pos() + camera.front(), camera.up());			// camera view
		Scene::updateViewSettings(camera);
		//light.pos = camera.pos();
		//light.spotlightDIR = camera.front();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);															// set the back ground color of the window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set lighting for floor and cube
		light.ambient = 0.1f;
		shader::setLighting(&(Scene::floorShader), light);

		light.ambient = 0.3f;
		shader::setLighting(&(Scene::cubeShader), light);

		// draw floor first
		Scene::drawFloor(floor);
		if (first_time) {
			Scene::drawCube(centerCube);
			first_time = false;
		}

		if (spawn) {
			Scene::spawnCube(camera);
			spawn = false;
		}

		// Simulate Physx and render the Cube actors :
		physx::PxReal dt = 1.0f / 60.0f;
		Scene::gScene->simulate(dt);
		Scene::gScene->fetchResults();

		// first render the center cube at its new position
		if (!first_time) {
			centerCube.computePhysxModelMatrix();
			Scene::drawCube(centerCube);
		}

		// then render each cube spawned into the scene, remove one at a time if number of cubes exceed 64
		if (!Scene::cubeRender.empty()) {
			if (Scene::cubeRender.size() > 64) {
				Scene::cubeRender[0].getActor()->release();						// free the oldest cube actor
				Scene::cubeRender.erase(Scene::cubeRender.begin());				// erase the cube from the render list
			}

			for (std::vector<Cube>::iterator cube = Scene::cubeRender.begin(); cube != Scene::cubeRender.end(); ++cube) {
				Cube c = *cube;
				c.computePhysxModelMatrix();
				Scene::drawCube(c);
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	
	Scene::destroyAll(window);
	Scene::releasePhysx();
}