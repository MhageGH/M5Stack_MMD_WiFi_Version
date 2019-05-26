#pragma once
#include "Camera.h"
#include "VmdMotionController.h"
#include "PmdFileLoader.h"
#include "tstring.h"

/// �X�L�����b�V��
class SkinMesh {
protected:
	/// ��{ ///
	SkinMesh(LPDIRECT3DDEVICE9 pDevice);// �I�u�W�F�N�g�����֎~
	LPDIRECT3DDEVICE9 pDevice;			// Direct3D�f�o�C�X�I�u�W�F�N�g
	/// ���b�V�� ///
	LPD3DXMESH pMesh;					// ���b�V��
	vector<D3DMATERIAL9> materials;		// �}�e���A���z��
	vector<LPDIRECT3DTEXTURE9> textures;// �e�N�X�`���z��
	vector<D3DXBONECOMBINATION> boneCombination;// �{�[���R���r�l�[�V�����z��
	virtual LPD3DXSKININFO CreateSkinInfo(const vector<BlendVertex>, const unsigned int, const D3DVERTEXELEMENT9*);	// ���b�V�������̂��߂̃X�L�������쐬
	virtual void DivideMesh(unsigned int numFace, LPD3DXMESH pOrgMesh, LPD3DXSKININFO pSkinInfo);					// �u�����h�s��p���b�g�̃T�C�Y���z���Ȃ��悤�Ƀ��b�V���𕪊�
	/// �{�[�� ///
	vector<Bone> bones;					// �{�[���\����
	ID3DXMesh* boneMesh;				// �{�[���p���b�V��
	virtual void CreateBoneMesh();		// �{�[���p���b�V�����쐬
	virtual vector<D3DXMATRIX> GetWorlds(const D3DXMATRIX* world);					// �{�[���s�񂩂�ϊ��������[���h�ϊ��s��̔z����擾
	/// �V�F�[�_ ///
	D3DXHANDLE hTech, hWorld, hView, hProj, hAmbient, hDiffuse, hLightDir, hTexture;// �n���h��
	LPD3DXEFFECT pFX;		// �V�F�[�_
	void CreateShader();	// �V�F�[�_�̍쐬
public:
	virtual ~SkinMesh();
	virtual void Draw(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera);				// �`��
	virtual void DrawBoneMeshes(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera);	// �{�[����\��
};

/// MMD�X�L�����b�V��
/// ���ӁFPMD�`���̏����{�[���͉�]�����������Ȃ��I���Ȃ킿�A�����p���s��A�{�[���I�t�Z�b�g�s��͏�ɉ�]�����������Ȃ��B
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

	// VmdMotionController�ւ̃����N�p
	vector<Bone>*					GetBonesAsPointer()		{ return &bones; }
	vector<MmdStruct::PmdIkData>*	GetPmdIkDataAsPointer()	{ return &pmdIkData; }



	// �⏕�p�֐�
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

