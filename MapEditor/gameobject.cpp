//===============================================================================
//
//Object.cpp
// 
//===============================================================================
#include "main.h"
#include "manager.h"
#include "gameobject.h"

//===================================================================
//
//�e�N���X
//
//===================================================================
GameObject::GameObject()
{
    // �ʒu�E�X�P�[���ȂǏ�����
    SetPos(D3DXVECTOR3(0, 0, 0));
    SetScale(D3DXVECTOR3(1, 1, 1));
    SetRot(D3DXVECTOR3(0, 0, 0));
    SetSummonCount(0);
}

GameObject::~GameObject()
{
    if (m_pMesh) {
        m_pMesh->Release();
        m_pMesh = nullptr;
    }

    if (m_pBuffMat) {
        m_pBuffMat->Release();
        m_pBuffMat = nullptr;
    }

    // �e�N�X�`�����Y�ꂸ�ɉ���i�K�v�ł���΁j
    for (int i = 0; i < NUMTEXTURE; ++i) {
        if (m_pTexture[i]) {
            m_pTexture[i]->Release();
            m_pTexture[i] = nullptr;
        }
    }
}

HRESULT GameObject::Init()
{
    
    CObjectX::Init();

    return S_OK;
}



void GameObject::Update()
{
    // CObjectX �̊�{�X�V + GameObject �Ǝ�����
    CObjectX::Update();
}

void GameObject::Draw()
{
    // CObjectX �ɕ`��C���Ė��Ȃ���΂��̂܂�
    CObjectX::Draw();
}


GameObject* GameObject::Loadjson(const json& objData)
{
    this->Init();

    if (objData.contains("Move")) {
        auto m = objData["Move"];
        this->SetMove(D3DXVECTOR3(m[0], m[1], m[2]));
    }
    if (objData.contains("Name")) {
        std::string typeStr = objData["Name"];
        this->SetObjectType(GameObject::FromTypeString(typeStr));
    }
    if (objData.contains("Pos")) {
        auto p = objData["Pos"];
        this->SetPos(D3DXVECTOR3(p[0], p[1], p[2]));
    }
    if (objData.contains("rotation")) {
        auto r = objData["rotation"];
        this->SetRot(D3DXVECTOR3(r[0], r[1], r[2]));
    }
    if (objData.contains("scale")) {
        auto s = objData["scale"];
        this->SetScale(D3DXVECTOR3(s[0], s[1], s[2]));
    }
    if (objData.contains("SummonFrame")) {
        int summonframe = objData["SummonFrame"];
        this->SetSummonCount(summonframe);
    }
    if (objData.contains("ModelName")) {
        std::string path = objData["ModelName"];
        this->SetModelPath(path);  // �����o�ϐ��Ɋi�[�im_modelPath �Ȃǁj
    }

    return this;
}

const char* GameObject::GetTypeString() {
    switch (m_type) {
    case GameObjectType::SafeZone: return "SafeZone";
    case GameObjectType::Obstacle: return "Obstacle";
    default: return "Unknown";
    }
}

GameObject::GameObjectType GameObject::FromTypeString(const std::string& str) {
    if (str == "SafeZone") return GameObjectType::SafeZone;
    if (str == "Obstacle") return GameObjectType::Obstacle;
    return GameObjectType::SafeZone; // �f�t�H���g
}


//===================================================================
//
//���I�u�W�F�N�g�N���X
//
//===================================================================
HRESULT ArrowObject::Init()
{
    Load();

    CObjectX::Init();

    return S_OK;
}

void ArrowObject::Load()
{
    CObject::SetType(TYPE::BLOCK);
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    //X�t�@�C���̓ǂݍ���
    D3DXLoadMeshFromX("data\\arrow.x",
        D3DXMESH_SYSTEMMEM, pDevice,
        NULL,
        &m_pBuffMat_arrow,
        NULL,
        &m_dwNumMat_arrow,
        &m_pMesh_arrow);

    // ���f���̃e�N�X�`���t�@�C��
    m_pMaterial_arrow = (D3DXMATERIAL*)m_pBuffMat_arrow->GetBufferPointer();
    for (DWORD i = 0; i < m_dwNumMat_arrow; ++i)
    {
        if (m_pMaterial_arrow[i].pTextureFilename)
        {
            D3DXCreateTextureFromFile(pDevice, m_pMaterial_arrow[i].pTextureFilename, &m_pTexture_arrow[i]);
        }
    }
    // BindMesh�Ăяo��
    BindMesh(m_pMesh_arrow, m_pBuffMat_arrow, m_dwNumMat_arrow, m_pMaterial_arrow, m_pTexture_arrow);

}

void ArrowObject::Draw()
{
    if (!m_isVisible) {
        return;  // ��\���Ȃ�`�悵�Ȃ�
    }

    // �ʏ�̕`�揈��
    CObjectX::Draw();
}

ArrowObject* ArrowObject::Create()
{
    ArrowObject* obj = new ArrowObject();
    obj->Init();
    obj->SetMove(D3DXVECTOR3(0, 0, 0));
    obj->SetPos(D3DXVECTOR3(0, 0, 0));
    obj->SetRot(D3DXVECTOR3(0, 0, 0));
    obj->SetScale(D3DXVECTOR3(1, 1, 1));
    obj->SetSummonCount(0);
    return obj;
}

//===================================================================
//
//�����Ή��^�I�u�W�F�N�g�N���X
//
//===================================================================

HRESULT CGenericObject::Init()
{
    CObjectX::Init();

    return S_OK;
}

void CGenericObject::Load()
{
    ReleaseModelResources();

    CObject::SetType(TYPE::BLOCK);
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    //X�t�@�C���̓ǂݍ���
    D3DXLoadMeshFromX(m_modelPath.c_str(),
        D3DXMESH_SYSTEMMEM, pDevice,
        NULL,
        &m_pBuffMat_Gn_Object,
        NULL,
        &m_dwNumMat_Gn_Object,
        &m_pMesh_Gn_Object);

    // ���f���̃e�N�X�`���t�@�C��
    m_pMaterial_Gn_Object = (D3DXMATERIAL*)m_pBuffMat_Gn_Object->GetBufferPointer();

    for (DWORD i = 0; i < m_dwNumMat_Gn_Object; ++i)
    {
        if (m_pMaterial_Gn_Object[i].pTextureFilename)
        {
            D3DXCreateTextureFromFile(pDevice, m_pMaterial_Gn_Object[i].pTextureFilename, &m_pTexture_Gn_Object[i]);
        }
    }
    // BindMesh�Ăяo��
    BindMesh(m_pMesh_Gn_Object, m_pBuffMat_Gn_Object, m_dwNumMat_Gn_Object, m_pMaterial_Gn_Object, m_pTexture_Gn_Object);

}

void CGenericObject::Draw()
{

    // �ʏ�̕`�揈��
    CObjectX::Draw();
}

CGenericObject* CGenericObject::Create(const std::string& modelPath)
{
    CGenericObject* obj = new CGenericObject();
    obj->SetModelPath(modelPath);
    obj->Init();
    obj->SetMove(D3DXVECTOR3(0, 0, 0));
    obj->SetPos(D3DXVECTOR3(0, 0, 0));
    obj->SetRot(D3DXVECTOR3(0, 0, 0));
    obj->SetScale(D3DXVECTOR3(1, 1, 1));
    obj->SetSummonCount(0);
    
    return obj;
}

void CGenericObject::ChangeModel(const std::string& modelPath) {
    SetModelPath(modelPath);  // ���f���p�X�X�V
    Load();                   // �ēǂݍ���
}

void CGenericObject::ReleaseModelResources() {
    if (m_pMesh) { m_pMesh->Release(); m_pMesh = nullptr; }
    if (m_pBuffMat) { m_pBuffMat->Release(); m_pBuffMat = nullptr; }
    for (int i = 0; i < NUMTEXTURE; ++i) {
        if (m_pTexture[i]) { m_pTexture[i]->Release(); m_pTexture[i] = nullptr; }
    }
}