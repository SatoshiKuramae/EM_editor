//==============================================================
//
// manager.cpp
//
//==============================================================

#include<Windows.h>
#include "main.h"
#include "object.h"
#include "object2D.h"

#include "manager.h"
#include "input.h"

#include "camera.h"
#include "light.h"
#include "object3D.h"

#include "objectX.h"
#include "player.h"
#include "floor.h"
#include "GUImanager.h"
#include "gameobject.h"

//静的メンバ初期化
CRenderer* CManager::m_pRenderer = nullptr;
CInputKeyboard* CManager::m_pKeyboard = nullptr;
CCamera* CManager::m_pCamera = nullptr;
CLight* CManager::m_pLight = nullptr;


GUIManager* g_pGuimanager = nullptr;
//コンストラクタ
CManager::CManager()
{

}

//デストラクタ
CManager::~CManager()
{

}

//初期化
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダラーの生成
	m_pRenderer = new CRenderer();
	m_pRenderer->Init(hWnd, bWindow);

	//キーボードの生成
	m_pKeyboard = new CInputKeyboard();
	m_pKeyboard->Init(hInstance, hWnd);

	//カメラ生成
	m_pCamera = new CCamera();
	m_pCamera->Init();

	//ライト生成
	m_pLight = new CLight();
	m_pLight->Init();

	g_pGuimanager = new GUIManager();

	//マネージャーを初期化
	if (!g_pGuimanager->Initialize(hWnd, GetRenderer()->GetDevice()))
	{
		return -1;
	}
	//上から順に描画される



	CPlayer::Load();
	CFloor::Load();
	

	//GameObject::Create();

	//CPlayer::Create();
	//CFloor::Create();


	return S_OK;
}

//終了処理
void CManager::Uninit()
{

	CObject::ReleaseAll();

	if (m_pRenderer != nullptr)
	{

		m_pKeyboard->Uninit();

		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;

		g_pGuimanager->Shutdown();
		delete g_pGuimanager;
	}
}

//更新処理
void CManager::Update()
{
	g_pGuimanager->BeginFrame();

	m_pRenderer->Update();
	m_pKeyboard->Update();
	m_pCamera->Update();

	g_pGuimanager->Update();

	g_pGuimanager->EndFrame(GetRenderer()->GetDevice());
}

//描画処理
void CManager::Draw()
{
	m_pRenderer->Draw();
}

//レンダラー取得
CRenderer* CManager::GetRenderer()
{
	return m_pRenderer;
}

//キーボード取得
CInputKeyboard* CManager::GetKeyboard()
{
	return m_pKeyboard;
}

//カメラの取得
CCamera* CManager::GetCamera()
{
	return m_pCamera;
}

//ライトの取得
CLight* CManager::GetLight()
{
	return m_pLight;
}