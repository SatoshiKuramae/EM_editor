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

using json = nlohmann::json; // �ȗ����Ȃ��Ȃ� nlohmann::json �𖈉�g���Ă�OK


//�R���X�g���N�^
GUIManager::GUIManager()
    : m_Initialized(false)
{
}

//�f�X�g���N�^
GUIManager::~GUIManager()
{
    Shutdown();
}

//����������
bool GUIManager::Initialize(HWND hwnd, IDirect3DDevice9* device)
{
    if (m_Initialized)
        return true;

    // ImGui�R���e�L�X�g�̍쐬
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;


    ImFontConfig config;
    config.DstFont = 0; // �����̏ꍇ��0��OK
    config.FontDataOwnedByAtlas = true;

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\meiryo.ttc", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese());
    
    // �X�^�C���ݒ�
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // �����t�H���g

    // ������
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(device);


    m_Initialized = true;
    return true;
}

//�I������
void GUIManager::Shutdown()
{
    if (!m_Initialized)
        return;

    // ���L���Ă��� GameObject �����ׂč폜
    /*for (auto* obj : m_gameObjects) {
        delete obj;
    }
    m_gameObjects.clear();
    m_selectedIndex = -1;*/

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    m_Initialized = false;
}

//�`��J�n
void GUIManager::BeginFrame()
{
    if (!m_Initialized)
        return;

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

//�X�V����
void GUIManager::Update() 
{
    static bool showSaveConfirm = false;
    static bool confirmedSave = false;

    if (!m_Initialized)
        return;

    // ====== �����ɕ`�揈�������� ======
    ImGui::Begin("Object List");

    ImGui::Text("Selected Object: %d", m_selectedIndex);


    // �X�N���[���\�ȃ��X�g�̈�
    ImGui::BeginChild("List", ImVec2(200, 300), true);
    for (int i = 0; i < static_cast<int>(m_gameObjects.size()); ++i) {
        char label[32];
        sprintf(label, "Object %d", i);
        if (ImGui::Selectable(label, m_selectedIndex == i)) {
            m_selectedIndex = i;
            selectedObject = m_gameObjects[i];
        }
    }

    ImGui::EndChild();


    //�I�u�W�F�N�g����
    

    if (ImGui::Button("Add GameObject")) {
        
        GameObject* newObj = new GameObject();
        newObj->Init();
        newObj->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
        newObj->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
        newObj->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
        newObj->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
        newObj->SetSummonCount(0);
        m_gameObjects.push_back(newObj);
        m_selectedIndex = static_cast<int>(m_gameObjects.size()) - 1;
    }

    //�f�[�^�����o��
    if (ImGui::Button("Export Json")) {

        showSaveConfirm = true; // �m�F�E�B���h�E���o���g���K�[
        ImGui::OpenPopup("Save Confirmation");

        std::ofstream out("data\\gameobjects.txt");
    }
    // ���[�_���|�b�v�A�b�v
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
                D3DXVECTOR3 move = obj->GetMove();
                int summonsnt = obj->GetSummonCount();

                nlohmann::json objData;
                //�ړ��l�A���O�A�ʒu�A�����t���[��
                //SafeZone,Obstacle
                objData["Move"] = { move.x,move.y,move.z };
                objData["Name"] = obj->GetTypeString();  // ��: "SafeZone"
                objData["Pos"] = { pos.x, pos.y, pos.z };
                //objData["rotation"] = { rot.x, rot.y, rot.z };
                //objData["scale"] = { scale.x, scale.y, scale.z };
                objData["SummonFrame"] = summonsnt;
                jsonOutput.push_back(objData);
            }

            std::ofstream out("data\\gameobjects.JSON");
            out << jsonOutput.dump(4); // 4�̓C���f���g��
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

    if (ImGui::Button("Import Json")) {

        showSaveConfirm = true; // �m�F�E�B���h�E���o���g���K�[
        ImGui::OpenPopup("Import Json");

        std::ofstream out("data\\gameobjects.txt");
    }
    if (ImGui::BeginPopupModal("Import Json", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Import?");
        ImGui::Separator();
        if (ImGui::Button("Yes", ImVec2(120, 0))) {
            nlohmann::json jsonOutput;

            std::ifstream in("data\\gameobjects.json");
            if (in) {
                nlohmann::json jsonInput;
                in >> jsonInput;
                in.close();

                // �����I�u�W�F�N�g�̍폜
                for (auto* obj : m_gameObjects) {
                    //delete obj;
                    obj->Uninit();
                    obj = nullptr;
                }
                m_selectedIndex = -1; // �I�������Z�b�g
                m_gameObjects.clear(); // �����I�u�W�F�N�g���폜�i�K�v�ɉ����āj


                for (const auto& objData : jsonInput) {
                    GameObject* newObj = new GameObject();
                    newObj->Loadjson(objData); // GameObject��������S��
                    m_gameObjects.push_back(newObj);
                }

                m_selectedIndex = m_gameObjects.empty() ? -1 : 0; // �I�����Z�b�g
            }

            //std::ofstream out("data\\gameobjects.JSON");
            //out << jsonOutput.dump(4); // 4�̓C���f���g��
            //out.close();

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
    //�I�u�W�F�N�g�폜
    if (ImGui::Button("Delete Object")) {
        if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_gameObjects.size())) {
            m_gameObjects[m_selectedIndex]->Uninit(); // �����������
            m_gameObjects.erase(m_gameObjects.begin() + m_selectedIndex); // ���X�g����폜

            // �C���f�b�N�X�𒲐�
            if (m_gameObjects.empty()) {
                m_selectedIndex = -1;
            }
            else if (m_selectedIndex >= static_cast<int>(m_gameObjects.size())) {
                m_selectedIndex = static_cast<int>(m_gameObjects.size()) - 1;
            }
        }
    }

    ImGui::End();


    // --- �I�����ꂽ�I�u�W�F�N�g�̃p�����[�^����E�B���h�E ---
    ImGui::Begin("Object Parameters");

    if (m_selectedIndex >= 0 && m_selectedIndex < m_gameObjects.size()) {

        GameObject* obj = m_gameObjects[m_selectedIndex];
        GameObject::GameObjectType type = obj->GetObjectType(); // �^�C�v�擾
        int currentType = static_cast<int>(type);
        const char* typeItems[] = { "SafeZone", "Obstacle" };

        D3DXVECTOR3 pos = obj->GetPos();
        D3DXVECTOR3 rot = obj->GetRot();
        D3DXVECTOR3 scale = obj->GetScale();
        D3DXVECTOR3 move = obj->GetMove();
        int summonframe = obj->GetSummonCount();

        if (ImGui::DragFloat3("Move{x,y,z}", (float*)&move, 0.1f)) {
            obj->SetMove(move);
        }
        if (ImGui::Combo("Name", &currentType, typeItems, IM_ARRAYSIZE(typeItems))) {
            obj->SetObjectType(static_cast<GameObject::GameObjectType>(currentType));
        }
        if (ImGui::DragFloat3("Pos{x,y,z}", (float*)&pos, 0.1f)) {
            obj->SetPos(pos);
        }
        if (ImGui::DragInt("SummonFrame", &summonframe, 1)) {
            obj->SetSummonCount(summonframe);
        }
        /*if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.1f)) {
            obj->SetRot(rot);
        }
        if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f)) {
            obj->SetScale(scale);
        }*/
    }
    else {
        ImGui::Text("No object selected.");
    }

    ImGui::End();


    for (auto obj : m_gameObjects)
    {
        if (obj == selectedObject)
        {
            // �A�E�g���C���`��
            obj->DrawOutline();

            // ���C���[�t���[���L����
            CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

            obj->Draw(); // �ʏ�`��i���[�h�����C���[�t���[���ɂȂ��Ă�j

            // ���ɖ߂�
            CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        }
        else
        {
            obj->Draw(); // �ʏ�`��
        }
    }
    // ==================================

}

//�t���[���I��
void GUIManager::EndFrame(IDirect3DDevice9* device)
{
    if (!m_Initialized)
        return;
    ImGui::EndFrame();
    
    ImGui::Render();

    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//�E�B���h�E�v���V�[�W��
bool GUIManager::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (!m_Initialized)
        return false;

    return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
}

void GUIManager::SetSelectedObject(CObjectX* obj) {
    selectedObject = obj;
}