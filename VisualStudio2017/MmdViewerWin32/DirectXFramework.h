#pragma once

/// DirectX基本クラス
/// このクラスはシーンの開始・終了を行なう。 描画はシーン中に行う。
class DirectXFramework {
private:
	LPDIRECT3D9	pD3d;
	LPDIRECT3DDEVICE9 pDevice;					// Direct3Dデバイスオブジェクト

private: // 初期化補助関数
	void CreateDirect3DDeviceObject(HWND hWnd);	// DIRECT3Dデバイスオブジェクトの作成
	void SetRenderingConfiguration();			// レンダリング設定
	void SetFogConfiguration();					// フォグの設定

public:
	/// コンストラクタ
	DirectXFramework(HWND hWnd);

	/// デストラクタ
	~DirectXFramework();

	/// Direct3Dデバイスオブジェクトを取得
	LPDIRECT3DDEVICE9 GetD3DDevice();

	/// 画面を背景色でクリアし、シーンを開始する。描画関数はシーン中に呼び出す
	/// 引数：背景色のRGB成分。0～255
	void BeginScene(int red, int green, int blue);

	/// シーンを終了する
	void EndScene();
};
