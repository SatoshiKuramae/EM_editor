#include "light.h"
#include "manager.h"
#include "renderer.h"
//�R���X�g���N�^
CLight::CLight()
{
	for (int i = 0; i < NUM_LIGHT; i++)
	{
		//���C�g���̃N���A
		ZeroMemory(&m_Light[i], sizeof(D3DLIGHT9));
	}
}

//�f�X�g���N�^
CLight::~CLight()
{

}

//�����ݒ�
HRESULT CLight::Init()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	for (int i = 0; i < NUM_LIGHT; i++)
	{
		//���C�g�̎�ނ�ݒ�
		m_Light[i].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
		m_Light[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (i == 0)
		{
			//���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.5f, -0.5f, -0.4f);
		}
		if (i == 1)
		{//���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.2f, -0.5f, -0.4f);
		}
		if (i == 2)
		{
			//���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(-0.3f, -0.5f, -0.4f);
		}


		D3DXVec3Normalize(&vecDir, &vecDir);	//�x�N�g���𐳋K��
		m_Light[i].Direction = vecDir;

		//���C�g��ݒ�
		pDevice->SetLight(0, &m_Light[i]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(i, TRUE);
	}
	return S_OK;
}

//�I������
void CLight::Uninit()
{

}

//�X�V����
void CLight::Update()
{

}