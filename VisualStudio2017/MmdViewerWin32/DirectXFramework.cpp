#include "stdafx.h"
#include "DirectXFramework.h"

DirectXFramework::DirectXFramework(HWND hWnd){
	if (!hWnd) throw TEXT("Direct3D�̍쐬�Ɏ��s���܂���\n�E�B���h�E�n���h��������܂���");		// �E�B���h�E�n���h���m�F
	if (!(pD3d = Direct3DCreate9( D3D_SDK_VERSION))) throw TEXT("Direct3D�̍쐬�Ɏ��s���܂���");	// Direct3D�I�u�W�F�N�g�̍쐬
	D3DCAPS9 Caps;
	pD3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps);	// �_�u���X�e���V���̃T�|�[�g���m�F
	if (!(Caps.StencilCaps & D3DSTENCILCAPS_TWOSIDED)) MessageBoxW(0,TEXT("�_�u���X�e���V���ɑΉ����Ă��܂���"),TEXT(""),MB_OK);

	// test
	//int s0 = Caps.MaxVertexBlendMatrixIndex;	// = 0 �� �����̃O���{�͒��_�u�����h�s��C���f�b�N�X���T�|�[�g���Ă��Ȃ��I�I
	//int s1 = Caps.MaxVertexBlendMatrices;	// = 4 �� �����̃O���{��1�|���S��������4�u�����h�s�񐔈ȏ���T�|�[�g���Ă��Ȃ��I�I
	//										// ���_�u�����h�s��C���f�b�N�X���g��Ȃ��Ă��ő�3���_�~4�u�����h = 12�u�����h�s��̉\��������̂Œ��ӁI

	CreateDirect3DDeviceObject(hWnd);	// DIRECT3D�f�o�C�X�I�u�W�F�N�g�̍쐬
	SetRenderingConfiguration();	// �����_�����O��{�ݒ�
	SetFogConfiguration();			// �t�H�O�̐ݒ�
}

void DirectXFramework::CreateDirect3DDeviceObject(HWND hWnd){
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );   // �Öق� D3DPRESENT_INTERVAL_DEFAULT = 0 ���ݒ聨���j�^�̃��t���b�V�����[�g = FPS
	d3dpp.BackBufferFormat =D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount=1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	
	d3dpp.Windowed = TRUE;
    d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;		//Z�o�b�t�@24�r�b�g�A�X�e���V���o�b�t�@8�r�b�g �Ȃ̂�OK
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;	//�_�u���X�e���V��
	if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice))){
		if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice))){
			MessageBox(0,TEXT("HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�"),NULL,MB_OK);
			if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice))){
				if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice))){
					throw TEXT("DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���");
				}
			}
		}
	} 
}

void DirectXFramework::SetRenderingConfiguration(){
	pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);					// Z�o�b�t�@�[������L���ɂ���
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// �J�����O����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);					// ���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_AMBIENT, 0);						// �A���r�G���g���C�g�i�����j��ݒ肷��
	pDevice->SetRenderState(D3DRS_SPECULARENABLE,TRUE);				// �X�y�L�����i���ʔ��ˁj��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// �A���t�@�u�����f�B���O�L��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void DirectXFramework::SetFogConfiguration(){
	const FLOAT fogStart = 300;
	const FLOAT fogEnd = 2000;
	D3DCAPS9 caps;
	ZeroMemory(&caps,sizeof(D3DCAPS9));
	pDevice->GetDeviceCaps(&caps);	
	if ((caps.RasterCaps & D3DPRASTERCAPS_FOGTABLE) == 0)
		if(MessageBox(0, TEXT("�s�N�Z���t�H�O���g���Ȃ��\��������܂��B���s���܂���?"), TEXT("����"), MB_ABORTRETRYIGNORE) == IDABORT) throw TEXT("�t�H�O�ݒ�Ɏ��s���܂���");
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);	
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(150,255,255,255)); 		
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE );	
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR );		 
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fogStart));		
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&fogEnd));
}

DirectXFramework::~DirectXFramework(void) {
	SAFE_RELEASE(pBitmapBuf);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pD3d);
}

LPDIRECT3DDEVICE9 DirectXFramework::GetD3DDevice() {
	return pDevice;
}

void DirectXFramework::BeginScene(int r, int g, int b) {
	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(r, g, b), 1.0f, 0 );
	if (!SUCCEEDED(pDevice->BeginScene())) throw TEXT("BeginScene�Ɏ��s���܂���");
}

void DirectXFramework::EndScene() {
	pDevice->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);
}

byte* DirectXFramework::ReviseBitmap() {
	// �o�b�N�o�t�@�̎擾
	LPDIRECT3DSURFACE9 pBackBuf;
	pDevice->GetRenderTarget(0, &pBackBuf);

	// �X�N�V���o��
	D3DXSaveSurfaceToFileInMemory(&pBitmapBuf, D3DXIFF_BMP, pBackBuf, NULL, NULL);	// �w�b�_���܂߂ăr�b�g�}�b�v�t�@�C�����̂��̂��������ɕۑ������

	// Get�n�Ŏ擾�����T�[�t�F�C�X��AddRef���Ă΂�Ă���̂ŖY�ꂸ�ɉ������
	pBackBuf->Release();

	return (byte*)pBitmapBuf->GetBufferPointer();	// �A�h���X�͕ς��̂Ń|�C���^�X�V���K�v
}

int DirectXFramework::CreateBitmapBuffer(int width, int height) {
	const int headersize = 54;
	numBytes = width * height * 4 + headersize;
	D3DXCreateBuffer(numBytes, &pBitmapBuf);
	return numBytes;
}
