#include "StdAfx.h"
#include "PmdFileLoader.h"

PmdFileLoader::PmdFileLoader(LPCTSTR filename) {
	// ファイルオープン
    ifstream ifs(filename, ios::binary);
	if (ifs.fail()) throw TEXT("ファイルがありません");
	// ヘッダ
	MmdStruct::PmdHeader pmdHeader;
	ifs.read((char*)&pmdHeader, sizeof(pmdHeader));
	// 頂点データ
	unsigned long numPmdVertex;
	ifs.read((char*)&numPmdVertex, sizeof(numPmdVertex));
	pmdVertices.resize(numPmdVertex);
	ifs.read((char*)&pmdVertices[0], sizeof(MmdStruct::PmdVertex)*numPmdVertex);
	// ポリゴンデータ
	unsigned long numPmdFace;
	ifs.read((char*)&numPmdFace, sizeof(numPmdFace));
	pmdFaces.resize(numPmdFace);
	ifs.read((char*)&pmdFaces[0], sizeof(unsigned short)*numPmdFace);
	// 材料データ
	unsigned long numPmdMaterial;
	ifs.read((char*)&numPmdMaterial, sizeof(numPmdMaterial));
	pmdMaterials.resize(numPmdMaterial);
	ifs.read((char*)&pmdMaterials[0], sizeof(MmdStruct::PmdMaterial)*numPmdMaterial);
	// ボーンデータ
	unsigned short numPmdBone;
	ifs.read((char*)&numPmdBone, sizeof(numPmdBone));
	pmdBones.resize(numPmdBone);
	ifs.read((char*)&pmdBones[0], sizeof(MmdStruct::PmdBone)*numPmdBone);
	// IKデータ
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
	// 表情データ
	unsigned short numPmdSkin;
	ifs.read((char*)&numPmdSkin, sizeof(numPmdSkin));
	for (unsigned short i = 0; i < numPmdSkin; ++i) {
		MmdStruct::PmdSkinDataWithoutArray pmdSkinDataWithoutArray;
		ifs.read((char*)&pmdSkinDataWithoutArray, sizeof(pmdSkinDataWithoutArray));
		ifs.seekg(sizeof(MmdStruct::PmdSkinVertexData)*pmdSkinDataWithoutArray.skin_vert_count, std::ios::cur);
	}
	// 表情用枠表示リスト
	unsigned char numSkinDisp;
	ifs.read((char*)&numSkinDisp, sizeof(numSkinDisp));
	ifs.seekg(sizeof(unsigned short)*numSkinDisp, std::ios::cur);
	// ボーン枠用枠名リスト
	unsigned char numBoneDispName;
	ifs.read((char*)&numBoneDispName, sizeof(numBoneDispName));
	ifs.seekg(sizeof(char)*50*numBoneDispName, std::ios::cur);
	// ボーン枠用表示リスト
	unsigned long numBoneDisp;
	ifs.read((char*)&numBoneDisp, sizeof(numBoneDisp));
	ifs.seekg(sizeof(MmdStruct::PmdBoneDisp)*numBoneDisp, std::ios::cur);
	// 英名対応
	unsigned char english_name_compatibility;
	ifs.read((char*)&english_name_compatibility, sizeof(english_name_compatibility));
	// 各種英名
	if (english_name_compatibility == 1) {
		ifs.seekg(20, std::ios::cur);	// モデル名
		ifs.seekg(256, std::ios::cur);	// コメント
		ifs.seekg(20*numPmdBone, std::ios::cur);						// ボーンリスト
		if(numPmdSkin) ifs.seekg(20*(numPmdSkin - 1), std::ios::cur);	// 表情リスト
		ifs.seekg(50*numBoneDispName, std::ios::cur);					// ボーン枠用枠名リスト
	}
	// トゥーンテクスチャリスト
	ifs.seekg(1000, std::ios::cur);
	// 剛体データ
	unsigned long numPmdRigidBody;
	ifs.read((char*)&numPmdRigidBody, sizeof(numPmdRigidBody));
	pmdRigidBodies.resize(numPmdRigidBody);
	if (numPmdRigidBody) ifs.read((char*)&pmdRigidBodies[0], sizeof(MmdStruct::PmdRigidBody)*numPmdRigidBody);
	// ジョイントデータ
	unsigned long numPmdJoint;
	ifs.read((char*)&numPmdJoint, sizeof(numPmdJoint));
	pmdJoints.resize(numPmdJoint);
	if (numPmdJoint) ifs.read((char*)&pmdJoints[0], sizeof(MmdStruct::PmdJoint)*numPmdJoint);
}