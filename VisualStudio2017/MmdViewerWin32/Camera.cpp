#include "StdAfx.h"
#include "Camera.h"

///// ���b�V���p�J���� /////
Camera::Camera(D3DXVECTOR3 eyePoint, D3DXVECTOR3 lookAtPoint, float aspect) {
	const D3DXVECTOR3 upVec(0, 1, 0);									// ����x�N�g��
	const float viewAngle = D3DX_PI/4;									// ����p
	const float nearZ = 0.1f;											// �ŋߓ_ (0�ɂ���ƃ|���S�����߂����Ƀ`���c�L���N����)
	const float farZ = 2000.0f;											// �ŉ��_
	D3DXMatrixLookAtLH(&view, &eyePoint, &lookAtPoint, &upVec);
	D3DXMatrixPerspectiveFovLH(&projection, viewAngle, aspect, nearZ, farZ);
}

void Camera::GetMatrix(D3DXMATRIX* matView, D3DXMATRIX* matProj) {
	*matView = view;
	*matProj = projection;
}
