#pragma once

/// 0�`1�ɋK�i�����ꂽ�x�W�F�Ȑ�
class Bezie {
private:
	D3DXVECTOR2 p1, p2; /// ����_
public:
	Bezie(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
	float GetY(float x);	/// x�ɂ�����y���擾
};
