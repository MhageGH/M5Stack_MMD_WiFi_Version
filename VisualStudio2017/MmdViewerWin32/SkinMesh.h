#pragma once
#include "Camera.h"
#include "VmdMotionController.h"
#include "PmdFileLoader.h"
#include "tstring.h"

/// スキンメッシュ
class SkinMesh {
protected:
	/// 基本 ///
	SkinMesh(LPDIRECT3DDEVICE9 pDevice);// オブジェクト生成禁止
	LPDIRECT3DDEVICE9 pDevice;			// Direct3Dデバイスオブジェクト
	/// メッシュ ///
	LPD3DXMESH pMesh;					// メッシュ
	vector<D3DMATERIAL9> materials;		// マテリアル配列
	vector<LPDIRECT3DTEXTURE9> textures;// テクスチャ配列
	vector<D3DXBONECOMBINATION> boneCombination;// ボーンコンビネーション配列
	virtual LPD3DXSKININFO CreateSkinInfo(const vector<BlendVertex>, const unsigned int, const D3DVERTEXELEMENT9*);	// メッシュ分割のためのスキン情報を作成
	virtual void DivideMesh(unsigned int numFace, LPD3DXMESH pOrgMesh, LPD3DXSKININFO pSkinInfo);					// ブレンド行列パレットのサイズを越えないようにメッシュを分割
	/// ボーン ///
	vector<Bone> bones;					// ボーン構造体
	ID3DXMesh* boneMesh;				// ボーン用メッシュ
	virtual void CreateBoneMesh();		// ボーン用メッシュを作成
	virtual vector<D3DXMATRIX> GetWorlds(const D3DXMATRIX* world);					// ボーン行列から変換したワールド変換行列の配列を取得
	/// シェーダ ///
	D3DXHANDLE hTech, hWorld, hView, hProj, hAmbient, hDiffuse, hLightDir, hTexture;// ハンドル
	LPD3DXEFFECT pFX;		// シェーダ
	void CreateShader();	// シェーダの作成
public:
	virtual ~SkinMesh();
	virtual void Draw(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera);				// 描画
	virtual void DrawBoneMeshes(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera);	// ボーンを表示
};

/// MMDスキンメッシュ
/// 注意：PMD形式の初期ボーンは回転成分を持たない！すなわち、初期姿勢行列、ボーンオフセット行列は常に回転成分を持たない。
class MmdSkinMesh sealed : public SkinMesh {
private:
	vector<MmdStruct::PmdIkData> pmdIkData;

public:
	MmdSkinMesh(
		LPDIRECT3DDEVICE9 pDevice, 
		vector<MmdStruct::PmdVertex> pmdVertices, 
		vector<unsigned short> pmdFaces, 
		vector<MmdStruct::PmdMaterial> pmdMaterials, 
		vector<MmdStruct::PmdBone> pmdBones, 
		vector<MmdStruct::PmdIkData> pmdIkData,
		tstring directoryName
		);

	// VmdMotionControllerへのリンク用
	vector<Bone>*					GetBonesAsPointer()		{ return &bones; }
	vector<MmdStruct::PmdIkData>*	GetPmdIkDataAsPointer()	{ return &pmdIkData; }



	// 補助用関数
private:
	void LoadPmdfile(const LPCTSTR&, vector<MmdStruct::PmdVertex>&, vector<unsigned short>&, vector<MmdStruct::PmdMaterial>&, vector<MmdStruct::PmdBone>&);
	void CreateMesh(vector<MmdStruct::PmdVertex>, vector<unsigned short>, vector<MmdStruct::PmdMaterial>, const unsigned int numBone, tstring directoryName);
	void CreateBoneMatrix(vector<MmdStruct::PmdBone> pmdBones);
	void CopyMaterial(D3DMATERIAL9&, const MmdStruct::PmdMaterial&);
	vector<unsigned long> CreateMaterial(unsigned int numFace, vector<MmdStruct::PmdMaterial> pmdMaterials, tstring directoryName);
	vector<BlendVertex> CreateVertices(LPD3DXMESH pOrgMesh, vector<MmdStruct::PmdVertex> pmdVertices);
	void CreatePolygon(LPD3DXMESH pOrgMesh, vector<unsigned short> pmdFaces);
	void CreateAttribute(LPD3DXMESH pOrgMesh, vector<unsigned long> material_numbers);
	tstring GetTextureFilename(const char* texture_file_name, tstring directoryName);
};

