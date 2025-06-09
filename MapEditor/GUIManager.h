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

//GUIマネージャークラス
class GUIManager
{
public:

	//コンストラクタ
	GUIManager();
	//デストラクタ
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

	void SetSelectedObject(CObjectX* obj);
private:
	int patternIndex;
	const int maxPattern = NUM_PATTERN; // パターン数（必要に応じて増やせる）
	bool m_Initialized;
	bool showSaveConfirm = false;
	bool confirmedSave = false;
	//配置するゲームオブジェクトモデルのパス
	std::string gameobjectpath = ("Data\\gameobject\\");
	//指定フォルダ内のファイル名を取得する
	std::vector<std::string> modelFiles = GetXFileNamesInDirectory(gameobjectpath, "x");
	
	nlohmann::json jsonOutput;
	int selected = 0;
	std::string selectedModelPath; // 選ばれたモデルのパス
	std::vector<GameObject*> m_gameObjects;
	int m_selectedIndex = -1;
	CObjectX* selectedObject = nullptr;	
	ArrowObject* m_arrowObject = nullptr;	//矢印オブジェクトのポインタ
};

#endif
