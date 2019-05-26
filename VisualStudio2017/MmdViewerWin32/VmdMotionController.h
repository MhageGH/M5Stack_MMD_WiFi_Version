#pragma once
#include "MmdStruct.h"
#include "SkinMeshStruct.h"

class VmdMotionController sealed {
private:
	int time;							// ����
	vector<Bone>* bones;
	vector<MmdStruct::PmdIkData>* pmdIkData;
	/// �L�[�t���[���A�j���[�V����
	vector<D3DXQUATERNION> boneRot;					// ���݂̃{�[���̉�]
	vector<D3DXVECTOR3> bonePos;					// ���݂̃{�[���̈ʒu
	vector<list<KeyFrame>> keyFrames;				// �{�[�����ƂɃL�[�t���[�������X�g�Ƃ��Ă���
	vector<list<KeyFrame>::iterator> ite_keyFrames;	// �L�[�t���[���̃C�e���[�^
	/// IK
	void UpdateIK(const MmdStruct::PmdIkData&);		// IK�{�[���e�����{�[���̍s����X�V
public:
	VmdMotionController(LPCTSTR filename, vector<Bone>* bones, vector<MmdStruct::PmdIkData>* pmdIkData);
	~VmdMotionController();
	void UpdateBoneMatrix();				// �L�[�t���[���f�[�^�����Ƀ{�[���s����X�V
	void AdvanceTime();
};
