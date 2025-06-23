//============================================================
//
//renderer.h
//
// Auther : SatoshiKuramae
//============================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"d3dx9.h"
#include <Windows.h>
#include "grid.h"

//レンダラークラス
class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, BOOL bwindow);
	void Uninit();
	void Update();
	void Draw();
	LPDIRECT3DDEVICE9 GetDevice();	//3Dデバイスの取得

private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	CGrid m_Grid;
};

#endif
