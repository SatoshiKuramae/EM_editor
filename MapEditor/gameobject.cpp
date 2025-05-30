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
    //GameObject::Load();
    
    CObjectX::Init();

    return S_OK;
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

//================================
//仮オブジェクト
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

    //Xファイルの読み込み
    D3DXLoadMeshFromX("data\\model\\obj01.x",
        D3DXMESH_SYSTEMMEM, pDevice,
        NULL,
        &m_pBuffMat_cube,
        NULL,
        &m_dwNumMat_cube,
        &m_pMesh_cube);

    // モデルのテクスチャファイル
    m_pMaterial_cube = (D3DXMATERIAL*)m_pBuffMat_cube->GetBufferPointer();
    for (int nCntmodel = 0; nCntmodel < NUMTEXTURE; nCntmodel++)
    {
        for (int nCntMat = 0; nCntMat < (int)m_dwNumMat_cube; nCntMat++)
        {
            if (m_pMaterial_cube[nCntMat].pTextureFilename != nullptr)
            { // テクスチャがあるとき
                D3DXCreateTextureFromFile(pDevice, m_pMaterial_cube[nCntMat].pTextureFilename, &m_pTexture_cube[nCntmodel]); // テクスチャを読み込む
            }
        }
    }
    // BindMesh呼び出し
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

    //Xファイルの読み込み
    D3DXLoadMeshFromX("data\\model\\arrow.x",
        D3DXMESH_SYSTEMMEM, pDevice,
        NULL,
        &m_pBuffMat_arrow,
        NULL,
        &m_dwNumMat_arrow,
        &m_pMesh_arrow);

    // モデルのテクスチャファイル
    m_pMaterial_arrow = (D3DXMATERIAL*)m_pBuffMat_arrow->GetBufferPointer();
    for (int nCntmodel = 0; nCntmodel < NUMTEXTURE; nCntmodel++)
    {
        for (int nCntMat = 0; nCntMat < (int)m_dwNumMat_arrow; nCntMat++)
        {
            if (m_pMaterial_arrow[nCntMat].pTextureFilename != nullptr)
            { // テクスチャがあるとき
                D3DXCreateTextureFromFile(pDevice, m_pMaterial_arrow[nCntMat].pTextureFilename, &m_pTexture_arrow[nCntmodel]); // テクスチャを読み込む
            }
        }
    }
    // BindMesh呼び出し
    BindMesh(m_pMesh_arrow, m_pBuffMat_arrow, m_dwNumMat_arrow, m_pMaterial_arrow, m_pTexture_arrow);

}

void ArrowObject::Draw()
{
    if (!m_isVisible) {
        return;  // 非表示なら描画しない
    }

    // 通常の描画処理
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