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

    //X�t�@�C���̓ǂݍ���
    D3DXLoadMeshFromX("data\\obj01.x",
        D3DXMESH_SYSTEMMEM, pDevice,
        NULL,
        &m_pBuffMat,
        NULL,
        &m_dwNumMat,
        &m_pMesh);

    //for (int nCntmodel = 0; nCntmodel < NUMTEXTURE; nCntmodel++)
    //{
    //    // ���f���̃e�N�X�`���t�@�C��
    //    m_pMaterial = (D3DXMATERIAL*)m_pBuffMat[nCntmodel].GetBufferPointer();

    //    for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
    //    {
    //        if (m_pMaterial[nCntMat].pTextureFilename != nullptr)
    //        { // �e�N�X�`��������Ƃ�
    //            D3DXCreateTextureFromFile(pDevice, m_pMaterial[nCntMat].pTextureFilename, &m_pTexture[nCntmodel]); // �e�N�X�`����ǂݍ���
    //        }
    //    }
    //}
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