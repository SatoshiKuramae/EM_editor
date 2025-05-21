#include "object3D.h"
#include "main.h"
#include "manager.h"
#include "floor.h"

LPDIRECT3DTEXTURE9 CFloor::m_pTexture = nullptr;
//コンストラクタ
CFloor::CFloor()
{

}
// デストラクタ
CFloor::~CFloor()
{

}

//初期化
HRESULT CFloor::Init()
{
	CObject::SetType(TYPE::FLOOR);
	CObject3D::Init();
	;	return S_OK;
}

//描画処理
void CFloor::Draw()
{
	CObject3D::Draw();
}

//更新処理
void CFloor::Update()
{

}

//終了処理
void CFloor::Uninit()
{
	CObject3D::Uninit();
}

//床の生成
CFloor* CFloor::Create()
{
	CFloor* pObject3D = new CFloor;

	pObject3D->BindTexture(m_pTexture, D3DXVECTOR2(1, 1));
	pObject3D->Init();
	return pObject3D;
}


//ロード
HRESULT CFloor::Load()
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();
	if (m_pTexture == nullptr)
	{
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice, "data\\texture\\block.png", &m_pTexture);
	}
	return S_OK;
}

//アンロード
void CFloor::Unload()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}
