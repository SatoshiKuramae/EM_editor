//=======================
//
//�E�B���h�E�\������:main.cpp
//Author Kuramaesatoshi
//
//=======================

#include "main.h"
#include "manager.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include "imgui.h"
#include <algorithm> // �� std::transform �̂���
#include <string>    // �� std::string �̂���
#include <cctype>    // �� std::tolower �Ȃǂ̕����ϊ��p
#include <shlwapi.h>  // PathFindFileNameA �̐錾
#pragma comment(lib, "shlwapi.lib") // �K�v�ȃ��C�u����

//PathFindFileNameA���s�����߂ɕK�v
const char* fullPath = "C:\\Users\\YourName\\Desktop\\model.x";
const char* fileName = PathFindFileNameA(fullPath);
CManager* g_pManager = nullptr;



//===============
//���C���֐�		
//===============
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{

	DWORD dwCurrentTime;
	DWORD dwExecLastTime;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL,IDI_APPLICATION)
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//	�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�B���h�E�𐶐�
	hWnd = CreateWindowEx(0,						//�g���E�B���h�E�X�^�C��
		CLASS_NAME,				//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,			//�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,			//�E�B���h�E�̍���X���W
		CW_USEDEFAULT,			//�E�B���h�E�̍���Y���W
		(rect.right - rect.left),			//�E�B���h�E�̕�
		(rect.bottom - rect.top),			//�E�B���h�E�̍���
		NULL,					//�e�E�B���h�E�̃n���h��
		NULL,					//���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,				//�C���X�^���g�n���h��
		NULL);					//�E�B���h�E�쐬�f�[�^



	//�S�I�u�W�F�N�g����
	g_pManager = new CManager();
	g_pManager->Init(hInstance,hWnd,TRUE);


	if (g_pManager == nullptr)
	{
		return-1;
	}
	//����\��ݒ�
	{
		timeBeginPeriod(1);
		dwCurrentTime = 0;
		dwExecLastTime = timeGetTime();
	}
	//�E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	DragAcceptFiles(hWnd, TRUE); 

	ZeroMemory(&msg, sizeof(msg));

	//���C�����[�v
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W�O���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�̌o��
				dwExecLastTime = dwCurrentTime;
				//���X�̏���
				// �t���[���J�n
				g_pManager->Update();
				g_pManager->Draw();

			}
		}
	}

	//�I������
	g_pManager->Uninit();

	

	delete g_pManager;


	//����\��߂�
	timeEndPeriod(1);
	//�E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static POINT pos = { 100,100 };
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	int nID;
	static HWND hWndEditOutput;



	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	switch (uMsg)
	{
	case WM_DESTROY:
		//WM_QUIT�Ƀ��b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:

			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				//�E�B���h�E�����
				DestroyWindow(hWnd);
			}
			break;
		}
		break;


	case WM_CLOSE:				//����{�^������������
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
		if (nID == IDYES)
		{
			//�E�B���h�E��j��
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;

	case WM_DROPFILES:
	
		//MessageBox(hWnd, "�t�@�C�����h���b�v����܂���", "���b�Z�[�W", MB_OK);

		/*HDROP hDrop = (HDROP)wParam;
		UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);*/

		//for (UINT i = 0; i < fileCount; ++i)
		//{
		//	TCHAR filePath[MAX_PATH];
		//	DragQueryFile(hDrop, i, filePath, MAX_PATH);

		//	// �g���q�� .x ���m�F�i�������O��A�K�v�Ȃ�啶���ɂ��Ή��j
		//	std::string pathStr = filePath;
		//	size_t dotPos = pathStr.find_last_of('.');
		//	if (dotPos == std::string::npos) continue;

		//	std::string ext = pathStr.substr(dotPos + 1);
		//	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower); // �������ɕϊ�

		//	if (ext != "x") continue; // .x �ȊO�͖���

		//	// �t�@�C�����擾�i��: enemy.x�j
		//	std::string fileName = PathFindFileNameA(filePath);

		//	// �ۑ���t�H���_�̐�΃p�X
		//	std::string destFolder = "Assets/Models/";
		//	std::string destPath = destFolder + fileName;

		//	// ���ɑ��݂���Ȃ�X�L�b�v
		//	if (PathFileExistsA(destPath.c_str())) {
		//		MessageBoxA(hWnd, ("�t�@�C���͊��ɑ��݂��Ă��܂�: " + fileName).c_str(), "�x��", MB_OK);
		//		continue;
		//	}

		//	// �R�s�[����
		//	if (CopyFileA(filePath, destPath.c_str(), TRUE)) {
		//		MessageBoxA(hWnd, ("�R�s�[����: " + fileName).c_str(), "����", MB_OK);
		//	}
		//	else {
		//		MessageBoxA(hWnd, ("�R�s�[���s: " + fileName).c_str(), "�G���[", MB_OK);
		//	}
		//}

		//DragFinish(hDrop);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);    // �K��̏�����Ԃ�


}


