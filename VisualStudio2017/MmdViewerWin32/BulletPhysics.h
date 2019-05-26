#pragma once

// Bulletライブラリはマネージオプションでコンパイルできない！
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

/// 物理演算Bulletのラッパークラス
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
	
	/// 剛体オブジェクト生成
	/// 質量0, kinematicをfalseにすると、動かないstatic剛体になる。
	/// 質量0, kinematicをtrueにすると、手動で動かせるが、物理演算の影響を受けないKinematic剛体になる
	btRigidBody* CreateBox(float width, float height, float depth, const D3DXMATRIX* world, 
		float mass = 0, float restitution = 0, float friction = 0.5f, float linear_damp = 0, float angular_damp = 0, bool kinematic = 0, unsigned short group = 1, unsigned short mask = 0xFFFF);
	btRigidBody* CreateSphere(float radius, const D3DXMATRIX* world, 
		float mass = 0, float restitution = 0, float friction = 0.5f, float linear_damp = 0, float angular_damp = 0, bool kinematic = 0, unsigned short group = 1, unsigned short mask = 0xFFFF);
	btRigidBody* CreateCylinder(float radius, float length, const D3DXMATRIX* world,	// 中心軸はZ軸
		float mass = 0, float restitution = 0, float friction = 0.5f, float linear_damp = 0, float angular_damp = 0, bool kinematic = 0, unsigned short group = 1, unsigned short mask = 0xFFFF);
	btRigidBody* CreateCapsule(float radius, float height, const D3DXMATRIX* world,	// 中心軸はZ軸. heightは球の中心間の距離
		float mass = 0, float restitution = 0, float friction = 0.5f, float linear_damp = 0, float angular_damp = 0, bool kinematic = 0, unsigned short group = 1, unsigned short mask = 0xFFFF);

	// 拘束条件追加
	void AddPointToPointConstraint(btRigidBody* body, const D3DXVECTOR3& pivot);
	void AddPointToPointConstraint(btRigidBody* bodyA, btRigidBody* bodyB, const D3DXVECTOR3& pivotInA, const D3DXVECTOR3& pivotInB);

	/// 6軸ジョイントを追加
	/// @param bodyA 剛体A
	/// @param bodyB 剛体B
	/// @param frameInA ジョイントのワールド変換行列(剛体Aローカル座標系)
	/// @param frameInB ジョイントのワールド変換行列(剛体Bローカル座標系)
	/// @param c_p1 移動制限1
	/// @param c_p2 移動制限2
	/// @param c_r1 回転制限1
	/// @param c_r2 回転制限2
	/// @param stiffness バネ剛性(平行移動x, y, z, 回転移動x, y, zの順の6要素)
	void Add6DofSpringConstraint(btRigidBody* bodyA, btRigidBody* bodyB, const D3DXMATRIX& frameInA, const D3DXMATRIX& frameInB,
								const D3DXVECTOR3& c_p1, const D3DXVECTOR3& c_p2, const D3DXVECTOR3& c_r1, const D3DXVECTOR3& c_r2, vector<float> stiffness);

	// 剛体を移動
	void MoveRigidBody(btRigidBody* body, const D3DXMATRIX* world);
	
	// 物理演算の世界の時間を1/60秒進める
	void StepSimulation();
	
	// 結果のワールド行列を取得
	D3DXMATRIX GetWorld(btRigidBody* body);
};
