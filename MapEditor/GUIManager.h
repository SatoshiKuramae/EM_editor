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

	//�������A�V���b�g�_�E��
	bool Initialize(HWND hwnd, IDirect3DDevice9* device);
	void Shutdown();

	//���t���[���X�V�������
	void BeginFrame();
	void Update();        // �����ɕ`�揈�����܂Ƃ߂�
	void EndFrame(IDirect3DDevice9* device);
	//void SetObjectList(std::vector<GameObject*>& objectList);

	// WndProc����
	bool WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// �������`�F�b�N�p
	bool IsInitialized() const { return m_Initialized; }

private:
	bool m_Initialized;

	std::vector<GameObject*> m_gameObjects;
	int m_selectedIndex = -1;
};

#endif
