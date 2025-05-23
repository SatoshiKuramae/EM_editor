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
    // 位置・スケールなど初期化
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

    // テクスチャも忘れずに解放（必要であれば）
    for (int i = 0; i < NUMTEXTURE; ++i) {
        if (m_pTexture[i]) {
            m_pTexture[i]->Release();
            m_pTexture[i] = nullptr;
        }
    }
}
HRESULT GameObject::Init()
{
    GameObject::Load();
    
    CObjectX::Init();


    return S_OK;
}
void GameObject::Load()
{
    CObject::SetType(TYPE::BLOCK);
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    //Xファイルの読み込み
    D3DXLoadMeshFromX("data\\obj01.x",
        D3DXMESH_SYSTEMMEM, pDevice,
        NULL,
        &m_pBuffMat,
        NULL,
        &m_dwNumMat,
        &m_pMesh);

    //for (int nCntmodel = 0; nCntmodel < NUMTEXTURE; nCntmodel++)
    //{
    //    // モデルのテクスチャファイル
    //    m_pMaterial = (D3DXMATERIAL*)m_pBuffMat[nCntmodel].GetBufferPointer();

    //    for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
    //    {
    //        if (m_pMaterial[nCntMat].pTextureFilename != nullptr)
    //        { // テクスチャがあるとき
    //            D3DXCreateTextureFromFile(pDevice, m_pMaterial[nCntMat].pTextureFilename, &m_pTexture[nCntmodel]); // テクスチャを読み込む
    //        }
    //    }
    //}
}


void GameObject::Update()
{
    // CObjectX の基本更新 + GameObject 独自処理
    CObjectX::Update();
}

void GameObject::Draw()
{
    // CObjectX に描画任せて問題なければそのまま
    CObjectX::Draw();
}


GameObject* GameObject::Create()
{
    GameObject* pGameobject = new GameObject;

    pGameobject->Init();

    pGameobject->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

    return pGameobject;
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
    return GameObjectType::SafeZone; // デフォルト
}