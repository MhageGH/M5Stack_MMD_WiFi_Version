#include "StdAfx.h"
#include "PmdFileLoader.h"

PmdFileLoader::PmdFileLoader(LPCTSTR filename) {
	// �t�@�C���I�[�v��
    ifstream ifs(filename, ios::binary);
	if (ifs.fail()) throw TEXT("�t�@�C��������܂���");
	// �w�b�_
	MmdStruct::PmdHeader pmdHeader;
	ifs.read((char*)&pmdHeader, sizeof(pmdHeader));
	// ���_�f�[�^
	unsigned long numPmdVertex;
	ifs.read((char*)&numPmdVertex, sizeof(numPmdVertex));
	pmdVertices.resize(numPmdVertex);
	ifs.read((char*)&pmdVertices[0], sizeof(MmdStruct::PmdVertex)*numPmdVertex);
	// �|���S���f�[�^
	unsigned long numPmdFace;
	ifs.read((char*)&numPmdFace, sizeof(numPmdFace));
	pmdFaces.resize(numPmdFace);
	ifs.read((char*)&pmdFaces[0], sizeof(unsigned short)*numPmdFace);
	// �ޗ��f�[�^
	unsigned long numPmdMaterial;
	ifs.read((char*)&numPmdMaterial, sizeof(numPmdMaterial));
	pmdMaterials.resize(numPmdMaterial);
	ifs.read((char*)&pmdMaterials[0], sizeof(MmdStruct::PmdMaterial)*numPmdMaterial);
	// �{�[���f�[�^
	unsigned short numPmdBone;
	ifs.read((char*)&numPmdBone, sizeof(numPmdBone));
	pmdBones.resize(numPmdBone);
	ifs.read((char*)&pmdBones[0], sizeof(MmdStruct::PmdBone)*numPmdBone);
	// IK�f�[�^
	unsigned short numPmdIkData;
	ifs.read((char*)&numPmdIkData, sizeof(numPmdIkData));
	pmdIkData.resize(numPmdIkData);
	for (unsigned short i = 0; i < numPmdIkData; ++i) {
		MmdStruct::PmdIkDataWithoutArray pmdIkDataWithoutArray;
		ifs.read((char*)&pmdIkDataWithoutArray, sizeof(MmdStruct::PmdIkDataWithoutArray));
		vector<unsigned short> ik_child_bone_index(pmdIkDataWithoutArray.ik_chain_length);
		ifs.read((char*)&ik_child_bone_index[0], sizeof(unsigned short)*pmdIkDataWithoutArray.ik_chain_length);
		pmdIkData[i].ik_bone_index = pmdIkDataWithoutArray.ik_bone_index;
		pmdIkData[i].ik_target_bone_index = pmdIkDataWithoutArray.ik_target_bone_index;
		pmdIkData[i].ik_chain_length = pmdIkDataWithoutArray.ik_chain_length;
		pmdIkData[i].iterations = pmdIkDataWithoutArray.iterations;
		pmdIkData[i].control_weight = pmdIkDataWithoutArray.control_weight;
		pmdIkData[i].ik_child_bone_index = ik_child_bone_index;
	}
	// �\��f�[�^
	unsigned short numPmdSkin;
	ifs.read((char*)&numPmdSkin, sizeof(numPmdSkin));
	for (unsigned short i = 0; i < numPmdSkin; ++i) {
		MmdStruct::PmdSkinDataWithoutArray pmdSkinDataWithoutArray;
		ifs.read((char*)&pmdSkinDataWithoutArray, sizeof(pmdSkinDataWithoutArray));
		ifs.seekg(sizeof(MmdStruct::PmdSkinVertexData)*pmdSkinDataWithoutArray.skin_vert_count, std::ios::cur);
	}
	// �\��p�g�\�����X�g
	unsigned char numSkinDisp;
	ifs.read((char*)&numSkinDisp, sizeof(numSkinDisp));
	ifs.seekg(sizeof(unsigned short)*numSkinDisp, std::ios::cur);
	// �{�[���g�p�g�����X�g
	unsigned char numBoneDispName;
	ifs.read((char*)&numBoneDispName, sizeof(numBoneDispName));
	ifs.seekg(sizeof(char)*50*numBoneDispName, std::ios::cur);
	// �{�[���g�p�\�����X�g
	unsigned long numBoneDisp;
	ifs.read((char*)&numBoneDisp, sizeof(numBoneDisp));
	ifs.seekg(sizeof(MmdStruct::PmdBoneDisp)*numBoneDisp, std::ios::cur);
	// �p���Ή�
	unsigned char english_name_compatibility;
	ifs.read((char*)&english_name_compatibility, sizeof(english_name_compatibility));
	// �e��p��
	if (english_name_compatibility == 1) {
		ifs.seekg(20, std::ios::cur);	// ���f����
		ifs.seekg(256, std::ios::cur);	// �R�����g
		ifs.seekg(20*numPmdBone, std::ios::cur);						// �{�[�����X�g
		if(numPmdSkin) ifs.seekg(20*(numPmdSkin - 1), std::ios::cur);	// �\��X�g
		ifs.seekg(50*numBoneDispName, std::ios::cur);					// �{�[���g�p�g�����X�g
	}
	// �g�D�[���e�N�X�`�����X�g
	ifs.seekg(1000, std::ios::cur);
	// ���̃f�[�^
	unsigned long numPmdRigidBody;
	ifs.read((char*)&numPmdRigidBody, sizeof(numPmdRigidBody));
	pmdRigidBodies.resize(numPmdRigidBody);
	if (numPmdRigidBody) ifs.read((char*)&pmdRigidBodies[0], sizeof(MmdStruct::PmdRigidBody)*numPmdRigidBody);
	// �W���C���g�f�[�^
	unsigned long numPmdJoint;
	ifs.read((char*)&numPmdJoint, sizeof(numPmdJoint));
	pmdJoints.resize(numPmdJoint);
	if (numPmdJoint) ifs.read((char*)&pmdJoints[0], sizeof(MmdStruct::PmdJoint)*numPmdJoint);
}