#include "Floor.h"


float floorVertices[] = {
	// vertex vec3				// normal vec3			//texCoord vec2
	15.0f, 0.0f, -15.0f,		0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
	 15.0f, 0.0f, 15.0f,		0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
	 -15.0f, 0.0f, 15.0f,		0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
	 -15.0f, 0.0f, 15.0f,		0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
	-15.0f, 0.0f, -15.0f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
	15.0f, 0.0f, -15.0f,		0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
};


Floor::Floor() {
	Floor::model = glm::mat4(1.0f);
	Floor::model = glm::translate(model, Floor::pos);
}

Floor::~Floor() {

}

void Floor::setPhysxMaterial(PxPhysics* gPhysics, PxVec3 material) {
	Floor::pMaterial = gPhysics->createMaterial(material.x, material.y, material.z);
}

glm::mat4 Floor::getModel() {
	return Floor::model;
}

// make it a static actor
void Floor::makeStaticActor(PxPhysics* gPhysics) {
	Floor::actor = physx::PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *Floor::pMaterial);  // create x-z plane by setting normal as y-axis
}

PxRigidStatic* Floor::getActor() {
	return Floor::actor;
}

void Floor::adjustPhysxModelMatrix() {
	PxTransform modelPx = Floor::actor->getGlobalPose(); // get the model matrix transformation : local -> world space
	Floor::model = glm::mat4(1.0f); // identity matrix
	Floor::model = glm::translate(Floor::model, glm::vec3((modelPx.p).x, (modelPx.p).y, (modelPx.p).z)); // translate to new position
}