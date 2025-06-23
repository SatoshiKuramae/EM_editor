//===============================================================================
//
//object3D.cpp
//
//Author Kuramaesatoshi
//===============================================================================

#include "object3D.h"
#include "main.h"
#include "manager.h"

//コンストラクタ
CObject3D::CObject3D()
{

}
// デストラクタ
CObject3D::~CObject3D()
{

}

//初期化
HRESULT CObject3D::Init()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	pDevice->CreateVertexBuffer(sizeof(CObject3D::VERTEX_3D) 
								* 4 * NUM_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, 
								D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_3D* pVtx; //頂点座標へのポインタ

	//頂点バッファをロックし、頂点座標へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - 1000.0f, m_pos.y, m_pos.z + 1000.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + 1000.0f, m_pos.y, m_pos.z + 1000.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - 1000.0f, m_pos.y, m_pos.z - 1000.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + 1000.0f, m_pos.y, m_pos.z - 1000.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	pVtx[1].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	pVtx[2].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	pVtx[3].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_NowTexpos_x), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / m_NowTexpos_y));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_NowTexpos_x), (1.0f / m_NowTexpos_y));


	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//描画処理
void CObject3D::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//更新処理
void CObject3D::Update()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	VERTEX_3D* pVtx; //頂点座標へのポインタ

	//頂点バッファをロックし、頂点座標へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定


	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	pVtx[1].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	pVtx[2].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);
	pVtx[3].col = D3DCOLOR_RGBA(RED, GREEN, BLUE, A);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_NowTexpos_x), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / m_NowTexpos_y));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_NowTexpos_x), (1.0f / m_NowTexpos_y));


	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}

//終了処理
void CObject3D::Uninit()
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}


//Object3D生成
CObject3D* CObject3D::Create()
{
	CObject3D* pObject3D = new CObject3D;
	pObject3D->Init();
	return pObject3D;
}
//テクスチャ設定
void CObject3D::BindTexture(LPDIRECT3DTEXTURE9 pTex, D3DXVECTOR2 fTexpos)
{
	m_pTexture = pTex;
	m_NowTexpos_x = (float)fTexpos.x;
	m_NowTexpos_y = (float)fTexpos.y;
}
