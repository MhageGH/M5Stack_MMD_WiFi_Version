#include "StdAfx.h"
#include "Coord.h"

Coord::Coord(LPDIRECT3DDEVICE9 pDev) : pDevice(pDev), numAxis(3), numNet(21) {
	/// ñ‘ê¸
	const float pitch = 5.0f;
	const float length = pitch*(numNet - 1);
	const unsigned long color = D3DCOLOR_ARGB(255, 255, 255, 255);
	net = new ColorPoint[numNet*4];
	for (int i = 0; i < numNet; ++i) {
		net[4*i].position = D3DXVECTOR3(-length/2 + i*pitch, 0, -length/2);
		net[4*i].color = color;
		net[4*i + 1].position = D3DXVECTOR3(-length/2 + i*pitch, 0, length/2);
		net[4*i + 1].color = color;
		net[4*i + 2].position = D3DXVECTOR3(-length/2, 0, -length/2 + i*pitch);
		net[4*i + 2].color = color;
		net[4*i + 3].position = D3DXVECTOR3(length/2, 0, -length/2 + i*pitch);
		net[4*i + 3].color = color;
	}
	// XYZé≤
	axis = new ColorPoint[numAxis*2];
	const float e = 0.01f;	// ñ‘ê¸Ç∆èdÇ»ÇÁÇ»Ç¢ÇÊÇ§Ç…ÇŸÇÒÇÃè≠ÇµÇ∏ÇÁÇ∑
	axis[0].position = D3DXVECTOR3(0, e, 0);		axis[0].color = D3DCOLOR_ARGB(255, 255, 0, 0);
	axis[1].position = D3DXVECTOR3(length, e, 0);	axis[1].color = D3DCOLOR_ARGB(255, 255, 0, 0);
	axis[2].position = D3DXVECTOR3(0, 0, 0);		axis[2].color = D3DCOLOR_ARGB(255, 0, 255, 0);
	axis[3].position = D3DXVECTOR3(0, length, 0);	axis[3].color = D3DCOLOR_ARGB(255, 0, 255, 0);
	axis[4].position = D3DXVECTOR3(0, e, 0);		axis[4].color = D3DCOLOR_ARGB(255, 0, 0, 255);
	axis[5].position = D3DXVECTOR3(0, e, -length);	axis[5].color = D3DCOLOR_ARGB(255, 0, 0, 255);
}

Coord::~Coord(void){
	delete[] net;
	delete[] axis;
}

void Coord::Draw(Camera* camera) {
	D3DXMATRIX world, view, proj;
	D3DXMatrixIdentity(&world);
	camera->GetMatrix(&view, &proj);
    pDevice->SetTransform(D3DTS_WORLD, &world);
	pDevice->SetTransform(D3DTS_VIEW, &view);
	pDevice->SetTransform(D3DTS_PROJECTION, &proj);
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetTexture(0, 0);
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 2*numNet, net, sizeof(ColorPoint));
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, numAxis, axis, sizeof(ColorPoint));
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}