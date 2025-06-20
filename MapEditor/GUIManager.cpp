//==============================================================
//
// GUImanager.cpp
//
//==============================================================
#include "GUImanager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include "object.h"
#include "gameobject.h"

using json = nlohmann::json; // 省略しないなら nlohmann::json を毎回使ってもOK


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

	m_arrowObject = new ArrowObject();
	m_arrowObject_offset = new ArrowObject();
	m_arrowObject->Init();
	m_arrowObject_offset->Init();
	m_arrowObject_offset->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	m_arrowObject->SetVisible(false);
	m_arrowObject_offset->SetVisible(false);
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
    //JSONファイルの保存先のパスと名前
    std::string filename = "data\\JSON\\gameobjects_pattern" + std::to_string(patternIndex) + ".json";
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
    if (ImGui::BeginPopupModal("Save Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(u8"セーブしますか?");
        ImGui::Separator();
        ImGui::Text(u8"ファイルの名前\n%s", filename.c_str());
		ImGui::InputInt(u8"このパターンのレベルを指定", &m_currentLevel);

        if (ImGui::Button("Yes", ImVec2(120, 0))) {

			nlohmann::json jsonOutput;
			jsonOutput["Level"] = m_currentLevel;  // レベル番号を格納
			nlohmann::json objectList = nlohmann::json::array();

            for (auto* obj : m_gameObjects) {
                D3DXVECTOR3 pos = obj->GetPos();
                D3DXVECTOR3 move = obj->GetMove();
                D3DXVECTOR3 rot = obj->GetLogicRotation();
				D3DXVECTOR3 rotation = obj->GetRot();
                D3DXVECTOR3 scale = obj->GetScale();
                int summonsnt = obj->GetSummonCount();
                nlohmann::json objData;
                objData["Move"] = { move.x, move.y, move.z };
                objData["Name"] = obj->GetTypeString();
                objData["Pos"] = { pos.x, pos.y, pos.z };
                objData["Rot"] = { rot.x, rot.y, rot.z };
				objData["Rotation"] = { rotation.x, rotation.y, rotation.z };
                objData["Scale"] = { scale.x, scale.y, scale.z };
                objData["SummonFrame"] = summonsnt;
                objData["ModelName"] = obj->GetModelPath();;

				//穴あきオブジェクトはオフセット情報を書き出す
				if (selectedObject) {
					if (HoleObject* hole = dynamic_cast<HoleObject*>(obj)) {
						D3DXVECTOR3 offset = hole->GetHoleOffset();
						objData["HoleOffset"] = { offset.x, offset.y, offset.z };
					}
				}
				

				objectList.push_back(objData); // ← こっちに追加する
            }
			jsonOutput["Objects"] = objectList;
            std::ofstream out(filename);
            out << jsonOutput.dump(4); // 4はインデント幅
            out.close();

            ImGui::CloseCurrentPopup();
            showSaveConfirm = false;
        }

        ImGui::SameLine();

        //セーブしない
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
        ImGui::InputInt(u8"パターン番号を指定", &patternIndex);
		

        ImGui::Separator();
        if (ImGui::Button("Yes", ImVec2(120, 0))) {

            std::ifstream in(filename);
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

				m_gameObjects.clear(); // 既存オブジェクトを削除（必要に応じて）
                m_selectedIndex = -1; // 選択をリセット
                
				if (jsonInput.contains("Level") && !jsonInput["Level"].is_null()) {
					int level = jsonInput["Level"].get<int>();
					std::cout << "読み込んだレベル: " << level << std::endl;
					m_currentLevel = level;
				}
				else {
					std::cout << "[警告] 'Level' の情報が見つからないか null でした。m_currentLevel は変更されません。\n";
					warningMessage = "警告: Level 情報が無効です";
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
						newObj = new HoleObject();  // 穴あきオブジェクト用クラス
					}
					else {
						newObj = new CGenericObject();
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
            m_gameObjects[m_selectedIndex]->Uninit(); // メモリを解放
            m_gameObjects.erase(m_gameObjects.begin() + m_selectedIndex); // リストから削除

            // インデックスを調整
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

        if (ImGui::Button(u8"確定")) {
            if (auto generic = dynamic_cast<CGenericObject*>(selectedObject)) {
                generic->ChangeModel(selectedModelPath);
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
    ImGui::Begin(u8"オブジェクトのパラメータ");

    if (m_selectedIndex >= 0 && m_selectedIndex < m_gameObjects.size()) {

        GameObject* obj = m_gameObjects[m_selectedIndex];
        GameObject::GameObjectType type = obj->GetObjectType(); // タイプ取得
        int currentType = static_cast<int>(type);

        const char* typeItems[] = { "SafeZone", "Obstacle" };

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
			
			if (posXY[0] < -POS_X_MAX) posXY[0] = -POS_X_MAX;
			if (posXY[0] > POS_X_MAX)  posXY[0] = POS_X_MAX;
			if (posXY[1] < -POS_Y_MAX) posXY[1] = -POS_Y_MAX;
			if (posXY[1] > POS_Y_MAX)  posXY[1] = POS_Y_MAX;

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
			D3DXVECTOR3 basePos = holeObj->GetPos();
			D3DXVECTOR3 offset = holeObj->GetHoleOffset();
			D3DXVECTOR3 arrowPos = basePos + offset;

			if (ImGui::DragFloat3(u8"穴のオフセット", (float*)&offset, 0.1f)) {
				holeObj->SetHoleOffset(offset);
			}
			
			m_arrowObject_offset->SetPos(arrowPos);  // オフセットに配置
			m_arrowObject_offset->SetVisible(true);
		}
		else
		{
			m_arrowObject_offset->SetVisible(false);
		}

		//矢印オブジェクトの配置
        m_arrowObject->SetPos(pos);  // 原点として配置
		m_arrowObject->SetVisible(true);
    }
    else {
        ImGui::Text(u8"選択されていません");
        m_arrowObject->SetVisible(false);
		m_arrowObject_offset->SetVisible(false);
    }

    ImGui::End();


    //描画処理呼び出し
    for (auto obj : m_gameObjects)
    {
        if (!obj)
        {
            if (obj == selectedObject)
            {
                // アウトライン描画
                obj->DrawOutline();

                // ワイヤーフレーム有効化
                CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

                obj->Draw(); // 通常描画（モードがワイヤーフレームになってる）

                // 元に戻す
                CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
            }
            else
            {
                obj->Draw(); // 通常描画
            }
        }
        
    }

    //矢印オブジェクトの表示を行うか否か
    if (m_arrowObject && m_arrowObject->IsVisible())
    {
        m_arrowObject->Draw();
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
