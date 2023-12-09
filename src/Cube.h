#pragma once
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>


using namespace physx;

struct cubematerial {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

class Cube {
public:
	cubematerial rMaterial = {
		glm::vec3(1.0f, 1.0f, 1.0f), // ambient
		glm::vec3(1.0f, 1.0f, 1.0f), // diffuse (only this is needed in this applicaiton)
		glm::vec3(1.0f, 1.0f, 1.0f), // specular
		64.0f						// shininess (only this is needed in this application)
	}; // material of cube for rendering purposes

	Cube(float size);
	~Cube();

	float getSize();
	void updateModel();
	glm::mat4 getModel();
	void setPos(glm::vec3 pos); // set the position of the cube
	void setGraphicMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess); // set material properties for OpenGL rendering
	void setPhysxMaterial(PxPhysics* gPhysics, PxVec3 material, PxReal density); // set physx material properties
	PxRigidDynamic* getActor();
	void makeDynamicActor(PxPhysics* gPhysics); // make cube into a rigid dynamic actor
	void computePhysxModelMatrix(); // get new model matrix during simulation

private:
	float size = 1.0f; // size of the cube, will be used to scale the cube (modifies the model matrix)
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f); // position of cube in the scene, will be used to translate the cube (modifies the model matrix), default at origin
	glm::mat4 model;
	PxMaterial* pMaterial; // material of cube for physics engine
	PxReal density = 1.0f; // default density set to 1 kg/m^3
	PxRigidDynamic* actor;
};

