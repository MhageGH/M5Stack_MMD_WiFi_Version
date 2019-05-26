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
		if (pmdRigidBodies[i].shape_type == 0) {		// 球
			float radius = pmdRigidBodies[i].shape_w;
			rigidBodies.push_back(bulletPhysics->CreateSphere(
				radius, &world, pmdRigidBodies[i].rigidbody_weight, pmdRigidBodies[i].rigidbody_recoil, pmdRigidBodies[i].rigidbody_friction, pmdRigidBodies[i].rigidbody_pos_dim,
				pmdRigidBodies[i].rigidbody_rot_dim, pmdRigidBodies[i].rigidbody_type == 0, 1 << pmdRigidBodies[i].rigidbody_group_index, pmdRigidBodies[i].rigidbody_group_target));
			const UINT slices = 10, stacks = 5;
			D3DXCreateSphere(pDevice, radius, slices, stacks, &mesh, 0);
			rigidbody_mesh.push_back(mesh);
		}
		else if (pmdRigidBodies[i].shape_type == 1) {	// 箱
			float width = 2*pmdRigidBodies[i].shape_w, height = 2*pmdRigidBodies[i].shape_h, depth = 2*pmdRigidBodies[i].shape_d;
			rigidBodies.push_back(bulletPhysics->CreateBox(
				width, height, depth, &world, pmdRigidBodies[i].rigidbody_weight, pmdRigidBodies[i].rigidbody_recoil, pmdRigidBodies[i].rigidbody_friction, pmdRigidBodies[i].rigidbody_pos_dim,
				pmdRigidBodies[i].rigidbody_rot_dim, pmdRigidBodies[i].rigidbody_type == 0, 1 << pmdRigidBodies[i].rigidbody_group_index, pmdRigidBodies[i].rigidbody_group_target));
			D3DXCreateBox(pDevice, width, height, depth, &mesh, 0);
			rigidbody_mesh.push_back(mesh);
		}
		else if (pmdRigidBodies[i].shape_type == 2){	// カプセル
			float radius = pmdRigidBodies[i].shape_w, height = pmdRigidBodies[i].shape_h;
			rigidBodies.push_back(bulletPhysics->CreateCapsule(
				radius, height, &world, pmdRigidBodies[i].rigidbody_weight, pmdRigidBodies[i].rigidbody_recoil, pmdRigidBodies[i].rigidbody_friction, pmdRigidBodies[i].rigidbody_pos_dim,
				pmdRigidBodies[i].rigidbody_rot_dim, pmdRigidBodies[i].rigidbody_type == 0, 1 << pmdRigidBodies[i].rigidbody_group_index, pmdRigidBodies[i].rigidbody_group_target));
			const UINT slices = 10, stacks = 5;
			D3DXCreateCylinder(pDevice, radius, radius, height, slices, stacks, &mesh, 0);	// カプセル形状の代わりに円柱で表示
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
		world = rotation*trans;	// ジョイントの行列（モデルローカル座標系）
		btRigidBody* rigidbody_a = rigidBodies[pmdJoints[i].joint_rigidbody_a];
		btRigidBody* rigidbody_b = rigidBodies[pmdJoints[i].joint_rigidbody_b];
		D3DXMATRIX a = bulletPhysics->GetWorld(rigidbody_a);	// 剛体の行列（モデルローカル座標系）
		D3DXMATRIX b = bulletPhysics->GetWorld(rigidbody_b);
		D3DXMatrixInverse(&a, 0, &a);
		D3DXMatrixInverse(&b, 0, &b);
		D3DXMATRIX frameInA = world*a, frameInB = world*b;	// ジョイントの行列（剛体ローカル座標系）
		bulletPhysics->Add6DofSpringConstraint(rigidbody_a, rigidbody_b, frameInA, frameInB, c_p1, c_p2, c_r1, c_r2, stiffness);
		joint_relatedRigidIndex.push_back(pmdJoints[i].joint_rigidbody_a);
		joint_matrix.push_back(frameInA);
	}
	const float length = 0.3f;
	D3DXCreateBox(pDevice, length, length, length, &joint_mesh, 0);
}


void MmdPhysics::BoneUpdate(bool physicsEnabled) {
	for (unsigned int i = 0; i < rigidBodies.size(); ++i) {
		if (rigidbody_relatedBoneIndex[i] != 0xFFFF && rigidbody_type[i] == 0) {	// ボーン追従タイプの剛体にボーン行列を設定
			Bone* bone = &((*bones)[rigidbody_relatedBoneIndex[i]]);
			D3DXMATRIX rigidMat = rigidbody_init[i]*bone->offsetMatML*bone->CalBoneMatML();	// ボーンの移動量を剛体の初期姿勢に適用したものが剛体の現在の姿勢
			bulletPhysics->MoveRigidBody(rigidBodies[i], &rigidMat);
		}
	}
	if (physicsEnabled) bulletPhysics->StepSimulation();	// 物理シミュレーション
	for (unsigned int i = 0; i < rigidBodies.size(); ++i) {
		if (rigidbody_type[i] == 2) {	// ボーン位置あわせタイプの剛体の位置移動量にボーンの位置移動量を設定
			Bone* bone = &((*bones)[rigidbody_relatedBoneIndex[i]]);
			D3DXVECTOR3 v = D3DXVECTOR3(bone->CalBoneMatML().m[3]) - D3DXVECTOR3(bone->initMatML.m[3]);	// ボーンの位置移動量
			D3DXVECTOR3 p = D3DXVECTOR3(rigidbody_init[i].m[3]) + v;	// 剛体の位置
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
			w = r*w;	// PMD, bulletのカプセルはY軸中心、DirectXの円柱プリミティブはZ軸中心なのでX軸を中心に90°回転させる
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

// private : 剛体のワールド変換行列を作成
// @param pos		剛体の位置		：MmdStruct::PmdRigidBody.pos_pos[3]
// @param rot		剛体の回転		：MmdStruct::PmdRigidBody.pos_rot[3]
// @param i			関連ボーン番号
// @return			ワールド変換行列
D3DXMATRIX MmdPhysics::CreateRigidMatrix(float* pos, float* rot, int i) {
	D3DXVECTOR3 p = D3DXVECTOR3(pos) ;
	if (i != 0xFFFF) {
		p += D3DXVECTOR3((*bones)[i].initMatML.m[3]); // 関連ボーンがある場合は、ボーン相対座標からモデルローカル座標に変換。MmdStruct::PmdRigidBody.pos_posを参照
		(*bones)[i].extraBoneControl = true;
	}
	D3DXMATRIX trans, rotation;
	D3DXMatrixTranslation(&trans, p.x, p.y, p.z);
	D3DXVECTOR3 r(rot);
	D3DXMatrixRotationYawPitchRoll(&rotation, r.y, r.x, r.z);
	return rotation*trans;
}

// private: ワイヤーフレームの描画を開始する
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

// private: ワイヤーフレームの描画を終了する
void MmdPhysics::EndWireframe() {
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}