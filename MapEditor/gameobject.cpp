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
}

HRESULT GameObject::Init()
{
    CObject::SetType(TYPE::PLAYER);
    GameObject::Load();
    
    CObjectX::Init();


    return S_OK;
}
void GameObject::Load()
{
    CObject::SetType(TYPE::PLAYER);
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

    if (objData.contains("position")) {
        auto p = objData["position"];
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

    return this;
}