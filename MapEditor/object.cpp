//============================================================
//
//object.cpp
//
// Auther : SatoshiKuramae
//============================================================
#include "object.h"
#include "main.h"


//静的メンバ初期化
int CObject::m_nNumAll = 0;
//bool CObject::m_bPause = false;
CObject* CObject::m_apObject[MAX_PRIORITY][NUMOBJECT] = {};

//コンストラクタ（自分自身の登録）
CObject::CObject(int nPriority)
{
	m_nPriority = nPriority;

	for (int i = 0; i < NUMOBJECT; i++)
	{
		if (m_apObject[m_nPriority][i] == nullptr)
		{
			m_apObject[m_nPriority][i] = this;		//自分自身のを代入
			m_nID = i;					//自分自身のIDを代入
			m_nNumAll++;				//総数をカウントアップ
			break;						//ループを抜ける
		}
	}
}

//デストラクタ
CObject::~CObject()
{
	//for (int i = 0; i < MAX_PRIORITY; ++i) {
	//	for (int j = 0; j < NUMOBJECT; ++j) {
	//		m_apObject[i][j] = nullptr;
	//	}
	//}
}

//全オブジェクト解放
void CObject::ReleaseAll()
{
	//CObject* pObj = m_pTop;	//先頭取得
	////最大数が不明なのでwhileを使用
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

//全オブジェクト更新
void CObject::UpdateAll()
{
	//CObject* pObj = m_pTop;	//先頭取得
	////最大数が不明なのでwhileを使用
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

//全オブジェクト描画
void CObject::DrawAll()
{
	//CObject* pObj = m_pTop;	//先頭取得
	////最大数が不明なのでwhileを使用
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

//自分自身の解放
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

//オブジェクト取得
CObject* CObject::GetObject(int nIdx, int nPri)
{
	return m_apObject[nPri][nIdx];
}

