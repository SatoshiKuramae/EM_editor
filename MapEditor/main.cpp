//=======================
//
//ウィンドウ表示処理:main.cpp
//Author Kuramaesatoshi
//
//=======================

#include "main.h"
#include "manager.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include "imgui.h"
#include <algorithm> // ← std::transform のため
#include <string>    // ← std::string のため
#include <cctype>    // ← std::tolower などの文字変換用
#include <shlwapi.h>  // PathFindFileNameA の宣言
#pragma comment(lib, "shlwapi.lib") // 必要なライブラリ

//PathFindFileNameAを行うために必要
const char* fullPath = "C:\\Users\\YourName\\Desktop\\model.x";
const char* fileName = PathFindFileNameA(fullPath);
CManager* g_pManager = nullptr;



//===============
//メイン関数		
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

	//	ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを生成
	hWnd = CreateWindowEx(0,						//拡張ウィンドウスタイル
		CLASS_NAME,				//ウィンドウクラスの名前
		WINDOW_NAME,			//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル
		CW_USEDEFAULT,			//ウィンドウの左上X座標
		CW_USEDEFAULT,			//ウィンドウの左上Y座標
		(rect.right - rect.left),			//ウィンドウの幅
		(rect.bottom - rect.top),			//ウィンドウの高さ
		NULL,					//親ウィンドウのハンドル
		NULL,					//メニューハンドルまたは子ウィンドウID
		hInstance,				//インスタントハンドル
		NULL);					//ウィンドウ作成データ



	//全オブジェクト生成
	g_pManager = new CManager();
	g_pManager->Init(hInstance,hWnd,TRUE);


	if (g_pManager == nullptr)
	{
		return-1;
	}
	//分解能を設定
	{
		timeBeginPeriod(1);
		dwCurrentTime = 0;
		dwExecLastTime = timeGetTime();
	}
	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	DragAcceptFiles(hWnd, TRUE); 

	ZeroMemory(&msg, sizeof(msg));

	//メインループ
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージグループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒の経過
				dwExecLastTime = dwCurrentTime;
				//諸々の処理
				// フレーム開始
				g_pManager->Update();
				g_pManager->Draw();

			}
		}
	}

	//終了処理
	g_pManager->Uninit();

	

	delete g_pManager;


	//分解能を戻す
	timeEndPeriod(1);
	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//ウィンドウプロシージャ
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
		//WM_QUITにメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:

			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウィンドウを閉じる
				DestroyWindow(hWnd);
			}
			break;
		}
		break;


	case WM_CLOSE:				//閉じるボタンを押した時
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			//ウィンドウを破棄
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;

	case WM_DROPFILES:
	
		//MessageBox(hWnd, "ファイルがドロップされました", "メッセージ", MB_OK);

		/*HDROP hDrop = (HDROP)wParam;
		UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);*/

		//for (UINT i = 0; i < fileCount; ++i)
		//{
		//	TCHAR filePath[MAX_PATH];
		//	DragQueryFile(hDrop, i, filePath, MAX_PATH);

		//	// 拡張子が .x か確認（小文字前提、必要なら大文字にも対応可）
		//	std::string pathStr = filePath;
		//	size_t dotPos = pathStr.find_last_of('.');
		//	if (dotPos == std::string::npos) continue;

		//	std::string ext = pathStr.substr(dotPos + 1);
		//	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower); // 小文字に変換

		//	if (ext != "x") continue; // .x 以外は無視

		//	// ファイル名取得（例: enemy.x）
		//	std::string fileName = PathFindFileNameA(filePath);

		//	// 保存先フォルダの絶対パス
		//	std::string destFolder = "Assets/Models/";
		//	std::string destPath = destFolder + fileName;

		//	// 既に存在するならスキップ
		//	if (PathFileExistsA(destPath.c_str())) {
		//		MessageBoxA(hWnd, ("ファイルは既に存在しています: " + fileName).c_str(), "警告", MB_OK);
		//		continue;
		//	}

		//	// コピー処理
		//	if (CopyFileA(filePath, destPath.c_str(), TRUE)) {
		//		MessageBoxA(hWnd, ("コピー成功: " + fileName).c_str(), "成功", MB_OK);
		//	}
		//	else {
		//		MessageBoxA(hWnd, ("コピー失敗: " + fileName).c_str(), "エラー", MB_OK);
		//	}
		//}

		//DragFinish(hDrop);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);    // 規定の処理を返す


}


