//===============================================================================
//
//light.cpp
//
//Author Kuramaesatoshi
//===============================================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"
//コンストラクタ
CLight::CLight()
{
	for (int i = 0; i < NUM_LIGHT; i++)
	{
		//ライト情報のクリア
		ZeroMemory(&m_Light[i], sizeof(D3DLIGHT9));
	}
}

//デストラクタ
CLight::~CLight()
{

}

//初期設定
HRESULT CLight::Init()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	for (int i = 0; i < NUM_LIGHT; i++)
	{
		//ライトの種類を設定
		m_Light[i].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		m_Light[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (i == 0)
		{
			//ライトの方向を設定
			vecDir = D3DXVECTOR3(0.5f, -0.5f, -0.4f);
		}
		if (i == 1)
		{//ライトの方向を設定
			vecDir = D3DXVECTOR3(0.2f, -0.5f, -0.4f);
		}
		if (i == 2)
		{
			//ライトの方向を設定
			vecDir = D3DXVECTOR3(-0.3f, -0.5f, -0.4f);
		}


		D3DXVec3Normalize(&vecDir, &vecDir);	//ベクトルを正規化
		m_Light[i].Direction = vecDir;

		//ライトを設定
		pDevice->SetLight(0, &m_Light[i]);

		//ライトを有効にする
		pDevice->LightEnable(i, TRUE);
	}
	return S_OK;
}

//終了処理
void CLight::Uninit()
{

}

//更新処理
void CLight::Update()
{

}
