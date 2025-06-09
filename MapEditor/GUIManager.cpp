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
#include "FileUtil.h"
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

    m_arrowObject = new ArrowObject();
    m_arrowObject->Init();
    m_arrowObject->SetVisible(false);
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
    ImGui::Begin(u8"�I�u�W�F�N�g���X�g");

 
    //m_selectedIndex��-1�Ȃ牽���I������Ă��Ȃ�
    if (m_selectedIndex == -1)
    {
        ImGui::Text(u8"�����I������Ă��܂���");
    }
    else
    {
        ImGui::Text(u8"�I�𒆂̃I�u�W�F�N�g: %d", m_selectedIndex);
    }

    static int patternIndex = 1;
    const int maxPattern = 10; // �p�^�[�����i�K�v�ɉ����đ��₹��j

    //�p�^�[����ύX
    if (ImGui::ArrowButton("##left", ImGuiDir_Left)) {
        patternIndex--;
        if (patternIndex < 1) patternIndex = maxPattern;
        showSaveConfirm = true; // �m�F�E�B���h�E���o���g���K�[
        ImGui::OpenPopup("Import Json");
    }

    ImGui::SameLine();
    ImGui::Text(u8"�p�^�[�� %d/%d", patternIndex, maxPattern);
    ImGui::SameLine();

    if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {
        patternIndex++;
        if (patternIndex > maxPattern) patternIndex = 1;
        showSaveConfirm = true; // �m�F�E�B���h�E���o���g���K�[
        ImGui::OpenPopup("Import Json");
    }

    // �X�N���[���\�ȃ��X�g�̈�
    ImGui::BeginChild(u8"���X�g", ImVec2(200, 300), true);
    for (int i = 0; i < static_cast<int>(m_gameObjects.size()); ++i) {
        char label[32];
        sprintf(label, u8"�I�u�W�F�N�g %d", i);
        if (ImGui::Selectable(label, m_selectedIndex == i)) {
            m_selectedIndex = i;
            selectedObject = m_gameObjects[i];
        }
    }

    ImGui::EndChild();

    //���f���̃p�X���w��t�H���_������擾
    std::vector<std::string> modelFiles = GetXFileNamesInDirectory("Data\\model\\","x");
    static int selected = 0;
    static std::string selectedModelPath; // �I�΂ꂽ���f���̃p�X

    //=======================================================
    //����ޑΉ��̃I�u�W�F�N�g����
    //=======================================================
    if (!selectedModelPath.empty()) {
        ImGui::Text(u8"�I�𒆂̃��f��: %s", selectedModelPath.c_str());
    }
    else
    {
        ImGui::Text(u8"�������郂�f�����I������Ă��܂���");
    }
    if (!modelFiles.empty()) {
        ImGui::Text(u8"���f���t�@�C���ꗗ:");

        for (int i = 0; i < modelFiles.size(); ++i) {
            if (ImGui::Selectable(modelFiles[i].c_str(), selected == i)) {
                selected = i;
                selectedModelPath = "data\\model\\" + modelFiles[i]; // �t���p�X�ŕۑ�
            }
        }

        // �� ���f�������{�^����ǉ�
        if (ImGui::Button(u8"���̃��f���ŃI�u�W�F�N�g�𐶐�")) {
            // ��: GenericObject�����Ɏg��
            if (!selectedModelPath.empty())
            {
                GameObject* newObj = CGenericObject::Create(selectedModelPath);
                newObj->Load();
                if (newObj)
                {
                    m_gameObjects.push_back(newObj);
                    m_selectedIndex = m_gameObjects.size() - 1;
                    selectedObject = newObj;
                }
            }
        }
    }
    else {

        ImGui::Text("���f���t�@�C����������܂���");
    }

    //�f�[�^�����o��
    if (ImGui::Button(u8"�Z�[�u")) {

        showSaveConfirm = true; // �m�F�E�B���h�E���o���g���K�[
        ImGui::OpenPopup("Save Confirmation");
    }

    //�Z�[�u���|�b�v�A�b�v
    if (ImGui::BeginPopupModal("Save Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(u8"�Z�[�u���܂���?");
        ImGui::Separator();
        std::string filename = "data\\JSON\\gameobjects_pattern" + std::to_string(patternIndex) + ".json";
        nlohmann::json jsonOutput;
        ImGui::Text(u8"�t�@�C���̖��O\n%s", filename.c_str());


        if (ImGui::Button("Yes", ImVec2(120, 0))) {
            

            for (auto* obj : m_gameObjects) {
                D3DXVECTOR3 pos = obj->GetPos();
                D3DXVECTOR3 move = obj->GetMove();
                D3DXVECTOR3 rot = obj->GetLogicRotation();
                D3DXVECTOR3 scale = obj->GetScale();
                int summonsnt = obj->GetSummonCount();
                nlohmann::json objData;
                objData["Move"] = { move.x, move.y, move.z };
                objData["Name"] = obj->GetTypeString();
                objData["Pos"] = { pos.x, pos.y, pos.z };
                objData["Rot"] = { rot.x, rot.y, rot.z };
                objData["Scale"] = { scale.x, scale.y, scale.z };
                objData["SummonFrame"] = summonsnt;
                objData["ModelName"] = obj->GetModelPath();;
                jsonOutput.push_back(objData);
            }

            std::ofstream out(filename);
            out << jsonOutput.dump(4); // 4�̓C���f���g��
            out.close();

            ImGui::CloseCurrentPopup();
            showSaveConfirm = false;
        }

        ImGui::SameLine();

        //�Z�[�u���Ȃ�
        if (ImGui::Button("No", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            showSaveConfirm = false;
        }
        ImGui::EndPopup();
    }


    //Json�ǂݍ���
    if (ImGui::Button(u8"���[�h")) {

        showSaveConfirm = true; // �m�F�E�B���h�E���o���g���K�[
        ImGui::OpenPopup("Import Json");

    }

    //�ǂݍ��ݎ��|�b�v�A�b�v
    if (ImGui::BeginPopupModal("Import Json", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(u8"Json��ǂݍ��݂܂���?");
        ImGui::Separator();
        std::string filename = "data\\JSON\\gameobjects_pattern" + std::to_string(patternIndex) + ".json";
        nlohmann::json jsonOutput;
        ImGui::Text(u8"�t�@�C���̖��O\n%s", filename.c_str());
        if (ImGui::Button("Yes", ImVec2(120, 0))) {
            
            std::ifstream in(filename);
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
                    GameObject* newObj = new CGenericObject;
                    newObj->Loadjson(objData); // GameObject��������S��
                    m_gameObjects.push_back(newObj);
                    newObj->Init();     // ���f���p�X���ݒ肳�ꂽ��ɏ�����
                    newObj->Load();     // �K�v�Ȃ� Load �������I��
                }

                m_selectedIndex = m_gameObjects.empty() ? -1 : 0; // �I�����Z�b�g
            }

            ImGui::CloseCurrentPopup();
            showSaveConfirm = false;
        }
        
        ImGui::SameLine();

        //�ǂݍ��݂��s��Ȃ�
        if (ImGui::Button("No", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            showSaveConfirm = false;
        }

        ImGui::EndPopup();
    }

    //�I�u�W�F�N�g�폜
    if (ImGui::Button(u8"�I�𒆂̃I�u�W�F�N�g�폜")) {
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


    //���f�������ւ�
    if (ImGui::Button(u8"���f���������ւ�"))
    {
        confirmedSave = true;
        ImGui::OpenPopup("Change Model");
    }
    if (ImGui::BeginPopupModal("Change Model", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        

        ImGui::Text(u8"�ύX��̃��f���p�X��I��ł�������");
        ImGui::Separator();
        for (int i = 0; i < modelFiles.size(); ++i) {
            if (ImGui::Selectable(modelFiles[i].c_str(), selected == i, ImGuiSelectableFlags_DontClosePopups)) {
                selected = i;
                selectedModelPath = "data\\model\\" + modelFiles[i];
            }
        }

        if (ImGui::Button(u8"�m��")) {
            if (auto generic = dynamic_cast<CGenericObject*>(selectedObject)) {
                generic->ChangeModel(selectedModelPath);
            }

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button(u8"�L�����Z��")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::End();

    //==================================================================
    // --- �I�����ꂽ�I�u�W�F�N�g�̃p�����[�^����E�B���h�E ---
    //==================================================================
    ImGui::Begin(u8"�I�u�W�F�N�g�̃p�����[�^");

    if (m_selectedIndex >= 0 && m_selectedIndex < m_gameObjects.size()) {

        // ���I�u�W�F�N�g����
        GameObject* obj = m_gameObjects[m_selectedIndex];
        GameObject::GameObjectType type = obj->GetObjectType(); // �^�C�v�擾
        int currentType = static_cast<int>(type);
        const char* typeItems[] = { "SafeZone", "Obstacle" };

        D3DXVECTOR3 pos = obj->GetPos();
        D3DXVECTOR3 rot = obj->GetLogicRotation();
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

            // X�͈̔͐���
            if (pos.x < -260.0f) pos.x = -260.0f;
            if (pos.x > 260.0f) pos.x = 260.0f;

            // Y�͈̔͐���
            if (pos.y < -135.0f) pos.y = -135.0f;
            if (pos.y > 135.0f) pos.y = 135.0f;

            obj->SetPos(pos);
        }
        if (ImGui::DragInt("SummonFrame", &summonframe, 1)) {
            obj->SetSummonCount(summonframe);
        }
        if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.1f)) {
            obj->SetLogicRotation(rot);
        }
        if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f)) {
            obj->SetScale(scale);
        }

        m_arrowObject->SetPos(pos);  // ���_�Ƃ��Ĕz�u
        m_arrowObject->SetVisible(true);
    }
    else {
        ImGui::Text(u8"�I������Ă��܂���");
        m_arrowObject->SetVisible(false);
    }

    ImGui::End();


    //�`�揈���Ăяo��
    for (auto obj : m_gameObjects)
    {
        if (!obj)
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
        
    }

    //���I�u�W�F�N�g�̕\�����s�����ۂ�
    if (m_arrowObject && m_arrowObject->IsVisible())
    {
        m_arrowObject->Draw();
    }


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