#pragma once
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace physx;

struct floormaterial {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

class Floor {
public:

	Floor();
	~Floor();
	
	floormaterial rMaterial = {
		glm::vec3(1.0f, 1.0f, 1.0f), // ambient
		glm::vec3(1.0f, 1.0f, 1.0f), // diffuse
		glm::vec3(1.0f, 1.0f, 1.0f), // specular
		64.0f						// only this is needed since the floor is made up with textures
	};

	void setPhysxMaterial(PxPhysics* gPhysics, PxVec3 material);
	glm::mat4 getModel();
	void makeStaticActor(PxPhysics* gPhysics);
	void adjustPhysxModelMatrix();
	PxRigidStatic* getActor();

private:
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);	// position of floor, fixed at origin (adjusted the y-axis)
	glm::mat4 model = glm::mat4(1.0f);
	PxMaterial* pMaterial;							// material of floor for physics engine, (static friction coef, dynamic friction coef, restitution [1 for elastic collision])
	PxRigidStatic* actor;							// floor is always a static actor
};