#pragma once
#include "MmdStruct.h"

/// PMDファイルからデータを抽出するクラス
class PmdFileLoader sealed {
private:
	vector<MmdStruct::PmdVertex>	pmdVertices;
	vector<unsigned short>			pmdFaces;
	vector<MmdStruct::PmdMaterial>	pmdMaterials;
	vector<MmdStruct::PmdBone>		pmdBones;
	vector<MmdStruct::PmdIkData>	pmdIkData;
	vector<MmdStruct::PmdRigidBody> pmdRigidBodies;
	vector<MmdStruct::PmdJoint>		pmdJoints;

public:
	PmdFileLoader(LPCTSTR filename);
	vector<MmdStruct::PmdVertex>	GetPmdVertices()	{ return pmdVertices; }
	vector<unsigned short>			GetPmdFaces()		{ return pmdFaces; }
	vector<MmdStruct::PmdMaterial>	GetPmdMaterials()	{ return pmdMaterials; }
	vector<MmdStruct::PmdBone>		GetPmdBones()		{ return pmdBones; }
	vector<MmdStruct::PmdIkData>	GetPmdIkData()		{ return pmdIkData; }
	vector<MmdStruct::PmdRigidBody> GetPmdRigidBodies() { return pmdRigidBodies; }
	vector<MmdStruct::PmdJoint>		GetPmdJoints()		{ return pmdJoints; }

};
