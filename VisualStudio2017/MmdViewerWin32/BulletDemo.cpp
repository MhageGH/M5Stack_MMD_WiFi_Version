#include "StdAfx.h"
#include "BulletDemo.h"
#include "Path.h"

BulletDemo::BulletDemo(HWND hWnd) {
	directXFramework = new DirectXFramework(hWnd);
	pDevice = directXFramework->GetD3DDevice();
	coord = new Coord(pDevice);
	aspect = GetAspect(hWnd);

	// Bullet���E�쐬
	const D3DXVECTOR3 gravity(0, -3.0f, 0);
	bulletPhysics = new BulletPhysics(gravity);
}

BulletDemo::~BulletDemo() {
	SAFE_DELETE(bulletPhysics);
	SAFE_DELETE(coord);
	SAFE_DELETE(directXFramework);
}

// private : �A�X�y�N�g����擾
float BulletDemo::GetAspect(HWND hWnd) {
	RECT rc;
	GetClientRect(hWnd, &rc);
	return (float)rc.right/(float)rc.bottom;	// �A�X�y�N�g�� = �r���[��Ԃ̕�/���� (MSDN�ł͍���/���ɂȂ��Ă��邪�ԈႢ)
}



FreeDropDemo::FreeDropDemo(HWND hWnd) : BulletDemo(hWnd) {
	tstring filename(TEXT("C:\\MyProgram\\MmdViewer\\ModelData\\1.pmd"));
	PmdFileLoader* pmdFileLoader = new PmdFileLoader(filename.c_str());
	tstring directoryName = Path::GetDirectoryName(filename);
	mesh = new MmdSkinMesh(pDevice, pmdFileLoader->GetPmdVertices(), pmdFileLoader->GetPmdFaces(), pmdFileLoader->GetPmdMaterials(), pmdFileLoader->GetPmdBones(), pmdFileLoader->GetPmdIkData(), directoryName);
	CreateSphereRigid(pmdFileLoader->GetPmdRigidBodies()[0]);
	CreateBoxRigid();
}

// private
void FreeDropDemo::CreateBoxRigid() {
	const float width(10), height(10), depth(10);
	const float mass(0);
	const float restitution(1.0f);
	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, 0, -height/2, 0);
	box = bulletPhysics->CreateBox(width, height, depth, &world, mass, restitution);	/// ������(�n��)
}

// private
void FreeDropDemo::CreateSphereRigid(MmdStruct::PmdRigidBody body) {
	const int N(50);			// ���̌�
	for (int i = 0; i < N; ++i)	{
		D3DXMATRIX world;
		D3DXMatrixTranslation(&world, i*0.003f, 1.0f*i + 1.0f, i*0.002f);
		spheres.push_back(bulletPhysics->CreateSphere(body.shape_w, &world, body.rigidbody_weight, body.rigidbody_recoil, body.rigidbody_friction, body.rigidbody_pos_dim, body.rigidbody_rot_dim));
	}
}

FreeDropDemo::~FreeDropDemo() {
	SAFE_DELETE(mesh);
}

void FreeDropDemo::Run() {
	// ���C�g
	D3DLIGHT9 light = {D3DLIGHT_DIRECTIONAL, {1, 1, 1, 0}, {1, 1, 1, 0}, {1, 1, 1, 0}};	// �F
	light.Direction = D3DXVECTOR3(-0.5f, -1, 0.5f);		// ����

	// �J����
	static int t;
	if (t < 600) ++t;
	float theta = 0.001f*t;
	float r = 10.0f - 3.0f*t/600.0f;
	D3DXVECTOR3 eyePoint = D3DXVECTOR3(-r*sinf(theta), 2.0f, -r*(cosf(theta)));		// ���_
	D3DXVECTOR3 lookAtPoint = D3DXVECTOR3(0, 0.8f, 0);		// �����_
	Camera camera(eyePoint, lookAtPoint, aspect);

	// �������Z
	bulletPhysics->StepSimulation();

	// �`��
	directXFramework->BeginScene(240, 180, 180);// �V�[���J�n
	coord->Draw(&camera);
	for (unsigned int i = 0; i < spheres.size(); ++i) {
		D3DXMATRIX world = (*(mesh->GetBonesAsPointer()))[0].offsetMatML*bulletPhysics->GetWorld(spheres[i]);
		mesh->Draw(&world, &light, &camera);
	}
	directXFramework->EndScene();	// �V�[���I��
}