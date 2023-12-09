/**
#include <iostream>
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

#define PVD_HOST "isaa8888" // change the TCP/IP host IP address for streaming PVD based on network setup

// declare variables (very important)
physx::PxDefaultAllocator gDefaultAllocatorCallback;
physx::PxDefaultErrorCallback gDefaultErrorCallback;
physx::PxPhysics* gPhysics = NULL;
physx::PxFoundation* gFoundation = NULL;
physx::PxCooking* gCooking = NULL;
physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
physx::PxScene* gScene = NULL;
physx::PxMaterial* capMat; // capsule material
physx::PxMaterial* boxMat; // box material
physx::PxMaterial* meshMat; // mesh material (can be any objecct)
physx::PxPvd* gPvd = NULL;
physx::PxTolerancesScale gToleranceScale;  // defines the scale like units, gravity in the scene
physx::PxReal stackZ = 10.0f;
physx::PxPvdTransport* transport; // for pvd

int demoMain();

int demoMain() {

	initPhysics();
	
	// do something here
	
	//create one capsule
	capMat = gPhysics->createMaterial(0.5f, 0.5f, 0.6f); // define capsule material
	physx::PxReal capDensity = 1.0f; // set capsule density
	float capRadius = 10.0f; // capsule radius
	float capHHeight = 5.0f; // capsule half height
	physx::PxVec3 capPos(0.5f, 0.0f, 0.5f); // capsule position
	createCapsuleActor(capPos, capRadius, capHHeight, capMat, capDensity);

	// create one box
	boxMat = gPhysics->createMaterial(0.1f, 0.1f, 0.1f); // define box material
	physx::PxReal boxDensity = 2.0f; // set box density
	physx::PxVec3 boxPos(0.5f, 0.0f, 0.5f); // position of the box
	physx::PxVec3 boxDim(5.0f, 5.0f, 5.0f); // 5 by 5 by 5 cube box
	createBoxActor(boxPos, boxDim, boxMat, boxDensity);

	// create a convex mesh, a simple square pyramid shape

	// vertices of a convex mesh
	physx::PxVec3 convexVerts[] = { 
		physx::PxVec3(0,1,0),
		physx::PxVec3(1,0,0),
		physx::PxVec3(-1,0,0),
		physx::PxVec3(0,0,1),
		physx::PxVec3(0,0,-1)
	};
	meshMat = gPhysics->createMaterial(0.2f, 0.8f, 1.0f); // define mesh material, (static friction coef, dynamic friction coef, restitution [1 for elastic collision])
	physx::PxVec3 meshPos(0.0f, 0.0f, 0.0f); // set at origin
	physx::PxReal meshDensity = 0.2f;
	createConvexMesh(convexVerts, meshPos, meshMat, meshDensity);

	runPhysics(); // run simulation

	// end of task

	releasePhysics();

	return 0;
}

void initPhysics() {
	// Foundation is needed
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback);
	if (!gFoundation) {
		std::cout << "Failed to create PxFoundation" << std::endl;
		std::exit(1);
	}

	// create top-level PxPhysics object
	bool recordMemoryAllocation = true;

	// Uncomment to allow debugging with PhysX Visual Debugger
	gPvd = physx::PxCreatePvd(*gFoundation);
	transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation,
		gToleranceScale, recordMemoryAllocation);
	if (!gPhysics) {
		std::cout << "Failed to create PxPhysics object" << std::endl;
		std::exit(1);
	}

	// Initialize cooking, needed for creating convex meshes
	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation,
		physx::PxCookingParams(gToleranceScale));
	if (!gCooking) {
		std::cout << "Failed to initialize PxCooking object" << std::endl;
		std::exit(1);
	}
}

void releasePhysics() {
	// Once done, destroy the physics, foundation and pvd object
	gPhysics->release();
	gPvd->release();
	transport->release();
	gFoundation->release();
}

// create a single capsule
void createCapsuleActor(physx::PxVec3 position, float radius, float halfHeight, physx::PxMaterial* material, physx::PxReal capsuleDensity) {
	physx::PxRigidDynamic* aCapsuleActor = gPhysics->createRigidDynamic(physx::PxTransform(position));
	physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	physx::PxShape* aCapsuleShape = physx::PxRigidActorExt::createExclusiveShape(*aCapsuleActor,
		physx::PxCapsuleGeometry(radius, halfHeight), *material);
	aCapsuleShape->setLocalPose(relativePose);
	physx::PxRigidBodyExt::updateMassAndInertia(*aCapsuleActor, capsuleDensity);
	gScene->addActor(*aCapsuleActor);
}

// create a single box
void createBoxActor(physx::PxVec3 position, physx::PxVec3 dim, physx::PxMaterial* material, physx::PxReal boxDensity) {
	physx::PxRigidDynamic* aBoxActor = gPhysics->createRigidDynamic(physx::PxTransform(position));
	physx::PxShape* aBoxShape = physx::PxRigidActorExt::createExclusiveShape(*aBoxActor,
		physx::PxBoxGeometry(dim.x/2.0, dim.y/2.0, dim.z/2.0), *material); 
	physx::PxRigidBodyExt::updateMassAndInertia(*aBoxActor, boxDensity);
	gScene->addActor(*aBoxActor);
}

// create one convex mesh
void createConvexMesh(physx::PxVec3 vertices[], physx::PxVec3 position, physx::PxMaterial* material, physx::PxReal meshDensity) {
	physx::PxConvexMeshDesc convexDesc; // creates a description object for the convex shape
	convexDesc.points.count = sizeof(vertices)/sizeof(physx::PxVec3); // number of vertices
	convexDesc.points.stride = sizeof(physx::PxVec3); // length of stride
	convexDesc.points.data = vertices; // vertex data
	convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX; // use this flag if only vertices are given to build the mesh

	// Create convex mesh through stream serialization (recommended)
	physx::PxDefaultMemoryOutputStream buf;
	physx::PxConvexMeshCookingResult::Enum result;
	if (!gCooking->cookConvexMesh(convexDesc, buf, &result)) {
		return;
	}
	physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	physx::PxConvexMesh* convexMesh = gPhysics->createConvexMesh(input);
	physx::PxRigidDynamic* aConvexActor = gPhysics->createRigidDynamic(physx::PxTransform(position));
	physx::PxShape* aConvexShape = physx::PxRigidActorExt::createExclusiveShape(*aConvexActor,
		physx::PxConvexMeshGeometry(convexMesh), *material);
	physx::PxRigidBodyExt::updateMassAndInertia(*aConvexActor, meshDensity);
	gScene->addActor(*aConvexActor);
}

void runPhysics() {
	physx::PxReal dt = 1.0f / 60.0f; // 60 Hz
	gScene->simulate(dt);
	gScene->fetchResults();
}

*/