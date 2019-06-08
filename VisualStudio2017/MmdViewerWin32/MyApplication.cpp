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
	// �t�@�C�����J��
	if (controlVariable->openPmdFileFlag) OpenPmdFile(controlVariable->pmdFileName.c_str());
	if (controlVariable->openVmdFileFlag) OpenVmdFile(controlVariable->vmdFileName.c_str());
	controlVariable->openPmdFileFlag = controlVariable->openVmdFileFlag = 0;

	// �z�u
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	// �A�j���[�V����
	if (vmdMotionController) vmdMotionController->AdvanceTime();
	if (vmdMotionController) vmdMotionController->UpdateBoneMatrix();

	// �������Z
	mmdPhysics->BoneUpdate(controlVariable->physicsEnabled);

	// ���C�g
	D3DLIGHT9 light = { D3DLIGHT_DIRECTIONAL, {1, 1, 1, 0}, {1, 1, 1, 0}, {1, 1, 1, 0} };	// �F
	light.Direction = D3DXVECTOR3(-1, -0.8f, 0.7f);		// ����

	// �J����
	Camera camera(controlVariable->eyePoint, controlVariable->lookAtPoint, aspect);
	camera.SetViewMatrix(controlVariable->view);

	// �`��
	directXFramework->BeginScene(10, 10, 10);// �V�[���J�n
	coord->Draw(&camera);
	if (mmdSkinMesh) mmdSkinMesh->Draw(&world, &light, &camera);
	if (mmdSkinMesh && controlVariable->boneMeshEnabled) mmdSkinMesh->DrawBoneMeshes(&world, &light, &camera);
	if (mmdSkinMesh && controlVariable->rigidMeshEnabled) mmdPhysics->DrawRigidMesh(&world, &light, &camera);
	if (mmdSkinMesh && controlVariable->jointMeshEnabled) mmdPhysics->DrawJointMesh(&world, &light, &camera);
	directXFramework->EndScene();	// �V�[���I��

	// �r�b�g�}�b�v�o�b�t�@�X�V
	if (controlVariable->bitmapRevising) {
		this->controlVariable->bitmapBuffer = directXFramework->ReviseBitmap();
		controlVariable->bitmapRevising = false;
	}

}


// private : �A�X�y�N�g����擾
// @param hWnd �E�B���h�E�n���h��
// @return �A�X�y�N�g��
float MyApplication::GetAspect(HWND hWnd) {
	RECT rc;
	GetClientRect(hWnd, &rc);
	return (float)rc.right / (float)rc.bottom;	// �A�X�y�N�g�� = �r���[��Ԃ̕�/���� (MSDN�ł͍���/���ɂȂ��Ă��邪�ԈႢ)
}

// private: PMD�t�@�C�����J��
// @param filename PMD�t�@�C����
void MyApplication::OpenPmdFile(tstring filename) {
	// TODO ���̃I�u�W�F�N�g���N���A
	vmdMotionController = 0;
	PmdFileLoader* pmdFileLoader = new PmdFileLoader(filename.c_str());
	mmdSkinMesh = new MmdSkinMesh(pDevice, pmdFileLoader->GetPmdVertices(), pmdFileLoader->GetPmdFaces(), pmdFileLoader->GetPmdMaterials(), pmdFileLoader->GetPmdBones(), pmdFileLoader->GetPmdIkData(), Path::GetDirectoryName(filename));
	mmdPhysics->SetBones(mmdSkinMesh->GetBonesAsPointer());
	mmdPhysics->CreateRigid(pmdFileLoader->GetPmdRigidBodies(), pmdFileLoader->GetPmdBones());
	mmdPhysics->CreateJoint(pmdFileLoader->GetPmdJoints());
	delete pmdFileLoader;
}

// private: VMD�t�@�C�����J��
void MyApplication::OpenVmdFile(tstring filename) {
	if (mmdSkinMesh) vmdMotionController = new VmdMotionController(filename.c_str(), mmdSkinMesh->GetBonesAsPointer(), mmdSkinMesh->GetPmdIkDataAsPointer());
}