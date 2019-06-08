#include "StdAfx.h"
#include "MyApplication.h"
#include "Path.h"

MyApplication::MyApplication(HWND hWnd, ControlVariable* controlVariable, int width, int height) {
	this->controlVariable = controlVariable;
	directXFramework = new DirectXFramework(hWnd);
	pDevice = directXFramework->GetD3DDevice();
	coord = new Coord(pDevice);
	aspect = GetAspect(hWnd);
	mmdSkinMesh = 0;
	vmdMotionController = 0;
	const D3DXVECTOR3 gravity(0, -9.8f*2.5f, 0);
	bulletPhysics = new BulletPhysics(gravity);
	mmdPhysics = new MmdPhysics(pDevice, bulletPhysics);
	this->controlVariable->bitmapBufferSize = directXFramework->CalculateBitmapBufferSize(width, height);
	;
}

MyApplication::~MyApplication() {
	SAFE_DELETE(bulletPhysics);
	SAFE_DELETE(mmdPhysics);
	SAFE_DELETE(coord);
	SAFE_DELETE(vmdMotionController);
	SAFE_DELETE(mmdSkinMesh);
	SAFE_DELETE(directXFramework);
}

void MyApplication::Run() {
	// ファイルを開く
	if (controlVariable->openPmdFileFlag) OpenPmdFile(controlVariable->pmdFileName.c_str());
	if (controlVariable->openVmdFileFlag) OpenVmdFile(controlVariable->vmdFileName.c_str());
	controlVariable->openPmdFileFlag = controlVariable->openVmdFileFlag = 0;

	// 配置
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	// アニメーション
	if (vmdMotionController) vmdMotionController->AdvanceTime();
	if (vmdMotionController) vmdMotionController->UpdateBoneMatrix();

	// 物理演算
	mmdPhysics->BoneUpdate(controlVariable->physicsEnabled);

	// ライト
	D3DLIGHT9 light = { D3DLIGHT_DIRECTIONAL, {1, 1, 1, 0}, {1, 1, 1, 0}, {1, 1, 1, 0} };	// 色
	light.Direction = D3DXVECTOR3(-1, -0.8f, 0.7f);		// 方向

	// カメラ
	Camera camera(controlVariable->eyePoint, controlVariable->lookAtPoint, aspect);
	camera.SetViewMatrix(controlVariable->view);

	// 描画
	directXFramework->BeginScene(10, 10, 10);// シーン開始
	coord->Draw(&camera);
	if (mmdSkinMesh) mmdSkinMesh->Draw(&world, &light, &camera);
	if (mmdSkinMesh && controlVariable->boneMeshEnabled) mmdSkinMesh->DrawBoneMeshes(&world, &light, &camera);
	if (mmdSkinMesh && controlVariable->rigidMeshEnabled) mmdPhysics->DrawRigidMesh(&world, &light, &camera);
	if (mmdSkinMesh && controlVariable->jointMeshEnabled) mmdPhysics->DrawJointMesh(&world, &light, &camera);
	directXFramework->EndScene();	// シーン終了

	// ビットマップバッファ更新
	if (controlVariable->bitmapRevising) {
		this->controlVariable->bitmapBuffer = directXFramework->ReviseBitmap();
		controlVariable->bitmapRevising = false;
	}

}


// private : アスペクト比を取得
// @param hWnd ウィンドウハンドル
// @return アスペクト比
float MyApplication::GetAspect(HWND hWnd) {
	RECT rc;
	GetClientRect(hWnd, &rc);
	return (float)rc.right / (float)rc.bottom;	// アスペクト比 = ビュー空間の幅/高さ (MSDNでは高さ/幅になっているが間違い)
}

// private: PMDファイルを開く
// @param filename PMDファイル名
void MyApplication::OpenPmdFile(tstring filename) {
	// TODO 剛体オブジェクトをクリア
	vmdMotionController = 0;
	PmdFileLoader* pmdFileLoader = new PmdFileLoader(filename.c_str());
	mmdSkinMesh = new MmdSkinMesh(pDevice, pmdFileLoader->GetPmdVertices(), pmdFileLoader->GetPmdFaces(), pmdFileLoader->GetPmdMaterials(), pmdFileLoader->GetPmdBones(), pmdFileLoader->GetPmdIkData(), Path::GetDirectoryName(filename));
	mmdPhysics->SetBones(mmdSkinMesh->GetBonesAsPointer());
	mmdPhysics->CreateRigid(pmdFileLoader->GetPmdRigidBodies(), pmdFileLoader->GetPmdBones());
	mmdPhysics->CreateJoint(pmdFileLoader->GetPmdJoints());
	delete pmdFileLoader;
}

// private: VMDファイルを開く
void MyApplication::OpenVmdFile(tstring filename) {
	if (mmdSkinMesh) vmdMotionController = new VmdMotionController(filename.c_str(), mmdSkinMesh->GetBonesAsPointer(), mmdSkinMesh->GetPmdIkDataAsPointer());
}