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
#include "objectX.h"
#include "manager.h"
#include "pch.h"
#include "FileUtil.h"

constexpr double POS_X_MAX = 260.0f;
constexpr double POS_Y_MAX = 135.0f;
constexpr int NUM_PATTERN = 50;

//GUI�}�l�[�W���[�N���X
class GUIManager
{
public:

	//�R���X�g���N�^
	GUIManager();
	//�f�X�g���N�^
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

	void SetSelectedObject(CObjectX* obj);
private:
	int patternIndex;
	const int maxPattern = NUM_PATTERN; // �p�^�[�����i�K�v�ɉ����đ��₹��j
	bool m_Initialized;
	bool showSaveConfirm = false;
	bool confirmedSave = false;
	//�z�u����Q�[���I�u�W�F�N�g���f���̃p�X
	std::string gameobjectpath = ("Data\\gameobject\\");
	//�w��t�H���_���̃t�@�C�������擾����
	std::vector<std::string> modelFiles = GetXFileNamesInDirectory(gameobjectpath, "x");
	
	nlohmann::json jsonOutput;
	int selected = 0;
	std::string selectedModelPath; // �I�΂ꂽ���f���̃p�X
	std::vector<GameObject*> m_gameObjects;
	int m_selectedIndex = -1;
	CObjectX* selectedObject = nullptr;	
	ArrowObject* m_arrowObject = nullptr;	//���I�u�W�F�N�g�̃|�C���^
};

#endif
