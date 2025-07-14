//==============================================================
//
// GUImanager.cpp
//
// Auther : SatoshiKuramae
//==============================================================
#include "GUImanager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include "object.h"
#include "gameobject.h"

using ordered_json = nlohmann::ordered_json;//追加順を保持する時に使う
using json = nlohmann::json; // 省略しないなら nlohmann::json を毎回使ってもOK
const char* GUIManager::tagOptions[] = { "Nomal", "Leaf", "Smork" };  // 定義

//コンストラクタ
GUIManager::GUIManager()
    : m_Initialized(false)
{
}

//デストラクタ
GUIManager::~GUIManager()
{
    Shutdown();
}

//初期化処理
bool GUIManager::Initialize(HWND hwnd, IDirect3DDevice9* device)
{
    if (m_Initialized)
        return true;
    patternIndex = 1;
    // ImGuiコンテキストの作成
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();


    ImFontConfig config;
    config.DstFont = 0; // 多くの場合は0でOK
    config.FontDataOwnedByAtlas = true;

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\meiryo.ttc", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese());

    // スタイル設定
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // 白いフォント

    // 初期化
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(device);


    m_Initialized = true;

	//矢印オブジェクト生成(m_arrowObjectとm_arrowObject_offsetの初期化と生成を一括で行う)
	//ArrowObject* m_arrowObject;
	//m_arrowObject->Create();  // これを呼ぶ前に new してない → クラッシュ

	m_arrowObject = ArrowObject::Create(false);
	m_arrowObject_offset = ArrowObject::Create(true);
    return true;
}

//終了処理
void GUIManager::Shutdown()
{
    if (!m_Initialized)
        return;
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    m_Initialized = false;
}

//描画開始
void GUIManager::BeginFrame()
{
    if (!m_Initialized)
        return;

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}


//更新処理
void GUIManager::Update() 
{
	static std::string warningMessage = "";

	if (patternIndex < 1)
	{
		patternIndex = maxPattern;
	}
	else if (patternIndex > maxPattern)
	{
		patternIndex = 1;
	}

    if (!m_Initialized)
        return;

    // ====== ここに描画処理を書く ======
    ImGui::Begin(u8"オブジェクトリスト");

 
    //m_selectedIndexが-1なら何も選択されていない
    if (m_selectedIndex == -1)
    {
        ImGui::Text(u8"何も選択されていません");
    }
    else
    {
        ImGui::Text(u8"選択中のオブジェクト: %d", m_selectedIndex);
    }

    //パターンを変更
    if (ImGui::ArrowButton("##left", ImGuiDir_Left)) {
        patternIndex--;
        showSaveConfirm = true; // 確認ウィンドウを出すトリガー
        ImGui::OpenPopup("Import Json");
    }

    ImGui::SameLine();
    ImGui::Text(u8"パターン %d/%d", patternIndex, maxPattern);
    ImGui::SameLine();

    if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {
        patternIndex++;

        showSaveConfirm = true; // 確認ウィンドウを出すトリガー
        ImGui::OpenPopup("Import Json");
    }

    // スクロール可能なリスト領域
    ImGui::BeginChild("list", ImVec2(200, 300), true);
    for (int i = 0; i < static_cast<int>(m_gameObjects.size()); ++i) {
        char label[32];
        sprintf(label, u8"オブジェクト %d", i);
        if (ImGui::Selectable(label, m_selectedIndex == i)) {
            m_selectedIndex = i;
            selectedObject = m_gameObjects[i];
        }
    }

    ImGui::EndChild();

	ImGui::Text(u8"レベル:%d\n", m_currentLevel);

    //=======================================================
    //数種類対応のオブジェクト生成
    //=======================================================
    if (!selectedModelPath.empty()) {
        ImGui::Text(u8"選択中のモデル: %s", selectedModelPath.c_str());
    }
    else
    {
        ImGui::Text(u8"生成するモデルが選択されていません");
    }
    if (!modelFiles.empty()) {
        ImGui::Text(u8"モデルファイル一覧:");

        ImGui::BeginChild("ModelList", ImVec2(300, 100), true);  // ← 幅300、高さ100のスクロール可能な子ウィンドウ

        for (int i = 0; i < modelFiles.size(); ++i) {
            if (ImGui::Selectable(modelFiles[i].c_str(), selected == i)) {
                selected = i;
                selectedModelPath = gameobjectpath + modelFiles[i]; // フルパスで保存
            }
        }

        ImGui::EndChild();

        // ▼ モデル生成ボタンを追加
        if (ImGui::Button(u8"このモデルでオブジェクトを生成")) {
            // 例: GenericObject生成に使う
            if (!selectedModelPath.empty())
            {
				GameObject* newObj = nullptr;

				if (IsHoleObject(selectedModelPath)) {
					newObj = new HoleObject();  // ← 穴あき用クラス（CGenericObjectを継承）
				}
				else {
					newObj = CGenericObject::Create(selectedModelPath);
				}

				if (newObj) {
					newObj->SetModelPath(selectedModelPath);
					newObj->Init();
					newObj->Load();
					m_gameObjects.push_back(newObj);
					m_selectedIndex = m_gameObjects.size() - 1;
					selectedObject = newObj;
				}
            }
        }
    }
    else {

        ImGui::Text("モデルファイルが見つかりません");
    }

    //データ書き出し
    if (ImGui::Button(u8"セーブ")) {

        showSaveConfirm = true; // 確認ウィンドウを出すトリガー
        ImGui::OpenPopup("Save Confirmation");
    }

    //セーブ時ポップアップ
	static char fileInputBuffer[256]; // 初期パス（ユーザーが入力可能）

	if (ImGui::BeginPopupModal("Save Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text(u8"セーブしますか?");
		ImGui::Separator();

		// レベル番号指定
		ImGui::InputInt(u8"レベルを指定", &m_currentLevel);

		// 自動生成ファイル名の参考表示
		std::string autoFileName;

		ImGui::BeginChild("FileList", ImVec2(300, 100), true);
		for (int i = 0; i < jsonFiles.size(); ++i) {
			std::string label = jsonFiles[i] + "##" + std::to_string(i);
			bool isSelected = (selectedJsonIndex == i);

			if (ImGui::Selectable(label.c_str(), isSelected)) {
				selectedJsonIndex = i;

				// ファイル名だけ反映させる（保存処理はしない！）
				autoFileName = jsonFiles[i];
				std::strncpy(fileInputBuffer, autoFileName.c_str(), IM_ARRAYSIZE(fileInputBuffer));
				fileInputBuffer[IM_ARRAYSIZE(fileInputBuffer) - 1] = '\0';
			}
		}
		ImGui::EndChild();

		if (!m_loadedFileName.empty()) {
			// 読み込んだファイルがあるならそれをそのまま使う
			autoFileName = "Data/JSON/" + m_loadedFileName;
		}
		else {
			// ない場合はデフォルト名
			autoFileName = "Data/JSON/pattern.json";
		}

		ImGui::Text(u8"自動生成されるファイル名（空欄の場合この名前で出力します）\n: %s", autoFileName.c_str());

		// ファイル名の直接入力欄
		ImGui::InputText(u8"ファイル名を指定(パスは自動追加されます", fileInputBuffer, IM_ARRAYSIZE(fileInputBuffer));

		if (ImGui::Button("Yes", ImVec2(120, 0))) {
			std::string saveFile;

			if (std::strlen(fileInputBuffer) == 0) {
				saveFile = autoFileName;  // 自動生成名を使う
			}
			else {
				saveFile = std::string(fileInputBuffer);

				// パスと拡張子を補完
				if (saveFile.find("Data/JSON/") != 0)
					saveFile = "Data/JSON/" + saveFile;
				if (saveFile.length() < 5 || saveFile.substr(saveFile.length() - 5) != ".json")
					saveFile += ".json";
			}

			

			// JSON構築
			ordered_json jsonOutput;
			jsonOutput["Level"] = m_currentLevel;
			jsonOutput["Tag"] = m_stageTag;
			jsonOutput["AnticipationFrame"] = AnticipationFrame;
			ordered_json objectList = ordered_json::array();

			for (auto* obj : m_gameObjects) {
				D3DXVECTOR3 pos = RoundVec(obj->GetPos(), 2);
				D3DXVECTOR3 move = RoundVec(obj->GetMove(), 2);
				D3DXVECTOR3 rot = RoundVec(obj->GetLogicRotation(), 2);
				D3DXVECTOR3 rotation = RoundVec(obj->GetRot(), 2);
				D3DXVECTOR3 scale = RoundVec(obj->GetScale(), 3);

				nlohmann::json objData;
				objData["Move"] = { move.x, move.y, move.z };
				objData["Name"] = obj->GetTypeString();
				objData["Pos"] = { pos.x, pos.y, pos.z };
				objData["Rot"] = { rot.x, rot.y, rot.z };
				objData["Rotation"] = { rotation.x, rotation.y, rotation.z };
				objData["Scale"] = { scale.x, scale.y, scale.z };
				objData["SummonFrame"] = obj->GetSummonCount();
				objData["ModelName"] = obj->GetModelPath();

				if (HoleObject* hole = dynamic_cast<HoleObject*>(obj)) {
					D3DXVECTOR3 offset = RoundVec(hole->GetHoleOffset());
					D3DXVECTOR3 hrot = RoundVec(hole->GetHoleRot(), 2);
					D3DXVECTOR3 hscale = RoundVec(hole->GetHoleScale(), 3);
					objData["HoleOffset"] = { offset.x, offset.y, offset.z };
					objData["HoleRot"] = { hrot.x, hrot.y, hrot.z };
					objData["HoleScale"] = { hscale.x, hscale.y, hscale.z };
				}

				objectList.push_back(objData);
			}

			jsonOutput["Objects"] = objectList;

			// 保存処理
			std::ofstream out(saveFile);
			out << std::fixed << std::setprecision(2) << jsonOutput.dump(4);
			out.close();

			ImGui::CloseCurrentPopup();
			showSaveConfirm = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("No", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
			showSaveConfirm = false;
		}

		ImGui::EndPopup();
	}


    //Json読み込み
    if (ImGui::Button(u8"ロード")) {

        showSaveConfirm = true; // 確認ウィンドウを出すトリガー
        ImGui::OpenPopup("Import Json");
    }


    //読み込み時ポップアップ
    if (ImGui::BeginPopupModal("Import Json", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(u8"Jsonを読み込みますか?");

		// ファイル選択リスト（スクロール可能）
		ImGui::BeginChild("FileList", ImVec2(400, 200), true);
		for (int i = 0; i < jsonFiles.size(); ++i) {
			std::string label = jsonFiles[i] + "##" + std::to_string(i);  // 表示名 + 内部ID
			if (ImGui::Selectable(label.c_str(), selected == i)) {
				selected = i;
				selectedJsonPath = jsonfilepath + jsonFiles[i];
			}
		}
		ImGui::EndChild();
		

        ImGui::Separator();
        if (ImGui::Button("Yes", ImVec2(120, 0))) {

            std::ifstream in(selectedJsonPath);
            if (in) {
                nlohmann::json jsonInput;
                in >> jsonInput;
                in.close();

                // 既存オブジェクトの削除
                for (auto* obj : m_gameObjects) {
                    //delete obj;
                    obj->Uninit();
                    obj = nullptr;
                }

				m_gameObjects.clear(); // 既存オブジェクトを削除
                m_selectedIndex = -1; // 選択をリセット

				//レベル読み込み
				if (jsonInput.contains("Level") && !jsonInput["Level"].is_null()) {
					int level = jsonInput["Level"].get<int>();
					std::cout << "読み込んだレベル: " << level << std::endl;
					m_currentLevel = level;
				}
				else {
					std::cout << "[警告] 'Level' の情報が見つからないか null でした。m_currentLevel は変更されません。\n";
					warningMessage = "警告: Level 情報が無効です";
				}
				//タグ読み込み
				if (jsonInput.contains("Tag")) {
					m_stageTag = jsonInput["Tag"].get<std::string>();

					// インデックスも再設定しておくと UI と同期できる
					for (int i = 0; i < IM_ARRAYSIZE(tagOptions); ++i) {
						if (m_stageTag == tagOptions[i]) {
							currentTagIndex = i;
							break;
						}
					}
				}
				//見切りフレーム読み込み
				if (jsonInput.contains("AnticipationFrame") && !jsonInput["AnticipationFrame"].is_null()) {
					int Frame = jsonInput["AnticipationFrame"].get<int>();
					AnticipationFrame = Frame;
				}
				else
				{
					//読み込めなかった場合60を代入
					AnticipationFrame = PATTERNFRAME;
				}
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", warningMessage.c_str());
				const auto& objects = jsonInput["Objects"];

                for (const auto& objData : objects) {
					GameObject* newObj = nullptr;

					// モデルパスを取得
					std::string modelPath;
					if (objData.contains("ModelName")) {
						modelPath = objData["ModelName"];
					}

					// モデルパスに応じて穴あきオブジェクトか判定
					if (IsHoleObject(modelPath)) {
						newObj = new HoleObject();  // 穴あきオブジェクト
					}
					else {
						newObj = new CGenericObject();	//通常オブジェクト
					}

					if (newObj) {
						newObj->SetModelPath(modelPath);
						newObj->Loadjson(objData);
						m_gameObjects.push_back(newObj);
						newObj->Init();     // モデルパスが設定された後に初期化
						newObj->Load();
					}
				}

                m_selectedIndex = m_gameObjects.empty() ? -1 : 0; // 選択リセット
            }

			//ファイル名のみ抽出
			m_loadedFileName = selectedJsonPath;
			size_t lastSlash = selectedJsonPath.find_last_of("/\\");

			if (lastSlash != std::string::npos) {
				m_loadedFileName = selectedJsonPath.substr(lastSlash + 1);
			}

            ImGui::CloseCurrentPopup();
            showSaveConfirm = false;
        }
        
        ImGui::SameLine();

        //読み込みを行わない
        if (ImGui::Button("No", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            showSaveConfirm = false;
        }

        ImGui::EndPopup();
    }

    //オブジェクト削除
    if (ImGui::Button(u8"選択中のオブジェクト削除")) {
        if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_gameObjects.size())) {
			GameObject* target = m_gameObjects[m_selectedIndex];

			// 穴マーカーを非表示に
			if (m_holemarker && target == dynamic_cast<HoleObject*>(target)) {
				m_holemarker->SetVisible(false);
			}

			//削除対象を参照している他のポインタを無効化
			if (selectedObject == target) {
				selectedObject = nullptr;
			}

			if (m_arrowObject && m_arrowObject->IsVisible() == true) {
				m_arrowObject->SetVisible(false);
			}
			target->Uninit();   // メモリ解放を含まないように注意

			m_gameObjects.erase(m_gameObjects.begin() + m_selectedIndex);

			// インデックス更新
			if (m_gameObjects.empty()) {
				m_selectedIndex = -1;
			}
			else if (m_selectedIndex >= static_cast<int>(m_gameObjects.size())) {
				m_selectedIndex = static_cast<int>(m_gameObjects.size()) - 1;
			}
        }
    }


    //モデル差し替え
    if (ImGui::Button(u8"モデルを差し替え"))
    {
        confirmedSave = true;
        ImGui::OpenPopup("Change Model");
    }
    if (ImGui::BeginPopupModal("Change Model", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(u8"変更後のモデルパスを選んでください");
        ImGui::Separator();
        for (int i = 0; i < modelFiles.size(); ++i) {
            if (ImGui::Selectable(modelFiles[i].c_str(), selected == i, ImGuiSelectableFlags_DontClosePopups)) {
                selected = i;
                selectedModelPath = gameobjectpath + modelFiles[i];
            }
        }

		//モデル情報のみの差し替えだと穴あきオブジェクトが生成できないので、位置等の情報を保存し、
		//再度生成した後にSetpos等パラメータ設定を行う方式にしました
		if (ImGui::Button(u8"確定")) {
			// モデル差し替え先が穴あきオブジェクトであるか判定
			bool isHole = IsHoleObject(selectedModelPath);

			// 現在のオブジェクトの位置やスケール等を保存
			D3DXVECTOR3 pos = selectedObject->GetPos();
			D3DXVECTOR3 rot = selectedObject->GetRot();
			D3DXVECTOR3 scale = selectedObject->GetScale();

			// 現在のインデックスを記録しておく
			auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), selectedObject);
			size_t index = (it != m_gameObjects.end()) ? std::distance(m_gameObjects.begin(), it) : m_gameObjects.size();

			// 古いオブジェクトを削除
			selectedObject->Uninit();
			selectedObject = nullptr;
			
			// 新しいオブジェクトを生成（穴あき or 汎用）
			GameObject* newObj = nullptr;
			if (isHole) {
				newObj = new HoleObject();
			}
			else {
				newObj = new CGenericObject();
			}

			if (newObj) {
				newObj->SetModelPath(selectedModelPath);
				newObj->SetPos(pos);
				newObj->SetRot(rot);
				newObj->SetScale(scale);
				newObj->Init();
				newObj->Load();

				// リストに差し替え
				if (index < m_gameObjects.size()) {
					m_gameObjects[index] = newObj;
				}
				else {
					m_gameObjects.push_back(newObj);
				}

				selectedObject = newObj;
			}

			ImGui::CloseCurrentPopup();
		}

        ImGui::SameLine();
        if (ImGui::Button(u8"キャンセル")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::End();

    //==================================================================
    // --- 選択されたオブジェクトのパラメータ操作ウィンドウ ---
    //==================================================================
    

	SetObjParam();

    


    //描画処理呼び出し
	for (auto obj : m_gameObjects)
	{
		if (!obj) continue;  // nullptr ならスキップ


		obj->Draw(); // 通常描画
		
	}

    //矢印オブジェクトの表示を行うか否か
    if (m_arrowObject && m_arrowObject->IsVisible())
    {
        m_arrowObject->Draw();
    }

	if (m_holemarker && m_holemarker->IsVisible())
	{
		m_holemarker->Draw();
	}
}

//フレーム終了
void GUIManager::EndFrame(IDirect3DDevice9* device)
{
    if (!m_Initialized)
        return;
    ImGui::EndFrame();
    
    ImGui::Render();

    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//ウィンドウプロシージャ
bool GUIManager::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (!m_Initialized)
        return false;

    return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
}

void GUIManager::SetSelectedObject(CObjectX* obj) {
    selectedObject = obj;
}

void GUIManager::AdjustObjectZPos(GameObject* obj, float offset)
{
	D3DXVECTOR3 pos = obj->GetPos();

	pos.z = offset;

	obj->SetPos(pos);
}


void GUIManager::AdjustObjectZMove(GameObject* obj, float offset)
{
	D3DXVECTOR3 move = obj->GetMove();

	move.z = offset;

	obj->SetMove(move);
}


bool GUIManager::IsHoleObject(const std::string& path)
{
	return path.find("hole") != std::string::npos;  // "hole" を含むモデルパスは穴あき扱い
}

void GUIManager::SetObjParam()
{
	ImGui::Begin(u8"オブジェクトのパラメータ");
	if (m_selectedIndex >= 0 && m_selectedIndex < m_gameObjects.size()) {

		GameObject* obj = m_gameObjects[m_selectedIndex];
		GameObject::GameObjectType type = obj->GetObjectType(); // タイプ取得
		int currentType = static_cast<int>(type);

		const char* typeItems[] = { "SafeZone", "Obstacle","HoleObstacle" };

		D3DXVECTOR3 pos = obj->GetPos();
		D3DXVECTOR3 rot = obj->GetLogicRotation();
		D3DXVECTOR3 rotation = obj->GetRot();
		D3DXVECTOR3 scale = obj->GetScale();
		D3DXVECTOR3 move = obj->GetMove();
		D3DXVECTOR2 posXY = { pos.x, pos.y };   // X, Y だけ抽出
		D3DXVECTOR2 moveXY = { move.x, move.y };
		zAxisOffsetPos = pos.z;
		zAxisOffsetMove = move.z;

		int summonframe = obj->GetSummonCount();

		//パラメータ操作
		if (ImGui::DragFloat2("Move{x,y}", (float*)&moveXY, 0.1f)) {
			move.x = moveXY[0];
			move.y = moveXY[1];
			obj->SetMove(move);
		}
		if (ImGui::Combo("Name", &currentType, typeItems, IM_ARRAYSIZE(typeItems))) {
			obj->SetObjectType(static_cast<GameObject::GameObjectType>(currentType));
		}
		if (ImGui::DragFloat2("Pos{x,y}", (float*)&posXY, 0.1f)) {

			//位置の制限を設けたが必要なし
			/*if (posXY[0] < -POS_X_MAX) posXY[0] = -POS_X_MAX;
			if (posXY[0] > POS_X_MAX)  posXY[0] = POS_X_MAX;
			if (posXY[1] < -POS_Y_MAX) posXY[1] = -POS_Y_MAX;
			if (posXY[1] > POS_Y_MAX)  posXY[1] = POS_Y_MAX;*/

			// pos.z は変更せずにそのまま保持
			pos.x = posXY[0];
			pos.y = posXY[1];

			obj->SetPos(pos);
		}
		if (ImGui::DragInt("SummonFrame", &summonframe, 1)) {
			obj->SetSummonCount(summonframe);
		}
		if (ImGui::DragFloat3(u8"Rot 回転量{X,Y,Z}", (float*)&rot, 0.1f)) {
			obj->SetLogicRotation(rot);
		}
		if (ImGui::DragFloat3(u8"Rotation 向き{X,Y,Z}", (float*)&rotation, 0.1f)) {
			obj->SetRot(rotation);
		}
		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f)) {
			obj->SetScale(scale);
		}
		if (ImGui::DragFloat(u8"位置の一括変更｛Z｝", &zAxisOffsetPos, 0.1f, -1000.0f, 1000.0f, "%.3f")) {

			for (auto* obj : m_gameObjects) {
				if (obj) {
					AdjustObjectZPos(obj, zAxisOffsetPos);
				}
			}
		}
		if (ImGui::DragFloat(u8"移動値の一括変更｛Z｝", &zAxisOffsetMove, 0.1f, -1000.0f, 1000.0f, "%.3f")) {

			for (auto* obj : m_gameObjects) {
				if (obj) {
					AdjustObjectZMove(obj, zAxisOffsetMove);
				}
			}
		}

		// HoleObject にキャスト可能ならオフセットを表示
		if (auto* holeObj = dynamic_cast<HoleObject*>(selectedObject)) {
			// マーカーが存在しなければ生成（最初だけ）
			if (!m_holemarker) {
				m_holemarker = HoleMarkerObject::Create(holeObj);
				m_holemarker->Init();
				m_holemarker->Load();
			}

			// 毎フレーム、holeObj に合わせて更新
			D3DXVECTOR3 offset = holeObj->GetHoleOffset();
			D3DXVECTOR3 holerot = holeObj->GetHoleRot();
			D3DXVECTOR3 holescale = holeObj->GetHoleScale();
			D3DXVECTOR3 worldPos = holeObj->GetPos() + offset;

			m_holemarker->SetPos(worldPos);
			m_holemarker->SetRot(holerot);
			m_holemarker->SetScale(holescale);
			m_holemarker->SetVisible(true);

			// 必要なら holeObj に登録
			holeObj->SetHoleVisibleObject(m_holemarker);

			// ImGui 操作
			if (ImGui::DragFloat3(u8"穴のオフセット", (float*)&offset, 0.1f)) {
				holeObj->SetHoleOffset(offset);
			}
			if (ImGui::DragFloat3(u8"穴の向き", (float*)&holerot, 0.1f)) {
				holeObj->SetHoleRot(holerot);
			}
			if (ImGui::DragFloat3(u8"穴のスケール", (float*)&holescale, 0.1f)) {
				holeObj->SetHoleScale(holescale);
			}

			// 矢印も更新
			D3DXVECTOR3 arrowPos = holeObj->GetPos() + offset;
			m_arrowObject_offset->SetPos(arrowPos);
			m_arrowObject_offset->SetVisible(true);
		}
		else {
			// HoleObject 以外が選ばれたら非表示に
			if (m_holemarker && m_arrowObject_offset) {
				m_holemarker->SetVisible(false);
				m_arrowObject_offset->SetVisible(false);
			}
		}

		//矢印オブジェクトの配置
		m_arrowObject->SetPos(pos);  // 原点として配置
		m_arrowObject->SetVisible(true);


		if (ImGui::Combo(u8"ステージタグ", &currentTagIndex, tagOptions, IM_ARRAYSIZE(tagOptions))) {
			m_stageTag = tagOptions[currentTagIndex];
		}

		if (ImGui::DragInt(u8"見切りフレーム", &AnticipationFrame, 1.0f))
		{
			if (AnticipationFrame < 0) AnticipationFrame = 0;
		}
	}
	else {
		ImGui::Text(u8"選択されていません");
		m_arrowObject->SetVisible(false);
		m_arrowObject_offset->SetVisible(false);

	}

	ImGui::End();
}
