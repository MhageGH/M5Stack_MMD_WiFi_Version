#pragma once

/// 0`1‚É‹KŠi‰»‚³‚ê‚½ƒxƒWƒF‹Èü
class Bezie {
private:
	D3DXVECTOR2 p1, p2; /// §Œä“_
public:
	Bezie(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
	float GetY(float x);	/// x‚É‚¨‚¯‚éy‚ğæ“¾
};
