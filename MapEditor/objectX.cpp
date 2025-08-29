//===============================================================================
//
// objectX.cpp
//
// Auther : SatoshiKuramae
//===============================================================================
#include "main.h"
#include "objectX.h"
#include "manager.h"

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

//アウトラインの表示（未実装）
void CObjectX::DrawOutline()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// === 1. アウトライン描画 ===
	D3DMATERIAL9 outlineMat = {};
	outlineMat.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 黒い縁
	outlineMat.Ambient = outlineMat.Diffuse;

	// 現在のマテリアルや設定を退避
	D3DMATERIAL9 prevMat;
	pDevice->GetMaterial(&prevMat);

	DWORD prevCull, prevZEnable, prevLighting;
	pDevice->GetRenderState(D3DRS_CULLMODE, &prevCull);
	pDevice->GetRenderState(D3DRS_ZENABLE, &prevZEnable);
	pDevice->GetRenderState(D3DRS_LIGHTING, &prevLighting);

	// ワールド行列（拡大版）
	D3DXMATRIX mtxScale, mtxRot, mtxTrans, mtxWorld;
	const float outlineScale = 20.0f; // 微妙に拡大

	D3DXMatrixScaling(&mtxScale, m_scale.x * outlineScale, m_scale.y * outlineScale, m_scale.z * outlineScale);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	mtxWorld = mtxScale * mtxRot * mtxTrans;
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// アウトライン用に設定変更
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);            // 奥行き無効
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);      // 裏面描画
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetMaterial(&outlineMat);
	pDevice->SetTexture(0, NULL);

	if (m_pMesh) {
		for (DWORD i = 0; i < m_dwNumMat; ++i) {
			m_pMesh->DrawSubset(i); // アウトライン表示
		}
	}

	// === 2. 通常描画 ===
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z); // 元のサイズ
	mtxWorld = mtxScale * mtxRot * mtxTrans;
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	pDevice->SetRenderState(D3DRS_ZENABLE, prevZEnable);
	pDevice->SetRenderState(D3DRS_CULLMODE, prevCull);
	pDevice->SetRenderState(D3DRS_LIGHTING, prevLighting);
	pDevice->SetMaterial(&prevMat);

	if (m_pMesh) {
		for (DWORD i = 0; i < m_dwNumMat; ++i) {
			m_pMesh->DrawSubset(i); // 通常表示
		}
	}
}


//バインドメッシュ
void CObjectX::BindMesh(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, D3DXMATERIAL* pMat, LPDIRECT3DTEXTURE9* pTexture)
{
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_dwNumMat = dwNumMat;
	m_pMaterial = pMat;

	//テクスチャ反映
	if (pTexture != nullptr)
	{
		for (int i = 0; i < (int)m_dwNumMat; i++)
		{
			if (m_pTexture[i] == nullptr)
			{
				m_pTexture[i] = pTexture[i];
				break;
			}
		}
	}
}
