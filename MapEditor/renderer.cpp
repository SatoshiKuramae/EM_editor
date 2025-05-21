//==============================================================
//
// renderer.cpp
//
//==============================================================

#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "camera.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

//�R���X�g���N�^
CRenderer::CRenderer()
{
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
}

//�f�X�g���N�^
CRenderer::~CRenderer()
{

}

//������
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;	//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^

	//DirectX�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;	//��
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;	//����
	d3dpp.BackBufferFormat = d3ddm.Format;	//�`��
	d3dpp.BackBufferCount = 1;				//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�_�u���o�b�t�@�̐؂�ւ�
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//�f�v�X�o�b�t�@��16bit�Ƃ���
	d3dpp.Windowed = bWindow;	//�E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;


	//Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
	{
		//Direct3D�f�o�C�X�̐����i�`�揈���̓n�[�h�E�F�A�A���_������CPU���s���j
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//�����_�[�X�e�[�g�ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//���X������
	//�|���S��������


	return S_OK;

}

//�X�V
void CRenderer::Update()	//�S�I�u�W�F�N�g�̍X�V
{
	CObject::UpdateAll();
}

//�`��
void CRenderer::Draw()		//�S�I�u�W�F�N�g�̕`��
{
	//��ʃN���A�i�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A�j
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(100, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//�`��J�n������
		//�J�����̐ݒ�
		CManager::GetCamera()->SetCamera();

		//�e��I�u�W�F�N�g�̕`�揈��
		CObject::DrawAll();

		// ImGui �̕`�揈��
		ImGui::Render();

		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		//�`��I��
		m_pD3DDevice->EndScene();
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//�I��
void CRenderer::Uninit()
{
	//Direct3D�f�o�C�X�̂̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//3D�f�o�C�X�̎擾
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}