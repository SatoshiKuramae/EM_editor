//===============================================================================
//
//Object.cpp
// 
//===============================================================================
#include "main.h"
#include "manager.h"
#include "gameobject.h"

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
    //GameObject::Load();
    
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
    /*if (objData.contains("rotation")) {
        auto r = objData["rotation"];
        this->SetRot(D3DXVECTOR3(r[0], r[1], r[2]));
    }
    if (objData.contains("scale")) {
        auto s = objData["scale"];
        this->SetScale(D3DXVECTOR3(s[0], s[1], s[2]));
    }*/
    if (objData.contains("SummonFrame")) {
        int summonframe = objData["SummonFrame"];
        this->SetSummonCount(summonframe);
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

//================================
//���I�u�W�F�N�g
//================================
HRESULT CubeObject::Init()
{
    Load();

    CObjectX::Init();

    return S_OK;
}

void CubeObject::Load()
{
    CObject::SetType(TYPE::BLOCK);
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    //X�t�@�C���̓ǂݍ���
    D3DXLoadMeshFromX("data\\model\\obj01.x",
        D3DXMESH_SYSTEMMEM, pDevice,
        NULL,
        &m_pBuffMat_cube,
        NULL,
        &m_dwNumMat_cube,
        &m_pMesh_cube);

    // ���f���̃e�N�X�`���t�@�C��
    m_pMaterial_cube = (D3DXMATERIAL*)m_pBuffMat_cube->GetBufferPointer();
    for (int nCntmodel = 0; nCntmodel < NUMTEXTURE; nCntmodel++)
    {
        for (int nCntMat = 0; nCntMat < (int)m_dwNumMat_cube; nCntMat++)
        {
            if (m_pMaterial_cube[nCntMat].pTextureFilename != nullptr)
            { // �e�N�X�`��������Ƃ�
                D3DXCreateTextureFromFile(pDevice, m_pMaterial_cube[nCntMat].pTextureFilename, &m_pTexture_cube[nCntmodel]); // �e�N�X�`����ǂݍ���
            }
        }
    }
    // BindMesh�Ăяo��
    BindMesh(m_pMesh_cube, m_pBuffMat_cube, m_dwNumMat_cube, m_pMaterial_cube, m_pTexture_cube);

}

CubeObject* CubeObject::Create()
{
    CubeObject* obj = new CubeObject();
    obj->Init();
    obj->SetMove(D3DXVECTOR3(0, 0, 0));
    obj->SetPos(D3DXVECTOR3(0, 0, 0));
    obj->SetRot(D3DXVECTOR3(0, 0, 0));
    obj->SetScale(D3DXVECTOR3(1, 1, 1));
    obj->SetSummonCount(0);

    return obj;
}

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
    D3DXLoadMeshFromX("data\\model\\arrow.x",
        D3DXMESH_SYSTEMMEM, pDevice,
        NULL,
        &m_pBuffMat_arrow,
        NULL,
        &m_dwNumMat_arrow,
        &m_pMesh_arrow);

    // ���f���̃e�N�X�`���t�@�C��
    m_pMaterial_arrow = (D3DXMATERIAL*)m_pBuffMat_arrow->GetBufferPointer();
    for (int nCntmodel = 0; nCntmodel < NUMTEXTURE; nCntmodel++)
    {
        for (int nCntMat = 0; nCntMat < (int)m_dwNumMat_arrow; nCntMat++)
        {
            if (m_pMaterial_arrow[nCntMat].pTextureFilename != nullptr)
            { // �e�N�X�`��������Ƃ�
                D3DXCreateTextureFromFile(pDevice, m_pMaterial_arrow[nCntMat].pTextureFilename, &m_pTexture_arrow[nCntmodel]); // �e�N�X�`����ǂݍ���
            }
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