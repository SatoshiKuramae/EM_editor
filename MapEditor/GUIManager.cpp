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
#include "pch.h"

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

    // ImGuiコンテキストの作成
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;


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
    static bool showSaveConfirm = false;
    static bool confirmedSave = false;

    if (!m_Initialized)
        return;

    // ====== ここに描画処理を書く ======
    ImGui::Begin("Object List");

    ImGui::Text("Selected Object: %d", m_selectedIndex);


    // スクロール可能なリスト領域
    ImGui::BeginChild("List", ImVec2(200, 300), true);
    for (int i = 0; i < static_cast<int>(m_gameObjects.size()); ++i) {
        char label[32];
        sprintf(label, "Object %d", i);
        if (ImGui::Selectable(label, m_selectedIndex == i)) {
            m_selectedIndex = i;
        }
    }
    ImGui::EndChild();

    if (ImGui::Button("Add GameObject")) {
        GameObject* newObj = new GameObject();
        newObj->Init();
        newObj->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
        newObj->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
        newObj->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
        m_gameObjects.push_back(newObj);
        m_selectedIndex = static_cast<int>(m_gameObjects.size()) - 1;
    }

    //データ書き出し
    if (ImGui::Button("Export Json")) {

        showSaveConfirm = true; // 確認ウィンドウを出すトリガー
        ImGui::OpenPopup("Save Confirmation");

        std::ofstream out("data\\gameobjects.txt");
    }
    // モーダルポップアップ
    if (ImGui::BeginPopupModal("Save Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Save?");
        ImGui::Separator();

        if (ImGui::Button("Yes", ImVec2(120, 0))) {
            nlohmann::json jsonOutput;

            for (size_t i = 0; i < m_gameObjects.size(); ++i) {
                GameObject* obj = m_gameObjects[i];
                D3DXVECTOR3 pos = obj->GetPos();
                D3DXVECTOR3 rot = obj->GetRot();
                D3DXVECTOR3 scale = obj->GetScale();

                nlohmann::json objData;
                objData["position"] = { pos.x, pos.y, pos.z };
                objData["rotation"] = { rot.x, rot.y, rot.z };
                objData["scale"] = { scale.x, scale.y, scale.z };

                jsonOutput["objects"].push_back(objData);
            }

            std::ofstream out("data\\gameobjects.JSON");
            out << jsonOutput.dump(4); // 4はインデント幅
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

    if (ImGui::Button("Import JSON")) {
        std::ifstream in("data\\gameobjects.json");
        if (in) {
            nlohmann::json jsonInput;
            in >> jsonInput;
            in.close();

            // 既存オブジェクトの削除
            for (auto* obj : m_gameObjects) {
                delete obj;
            }
            m_selectedIndex = -1; // 選択をリセット
            m_gameObjects.clear(); // 既存オブジェクトを削除（必要に応じて）

            for (const auto& objData : jsonInput["objects"]) {
                GameObject* newObj = new GameObject();
                newObj->Loadjson(objData); // GameObjectが処理を担当
                m_gameObjects.push_back(newObj);
            }

            m_selectedIndex = m_gameObjects.empty() ? -1 : 0; // 選択リセット
        }
    }


    ImGui::End();


    // --- 選択されたオブジェクトのパラメータ操作ウィンドウ ---
    ImGui::Begin("Object Parameters");

    if (m_selectedIndex >= 0 && m_selectedIndex < m_gameObjects.size()) {
        GameObject* obj = m_gameObjects[m_selectedIndex];
        D3DXVECTOR3 pos = obj->GetPos();
        D3DXVECTOR3 rot = obj->GetRot();
        D3DXVECTOR3 scale = obj->GetScale();

        if (ImGui::DragFloat3("Position", (float*)&pos, 0.1f)) {
            obj->SetPos(pos);
        }
        if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.1f)) {
            obj->SetRot(rot);
        }
        if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f)) {
            obj->SetScale(scale);
        }
    }
    else {
        ImGui::Text("No object selected.");
    }

    ImGui::End();

    // ==================================

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

void GUIManager::SetObjectList(std::vector<GameObject*>& objectList) {
    m_gameObjects = objectList;
}