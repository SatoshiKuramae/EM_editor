//============================================================
//
//GUImanager.h
//
//============================================================
#ifndef _GUIMANAGER_H_
#define _GUIMANAGER_H_
#include <windows.h>
#include <d3d9.h>
#include <vector>
#include "gameobject.h"
#include "main.h"
class GUIManager
{
public:

	GUIManager();
	~GUIManager();

	//初期化、シャットダウン
	bool Initialize(HWND hwnd, IDirect3DDevice9* device);
	void Shutdown();

	//毎フレーム更新するもの
	void BeginFrame();
	void Update();        // ここに描画処理をまとめる
	void EndFrame(IDirect3DDevice9* device);
	//void SetObjectList(std::vector<GameObject*>& objectList);

	// WndProc処理
	bool WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// 初期化チェック用
	bool IsInitialized() const { return m_Initialized; }

private:
	bool m_Initialized;

	std::vector<GameObject*> m_gameObjects;
	int m_selectedIndex = -1;
};

#endif
