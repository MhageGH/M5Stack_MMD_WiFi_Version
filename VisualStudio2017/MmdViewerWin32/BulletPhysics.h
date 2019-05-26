#pragma once

// Bullet���C�u�����̓}�l�[�W�I�v�V�����ŃR���p�C���ł��Ȃ��I
#pragma unmanaged
#include "btBulletDynamicsCommon.h"
#pragma managed

#ifdef _DEBUG
#pragma comment(lib,"../BulletLib/BulletCollision_vs2010_debug.lib")
#pragma comment(lib,"../BulletLib/BulletDynamics_vs2010_debug.lib")
#pragma comment(lib,"../BulletLib/BulletSoftBody_vs2010_debug.lib")
#pragma comment(lib,"../BulletLib/LinearMath_vs2010_debug.lib")
#else
#pragma comment(lib,"../BulletLib/BulletCollision_vs2010.lib")
#pragma comment(lib,"../BulletLib/BulletDynamics_vs2010.lib")
#pragma comment(lib,"../BulletLib/BulletSoftBody_vs2010.lib")
#pragma comment(lib,"../BulletLib/LinearMath_vs2010.lib")
#endif

/// �������ZBullet�̃��b�p�[�N���X
class BulletPhysics sealed {
private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	btRigidBody* CreateShape(btCollisionShape* shape, const D3DXMATRIX* world,
							float mass, float restitution, float friction, float linear_damp, float angular_damp, bool kinematic, unsigned short group, unsigned short mask);
	btVector3 ConvertVectorDxToBt(const D3DXVECTOR3&);
	D3DXVECTOR3 ConvertVectorBtToDx(const btVector3&);
	btTransform ConvertMatrixDxToBt(const D3DXMATRIX&);
	D3DXMATRIX ConvertMatrixBtToDx(const btTransform&);
public:
	BulletPhysics(const D3DXVECTOR3& gravity);
	~BulletPhysics();	
	
	/// ���̃I�u�W�F�N�g����
	/// ����0, kinematic��false�ɂ���ƁA�����Ȃ�static���̂ɂȂ�B
	/// ����0, kinematic��true�ɂ���ƁA�蓮�œ������邪�A�������Z�̉e�����󂯂Ȃ�Kinematic���̂ɂȂ�
	btRigidBody* CreateBox(float width, float height, float depth, const D3DXMATRIX* world, 
		float mass = 0, float restitution = 0, float friction = 0.5f, float linear_damp = 0, float angular_damp = 0, bool kinematic = 0, unsigned short group = 1, unsigned short mask = 0xFFFF);
	btRigidBody* CreateSphere(float radius, const D3DXMATRIX* world, 
		float mass = 0, float restitution = 0, float friction = 0.5f, float linear_damp = 0, float angular_damp = 0, bool kinematic = 0, unsigned short group = 1, unsigned short mask = 0xFFFF);
	btRigidBody* CreateCylinder(float radius, float length, const D3DXMATRIX* world,	// ���S����Z��
		float mass = 0, float restitution = 0, float friction = 0.5f, float linear_damp = 0, float angular_damp = 0, bool kinematic = 0, unsigned short group = 1, unsigned short mask = 0xFFFF);
	btRigidBody* CreateCapsule(float radius, float height, const D3DXMATRIX* world,	// ���S����Z��. height�͋��̒��S�Ԃ̋���
		float mass = 0, float restitution = 0, float friction = 0.5f, float linear_damp = 0, float angular_damp = 0, bool kinematic = 0, unsigned short group = 1, unsigned short mask = 0xFFFF);

	// �S�������ǉ�
	void AddPointToPointConstraint(btRigidBody* body, const D3DXVECTOR3& pivot);
	void AddPointToPointConstraint(btRigidBody* bodyA, btRigidBody* bodyB, const D3DXVECTOR3& pivotInA, const D3DXVECTOR3& pivotInB);

	/// 6���W���C���g��ǉ�
	/// @param bodyA ����A
	/// @param bodyB ����B
	/// @param frameInA �W���C���g�̃��[���h�ϊ��s��(����A���[�J�����W�n)
	/// @param frameInB �W���C���g�̃��[���h�ϊ��s��(����B���[�J�����W�n)
	/// @param c_p1 �ړ�����1
	/// @param c_p2 �ړ�����2
	/// @param c_r1 ��]����1
	/// @param c_r2 ��]����2
	/// @param stiffness �o�l����(���s�ړ�x, y, z, ��]�ړ�x, y, z�̏���6�v�f)
	void Add6DofSpringConstraint(btRigidBody* bodyA, btRigidBody* bodyB, const D3DXMATRIX& frameInA, const D3DXMATRIX& frameInB,
								const D3DXVECTOR3& c_p1, const D3DXVECTOR3& c_p2, const D3DXVECTOR3& c_r1, const D3DXVECTOR3& c_r2, vector<float> stiffness);

	// ���̂��ړ�
	void MoveRigidBody(btRigidBody* body, const D3DXMATRIX* world);
	
	// �������Z�̐��E�̎��Ԃ�1/60�b�i�߂�
	void StepSimulation();
	
	// ���ʂ̃��[���h�s����擾
	D3DXMATRIX GetWorld(btRigidBody* body);
};
