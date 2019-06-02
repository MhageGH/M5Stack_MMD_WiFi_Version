#pragma once
#include "tstring.h"

// �A�v���P�[�V������GUI�����Ԑ���ϐ�
struct ControlVariable {
	ControlVariable() {
		openPmdFileFlag = false;
		openVmdFileFlag = false;
		physicsEnabled = true;
		boneMeshEnabled = false;
		rigidMeshEnabled = false;
		jointMeshEnabled = false;
		D3DXMatrixIdentity(&view);
	}
	bool openPmdFileFlag;		// PMD�t�@�C�����J���t���O
	bool openVmdFileFlag;		// VMD�t�@�C�����J���t���O
	std::tstring pmdFileName;	// PMD�t�@�C����
	std::tstring vmdFileName;	// VMD�t�@�C����
	D3DXVECTOR3 eyePoint;		// ���_
	D3DXVECTOR3 lookAtPoint;	// �����_
	D3DXMATRIX view;			// �r���[�ϊ��s��
	bool physicsEnabled;		// �������Z�g�p�\�t���O
	bool boneMeshEnabled;		// �{�[�����b�V���g�p�\�t���O
	bool rigidMeshEnabled;		// ���̃��b�V���g�p�\�t���O
	bool jointMeshEnabled;		// �W���C���g���b�V���g�p�\�t���O
};
