//===============================================================================
//
// objectX.cpp
// 
//===============================================================================
#include "main.h"
#include "objectX.h"
#include "manager.h"
#include "player.h"
//�R���X�g���N�^
CObjectX::CObjectX()
{

}
//�f�X�g���N�^
CObjectX::~CObjectX()
{

}

//����������
HRESULT CObjectX::Init()
{
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	return S_OK;
}

//�I������
void CObjectX::Uninit()
{
	//���b�V���̔j��
	if (CObjectX::m_pMesh != nullptr)
	{
		CObjectX::m_pMesh->Release();
		CObjectX::m_pMesh = NULL;
	}

	//�}�e���A���̔j��
	if (CObjectX::m_pBuffMat != NULL)
	{
		CObjectX::m_pBuffMat->Release();
		CObjectX::m_pBuffMat = NULL;
	}

	Release();
}

//�X�V����
void CObjectX::Update()
{
	
}

//�`�揈��
void CObjectX::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);



	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		m_pMaterial = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&m_pMaterial[nCntMat].MatD3D);

		if (m_pMaterial[nCntMat].pTextureFilename != nullptr)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pTexture[nCntMat]);
		}

		else
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);
		}


		//���f���i�p�[�c�j�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//���[�h����
void CObjectX::Load()
{

}

////���X�擾
//void CObjectX::Setpos(D3DXVECTOR3 pos)
//{
//	m_pos = pos;
//}
//
//void CObjectX::SetMove(D3DXVECTOR3 move)
//{
//	m_move = move;
//}
//
//void CObjectX::SetRot(D3DXVECTOR3 rot)
//{
//	m_rot = rot;
//}
//
//void CObjectX::SetScale(D3DXVECTOR3 scale)
//{
//	m_scale = scale;
//}

void CObjectX::BindMesh(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, D3DXMATERIAL* pMat, LPDIRECT3DTEXTURE9 pTexture)
{
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_dwNumMat = dwNumMat;
	m_pMaterial = pMat;
	for (int i = 0; i < (int)m_dwNumMat; i++)
	{
		if (m_pTexture[i] == nullptr)
		{
			m_pTexture[i] = pTexture;
			break;
		}
	}

}