#include "StdAfx.h"
#include "Camera.h"

///// メッシュ用カメラ /////
Camera::Camera(D3DXVECTOR3 eyePoint, D3DXVECTOR3 lookAtPoint, float aspect) {
	const D3DXVECTOR3 upVec(0, 1, 0);									// 上方ベクトル
	const float viewAngle = D3DX_PI/4;									// 視野角
	const float nearZ = 0.1f;											// 最近点 (0にするとポリゴンが近い時にチラツキが起こる)
	const float farZ = 2000.0f;											// 最遠点
	D3DXMatrixLookAtLH(&view, &eyePoint, &lookAtPoint, &upVec);
	D3DXMatrixPerspectiveFovLH(&projection, viewAngle, aspect, nearZ, farZ);
}

void Camera::GetMatrix(D3DXMATRIX* matView, D3DXMATRIX* matProj) {
	*matView = view;
	*matProj = projection;
}
