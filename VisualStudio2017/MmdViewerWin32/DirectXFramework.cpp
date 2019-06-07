#include "stdafx.h"
#include "DirectXFramework.h"

DirectXFramework::DirectXFramework(HWND hWnd){
	if (!hWnd) throw TEXT("Direct3Dの作成に失敗しました\nウィンドウハンドルがありません");		// ウィンドウハンドル確認
	if (!(pD3d = Direct3DCreate9( D3D_SDK_VERSION))) throw TEXT("Direct3Dの作成に失敗しました");	// Direct3Dオブジェクトの作成
	D3DCAPS9 Caps;
	pD3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps);	// ダブルステンシルのサポートを確認
	if (!(Caps.StencilCaps & D3DSTENCILCAPS_TWOSIDED)) MessageBoxW(0,TEXT("ダブルステンシルに対応していません"),TEXT(""),MB_OK);

	// test
	//int s0 = Caps.MaxVertexBlendMatrixIndex;	// = 0 → うちのグラボは頂点ブレンド行列インデックスをサポートしていない！！
	//int s1 = Caps.MaxVertexBlendMatrices;	// = 4 → うちのグラボは1ポリゴンあたり4ブレンド行列数以上をサポートしていない！！
	//										// 頂点ブレンド行列インデックスを使わなくても最大3頂点×4ブレンド = 12ブレンド行列の可能性があるので注意！

	CreateDirect3DDeviceObject(hWnd);	// DIRECT3Dデバイスオブジェクトの作成
	SetRenderingConfiguration();	// レンダリング基本設定
	SetFogConfiguration();			// フォグの設定
}

void DirectXFramework::CreateDirect3DDeviceObject(HWND hWnd){
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );   // 暗黙に D3DPRESENT_INTERVAL_DEFAULT = 0 が設定→モニタのリフレッシュレート = FPS
	d3dpp.BackBufferFormat =D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount=1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	
	d3dpp.Windowed = TRUE;
    d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;		//Zバッファ24ビット、ステンシルバッファ8ビット なのでOK
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;	//ダブルステンシル
	if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice))){
		if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice))){
			MessageBox(0,TEXT("HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します"),NULL,MB_OK);
			if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice))){
				if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice))){
					throw TEXT("DIRECT3Dデバイスの作成に失敗しました");
				}
			}
		}
	} 
}

void DirectXFramework::SetRenderingConfiguration(){
	pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);					// Zバッファー処理を有効にする
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// カリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);					// ライトを有効にする
	pDevice->SetRenderState(D3DRS_AMBIENT, 0);						// アンビエントライト（環境光）を設定する
	pDevice->SetRenderState(D3DRS_SPECULARENABLE,TRUE);				// スペキュラ（鏡面反射）を有効にする
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// アルファブレンディング有効
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
		if(MessageBox(0, TEXT("ピクセルフォグが使えない可能性があります。続行しますか?"), TEXT("質問"), MB_ABORTRETRYIGNORE) == IDABORT) throw TEXT("フォグ設定に失敗しました");
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
	if (!SUCCEEDED(pDevice->BeginScene())) throw TEXT("BeginSceneに失敗しました");
}

void DirectXFramework::EndScene() {
	pDevice->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);
}

byte* DirectXFramework::ReviseBitmap() {
	// バックバファの取得
	LPDIRECT3DSURFACE9 pBackBuf;
	pDevice->GetRenderTarget(0, &pBackBuf);

	// スクショ出力
	D3DXSaveSurfaceToFileInMemory(&pBitmapBuf, D3DXIFF_BMP, pBackBuf, NULL, NULL);	// ヘッダも含めてビットマップファイルそのものがメモリに保存される

	// Get系で取得したサーフェイスはAddRefが呼ばれているので忘れずに解放する
	pBackBuf->Release();

	return (byte*)pBitmapBuf->GetBufferPointer();	// アドレスは変わるのでポインタ更新が必要
}

int DirectXFramework::CreateBitmapBuffer(int width, int height) {
	const int headersize = 54;
	numBytes = width * height * 4 + headersize;
	D3DXCreateBuffer(numBytes, &pBitmapBuf);
	return numBytes;
}
