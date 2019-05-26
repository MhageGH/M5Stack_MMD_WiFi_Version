#pragma once
#include "BulletPhysics.h"
#include "SkinMesh.h"

class MmdPhysics {
private:
	LPDIRECT3DDEVICE9 pDevice;
	vector<Bone>* bones;					// 全ボーン配列へのポインタ
	BulletPhysics* bulletPhysics;
	vector<btRigidBody*> rigidBodies;		// 1メッシュに対する剛体配列
	vector<int> rigidbody_relatedBoneIndex;	// 各剛体に関連するボーンのインデックス
	vector<int> rigidbody_type;				// 各剛体のタイプ
	vector<D3DXMATRIX> rigidbody_init;		// 各剛体の初期姿勢行列
	vector<D3DXMATRIX> rigidbody_offset;	// 各剛体のオフセット行列
	vector<ID3DXMesh*> rigidbody_mesh;		// 各剛体のメッシュ
	ID3DXMesh* joint_mesh;					// ジョイントのメッシュ
	vector<int> joint_relatedRigidIndex;	// 各ジョイントの関連剛体
	vector<D3DXMATRIX> joint_matrix;		// 各ジョイントの姿勢行列(剛体Aローカル座標系)

public:
	MmdPhysics(LPDIRECT3DDEVICE9 pDevice, BulletPhysics* bulletPhysics);
	~MmdPhysics();

	/// 全ボーン配列へのポインタをセット
	/// @param bones 全ボーン配列へのポインタ
	void SetBones(vector<Bone>* bones);

	/// 剛体を作成
	/// @param pmdRigidBodies Pmd剛体配列
	/// @param pmdBones Pmdボーン配列
	void CreateRigid(vector<MmdStruct::PmdRigidBody> pmdRigidBodies, vector<MmdStruct::PmdBone> pmdBones);

	/// ジョイントを作成
	/// @param pmdBones Pmdジョイント配列
	void CreateJoint(vector<MmdStruct::PmdJoint> pmdJoints);

	/// ボーン行列を更新
	/// @param 物理演算使用可能
	void BoneUpdate(bool physicsEnabled);

	/// 剛体メッシュを描画する
	void DrawRigidMesh(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera);

	/// ジョイントメッシュを描画する
	void DrawJointMesh(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera);


	// 補助関数
private:
	D3DXMATRIX CreateRigidMatrix(float* pos, float* rot, int i);
	void StartWireframe(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera, D3DCOLORVALUE color);
	void EndWireframe();

};
