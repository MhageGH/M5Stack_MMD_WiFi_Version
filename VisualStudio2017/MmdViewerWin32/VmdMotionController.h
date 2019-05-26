#pragma once
#include "MmdStruct.h"
#include "SkinMeshStruct.h"

class VmdMotionController sealed {
private:
	int time;							// 時間
	vector<Bone>* bones;
	vector<MmdStruct::PmdIkData>* pmdIkData;
	/// キーフレームアニメーション
	vector<D3DXQUATERNION> boneRot;					// 現在のボーンの回転
	vector<D3DXVECTOR3> bonePos;					// 現在のボーンの位置
	vector<list<KeyFrame>> keyFrames;				// ボーンごとにキーフレームをリストとしてもつ
	vector<list<KeyFrame>::iterator> ite_keyFrames;	// キーフレームのイテレータ
	/// IK
	void UpdateIK(const MmdStruct::PmdIkData&);		// IKボーン影響下ボーンの行列を更新
public:
	VmdMotionController(LPCTSTR filename, vector<Bone>* bones, vector<MmdStruct::PmdIkData>* pmdIkData);
	~VmdMotionController();
	void UpdateBoneMatrix();				// キーフレームデータを元にボーン行列を更新
	void AdvanceTime();
};
