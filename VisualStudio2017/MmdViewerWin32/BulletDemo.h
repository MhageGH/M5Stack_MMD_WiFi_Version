#pragma once
#include "DirectXFramework.h"
#include "Coord.h"
#include "BulletPhysics.h"
#include "SkinMesh.h"

/// Bullet‚Ìƒfƒ‚
class BulletDemo {
protected:
	DirectXFramework* directXFramework;
	LPDIRECT3DDEVICE9 pDevice;
	Coord* coord;
	BulletPhysics* bulletPhysics;
	float aspect;
public:
	BulletDemo(HWND hWnd);
	virtual ~BulletDemo();
	virtual void Run() = 0;

private:
	float GetAspect(HWND hWnd);
};




/// Ž©—R—Ž‰º
class FreeDropDemo sealed : public BulletDemo {
private:
	MmdSkinMesh* mesh;
	btRigidBody* box;
	vector<btRigidBody*> spheres;

public:
	FreeDropDemo(HWND hWnd);
	~FreeDropDemo();
	void Run();

private:
	void CreateBoxRigid();
	void CreateSphereRigid(MmdStruct::PmdRigidBody body);
};