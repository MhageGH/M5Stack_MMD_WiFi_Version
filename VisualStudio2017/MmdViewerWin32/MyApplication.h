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
	// コンストラクタ
	// @param hWnd ウィンドウハンドル
	MyApplication(HWND hWnd, ControlVariable* controlVariable, int width, int height);

	// デストラクタ
	~MyApplication();

	// 毎フレーム行なわれる処理
	void Run();


	// 補助関数
private:
	float GetAspect(HWND hWnd);
	void OpenPmdFile(tstring filename);
	void OpenVmdFile(tstring filename);
};
