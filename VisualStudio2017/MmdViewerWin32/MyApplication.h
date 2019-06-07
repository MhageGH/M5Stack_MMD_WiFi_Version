#pragma once
#include "Camera.h"
#include "DirectXFramework.h"
#include "SkinMesh.h"
#include "Coord.h"
#include "ControlVariable.h"
#include "BulletPhysics.h"
#include "MmdPhysics.h"

class MyApplication sealed {
private:
	DirectXFramework* directXFramework;
	MmdSkinMesh* mmdSkinMesh;
	VmdMotionController* vmdMotionController;
	Coord* coord;
	float aspect;
	ControlVariable* controlVariable;
	LPDIRECT3DDEVICE9 pDevice;
	BulletPhysics* bulletPhysics;
	MmdPhysics* mmdPhysics;

public:
	// �R���X�g���N�^
	// @param hWnd �E�B���h�E�n���h��
	MyApplication(HWND hWnd, ControlVariable* controlVariable, int width, int height);

	// �f�X�g���N�^
	~MyApplication();

	// ���t���[���s�Ȃ��鏈��
	void Run();


	// �⏕�֐�
private:
	float GetAspect(HWND hWnd);
	void OpenPmdFile(tstring filename);
	void OpenVmdFile(tstring filename);
};
