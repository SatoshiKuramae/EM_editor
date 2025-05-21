#include "object3D.h"
#include "main.h"
#include "manager.h"
#include "floor.h"

LPDIRECT3DTEXTURE9 CFloor::m_pTexture = nullptr;
//�R���X�g���N�^
CFloor::CFloor()
{

}
// �f�X�g���N�^
CFloor::~CFloor()
{

}

//������
HRESULT CFloor::Init()
{
	CObject::SetType(TYPE::FLOOR);
	CObject3D::Init();
	;	return S_OK;
}

//�`�揈��
void CFloor::Draw()
{
	CObject3D::Draw();
}

//�X�V����
void CFloor::Update()
{

}

//�I������
void CFloor::Uninit()
{
	CObject3D::Uninit();
}

//���̐���
CFloor* CFloor::Create()
{
	CFloor* pObject3D = new CFloor;

	pObject3D->BindTexture(m_pTexture, D3DXVECTOR2(1, 1));
	pObject3D->Init();
	return pObject3D;
}


//���[�h
HRESULT CFloor::Load()
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();
	if (m_pTexture == nullptr)
	{
		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data\\texture\\block.png", &m_pTexture);
	}
	return S_OK;
}

//�A�����[�h
void CFloor::Unload()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}
