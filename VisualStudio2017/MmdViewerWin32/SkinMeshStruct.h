#pragma once
#include "Bezie.h"

///// �X�L�����b�V���A�j���[�V�����p�\���� /////

///// ���̒�` �E�E�E �{�w�b�_���ł͈ȉ��̂悤�Ƀ{�[���̍��W�n�̖��̂��`����	/////
///// ���[���h���W�n�F ��΍��W�n												/////
///// ���f�����[�J�����W�n�F���[���h���W�ϊ����܂܂Ȃ�3D���f���̃��[�J�����W�n	/////
///// �{�[�����[�J�����W�n�F���g�̐e�{�[������ɂ������[�J�����W�n			/////

// �u�����h�d�ݕt���b�V���̒��_�f�[�^
struct BlendVertex {
	D3DXVECTOR3 position;
	float weight[3];
	unsigned char matrixIndex[4];
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texture;
};

// �{�[��
struct Bone {
public:
	bool extraBoneControl;	// �K�w�\�����g�킸����boneMatML�𑀍�
	int id;					// �{�[��ID�i�ʂ��ԍ��j
	string name;			// �{�[����
	unsigned char type;		// �{�[���^�C�v (MMD�̏ꍇ 0:��] 1:��]�ƈړ� 2:IK 3:�s�� 4:IK�e���� 5:��]�e���� 6:IK�ڑ��� 7:��\�� 8:�P�� 9:��]�^�� )
	Bone* parent;			// �e�{�[��
	Bone* firstChild;		// ��1�q�{�[��
	Bone* sibling;			// ���̌Z��{�[��
	D3DXMATRIX initMatBL;	// �����p���s��(�{�[�����[�J�����W�n)
	D3DXMATRIX initMatML;	// �����p���s��(���f�����[�J�����W�n)
	D3DXMATRIX boneMatBL;	// �{�[���s��(�{�[�����[�J�����W�n)
	D3DXMATRIX boneMatML;	// �{�[���s��(���f�����[�J�����W�n)
	D3DXMATRIX offsetMatML;	// �{�[���I�t�Z�b�g�s��(���f�����[�J�����W�n)
	Bone() : extraBoneControl(), type(), id(), parent(), firstChild(), sibling() {	// �R���X�g���N�^
		D3DXMatrixIdentity(&initMatBL);
		D3DXMatrixIdentity(&initMatML);
		D3DXMatrixIdentity(&offsetMatML);
		D3DXMatrixIdentity(&boneMatBL);
		D3DXMatrixIdentity(&boneMatML);
	}
	D3DXMATRIX CalBoneMatML() {	// ���f�����[�J�����W�n�ł̃{�[���s����v�Z
		if (parent) return boneMatBL*parent->CalBoneMatML();
		else return boneMatBL;
	}
};

// �L�[�t���[���A�j���[�V�����f�[�^
struct KeyFrame {
	string boneName;		// �{�[����
	unsigned long frameNo;	// �t���[���ԍ�
	D3DXVECTOR3 position;	// �ʒu
	D3DXQUATERNION rotation;// ��]
	Bezie* bezie_x;
	Bezie* bezie_y;
	Bezie* bezie_z;
	Bezie* bezie_r;
	bool operator < (const KeyFrame &k) const {	// �t���[���ԍ��Ń\�[�g���邽�߂̔�r���Z�q
		return frameNo < k.frameNo;
	}
};