#include "camera.h"
#include "manager.h"
#include"input.h"
#include "Player.h"
#include "imgui.h"
#include <algorithm>
#include <vector>
//�R���X�g���N�^
CCamera::CCamera()
{

}

//�f�X�g���N�^
CCamera::~CCamera()
{

}

//�����ݒ�
HRESULT CCamera::Init()
{
	m_TargetPosV.y = CAMERAPOS_V_Y;
	m_TargetPosV.z = CAMERAPOS_V_Z;
	m_flattery = false;
	CCamera::m_posV = D3DXVECTOR3(0.0f, 200.0f, -600.0f);
	CCamera::m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CCamera::m_vecU = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	CCamera::m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	return S_OK;
}

//�J�����̐ݒ�
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 3000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//�X�V����
void CCamera::Update()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();

	//�I�u�W�F�N�g�̈ʒu�ƃX�P�[��

	/*ImGui::DragFloat3("CameraPosV", &m_posV.x, 0.1f);
	ImGui::DragFloat3("CameraPosR", &m_posR.x, 0.1f);
	ImGui::DragFloat3("CameraRot", &m_rot.x, 0.1f);
	ImGui::DragFloat3("Target", &m_TargetPosV.x, 0.1f);*/

	// �}�E�X���W���擾����
	POINT currentPos;
	static POINT prevPos = { 0, 0 };
	GetCursorPos(&currentPos);

	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &currentPos);

	int deltaX = currentPos.x - prevPos.x; // ���̈ړ���
	int deltaY = currentPos.y - prevPos.y; // �c�̈ړ���

	prevPos = currentPos; // ����̂��߂Ɍ��݈ʒu��ۑ�

	//if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	//{
	//	// ���N���b�N��������Ă���
	//	if (m_rot.x < -1.25f)
	//	{
	//		m_rot.x = -1.25f;
	//	}
	//	if (m_rot.x > 1.25f)
	//	{
	//		m_rot.x = 1.25f;
	//	}
	//	if (deltaY > 0)
	//	{
	//		m_rot.x += 0.05f;

	//		m_posV.y = sinf(m_rot.x + D3DX_PI) * sqrtf(m_TargetPosV.x * m_TargetPosV.x + m_TargetPosV.z * m_TargetPosV.z) / 2 + CCamera::m_posR.y;
	//		m_posV.z = cosf(m_rot.x + D3DX_PI) * sqrtf(m_TargetPosV.x * m_TargetPosV.x + m_TargetPosV.z * m_TargetPosV.z) / 2 + CCamera::m_posR.y;
	//	}
	//	else if (deltaY < 0)
	//	{
	//		m_rot.x -= 0.05f;

	//		m_posV.y = sinf(m_rot.x + D3DX_PI) * sqrtf(m_TargetPosV.x * m_TargetPosV.x + m_TargetPosV.z * m_TargetPosV.z) / 2 + CCamera::m_posR.y;
	//		m_posV.z = cosf(m_rot.x + D3DX_PI) * sqrtf(m_TargetPosV.x * m_TargetPosV.x + m_TargetPosV.z * m_TargetPosV.z) / 2 + CCamera::m_posR.y;
	//	}
	//	if (deltaX > 0)
	//	{
	//		m_rot.y += 0.05f;

	//		m_posV.x = sinf(m_rot.y + D3DX_PI) * sqrtf(m_TargetPosV.z * m_TargetPosV.z + m_TargetPosV.y * m_TargetPosV.y) / 2 + CCamera::m_posR.x;
	//		m_posV.z = cosf(m_rot.y + D3DX_PI) * sqrtf(m_TargetPosV.z * m_TargetPosV.z + m_TargetPosV.y * m_TargetPosV.y) / 2 + CCamera::m_posR.x;
	//	}
	//	else if (deltaX < 0)
	//	{
	//		m_rot.y -= 0.05f;

	//		m_posV.x = sinf(m_rot.y + D3DX_PI) * sqrtf(m_TargetPosV.z * m_TargetPosV.z + m_TargetPosV.y * m_TargetPosV.y) / 2 + CCamera::m_posR.x;
	//		m_posV.z = cosf(m_rot.y + D3DX_PI) * sqrtf(m_TargetPosV.z * m_TargetPosV.z + m_TargetPosV.y * m_TargetPosV.y) / 2 + CCamera::m_posR.x;
	//	}
	//	
	//	
	//}
	
	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (CCamera::m_flattery == false)
		{
			CCamera::m_flattery = true;
		}
		else
		{
			CCamera::m_flattery = false;
		}
	}



	//posR=�����_posV=���_vecU=���_�̃x�N�g��

	//���_�̈ړ�
	if (pKeyboard->GetKeyboardPress(DIK_C) == true)
	{
		m_rot.y += 0.1f;

		m_posV.x = sinf(m_rot.y + D3DX_PI) * sqrtf(m_TargetPosV.z * m_TargetPosV.z + m_TargetPosV.y * m_TargetPosV.y) / 2 + CCamera::m_posR.x;
		m_posV.z = cosf(m_rot.y + D3DX_PI) * sqrtf(m_TargetPosV.z * m_TargetPosV.z + m_TargetPosV.y * m_TargetPosV.y) / 2 + CCamera::m_posR.x;
	}
	if (pKeyboard->GetKeyboardPress(DIK_Z) == true)
	{
		m_rot.y -= 0.1f;

		m_posV.x = sinf(m_rot.y + D3DX_PI) * sqrtf(m_TargetPosV.z * m_TargetPosV.z + m_TargetPosV.y * m_TargetPosV.y) / 2 + CCamera::m_posR.x;
		m_posV.z = cosf(m_rot.y + D3DX_PI) * sqrtf(m_TargetPosV.z * m_TargetPosV.z + m_TargetPosV.y * m_TargetPosV.y) / 2 + CCamera::m_posR.x;
	}
	if (pKeyboard->GetKeyboardPress(DIK_Y) == true)
	{
		m_rot.x += 2.0f;
		m_posV.y += 2.0f;
	}
	if (pKeyboard->GetKeyboardPress(DIK_N) == true)
	{
		m_rot.x -= 2.0f;
		m_posV.y -= 2.0f;
	}

	//��]�̕␳
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y = -D3DX_PI;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y = D3DX_PI;
	}

	//�����_�܂ł̋�����ς���
	if (pKeyboard->GetKeyboardPress(DIK_Q) == true)
	{
		m_posV.z += 1.0f;
		m_TargetPosV.z += 1.0f;
		m_TargetPosV.y -= 1.0f;
		m_TargetPosV.x -= 1.0f;
	}

	if (pKeyboard->GetKeyboardPress(DIK_E) == true)
	{
		m_posV.z -= 1.0f;
		m_TargetPosV.z += 1.0f;
		m_TargetPosV.y += 1.0f;
		m_TargetPosV.x += 1.0f;
	}

	//���_�̈ʒu�ړ��i�O�㍶�E�j
	if (pKeyboard->GetKeyboardPress(DIK_UP) == true)
	{
		m_posV.z += CAMERASPEED;
		m_posR.z += CAMERASPEED;
	}

	if (pKeyboard->GetKeyboardPress(DIK_DOWN) == true)
	{
		m_posV.z -= CAMERASPEED;
		m_posR.z -= CAMERASPEED;
	}

	if (pKeyboard->GetKeyboardPress(DIK_RIGHT) == true)
	{
		m_posV.x += CAMERASPEED;
		m_posR.x += CAMERASPEED;
	}
	if (pKeyboard->GetKeyboardPress(DIK_LEFT) == true)
	{
		m_posV.x -= CAMERASPEED;
		m_posR.x -= CAMERASPEED;
	}

	if (m_flattery == true)
	{
		for (int i = 0; i < NUMOBJECT; i++)
		{
			//�I�u�W�F�N�g�擾
			CObject* pObj = CObject::GetObject(i, 3);
			if (pObj != nullptr)
			{
				CPlayer* pPlayer = (CPlayer*)pObj;
				pPlayer->GetPos();
				m_posR.y = pPlayer->GetPos().y + 300.0f;
				//��ނ̎擾
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE::PLAYER)
				{
					m_posR.x = pPlayer->GetPos().x;

					m_posV.x = sinf(m_rot.y + D3DX_PI) * sqrtf(2000.0f * 2000.0f + 2000.0f * 2000.0f) / 2 + pPlayer->GetPos().x;
					m_posV.z = cosf(m_rot.y + D3DX_PI) * sqrtf(2000.0f * 2000.0f + 2000.0f * 2000.0f) / 2 + pPlayer->GetPos().z;

				}
			}
		}
	}
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}




void CCamera::UpdateFromMouse(const MouseInput& mouse)
{
	// �E�N���b�N�h���b�O�ŉ�]
	if (mouse.rightButtonHeld)
	{
		float dx = mouse.deltaX * m_rot.x;
		float dy = mouse.deltaY * m_rot.y;

		m_rot.x += dx;
		m_rot.y += dy;

		// �N�����v(���)
		if (m_rot.y > 89.0f) m_rot.y = 89.0f;
		if (m_rot.y < -89.0f) m_rot.y = -89.0f;
		UpdateViewMatrix();
	}

	//// �z�C�[���ŃY�[��
	//if (mouse.wheelDelta != 0)
	//{
	//	m_Distance -= mouse.wheelDelta * zoomSpeed;
	//	m_Distance = std::clamp(m_Distance, minZoom, maxZoom);

	//	UpdateViewMatrix();
	//}

	//// ���N���b�N or Shift+���N���b�N�ŕ��s�ړ�
	//if (mouse.middleButtonHeld || (mouse.shiftHeld && mouse.leftButtonHeld))
	//{
	//	float panX = -mouse.deltaX * panSpeed;
	//	float panY = mouse.deltaY * panSpeed;

	//	// �J�����̕����ɉ����ăp���������v�Z
	//	D3DXVECTOR3 right, up;
	//	CalculateCameraBasis(right, up); // viewMatrix����right/up�x�N�g�����o
	//	m_Target += right * panX + up * panY;

	//	UpdateViewMatrix();
	//}
}

void CCamera::UpdateViewMatrix()
{
	// ���ʍ��W�n�ŃJ�����ʒu���X�V
	float radYaw = D3DXToRadian(m_rot.x);
	float radPitch = D3DXToRadian(m_rot.y);
	float m_Distance=100.0f;
	D3DXVECTOR3 offset;
	offset.x = m_Distance * cosf(radPitch) * sinf(radYaw);
	offset.y = m_Distance * sinf(radPitch);
	offset.z = m_Distance * cosf(radPitch) * cosf(radYaw);

	m_posV = m_TargetPosV - offset;

	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_TargetPosV, &m_vecU);
}