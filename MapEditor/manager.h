//============================================================
//
//manager.h
//
//============================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include"d3dx9.h"
#include <Windows.h>
#include "renderer.h"
#include "input.h"

class CCamera;
class CLight;

//レンダラークラス
class CManager
{
public:
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();

	static CRenderer* GetRenderer();
	static CInputKeyboard* GetKeyboard();
	static CCamera* GetCamera();
	static CLight* GetLight();
private:
	static CRenderer* m_pRenderer;	//レンダラー
	static CInputKeyboard* m_pKeyboard;
	static CCamera* m_pCamera;	//ビュー情報
	static CLight* m_pLight;	//ライト情報
};

#endif
