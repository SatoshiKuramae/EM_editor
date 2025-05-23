//===============================================================================
//
// objectX.cpp
// 
//===============================================================================
#include "main.h"
#include "objectX.h"
#include "manager.h"
#include "player.h"
//コンストラクタ
CObjectX::CObjectX()
{

}
//デストラクタ
CObjectX::~CObjectX()
{

}

//初期化処理
HRESULT CObjectX::Init()
{
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	return S_OK;
}

//終了処理
void CObjectX::Uninit()
{
	//メッシュの破棄
	if (CObjectX::m_pMesh != nullptr)
	{
		CObjectX::m_pMesh->Release();
		CObjectX::m_pMesh = NULL;
	}

	//マテリアルの破棄
	if (CObjectX::m_pBuffMat != NULL)
	{
		CObjectX::m_pBuffMat->Release();
		CObjectX::m_pBuffMat = NULL;
	}

	Release();
}

//更新処理
void CObjectX::Update()
{
	
}

//描画処理
void CObjectX::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);



	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//マテリアルデータへのポインタを取得
		m_pMaterial = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		//マテリアルの設定
		pDevice->SetMaterial(&m_pMaterial[nCntMat].MatD3D);

		if (m_pMaterial[nCntMat].pTextureFilename != nullptr)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, m_pTexture[nCntMat]);
		}

		else
		{
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);
		}


		//モデル（パーツ）の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//ロード処理
void CObjectX::Load()
{

}

////諸々取得
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