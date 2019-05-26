#include "StdAfx.h"
#include "MmdPhysics.h"

MmdPhysics::MmdPhysics(LPDIRECT3DDEVICE9 pDevice, BulletPhysics* bulletPhysics) {
	this->pDevice = pDevice;
	this->bulletPhysics = bulletPhysics;
	this->joint_mesh = 0;
}

MmdPhysics::~MmdPhysics() {
	for (unsigned int i = 0; i < rigidbody_mesh.size(); ++i) SAFE_RELEASE(rigidbody_mesh[i]);
	SAFE_RELEASE(joint_mesh);
}

void MmdPhysics::SetBones(vector<Bone>* bones) {
	this->bones = bones;
}

void MmdPhysics::CreateRigid(vector<MmdStruct::PmdRigidBody> pmdRigidBodies, vector<MmdStruct::PmdBone> pmdBones) {
	for (unsigned int i = 0; i < pmdRigidBodies.size(); ++i) {
		rigidbody_relatedBoneIndex.push_back(pmdRigidBodies[i].rigidbody_rel_bone_index);
		rigidbody_type.push_back(pmdRigidBodies[i].rigidbody_type);
		D3DXMATRIX world, world_inv;
		world = CreateRigidMatrix(pmdRigidBodies[i].pos_pos, pmdRigidBodies[i].pos_rot, pmdRigidBodies[i].rigidbody_rel_bone_index);
		D3DXMatrixInverse(&world_inv, 0, &world);
		rigidbody_init.push_back(world);
		rigidbody_offset.push_back(world_inv);
		ID3DXMesh* mesh;
		if (pmdRigidBodies[i].shape_type == 0) {		// ��
			float radius = pmdRigidBodies[i].shape_w;
			rigidBodies.push_back(bulletPhysics->CreateSphere(
				radius, &world, pmdRigidBodies[i].rigidbody_weight, pmdRigidBodies[i].rigidbody_recoil, pmdRigidBodies[i].rigidbody_friction, pmdRigidBodies[i].rigidbody_pos_dim,
				pmdRigidBodies[i].rigidbody_rot_dim, pmdRigidBodies[i].rigidbody_type == 0, 1 << pmdRigidBodies[i].rigidbody_group_index, pmdRigidBodies[i].rigidbody_group_target));
			const UINT slices = 10, stacks = 5;
			D3DXCreateSphere(pDevice, radius, slices, stacks, &mesh, 0);
			rigidbody_mesh.push_back(mesh);
		}
		else if (pmdRigidBodies[i].shape_type == 1) {	// ��
			float width = 2*pmdRigidBodies[i].shape_w, height = 2*pmdRigidBodies[i].shape_h, depth = 2*pmdRigidBodies[i].shape_d;
			rigidBodies.push_back(bulletPhysics->CreateBox(
				width, height, depth, &world, pmdRigidBodies[i].rigidbody_weight, pmdRigidBodies[i].rigidbody_recoil, pmdRigidBodies[i].rigidbody_friction, pmdRigidBodies[i].rigidbody_pos_dim,
				pmdRigidBodies[i].rigidbody_rot_dim, pmdRigidBodies[i].rigidbody_type == 0, 1 << pmdRigidBodies[i].rigidbody_group_index, pmdRigidBodies[i].rigidbody_group_target));
			D3DXCreateBox(pDevice, width, height, depth, &mesh, 0);
			rigidbody_mesh.push_back(mesh);
		}
		else if (pmdRigidBodies[i].shape_type == 2){	// �J�v�Z��
			float radius = pmdRigidBodies[i].shape_w, height = pmdRigidBodies[i].shape_h;
			rigidBodies.push_back(bulletPhysics->CreateCapsule(
				radius, height, &world, pmdRigidBodies[i].rigidbody_weight, pmdRigidBodies[i].rigidbody_recoil, pmdRigidBodies[i].rigidbody_friction, pmdRigidBodies[i].rigidbody_pos_dim,
				pmdRigidBodies[i].rigidbody_rot_dim, pmdRigidBodies[i].rigidbody_type == 0, 1 << pmdRigidBodies[i].rigidbody_group_index, pmdRigidBodies[i].rigidbody_group_target));
			const UINT slices = 10, stacks = 5;
			D3DXCreateCylinder(pDevice, radius, radius, height, slices, stacks, &mesh, 0);	// �J�v�Z���`��̑���ɉ~���ŕ\��
			rigidbody_mesh.push_back(mesh);
		}
	}
}

void MmdPhysics::CreateJoint(vector<MmdStruct::PmdJoint> pmdJoints) {
	for (unsigned int i = 0; i < pmdJoints.size(); ++i) {
		D3DXVECTOR3 c_p1(pmdJoints[i].constrain_pos_1);
		D3DXVECTOR3 c_p2(pmdJoints[i].constrain_pos_2);
		D3DXVECTOR3 c_r1(pmdJoints[i].constrain_rot_1);
		D3DXVECTOR3 c_r2(pmdJoints[i].constrain_rot_2);
		D3DXVECTOR3 s_p(pmdJoints[i].spring_pos);
		D3DXVECTOR3 s_r(pmdJoints[i].spring_rot);
		vector<float> stiffness;
		for (int j =  0; j < 3; ++j) stiffness.push_back(pmdJoints[i].spring_pos[j]);
		for (int j =  0; j < 3; ++j) stiffness.push_back(pmdJoints[i].spring_rot[j]);
		D3DXVECTOR3 p(pmdJoints[i].joint_pos);
		D3DXVECTOR3 r(pmdJoints[i].joint_rot);
		D3DXMATRIX trans, rotation, world, rot_x, rot_y, rot_z;
		D3DXMatrixTranslation(&trans, p.x, p.y, p.z);
		D3DXMatrixRotationYawPitchRoll(&rotation, r.y, r.x, r.z);
		world = rotation*trans;	// �W���C���g�̍s��i���f�����[�J�����W�n�j
		btRigidBody* rigidbody_a = rigidBodies[pmdJoints[i].joint_rigidbody_a];
		btRigidBody* rigidbody_b = rigidBodies[pmdJoints[i].joint_rigidbody_b];
		D3DXMATRIX a = bulletPhysics->GetWorld(rigidbody_a);	// ���̂̍s��i���f�����[�J�����W�n�j
		D3DXMATRIX b = bulletPhysics->GetWorld(rigidbody_b);
		D3DXMatrixInverse(&a, 0, &a);
		D3DXMatrixInverse(&b, 0, &b);
		D3DXMATRIX frameInA = world*a, frameInB = world*b;	// �W���C���g�̍s��i���̃��[�J�����W�n�j
		bulletPhysics->Add6DofSpringConstraint(rigidbody_a, rigidbody_b, frameInA, frameInB, c_p1, c_p2, c_r1, c_r2, stiffness);
		joint_relatedRigidIndex.push_back(pmdJoints[i].joint_rigidbody_a);
		joint_matrix.push_back(frameInA);
	}
	const float length = 0.3f;
	D3DXCreateBox(pDevice, length, length, length, &joint_mesh, 0);
}


void MmdPhysics::BoneUpdate(bool physicsEnabled) {
	for (unsigned int i = 0; i < rigidBodies.size(); ++i) {
		if (rigidbody_relatedBoneIndex[i] != 0xFFFF && rigidbody_type[i] == 0) {	// �{�[���Ǐ]�^�C�v�̍��̂Ƀ{�[���s���ݒ�
			Bone* bone = &((*bones)[rigidbody_relatedBoneIndex[i]]);
			D3DXMATRIX rigidMat = rigidbody_init[i]*bone->offsetMatML*bone->CalBoneMatML();	// �{�[���̈ړ��ʂ����̂̏����p���ɓK�p�������̂����̂̌��݂̎p��
			bulletPhysics->MoveRigidBody(rigidBodies[i], &rigidMat);
		}
	}
	if (physicsEnabled) bulletPhysics->StepSimulation();	// �����V�~�����[�V����
	for (unsigned int i = 0; i < rigidBodies.size(); ++i) {
		if (rigidbody_type[i] == 2) {	// �{�[���ʒu���킹�^�C�v�̍��̂̈ʒu�ړ��ʂɃ{�[���̈ʒu�ړ��ʂ�ݒ�
			Bone* bone = &((*bones)[rigidbody_relatedBoneIndex[i]]);
			D3DXVECTOR3 v = D3DXVECTOR3(bone->CalBoneMatML().m[3]) - D3DXVECTOR3(bone->initMatML.m[3]);	// �{�[���̈ʒu�ړ���
			D3DXVECTOR3 p = D3DXVECTOR3(rigidbody_init[i].m[3]) + v;	// ���̂̈ʒu
			D3DXMATRIX m = bulletPhysics->GetWorld(rigidBodies[i]);
			m._41 = p.x; m._42 = p.y; m._43 = p.z;
			bulletPhysics->MoveRigidBody(rigidBodies[i], &m);
		}
	}
	for (unsigned int i = 0; i < rigidBodies.size(); ++i) {
		if (rigidbody_relatedBoneIndex[i] != 0xFFFF) {
			Bone* bone = &((*bones)[rigidbody_relatedBoneIndex[i]]);
			bone->boneMatML = bone->initMatML*rigidbody_offset[i]*bulletPhysics->GetWorld(rigidBodies[i]);
		}
	}
}

void MmdPhysics::DrawRigidMesh(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera) {
	D3DCOLORVALUE color = {0.96f, 0.714f, 0, 1.0f};
	StartWireframe(world, light, camera, color);
	for (unsigned int i = 0; i < rigidBodies.size(); ++i) {
		D3DXMATRIX w = bulletPhysics->GetWorld(rigidBodies[i])*(*world);
		if (rigidBodies[i]->getCollisionShape()->getShapeType() == CAPSULE_SHAPE_PROXYTYPE) {
			D3DXMATRIX r;
			D3DXMatrixRotationX(&r, D3DX_PI/2);
			w = r*w;	// PMD, bullet�̃J�v�Z����Y�����S�ADirectX�̉~���v���~�e�B�u��Z�����S�Ȃ̂�X���𒆐S��90����]������
		}
		pDevice->SetTransform(D3DTS_WORLD, &w);
		rigidbody_mesh[i]->DrawSubset(0);
	}
	EndWireframe();
}

void MmdPhysics::DrawJointMesh(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera) {
	D3DCOLORVALUE color = {0.91f, 0.91f, 0, 1.0f};
	StartWireframe(world, light, camera, color);
	for (unsigned int i = 0; i < joint_relatedRigidIndex.size(); ++i) {
		D3DXMATRIX w = joint_matrix[i]*bulletPhysics->GetWorld(rigidBodies[joint_relatedRigidIndex[i]])*(*world);
		pDevice->SetTransform(D3DTS_WORLD, &w);
		joint_mesh->DrawSubset(0);
	}
	EndWireframe();
}

// private : ���̂̃��[���h�ϊ��s����쐬
// @param pos		���̂̈ʒu		�FMmdStruct::PmdRigidBody.pos_pos[3]
// @param rot		���̂̉�]		�FMmdStruct::PmdRigidBody.pos_rot[3]
// @param i			�֘A�{�[���ԍ�
// @return			���[���h�ϊ��s��
D3DXMATRIX MmdPhysics::CreateRigidMatrix(float* pos, float* rot, int i) {
	D3DXVECTOR3 p = D3DXVECTOR3(pos) ;
	if (i != 0xFFFF) {
		p += D3DXVECTOR3((*bones)[i].initMatML.m[3]); // �֘A�{�[��������ꍇ�́A�{�[�����΍��W���烂�f�����[�J�����W�ɕϊ��BMmdStruct::PmdRigidBody.pos_pos���Q��
		(*bones)[i].extraBoneControl = true;
	}
	D3DXMATRIX trans, rotation;
	D3DXMatrixTranslation(&trans, p.x, p.y, p.z);
	D3DXVECTOR3 r(rot);
	D3DXMatrixRotationYawPitchRoll(&rotation, r.y, r.x, r.z);
	return rotation*trans;
}

// private: ���C���[�t���[���̕`����J�n����
void MmdPhysics::StartWireframe(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera, D3DCOLORVALUE color) {
	D3DMATERIAL9 material = {{0, 0, 0, 1.0f}, color};
	D3DXMATRIX view, projection;
	camera->GetMatrix(&view, &projection);
	pDevice->SetLight(0, light);
	pDevice->LightEnable(0, TRUE) ;
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetMaterial(&material);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetTransform(D3DTS_VIEW, &view);
	pDevice->SetTransform(D3DTS_PROJECTION, &projection);
}

// private: ���C���[�t���[���̕`����I������
void MmdPhysics::EndWireframe() {
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}