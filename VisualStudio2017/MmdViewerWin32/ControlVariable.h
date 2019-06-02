#pragma once
#include "tstring.h"

// アプリケーションとGUIを結ぶ制御変数
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
	bool openPmdFileFlag;		// PMDファイルを開くフラグ
	bool openVmdFileFlag;		// VMDファイルを開くフラグ
	std::tstring pmdFileName;	// PMDファイル名
	std::tstring vmdFileName;	// VMDファイル名
	D3DXVECTOR3 eyePoint;		// 視点
	D3DXVECTOR3 lookAtPoint;	// 注視点
	D3DXMATRIX view;			// ビュー変換行列
	bool physicsEnabled;		// 物理演算使用可能フラグ
	bool boneMeshEnabled;		// ボーンメッシュ使用可能フラグ
	bool rigidMeshEnabled;		// 剛体メッシュ使用可能フラグ
	bool jointMeshEnabled;		// ジョイントメッシュ使用可能フラグ
};
