#pragma once
#include "Bezie.h"

///// スキンメッシュアニメーション用構造体 /////

///// 名称定義 ・・・ 本ヘッダ中では以下のようにボーンの座標系の名称を定義する	/////
///// ワールド座標系： 絶対座標系												/////
///// モデルローカル座標系：ワールド座標変換を含まない3Dモデルのローカル座標系	/////
///// ボーンローカル座標系：自身の親ボーンを基準にしたローカル座標系			/////

// ブレンド重み付メッシュの頂点データ
struct BlendVertex {
	D3DXVECTOR3 position;
	float weight[3];
	unsigned char matrixIndex[4];
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texture;
};

// ボーン
struct Bone {
public:
	bool extraBoneControl;	// 階層構造を使わず直接boneMatMLを操作
	int id;					// ボーンID（通し番号）
	string name;			// ボーン名
	unsigned char type;		// ボーンタイプ (MMDの場合 0:回転 1:回転と移動 2:IK 3:不明 4:IK影響下 5:回転影響下 6:IK接続先 7:非表示 8:捻り 9:回転運動 )
	Bone* parent;			// 親ボーン
	Bone* firstChild;		// 第1子ボーン
	Bone* sibling;			// 次の兄弟ボーン
	D3DXMATRIX initMatBL;	// 初期姿勢行列(ボーンローカル座標系)
	D3DXMATRIX initMatML;	// 初期姿勢行列(モデルローカル座標系)
	D3DXMATRIX boneMatBL;	// ボーン行列(ボーンローカル座標系)
	D3DXMATRIX boneMatML;	// ボーン行列(モデルローカル座標系)
	D3DXMATRIX offsetMatML;	// ボーンオフセット行列(モデルローカル座標系)
	Bone() : extraBoneControl(), type(), id(), parent(), firstChild(), sibling() {	// コンストラクタ
		D3DXMatrixIdentity(&initMatBL);
		D3DXMatrixIdentity(&initMatML);
		D3DXMatrixIdentity(&offsetMatML);
		D3DXMatrixIdentity(&boneMatBL);
		D3DXMatrixIdentity(&boneMatML);
	}
	D3DXMATRIX CalBoneMatML() {	// モデルローカル座標系でのボーン行列を計算
		if (parent) return boneMatBL*parent->CalBoneMatML();
		else return boneMatBL;
	}
};

// キーフレームアニメーションデータ
struct KeyFrame {
	string boneName;		// ボーン名
	unsigned long frameNo;	// フレーム番号
	D3DXVECTOR3 position;	// 位置
	D3DXQUATERNION rotation;// 回転
	Bezie* bezie_x;
	Bezie* bezie_y;
	Bezie* bezie_z;
	Bezie* bezie_r;
	bool operator < (const KeyFrame &k) const {	// フレーム番号でソートするための比較演算子
		return frameNo < k.frameNo;
	}
};