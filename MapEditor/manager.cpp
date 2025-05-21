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

//�ÓI�����o������
CRenderer* CManager::m_pRenderer = nullptr;
CInputKeyboard* CManager::m_pKeyboard = nullptr;
CCamera* CManager::m_pCamera = nullptr;
CLight* CManager::m_pLight = nullptr;


GUIManager* g_pGuimanager = nullptr;
//�R���X�g���N�^
CManager::CManager()
{

}

//�f�X�g���N�^
CManager::~CManager()
{

}

//������
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//�����_���[�̐���
	m_pRenderer = new CRenderer();
	m_pRenderer->Init(hWnd, bWindow);

	//�L�[�{�[�h�̐���
	m_pKeyboard = new CInputKeyboard();
	m_pKeyboard->Init(hInstance, hWnd);

	//�J��������
	m_pCamera = new CCamera();
	m_pCamera->Init();

	//���C�g����
	m_pLight = new CLight();
	m_pLight->Init();

	g_pGuimanager = new GUIManager();

	//�}�l�[�W���[��������
	if (!g_pGuimanager->Initialize(hWnd, GetRenderer()->GetDevice()))
	{
		return -1;
	}
	//�ォ�珇�ɕ`�悳���



	CPlayer::Load();
	CFloor::Load();
	

	//GameObject::Create();

	//CPlayer::Create();
	//CFloor::Create();


	return S_OK;
}

//�I������
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

//�X�V����
void CManager::Update()
{
	g_pGuimanager->BeginFrame();

	m_pRenderer->Update();
	m_pKeyboard->Update();
	m_pCamera->Update();

	g_pGuimanager->Update();

	g_pGuimanager->EndFrame(GetRenderer()->GetDevice());
}

//�`�揈��
void CManager::Draw()
{
	m_pRenderer->Draw();
}

//�����_���[�擾
CRenderer* CManager::GetRenderer()
{
	return m_pRenderer;
}

//�L�[�{�[�h�擾
CInputKeyboard* CManager::GetKeyboard()
{
	return m_pKeyboard;
}

//�J�����̎擾
CCamera* CManager::GetCamera()
{
	return m_pCamera;
}

//���C�g�̎擾
CLight* CManager::GetLight()
{
	return m_pLight;
}