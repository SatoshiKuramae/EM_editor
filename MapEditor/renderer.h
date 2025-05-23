//============================================================
//
//renderer.h
//
//============================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"d3dx9.h"
#include <Windows.h>
#include "grid.h"

//�����_���[�N���X
class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, BOOL bwindow);
	void Uninit();
	void Update();
	void Draw();
	LPDIRECT3DDEVICE9 GetDevice();	//3D�f�o�C�X�̎擾

private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	CGrid m_Grid;
};

#endif