#include "StdAfx.h"
#include "Bezie.h"

Bezie::Bezie(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
	p1.x = x1/127.0f;
	p1.y = y1/127.0f;
	p2.x = x2/127.0f;
	p2.y = y2/127.0f;
}

/// X = (1 - t)^3*p0_x + 3*(1 - t)^2*t*p1_x + 3*(1 - t)*t^2*p2_x + t^3*p3_x
/// Y = (1 - t)^3*p0_y + 3*(1 - t)^2*t*p1_y + 3*(1 - t)*t^2*p2_y + t^3*p3_y
/// 0 �� t �� 1
/// (X, Y) = (0, 0), (X, Y) = (1, 1)��ʂ�K�i�����ꂽ�x�W�F�Ȑ����l���� �� p0_x = p0_y = p3_x = p3_y = 0
/// t = x ����t��ω������Ax = X�ƂȂ�Ƃ���Y�����߂�
float Bezie::GetY(float x) {
	float t = x;
	float max_t = 1.0f;
	float min_t = 0.0f;
	float val1, val2, val3;
	static const int N = 8;			// �v�Z�J��Ԃ���
	for( int i = 0; i < N; i++ ) {
		float it = 1.0f - t;
		val1 = 3.0f * t * it * it;
		val2 = 3.0f * t * t * it;
		val3 = t * t * t;
		float x_diff = x - (val1 * p1.x) - (val2 * p2.x) - val3;
		if( fabs(x_diff) < 1e-6 ) break;	// �덷���萔�ȓ��Ȃ�I��
		if( x_diff > 0 ) {					// �͈͂�ύX���čČv�Z
			min_t = t;
			t = (( max_t - t) / 2.0f ) + t;
		}
		else {
			max_t = t;
			t = (( t - min_t) / 2.0f ) + min_t;
		}
	}
	return ((val1 * p1.y) + (val2 * p2.y) + val3);
}