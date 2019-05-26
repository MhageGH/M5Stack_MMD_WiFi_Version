#pragma once
#include "BulletPhysics.h"
#include "SkinMesh.h"

class MmdPhysics {
private:
	LPDIRECT3DDEVICE9 pDevice;
	vector<Bone>* bones;					// �S�{�[���z��ւ̃|�C���^
	BulletPhysics* bulletPhysics;
	vector<btRigidBody*> rigidBodies;		// 1���b�V���ɑ΂��鍄�̔z��
	vector<int> rigidbody_relatedBoneIndex;	// �e���̂Ɋ֘A����{�[���̃C���f�b�N�X
	vector<int> rigidbody_type;				// �e���̂̃^�C�v
	vector<D3DXMATRIX> rigidbody_init;		// �e���̂̏����p���s��
	vector<D3DXMATRIX> rigidbody_offset;	// �e���̂̃I�t�Z�b�g�s��
	vector<ID3DXMesh*> rigidbody_mesh;		// �e���̂̃��b�V��
	ID3DXMesh* joint_mesh;					// �W���C���g�̃��b�V��
	vector<int> joint_relatedRigidIndex;	// �e�W���C���g�̊֘A����
	vector<D3DXMATRIX> joint_matrix;		// �e�W���C���g�̎p���s��(����A���[�J�����W�n)

public:
	MmdPhysics(LPDIRECT3DDEVICE9 pDevice, BulletPhysics* bulletPhysics);
	~MmdPhysics();

	/// �S�{�[���z��ւ̃|�C���^���Z�b�g
	/// @param bones �S�{�[���z��ւ̃|�C���^
	void SetBones(vector<Bone>* bones);

	/// ���̂��쐬
	/// @param pmdRigidBodies Pmd���̔z��
	/// @param pmdBones Pmd�{�[���z��
	void CreateRigid(vector<MmdStruct::PmdRigidBody> pmdRigidBodies, vector<MmdStruct::PmdBone> pmdBones);

	/// �W���C���g���쐬
	/// @param pmdBones Pmd�W���C���g�z��
	void CreateJoint(vector<MmdStruct::PmdJoint> pmdJoints);

	/// �{�[���s����X�V
	/// @param �������Z�g�p�\
	void BoneUpdate(bool physicsEnabled);

	/// ���̃��b�V����`�悷��
	void DrawRigidMesh(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera);

	/// �W���C���g���b�V����`�悷��
	void DrawJointMesh(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera);


	// �⏕�֐�
private:
	D3DXMATRIX CreateRigidMatrix(float* pos, float* rot, int i);
	void StartWireframe(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera, D3DCOLORVALUE color);
	void EndWireframe();

};
