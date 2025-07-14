//============================================================
//
//GUImanager.h
//
// Auther : SatoshiKuramae
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
constexpr int PATTERNFRAME = 60;
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


	// WndProc処理
	bool WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// 初期化チェック用
	bool IsInitialized() const { return m_Initialized; }

	void SetSelectedObject(CObjectX* obj);

	void AdjustObjectZPos(GameObject* obj, float offset);
	void AdjustObjectZMove(GameObject* obj, float offset);

	//パス名にholeが含まれているとき穴あきオブジェクトと判定する
	bool IsHoleObject(const std::string& path);
	//各UIごとの処理関数
	void SetObjParam();

	//出力されるデータの端数を丸める処理
	float RoundFloat(float value, int digits = 2) {
		float factor = std::pow(10.0f, digits);
		return std::round(value * factor) / factor;
	}
	//出力されるデータの端数を丸める処理
	D3DXVECTOR3 RoundVec(D3DXVECTOR3 v, int digits = 2) {
		return D3DXVECTOR3(
			RoundFloat(v.x, digits),
			RoundFloat(v.y, digits),
			RoundFloat(v.z, digits)
		);
	}
	double RoundedAsDouble(float value, int digits = 2) {
		double factor = std::pow(10.0, digits);
		return std::round(static_cast<double>(value) * factor) / factor;
	}

private:
	int m_currentLevel = 1;

	static const char* tagOptions[];
	int AnticipationFrame = PATTERNFRAME;		//初期値決めてね
	int currentTagIndex = 0;  // 0: None（初期）
	std::string m_stageTag = "None";  // 実際に保存されるタグ

	int patternIndex;					//今選択しているパターンインデックス
	const int maxPattern = NUM_PATTERN; // パターン数（必要に応じて増やせる）
	bool m_Initialized;
	bool showSaveConfirm = false;
	bool confirmedSave = false;
	float zAxisOffsetPos = 0.0f;
	float zAxisOffsetMove = 0.0f;

	//配置するゲームオブジェクトモデルのパス
	std::string gameobjectpath = ("Data\\Model\\");
	std::string jsonfilepath = ("Data\\JSON\\");
	std::string m_loadedFileName;
	char fileInputBuffer[128] = "";  // 直接入力用バッファ（GUIManagerのメンバーでOK）
	int selectedJsonIndex = -1;      // リスト上で選ばれているファイル
	//指定フォルダ内のファイル名を取得する
	std::vector<std::string> modelFiles = GetXFileNamesInDirectory(gameobjectpath, "x");
	std::vector<std::string> jsonFiles = GetJsonFileList(jsonfilepath);
	nlohmann::json jsonOutput;
	int selected = 0;
	std::string selectedModelPath; // 選ばれたモデルのパス
	std::string selectedJsonPath; // 選ばれたJsonのパス
	std::vector<GameObject*> m_gameObjects;
	int m_selectedIndex = -1;
	CObjectX* selectedObject = nullptr;	
	ArrowObject* m_arrowObject = nullptr;	//矢印オブジェクトのポインタ
	ArrowObject* m_arrowObject_offset = nullptr;	//穴のオフセットに配置する矢印オブジェクト
	HoleMarkerObject* m_holemarker = nullptr;		//穴の位置を可視化するオブジェクト
};

#endif
