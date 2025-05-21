//============================================================
//
//object.cpp
//
//============================================================
#include "object.h"
#include "main.h"


//�ÓI�����o������
int CObject::m_nNumAll = 0;
//bool CObject::m_bPause = false;
CObject* CObject::m_apObject[MAX_PRIORITY][NUMOBJECT] = {};

//�R���X�g���N�^�i�������g�̓o�^�j
CObject::CObject(int nPriority)
{
	m_nPriority = nPriority;

	for (int i = 0; i < NUMOBJECT; i++)
	{
		if (m_apObject[m_nPriority][i] == nullptr)
		{
			m_apObject[m_nPriority][i] = this;		//�������g�̂���
			m_nID = i;					//�������g��ID����
			m_nNumAll++;				//�������J�E���g�A�b�v
			break;						//���[�v�𔲂���
		}
	}
}

//�f�X�g���N�^
CObject::~CObject()
{

}

//�S�I�u�W�F�N�g���
void CObject::ReleaseAll()
{
	//CObject* pObj = m_pTop;	//�擪�擾
	////�ő吔���s���Ȃ̂�while���g�p
	//while (pObj != nullptr)
	//{
	//	CObject* pNext = pObj->m_pNext;
	//	pObj->Release();
	//	pObj = pNext;
	//}
	for (int nCntPri = 0; nCntPri < MAX_PRIORITY; nCntPri++)
	{
		for (int i = 0; i < NUMOBJECT; i++)
		{
			if (m_apObject[nCntPri][i] != nullptr)
			{
				m_apObject[nCntPri][i]->Release();
			}
		}
	}

}

//�S�I�u�W�F�N�g�X�V
void CObject::UpdateAll()
{
	//CObject* pObj = m_pTop;	//�擪�擾
	////�ő吔���s���Ȃ̂�while���g�p
	//while (pObj != nullptr)
	//{
	//	CObject* pNext = pObj->m_pNext;
	//	pObj->Update();
	//	pObj = pNext;
	//}
	//CObject* pGame = CGame::GetPause();
	//m_bPause = pGame->GetPause();


	for (int nCntPri = 0; nCntPri < MAX_PRIORITY; nCntPri++)
	{
		for (int i = 0; i < NUMOBJECT; i++)
		{
			if (m_apObject[nCntPri][i] != nullptr)
			{
				m_apObject[nCntPri][i]->Update();
			}
		}
	}
}

//�S�I�u�W�F�N�g�`��
void CObject::DrawAll()
{
	//CObject* pObj = m_pTop;	//�擪�擾
	////�ő吔���s���Ȃ̂�while���g�p
	//while (pObj != nullptr)
	//{
	//	CObject* pNext = pObj->m_pNext;
	//	pObj->Draw();
	//	pObj = pNext;
	//}
	for (int nCntPri = 0; nCntPri < MAX_PRIORITY; nCntPri++)
	{
		for (int i = 0; i < NUMOBJECT; i++)
		{
			if (m_apObject[nCntPri][i] != nullptr)
			{
				m_apObject[nCntPri][i]->Draw();
			}
		}
	}
}

//�������g�̉��
void CObject::Release()
{
	int nID = m_nID;
	int nPriority = m_nPriority;

	if (m_apObject[nPriority][nID] != nullptr)
	{
		delete m_apObject[nPriority][nID];
		m_apObject[nPriority][nID] = nullptr;
	}
}

//�I�u�W�F�N�g�擾
CObject* CObject::GetObject(int nIdx, int nPri)
{
	return m_apObject[nPri][nIdx];
}

