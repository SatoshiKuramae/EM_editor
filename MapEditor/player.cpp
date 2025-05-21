
#include "objectX.h"
#include "manager.h"
#include "player.h"
#include "camera.h"
#include "imgui.h"

//�ÓI�����o������
D3DXVECTOR3 CPlayer::m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//�R���X�g���N�^
CPlayer::CPlayer()
{

}

//�f�X�g���N�^
CPlayer::~CPlayer()
{

}

//����������
HRESULT CPlayer::Init()
{
	CObject::SetType(TYPE::PLAYER);
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	
	CPlayer::Load();
	
	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\Player.x",
		D3DXMESH_SYSTEMMEM, pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);



	CObjectX::Init();

	return S_OK;
}

//�I������
void CPlayer::Uninit()
{
	CObjectX::Uninit();
}

//�X�V����
void CPlayer::Update()
{
	D3DXVECTOR3 pos;
	pos = GetPos();
	
	m_PosOld = pos;
	Playermove();
	

	pos += m_move;
	
	//�ړ��ʂ��X�V�i�����j

	/*m_move.y -= JUMP_DEC;
	m_move.x += (0.0f - m_move.x) * 1.0f;
	m_move.z += (0.0f - m_move.z) * 1.0f;*/
	
	//pos = Collision(pos);
	SetPos(pos);
}

//�v���C���[�ړ�
void CPlayer::Playermove()
{
	//D3DXVECTOR3 pCameraRot = CManager::GetCamera()->GetRot();
	//CInputKeyboard* pKeyboard = CManager::GetKeyboard();


	//if (pKeyboard->GetKeyboardPress(DIK_W) == true)
	//{//W�L�[�������ꂽ

	//	if (pKeyboard->GetKeyboardPress(DIK_D) == true)
	//	{//�E��ړ�

	//		m_move.x = sinf(D3DX_PI * 0.25f) * PLAYER_SPEED;
	//		m_move.z = cosf(D3DX_PI * 0.25f) * PLAYER_SPEED;

	//	}
	//	else if (pKeyboard->GetKeyboardPress(DIK_A) == true)
	//	{//����ړ�

	//		m_move.x = sinf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
	//		m_move.z = cosf(-D3DX_PI * 0.25f) * PLAYER_SPEED;

	//	}
	//	else
	//	{//��ړ�
	//		m_move.z = PLAYER_SPEED;
	//	}
	//}

	//else if (pKeyboard->GetKeyboardPress(DIK_A) == true)
	//{//A�L�[�������ꂽ

	//	if (pKeyboard->GetKeyboardPress(DIK_W) == true)
	//	{//����ړ�

	//		m_move.x = sinf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
	//		m_move.z = cosf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
	//	}
	//	else if (pKeyboard->GetKeyboardPress(DIK_S) == true)
	//	{//�����ړ�

	//		m_move.x = sinf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
	//		m_move.z = cosf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
	//	}
	//	else
	//	{//���ړ�
	//		m_move.x = -PLAYER_SPEED;
	//	}

	//}
	//else if (pKeyboard->GetKeyboardPress(DIK_S) == true)
	//{//S�L�[�������ꂽ

	//	if (pKeyboard->GetKeyboardPress(DIK_A) == true)
	//	{//�����ړ�

	//		m_move.x = sinf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
	//		m_move.z = cosf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
	//	}
	//	else if (pKeyboard->GetKeyboardPress(DIK_D) == true)
	//	{//�E���ړ�


	//		m_move.x = sinf(D3DX_PI * 0.75f) * PLAYER_SPEED;
	//		m_move.z = cosf(D3DX_PI * 0.75f) * PLAYER_SPEED;
	//	}
	//	else
	//	{//���ړ�

	//		m_move.z = -PLAYER_SPEED;
	//	}
	//}
	//else if (pKeyboard->GetKeyboardPress(DIK_D) == true)
	//{//D�L�[�������ꂽ


	//	if (pKeyboard->GetKeyboardPress(DIK_S) == true)
	//	{//�E���ړ�

	//		//�ړ��ʂ��X�V�i�����j
	//		m_move.x = sinf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
	//		m_move.z = cosf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
	//	}
	//	else if (pKeyboard->GetKeyboardPress(DIK_W) == true)
	//	{//�E��ړ�


	//		//�ړ��ʂ��X�V�i�����j
	//		m_move.x = sinf(D3DX_PI * 0.75f) * PLAYER_SPEED;
	//		m_move.z = cosf(D3DX_PI * 0.75f) * -PLAYER_SPEED;
	//	}
	//	else
	//	{//�E�ړ�

	//		m_move.x = PLAYER_SPEED;
	//	}
	//}
	////�W�����v
	//if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) == true)
	//{
	//	m_move.y += JUMP_INC;
	//}
}
//�`�揈��
void CPlayer::Draw()
{
	//GUI�̕\�����s���Ă����I�I�I�I�I�I

	//ImGuiStyle& style = ImGui::GetStyle();
	//style.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.6f, 0.8f, 1.0f); // �ʏ펞
	//style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.7f, 0.9f, 1.0f); // �z�o�[��
	//style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2f, 0.5f, 0.7f, 1.0f); // ������

	//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	//style.FramePadding.y = 10.0f; // �X���C�_�[�̏c���𑝂₷

	////�I�u�W�F�N�g�̈ʒu�ƃX�P�[��

	//ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSize({ 500, 200 });
	//ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	//ImGui::Begin("config 1");
	//static float slider1 = 0.0;
	//static char text1[9] = "";

	//ImGui::SliderFloat("slider 1", &slider1, 0.0f, 1.0f);
	//ImGui::InputText("textbox 1", text1, sizeof(text1));
	//if (ImGui::Button("button 1")) {
	//	slider1 = 0.0f;
	//	strcpy(text1, "button 1");
	//}
	//ImGui::End();

	//ImGui::PushItemWidth(400); // �X���C�_�[�̕���200�s�N�Z���ɐݒ�
	//ImGui::Text("Object");
	//ImGui::DragFloat3("Position", &m_pos.x, 0.1f);
	//ImGui::DragFloat3("Scale", &m_scale.x, 0.1f);
	//ImGui::DragFloat3("Rot", &m_rot.x, 0.1f);
	//ImGui::PopItemWidth();     // ���̕��ɖ߂��i�����g���Ƃ��͖Y�ꂸ�Ɂj

	//ImGui::PopStyleColor();
	//ImGui::PopStyleColor();


	CObjectX::Draw();
}

//�����蔻��i��,y,z���W�j
//D3DXVECTOR3 CPlayer::Collision(D3DXVECTOR3 pos)
//{
//	//for (int i = 0; i < NUMBLOCK; i++)
//	//{
//	//	//�I�u�W�F�N�g�擾
//	//	CObject* pObj = CObject::GetObject(i, 3);
//
//	//	if (pObj != nullptr)
//	//	{
//	//		CGameobject* p3DBlock = (CGameobject*)pObj;
//	//		
//	//		p3DBlock->GetPos();
//	//		//��ނ̎擾
//	//		CObject::TYPE type = pObj->GetType();
//	//		if (type == CObject::TYPE::BLOCK)
//	//		{
//	//			//�u���b�N�̏�ʂƓ���������
//	//			if (pos.y - BLOCK_Y <= p3DBlock->GetPos().y + BLOCK_Y
//	//				&& m_PosOld.y - BLOCK_Y > p3DBlock->GetPos().y + BLOCK_Y
//	//				&& pos.x + BLOCK_Y >= p3DBlock->GetPos().x - BLOCK_X
//	//				&& pos.x - BLOCK_Y <= p3DBlock->GetPos().x + BLOCK_X
//	//				&& pos.z + BLOCK_Y >= p3DBlock->GetPos().z - BLOCK_Y
//	//				&& pos.z - BLOCK_Y <= p3DBlock->GetPos().z + BLOCK_Y)
//	//			{
//	//				m_move.y = 0.0f;
//	//				pos.y = p3DBlock->GetPos().y + RESETPOS;
//	//			}
//
//	//			//�u���b�N�̉��ʂɓ���������
//	//			if (pos.y + BLOCK_Y >= p3DBlock->GetPos().y - BLOCK_Y
//	//				&& m_PosOld.y + BLOCK_Y < p3DBlock->GetPos().y - BLOCK_X
//	//				&& pos.x + BLOCK_Y >= p3DBlock->GetPos().x - BLOCK_X
//	//				&& pos.x - BLOCK_Y <= p3DBlock->GetPos().x + BLOCK_X
//	//				&& pos.z + BLOCK_Y >= p3DBlock->GetPos().z - BLOCK_Y
//	//				&& pos.z - BLOCK_Y <= p3DBlock->GetPos().z + BLOCK_Y)
//	//			{
//	//				m_move.y = 0.0f;
//	//				pos.y = p3DBlock->GetPos().y - RESETPOS;
//	//			}
//	//			//�u���b�N�̉E�ʂƓ���������
//	//			if (pos.z + BLOCK_Y >= p3DBlock->GetPos().z - BLOCK_Y
//	//				&& pos.z - BLOCK_Y <= p3DBlock->GetPos().z + BLOCK_Y
//	//				&& m_PosOld.x - BLOCK_Y > p3DBlock->GetPos().x + BLOCK_X
//	//				&& pos.x - BLOCK_Y <= p3DBlock->GetPos().x + BLOCK_X
//	//				&& pos.y + BLOCK_Y >= p3DBlock->GetPos().y - BLOCK_Y
//	//				&& pos.y - BLOCK_Y <= p3DBlock->GetPos().y + BLOCK_Y)
//	//			{
//	//				m_move.x = 0.0f;
//	//				pos.x = p3DBlock->GetPos().x + RESETPOS;
//	//			}
//
//	//			//�u���b�N�̍��ʂƓ���������
//	//			if (pos.z + BLOCK_Y >= p3DBlock->GetPos().z - BLOCK_Y
//	//				&& pos.z - BLOCK_X <= p3DBlock->GetPos().z + BLOCK_Y
//	//				&& m_PosOld.x + BLOCK_X < p3DBlock->GetPos().x - BLOCK_X
//	//				&& pos.x + BLOCK_Y >= p3DBlock->GetPos().x - BLOCK_X
//	//				&& pos.y + BLOCK_Y >= p3DBlock->GetPos().y - BLOCK_Y
//	//				&& pos.y - BLOCK_Y <= p3DBlock->GetPos().y + BLOCK_Y)
//	//			{
//	//				m_move.x = 0.0f;
//	//				pos.x = p3DBlock->GetPos().x - RESETPOS;
//	//			}
//
//	//			//�u���b�N�̉��ʂƓ���������
//	//			if (pos.z + BLOCK_Y >= p3DBlock->GetPos().z - BLOCK_Y
//	//				&& m_PosOld.z - BLOCK_Y <= p3DBlock->GetPos().z + BLOCK_Y
//	//				&& pos.x + BLOCK_X >= p3DBlock->GetPos().x - BLOCK_X
//	//				&& pos.x - BLOCK_X <= p3DBlock->GetPos().x + BLOCK_X
//	//				&& pos.y + BLOCK_Y >= p3DBlock->GetPos().y - BLOCK_Y
//	//				&& pos.y - BLOCK_Y <= p3DBlock->GetPos().y + BLOCK_Y)
//	//			{
//	//				m_move.z = 0.0f;
//	//				pos.z = p3DBlock->GetPos().z - RESETPOS;
//	//			}
//
//	//			//�u���b�N�̎�O�Ɠ���������
//	//			if (pos.z - BLOCK_Y <= p3DBlock->GetPos().z + BLOCK_Y
//	//				&& m_PosOld.z + BLOCK_Y >= p3DBlock->GetPos().z - BLOCK_X
//	//				&& pos.x + BLOCK_X >= p3DBlock->GetPos().x - BLOCK_X
//	//				&& pos.x - BLOCK_X <= p3DBlock->GetPos().x + BLOCK_X
//	//				&& pos.y + BLOCK_Y >= p3DBlock->GetPos().y - BLOCK_Y
//	//				&& pos.y - BLOCK_Y <= p3DBlock->GetPos().y + BLOCK_Y)
//	//			{
//	//				m_move.z = 0.0f;
//	//				pos.z = p3DBlock->GetPos().z + RESETPOS;
//	//			}
//	//		}
//	//	
//	//	}
//	//}
//	//
//	//if (pos.y - 50.0f < 0.0f)
//	//{
//	//	m_move.y = 0.0f;
//	//	pos.y = 50.0f;
//	//}
//	//return pos;
//
//	return;
//}


//���[�h����
HRESULT CPlayer::Load()
{
	return S_OK;
}

//�A�����[�h
void Unload()
{

}

//����
CPlayer* CPlayer::Create()
{
	CPlayer* pObject3D = new CPlayer;

	pObject3D->Init();

	pObject3D->SetScale(D3DXVECTOR3(2.0f, 2.0f, 2.0f));

	return pObject3D;
}