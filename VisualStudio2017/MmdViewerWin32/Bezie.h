#pragma once

/// 0〜1に規格化されたベジェ曲線
class Bezie {
private:
	D3DXVECTOR2 p1, p2; /// 制御点
public:
	Bezie(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
	float GetY(float x);	/// xにおけるyを取得
};
