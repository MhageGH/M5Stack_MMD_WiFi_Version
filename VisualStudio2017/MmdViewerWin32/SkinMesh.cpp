#include "StdAfx.h"
#include "SkinMesh.h"

namespace {
	const unsigned long palleteSize = 50;	// �s��p���b�g�̃T�C�Y
}

///// SkinMesh�N���X /////
SkinMesh::SkinMesh(LPDIRECT3DDEVICE9 pDev) : pDevice(pDev), pFX(0), pMesh(0) {
}

SkinMesh::~SkinMesh(){
	SAFE_RELEASE(pFX);
	for (unsigned long i = 0; i < boneCombination.size(); ++i) delete[] boneCombination[i].BoneId;
	for (unsigned int i = 0; i < textures.size(); ++i) SAFE_RELEASE(textures[i]);
	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(boneMesh);
}

LPD3DXSKININFO SkinMesh::CreateSkinInfo(const vector<BlendVertex> blendVertices, const unsigned int numBone, const D3DVERTEXELEMENT9* declAry) {
	LPD3DXSKININFO pSkinInfo;
	unsigned long numVertex = blendVertices.size();
	D3DXCreateSkinInfo(numVertex, declAry, numBone, &pSkinInfo);
	for (unsigned int i = 0; i < numBone; ++i) {
		unsigned long numInfluence = 0;								// �{�[��i�ɉe�����钸�_�̐�
		unsigned long* vertices = new unsigned long[4*numVertex];	// �{�[��i�ɉe�����钸�_�̔z��
		float* weights = new float[4*numVertex];					// �{�[��i�ɉe������d�݂̔z��
		for (unsigned long j = 0; j < numVertex; ++j) {
			for (unsigned int k = 0; k < 4; ++k) {
				if (blendVertices[j].matrixIndex[k] == i) {
					vertices[numInfluence] = j;
					if (k < 3) weights[numInfluence] = blendVertices[j].weight[k];
					else {
						weights[numInfluence] = 1.0;
						for (unsigned int l = 0; l < 3; ++l) weights[numInfluence] -= blendVertices[j].weight[l];
					}
					++numInfluence;
				}
			}
		}
		if(FAILED(pSkinInfo->SetBoneInfluence(i, numInfluence, vertices, weights))) throw TEXT("SkinInfo�̍쐬�Ɏ��s���܂���");;
		delete[] vertices;
		delete[] weights;
	}
	return pSkinInfo;
}

void SkinMesh::DivideMesh(unsigned int numFace, LPD3DXMESH pOrgMesh, LPD3DXSKININFO pSkinInfo) {
	unsigned long* pAdjacencyIn = new unsigned long[3*numFace];
	if (FAILED(pOrgMesh->GenerateAdjacency(0, pAdjacencyIn))) throw TEXT("�אڐ����̍쐬�Ɏ��s���܂���");
	unsigned long maxFaceInfl;
	unsigned long numBoneCombination;
	LPD3DXBUFFER pBoneCombinationBuffer = 0;
	if(FAILED(pSkinInfo->ConvertToIndexedBlendedMesh(pOrgMesh, 0, palleteSize, pAdjacencyIn, 0, 0, 0, &maxFaceInfl, &numBoneCombination, &pBoneCombinationBuffer, &pMesh))) throw TEXT("���b�V���̕����Ɏ��s���܂���");
	LPD3DXBONECOMBINATION bc = (LPD3DXBONECOMBINATION)pBoneCombinationBuffer->GetBufferPointer();
	for (unsigned long i = 0; i < numBoneCombination; ++i) boneCombination.push_back(bc[i]);
	for (unsigned long i = 0; i < numBoneCombination; ++i) {
		boneCombination[i].BoneId = new unsigned long[palleteSize];
		memcpy(boneCombination[i].BoneId, bc[i].BoneId, palleteSize*sizeof(unsigned long));	 // �f�B�[�v�R�s�[
	}
	delete[] pAdjacencyIn;
	pBoneCombinationBuffer->Release();
}

void SkinMesh::CreateShader() {
	LPD3DXBUFFER errors = 0;
	HRESULT res = D3DXCreateEffectFromFile(pDevice, TEXT("Shader.fx"), 0, 0, 0, 0, &pFX, &errors);
	if (FAILED(res)) throw TEXT("�V�F�[�_�̃R���p�C���Ɏ��s���܂���");
	hTech = pFX->GetTechniqueByName("BlendTech");
	hWorld = pFX->GetParameterByName(0, "world");
	hView = pFX->GetParameterByName(0, "view");
	hProj = pFX->GetParameterByName(0, "proj");
	hAmbient = pFX->GetParameterByName(0, "ambient");
	hDiffuse = pFX->GetParameterByName(0, "diffuse");
	hLightDir = pFX->GetParameterByName(0, "lightDir");
	hTexture = pFX->GetParameterByName(0, "tex");
	if (!hTech || !hWorld || !hView || !hProj || !hAmbient || !hDiffuse || !hLightDir || !hTexture) throw TEXT("�V�F�[�_�t�@�C���Ɏw�肵���ϐ������݂���܂���ł���");
}

vector<D3DXMATRIX> SkinMesh::GetWorlds(const D3DXMATRIX* world) {
	class WorldsCalc {	// ���[���h�ϊ��s��̔z����v�Z����ċA�֐����`�����N���X
	public:
		static void Run(Bone* me, const D3DXMATRIX *parentWorldMat, vector<D3DXMATRIX>* worlds) {
			D3DXMATRIX m = me->boneMatBL*(*parentWorldMat);
			(*worlds)[me->id] = me->offsetMatML*m;
			if (me->firstChild) Run(me->firstChild, &m, worlds);
			if (me->sibling)	Run(me->sibling, parentWorldMat, worlds);
		}
	};
	vector<D3DXMATRIX> worlds(bones.size());
	WorldsCalc::Run(&bones[0], world, &worlds);
	for (unsigned int i = 0; i < bones.size(); ++i) if (bones[i].extraBoneControl) worlds[i] = bones[i].offsetMatML*bones[i].boneMatML*(*world);	// �������Z�{�[���͕������Z�̌��ʂŏ㏑������
	return worlds;
}

void SkinMesh::Draw(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera) {
	vector<D3DXMATRIX> worlds = GetWorlds(world);

	// �J�����ƃ��C�g
	D3DXMATRIX view, projection;
	camera->GetMatrix(&view, &projection);
	D3DXVECTOR3 lightDir = light->Direction;

	////// �v���O���}�u���V�F�[�_�ݒ�
	pFX->SetTechnique(hTech);
	pFX->SetMatrix(hView, &view);
	pFX->SetMatrix(hProj, &projection);
	pFX->SetFloatArray(hLightDir, (const float*)&lightDir, 3);

	// �`��
	pFX->Begin(0, 0);
	for (unsigned long i = 0; i < boneCombination.size(); ++i) {
		unsigned long numBlendMatrix = 0;
		for (unsigned long k = 0; k < palleteSize; ++k) if (boneCombination[i].BoneId[k] != UINT_MAX) numBlendMatrix = k + 1;
		vector<D3DXMATRIX> selectedCombMat;
		for (unsigned long k = 0; k < numBlendMatrix; ++k) selectedCombMat.push_back(worlds[boneCombination[i].BoneId[k]]);
		pFX->SetMatrixArray(hWorld, &selectedCombMat[0], numBlendMatrix);
		pFX->SetFloatArray(hAmbient, (const float*)&materials[boneCombination[i].AttribId].Ambient, 4);
		pFX->SetFloatArray(hDiffuse, (const float*)&materials[boneCombination[i].AttribId].Diffuse, 4);
		pFX->SetTexture(hTexture, textures[boneCombination[i].AttribId]);
		if (textures[boneCombination[i].AttribId]) pFX->BeginPass(0);
		else pFX->BeginPass(1);
		pMesh->DrawSubset(i);
		pFX->EndPass();
	}
	pFX->End();
}

void SkinMesh::CreateBoneMesh() {
	const float radius = 0.2f;
	const UINT slices = 10;
	const UINT stacks = 5;
	D3DXCreateSphere(pDevice, radius, slices, stacks, &boneMesh, 0);
}

void SkinMesh::DrawBoneMeshes(D3DXMATRIX* world, D3DLIGHT9* light, Camera* camera) {
	D3DMATERIAL9 material = {{0, 0, 0, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}};
	vector<D3DXMATRIX> worlds = GetWorlds(world);
	D3DXMATRIX view, projection;
	camera->GetMatrix(&view, &projection);
	pDevice->SetLight(0, light);
	pDevice->LightEnable(0, TRUE) ;
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetMaterial(&material);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetTransform(D3DTS_VIEW, &view);
	pDevice->SetTransform(D3DTS_PROJECTION, &projection);
	for (unsigned int i = 0; i < bones.size(); ++i) {
		// �\���p�{�[���̃��b�V���͂��Ƃ��ƌ��_�ɂ��邽�߁A�{�[���I�t�Z�b�g�s���������K�v���Ȃ�
		// ���[���h�ϊ��s��ɋt�s��������ă{�[���I�t�Z�b�g�s����L�����Z������
		D3DXMATRIX offsetCanceller;		
		D3DXMatrixInverse(&offsetCanceller, 0, &bones[i].offsetMatML);
		pDevice->SetTransform(D3DTS_WORLD, &(offsetCanceller*worlds[i]));
		if (bones[i].type != 7) boneMesh->DrawSubset(0);
	}
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}






///// MmdSkinMesh�N���X /////
MmdSkinMesh::MmdSkinMesh(LPDIRECT3DDEVICE9 pDevice, vector<MmdStruct::PmdVertex> pmdVertices, vector<unsigned short> pmdFaces, 
						 vector<MmdStruct::PmdMaterial> pmdMaterials, vector<MmdStruct::PmdBone> pmdBones, vector<MmdStruct::PmdIkData> pid, tstring directoryName) : SkinMesh(pDevice), pmdIkData(pid) {
	CreateMesh(pmdVertices, pmdFaces, pmdMaterials, pmdBones.size(), directoryName);
	CreateBoneMatrix(pmdBones);
	CreateBoneMesh();
	CreateShader();
}


void MmdSkinMesh::CopyMaterial(D3DMATERIAL9& material, const MmdStruct::PmdMaterial& pmdMaterial) {
	const float damp = 0.8f;	// �A���r�G���g���キ����W��
	material.Ambient.a = pmdMaterial.alpha;
	material.Ambient.r = damp*pmdMaterial.mirror_color[0];
	material.Ambient.g = damp*pmdMaterial.mirror_color[1];
	material.Ambient.b = damp*pmdMaterial.mirror_color[2];
	material.Diffuse.a = pmdMaterial.alpha;
	material.Diffuse.r = pmdMaterial.diffuse_color[0];
	material.Diffuse.g = pmdMaterial.diffuse_color[1];
	material.Diffuse.b = pmdMaterial.diffuse_color[2];
	material.Power = pmdMaterial.specularity;
	material.Specular.a = pmdMaterial.alpha;
	material.Specular.r = pmdMaterial.specular_color[0];
	material.Specular.g = pmdMaterial.specular_color[1];
	material.Specular.b = pmdMaterial.specular_color[2];
}


void MmdSkinMesh::CreateMesh(vector<MmdStruct::PmdVertex> pmdVertices, vector<unsigned short> pmdFaces, vector<MmdStruct::PmdMaterial> pmdMaterials, const unsigned int numBone, tstring directoryName) {
	// ���b�V���̓��ꕨ���쐬
	LPD3DXMESH pOrgMesh;
	D3DVERTEXELEMENT9 declAry[] = {
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
		{0, 24, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		{0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	unsigned int numFace = pmdFaces.size()/3;
	unsigned int numVertex = pmdVertices.size();
	D3DXCreateMesh(numFace, numVertex, D3DXMESH_MANAGED, declAry, pDevice, &pOrgMesh);
	vector<BlendVertex> blendVertices = CreateVertices(pOrgMesh, pmdVertices);
	CreatePolygon(pOrgMesh, pmdFaces);
	CreateAttribute(pOrgMesh, CreateMaterial(numFace, pmdMaterials, directoryName));
	LPD3DXSKININFO pSkinInfo = CreateSkinInfo(blendVertices, numBone, declAry);
	DivideMesh(numFace, pOrgMesh, pSkinInfo);
	SAFE_RELEASE(pSkinInfo);
	SAFE_RELEASE(pOrgMesh);
}


// private : ���_�f�[�^���쐬
// @param pOrgMesh �����O�̃��b�V��
// @param pmdVertices pmdVertex�z��
// @return ���_�f�[�^
vector<BlendVertex> MmdSkinMesh::CreateVertices(LPD3DXMESH pOrgMesh, vector<MmdStruct::PmdVertex> pmdVertices) {
	vector<BlendVertex> blendVertices;
	BlendVertex* vertexBuffer;
	pOrgMesh->LockVertexBuffer(0, (void**)&vertexBuffer);
	for (unsigned int i = 0; i < pmdVertices.size(); ++i) {
		BlendVertex bv;
		bv.position = D3DXVECTOR3(pmdVertices[i].pos[0], pmdVertices[i].pos[1], pmdVertices[i].pos[2]); // DirectX�̃X�P�[���ɍ��킹��!
		bv.normal = D3DXVECTOR3(pmdVertices[i].normal_vec[0], pmdVertices[i].normal_vec[1], pmdVertices[i].normal_vec[2]);
		bv.texture = D3DXVECTOR2(pmdVertices[i].uv[0], pmdVertices[i].uv[1]);
		bv.weight[0] = (float)pmdVertices[i].bone_weight/100.0f;
		bv.weight[1] = 1.0f - bv.weight[0];
		bv.weight[2] = 0;		// ���b�V�������ɕK�{
		bv.matrixIndex[0] = (unsigned char)pmdVertices[i].bone_num[0];
		bv.matrixIndex[1] = (unsigned char)pmdVertices[i].bone_num[1];
		bv.matrixIndex[2] = 0;	// ���b�V�������ɕK�{
		bv.matrixIndex[3] = 0;	// ���b�V�������ɕK�{
		vertexBuffer[i] = bv;
		blendVertices.push_back(bv);
	}
	pOrgMesh->UnlockVertexBuffer();
	return blendVertices;
}

// private : �|���S���f�[�^���쐬
// @param pOrgMesh �����O�̃��b�V��
void MmdSkinMesh::CreatePolygon(LPD3DXMESH pOrgMesh, vector<unsigned short> pmdFaces) {
	unsigned short* indexBuffer;
	pOrgMesh->LockIndexBuffer(0, (void**)&indexBuffer);
	for (unsigned int i = 0; i < pmdFaces.size(); ++i) indexBuffer[i] = pmdFaces[i];
	pOrgMesh->UnlockIndexBuffer();
}

// private : �ޗ��f�[�^���쐬
// @param numFace �|���S����
// @pram pmdMaterials pmdMaterial�z��
// @return �ޗ��ԍ�[�|���S���ԍ�]
vector<unsigned long> MmdSkinMesh::CreateMaterial(unsigned int numFace, vector<MmdStruct::PmdMaterial> pmdMaterials, tstring directoryName) {
	vector<unsigned long> material_numbers(numFace);	// �ޗ��ԍ�[�|���S���ԍ�]
	unsigned int j = 0, material_end = 0;
	for (unsigned int i = 0; i < pmdMaterials.size(); ++i) {
		D3DMATERIAL9 material = {0};
		CopyMaterial(material, pmdMaterials[i]);
		material_end += pmdMaterials[i].face_vert_count;
		for (; j < material_end; ++j) material_numbers[j/3] = i;
		materials.push_back(material);
		tstring filename = GetTextureFilename(pmdMaterials[i].texture_file_name, directoryName);
		textures.push_back(0);				// �z��̗v�f������₷
		if (lstrlen(filename.c_str()) > 0)
			if(FAILED(D3DXCreateTextureFromFileEx(pDevice, filename.c_str(), 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0xff000000, 0, 0, &textures[i])))
				throw TEXT("�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���");
	}
	return material_numbers;
}

// private : �e�N�X�`���t�@�C�������擾
// @param texture_file_name char�^�e�N�X�`���t�@�C����
// @param directoryName �f�B���N�g����
// @return �f�B���N�g������t������tstring�^�e�N�X�`���t�@�C����
tstring MmdSkinMesh::GetTextureFilename(const char* texture_file_name, tstring directoryName) {
	char tex[21] = {0};	// �t�@�C������20byte�̂Ƃ��̂��߂ɍŌ��0��ǉ�
	memcpy(tex, texture_file_name, 20);
	string str(tex);
	str = str.substr(0, str.find("*"));	// "*"�͔�Ή�
	char filename[256] = {0};			// UNICODE���Ή��e�N�X�`���t�@�C����
	TCHAR t_filename[256] = {0};		// UNICODE/�}���`�o�C�g���Ή��e�N�X�`���t�@�C����
	if (strcpy_s(filename, str.c_str())) throw TEXT("�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���");
#ifdef UNICODE
	if (strlen(filename) > 0) MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, filename, strlen(filename), t_filename, (sizeof t_filename)/2);
#else
	if (strlen(filename) > 0) strcpy_s(t_filename, filename);
#endif
	tstring r = t_filename;
	if (r.empty() || directoryName.empty()) return r;
	r = directoryName + tstring(TEXT("\\")) + r;
	return r;
}


// private : �ޗ��ԍ����[
// @param pOrgMesh �����O�̃��b�V��
// @param material_numbers �ޗ��ԍ�[�|���S���ԍ�]
void MmdSkinMesh::CreateAttribute(LPD3DXMESH pOrgMesh, vector<unsigned long> material_numbers) {
	unsigned long* attributeBuffer;
	pOrgMesh->LockAttributeBuffer(0, &attributeBuffer);
	for (unsigned int i = 0; i < material_numbers.size(); ++i) attributeBuffer[i] = material_numbers[i];
	pOrgMesh->UnlockAttributeBuffer();
}


void MmdSkinMesh::CreateBoneMatrix(vector<MmdStruct::PmdBone> pmdBones) {
	unsigned int size = pmdBones.size();
	bones.resize(size);
	for (unsigned int i = 0; i < size; ++i) {
		int parentIdx(-1), firstChildIdx(-1), siblingIdx(-1);
		if (pmdBones[i].parent_bone_index != 0xFFFF) parentIdx = pmdBones[i].parent_bone_index;
		for (unsigned int j = i + 1; j < size; ++j) {
			if (pmdBones[i].parent_bone_index == pmdBones[j].parent_bone_index) {
				siblingIdx = j;
				break;
			}
		}
		for (unsigned int j = 0; j < size; ++j) {
			if (i == pmdBones[j].parent_bone_index) {
				firstChildIdx = j;
				break;
			}
		}
		D3DXMATRIX modelLocalInitMat;
		D3DXMatrixTranslation(&modelLocalInitMat, pmdBones[i].bone_head_pos[0], pmdBones[i].bone_head_pos[1], pmdBones[i].bone_head_pos[2]);
		char boneName[21] = {0};	// �{�[������20byte�̂Ƃ��̂��߂ɍŌ��0��ǉ�
		memcpy(boneName, pmdBones[i].bone_name, 20);
		bones[i].id = i;
		if (parentIdx != -1) bones[i].parent = &bones[parentIdx];
		if (firstChildIdx != -1) bones[i].firstChild = &bones[firstChildIdx];
		if (siblingIdx != -1) bones[i].sibling = &bones[siblingIdx];
		bones[i].name = boneName;
		bones[i].type = pmdBones[i].bone_type;
		bones[i].initMatML = bones[i].initMatBL = modelLocalInitMat;	// ���f�����[�J�����W�n
		D3DXMatrixInverse(&bones[i].offsetMatML, 0, &bones[i].initMatML);
	}
	class InitMatCalc {	// InitMat���{�[�����[�J�����W�n�ɕϊ�����ċA�֐����`�����N���X
	public:
		static void Run(Bone* me, D3DXMATRIX *parentoffsetMat) {
			if (me->firstChild) Run(me->firstChild, &me->offsetMatML);
			if (me->sibling)	Run(me->sibling, parentoffsetMat);
			if (parentoffsetMat) me->initMatBL *= *parentoffsetMat;
		}
	};
	InitMatCalc::Run(&bones[0], 0);	// �{�[�����[�J�����W�n�ɕϊ�
	for (unsigned int i = 0; i < size; ++i) bones[i].boneMatBL = bones[i].initMatBL;
}

