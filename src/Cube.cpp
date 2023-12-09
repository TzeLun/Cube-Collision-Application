#include "Cube.h"


// cube vertex data
float cubeVertices[] =
{
	// positions vec3         // normals vec3     // texture coords vec2
	   -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	   -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	   -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	   -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	   -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	   -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	   -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	   -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	   -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	   -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	   -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	   -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	   -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	   -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	   -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	   -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	   -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	   -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};


Cube::Cube(float size) {
	Cube::size = size;
}

Cube::~Cube() {

}

float Cube::getSize() {
	return Cube::size;
}

// update the model matrix for the changes to the cube's position, used without physx, doesn't support orientation
void Cube::updateModel() {
	Cube::model = glm::mat4(1.0f);
	Cube::model = glm::translate(Cube::model, Cube::pos);
	Cube::model = glm::scale(Cube::model, glm::vec3(Cube::size));
}

glm::mat4 Cube::getModel() {
	return Cube::model;
}

// set the position of the cube in the scene, default at origin, have to manually update the model matrix
void Cube::setPos(glm::vec3 pos) {
	Cube::pos = pos;
}

// configure the cube render material properties
void Cube::setGraphicMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
	Cube::rMaterial.ambient = ambient;
	Cube::rMaterial.diffuse = diffuse;
	Cube::rMaterial.specular = specular;
	Cube::rMaterial.shininess = shininess;
}

// configure the physx material properties for the cube
void Cube::setPhysxMaterial(PxPhysics* gPhysics, PxVec3 material, PxReal density) {
	Cube::pMaterial = gPhysics->createMaterial(material.x, material.y, material.z);
	Cube::density = density;
}

PxRigidDynamic* Cube::getActor() {
	return Cube::actor;
}

// Make the cube a rigid dynamic actor, have to add manually to the scene
void Cube::makeDynamicActor(PxPhysics* gPhysics) {
	Cube::actor = gPhysics->createRigidDynamic(physx::PxTransform(PxVec3(Cube::pos.x, Cube::pos.y, Cube::pos.z)));
	physx::PxShape* cuboid = physx::PxRigidActorExt::createExclusiveShape(*Cube::actor,
		physx::PxBoxGeometry(Cube::size, Cube::size, Cube::size), *Cube::pMaterial);
	physx::PxRigidBodyExt::updateMassAndInertia(*Cube::actor, Cube::density);
}


// Obtain the new model matrix to compute the world space for rendering, model matrix : scale -> rotate -> translate (order), reverse for (execution)
void Cube::computePhysxModelMatrix() {
	PxTransform modelPx = Cube::actor->getGlobalPose(); // get the model matrix transformation : local -> world space
	glm::quat quat = glm::quat((modelPx.q).w, (modelPx.q).x, (modelPx.q).y, (modelPx.q).z); // quaternion of the rotation (orientation)
	Cube::model = glm::mat4(1.0f); // identity matrix
	Cube::model = glm::translate(Cube::model, glm::vec3((modelPx.p).x, (modelPx.p).y, (modelPx.p).z)); // translate to new position
	Cube::model = Cube::model * glm::toMat4(quat); // include the rotation matrix to the model matrix
}