#include "StdAfx.h"
#include "BulletPhysics.h"

BulletPhysics::BulletPhysics(const D3DXVECTOR3& gravityDX) {
	btVector3 gravity = ConvertVectorDxToBt(gravityDX);
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
	dynamicsWorld->setGravity(gravity);
}

BulletPhysics::~BulletPhysics() {
	for (int i = dynamicsWorld->getNumConstraints() - 1; i >= 0; --i){
		btTypedConstraint* constraint = dynamicsWorld->getConstraint(i);
		dynamicsWorld->removeConstraint(constraint);
		delete constraint;
	}
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; --i) {
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) delete body->getMotionState();
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}
	for (int i = 0; i < collisionShapes.size(); ++i) {
		btCollisionShape* shape = collisionShapes[i];
		collisionShapes[i] = 0;
		delete shape;
	}
	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;
	collisionShapes.clear();
}

btRigidBody* BulletPhysics::CreateShape(btCollisionShape* shape, const D3DXMATRIX* world,
										float mass, float restitution, float friction, float linear_damp, float angular_damp, bool kinematic, unsigned short group, unsigned short mask) {
	collisionShapes.push_back(shape);
	btVector3 localInertia(0, 0, 0);
	if (mass != 0) shape->calculateLocalInertia(mass, localInertia);
	btDefaultMotionState* motionState = new btDefaultMotionState(ConvertMatrixDxToBt(*world));
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setRestitution(restitution);
	body->setFriction(friction);
	body->setDamping(linear_damp, angular_damp);
	float linearDamp = body->getLinearDamping();
	float angularDamp = body->getAngularDamping();
	if (kinematic) {
		body->setCollisionFlags( body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}
	dynamicsWorld->addRigidBody(body, group, mask);
	return body;
}

btRigidBody* BulletPhysics::CreateBox(float width, float height, float depth, const D3DXMATRIX* world,
									  float mass, float restitution, float friction, float linear_damp, float angular_damp, bool kinematic, unsigned short group, unsigned short mask) {
	btVector3 halfExtents(width/2, height/2, depth/2);
	btCollisionShape* shape = new btBoxShape(halfExtents);
	if (kinematic) mass = 0;
	return CreateShape(shape, world, mass, restitution, friction, linear_damp, angular_damp, kinematic, group, mask);
}

btRigidBody* BulletPhysics::CreateSphere(float radius, const D3DXMATRIX* world,
									  float mass, float restitution, float friction, float linear_damp, float angular_damp, bool kinematic, unsigned short group, unsigned short mask) {
	btCollisionShape* shape = new btSphereShape(radius);
	if (kinematic) mass = 0;
	return CreateShape(shape, world, mass, restitution, friction, linear_damp, angular_damp, kinematic, group, mask);
}

btRigidBody* BulletPhysics::CreateCylinder(float radius, float length, const D3DXMATRIX* world,
									  float mass, float restitution, float friction, float linear_damp, float angular_damp, bool kinematic, unsigned short group, unsigned short mask) {
	btVector3 halfExtents(radius, radius, length/2);
	btCollisionShape* shape = new btCylinderShape(halfExtents);
	if (kinematic) mass = 0;
	return CreateShape(shape, world, mass, restitution, friction, linear_damp, angular_damp, kinematic, group, mask);
}

btRigidBody* BulletPhysics::CreateCapsule(float radius, float height, const D3DXMATRIX* world,
									  float mass, float restitution, float friction, float linear_damp, float angular_damp, bool kinematic, unsigned short group, unsigned short mask) {
	btCollisionShape* shape = new btCapsuleShape(radius, height);
	if (kinematic) mass = 0;
	return CreateShape(shape, world, mass, restitution, friction, linear_damp, angular_damp, kinematic, group, mask);
}

void BulletPhysics::AddPointToPointConstraint(btRigidBody* body, const D3DXVECTOR3& pivotDX) {
	btVector3 pivot = ConvertVectorDxToBt(pivotDX);
	dynamicsWorld->addConstraint(new btPoint2PointConstraint(*body, pivot));
}

void BulletPhysics::AddPointToPointConstraint(btRigidBody* bodyA, btRigidBody* bodyB, const D3DXVECTOR3& pivotInADX, const D3DXVECTOR3& pivotInBDX) {
	btVector3 pivotInA = ConvertVectorDxToBt(pivotInADX);
	btVector3 pivotInB = ConvertVectorDxToBt(pivotInBDX);
	dynamicsWorld->addConstraint(new btPoint2PointConstraint(*bodyA, *bodyB, pivotInA, pivotInB));
}

void BulletPhysics::Add6DofSpringConstraint(btRigidBody* bodyA, btRigidBody* bodyB, const D3DXMATRIX& frameInADX, const D3DXMATRIX& frameInBDX,
											const D3DXVECTOR3& c_p1, const D3DXVECTOR3& c_p2, const D3DXVECTOR3& c_r1, const D3DXVECTOR3& c_r2, vector<float> stiffness) {
	btTransform frameInA = ConvertMatrixDxToBt(frameInADX);
	btTransform frameInB = ConvertMatrixDxToBt(frameInBDX);
	btGeneric6DofSpringConstraint* constraint = new btGeneric6DofSpringConstraint(*bodyA, *bodyB, frameInA, frameInB, true);	// 第五引数の効果は謎。どちらでも同じ様に見える……。
	constraint->setLinearLowerLimit(btVector3(c_p1.x, c_p1.y, c_p1.z));	// 型はベクトルだがベクトル量ではないのでZは反転しない。
	constraint->setLinearUpperLimit(btVector3(c_p2.x, c_p2.y, c_p2.z));
	constraint->setAngularLowerLimit(btVector3(c_r1.x, c_r1.y, c_r1.z));
	constraint->setAngularUpperLimit(btVector3(c_r2.x, c_r2.y, c_r2.z));
	for (unsigned int i = 0; i < stiffness.size(); ++i) {
		if (stiffness[i] != 0.0f) {
			constraint->enableSpring(i, true);
			constraint->setStiffness(i, stiffness[i]);
		}
	}
	dynamicsWorld->addConstraint(constraint);	
}

void BulletPhysics::MoveRigidBody(btRigidBody* body, const D3DXMATRIX* world) {
	btTransform trans = ConvertMatrixDxToBt(*world);
	body->getMotionState()->setWorldTransform(trans);
}

void BulletPhysics::StepSimulation() {
	dynamicsWorld->stepSimulation(1.0f/60.0f, 10);
}

D3DXMATRIX BulletPhysics::GetWorld(btRigidBody* body) {
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	return ConvertMatrixBtToDx(trans);
}


/// private : Bullet形式とDirectX形式の変換
btVector3 BulletPhysics::ConvertVectorDxToBt(const D3DXVECTOR3& v) {
	return btVector3(v.x, v.y, -v.z);
}

/// private : Bullet形式とDirectX形式の変換
D3DXVECTOR3 BulletPhysics::ConvertVectorBtToDx(const btVector3& v) {
	return D3DXVECTOR3(v.x(), v.y(), -v.z());
}

/// private : Bullet形式とDirectX形式の変換
btTransform BulletPhysics::ConvertMatrixDxToBt(const D3DXMATRIX& m) {
	btMatrix3x3 basis(	// 鏡像変換＋転置
		m._11, m._21, -m._31, 
		m._12, m._22, -m._32, 
		-m._13, -m._23, m._33 );
	return btTransform(basis, btVector3(m._41, m._42, -m._43));
}

/// private : Bullet形式とDirectX形式の変換
D3DXMATRIX BulletPhysics::ConvertMatrixBtToDx(const btTransform& t) {
	btVector3 R = t.getBasis().getColumn(0);
	btVector3 U = t.getBasis().getColumn(1);
	btVector3 L = t.getBasis().getColumn(2);
	btVector3 P = t.getOrigin();
	D3DXMATRIX m(	// 鏡像変換＋転置
		R.x(), R.y(), -R.z(), 0.f, 
		U.x(), U.y(), -U.z(), 0.f,
		-L.x(), -L.y(), L.z(), 0.f, 
		P.x(), P.y(), -P.z(), 1.f );
	return m;
}