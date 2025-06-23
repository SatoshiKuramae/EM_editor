//===============================================================================
//
//player.cpp
//
//Author Kuramaesatoshi
//===============================================================================
#include "objectX.h"
#include "manager.h"
#include "player.h"
#include "camera.h"
#include "imgui.h"

//静的メンバ初期化
D3DXVECTOR3 CPlayer::m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//コンストラクタ
CPlayer::CPlayer()
{

}

//デストラクタ
CPlayer::~CPlayer()
{

}

//初期化処理
HRESULT CPlayer::Init()
{
	CObject::SetType(TYPE::PLAYER);
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	
	CPlayer::Load();
	
	//Xファイルの読み込み
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

//終了処理
void CPlayer::Uninit()
{
	CObjectX::Uninit();
}

//更新処理
void CPlayer::Update()
{
	D3DXVECTOR3 pos;
	pos = GetPos();
	
	m_PosOld = pos;
	Playermove();
	

	pos += m_move;
	
	//移動量を更新（減速）

	/*m_move.y -= JUMP_DEC;
	m_move.x += (0.0f - m_move.x) * 1.0f;
	m_move.z += (0.0f - m_move.z) * 1.0f;*/
	
	//pos = Collision(pos);
	SetPos(pos);
}

//プレイヤー移動
void CPlayer::Playermove()
{
	//D3DXVECTOR3 pCameraRot = CManager::GetCamera()->GetRot();
	//CInputKeyboard* pKeyboard = CManager::GetKeyboard();


	//if (pKeyboard->GetKeyboardPress(DIK_W) == true)
	//{//Wキーが押された

	//	if (pKeyboard->GetKeyboardPress(DIK_D) == true)
	//	{//右上移動

	//		m_move.x = sinf(D3DX_PI * 0.25f) * PLAYER_SPEED;
	//		m_move.z = cosf(D3DX_PI * 0.25f) * PLAYER_SPEED;

	//	}
	//	else if (pKeyboard->GetKeyboardPress(DIK_A) == true)
	//	{//左上移動

	//		m_move.x = sinf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
	//		m_move.z = cosf(-D3DX_PI * 0.25f) * PLAYER_SPEED;

	//	}
	//	else
	//	{//上移動
	//		m_move.z = PLAYER_SPEED;
	//	}
	//}

	//else if (pKeyboard->GetKeyboardPress(DIK_A) == true)
	//{//Aキーが押された

	//	if (pKeyboard->GetKeyboardPress(DIK_W) == true)
	//	{//左上移動

	//		m_move.x = sinf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
	//		m_move.z = cosf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
	//	}
	//	else if (pKeyboard->GetKeyboardPress(DIK_S) == true)
	//	{//左下移動

	//		m_move.x = sinf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
	//		m_move.z = cosf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
	//	}
	//	else
	//	{//左移動
	//		m_move.x = -PLAYER_SPEED;
	//	}

	//}
	//else if (pKeyboard->GetKeyboardPress(DIK_S) == true)
	//{//Sキーが押された

	//	if (pKeyboard->GetKeyboardPress(DIK_A) == true)
	//	{//左下移動

	//		m_move.x = sinf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
	//		m_move.z = cosf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
	//	}
	//	else if (pKeyboard->GetKeyboardPress(DIK_D) == true)
	//	{//右下移動


	//		m_move.x = sinf(D3DX_PI * 0.75f) * PLAYER_SPEED;
	//		m_move.z = cosf(D3DX_PI * 0.75f) * PLAYER_SPEED;
	//	}
	//	else
	//	{//下移動

	//		m_move.z = -PLAYER_SPEED;
	//	}
	//}
	//else if (pKeyboard->GetKeyboardPress(DIK_D) == true)
	//{//Dキーが押された


	//	if (pKeyboard->GetKeyboardPress(DIK_S) == true)
	//	{//右下移動

	//		//移動量を更新（増加）
	//		m_move.x = sinf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
	//		m_move.z = cosf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
	//	}
	//	else if (pKeyboard->GetKeyboardPress(DIK_W) == true)
	//	{//右上移動


	//		//移動量を更新（増加）
	//		m_move.x = sinf(D3DX_PI * 0.75f) * PLAYER_SPEED;
	//		m_move.z = cosf(D3DX_PI * 0.75f) * -PLAYER_SPEED;
	//	}
	//	else
	//	{//右移動

	//		m_move.x = PLAYER_SPEED;
	//	}
	//}
	////ジャンプ
	//if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) == true)
	//{
	//	m_move.y += JUMP_INC;
	//}
}
//描画処理
void CPlayer::Draw()
{
	//GUIの表示を行っているよ！！！！！！

	//ImGuiStyle& style = ImGui::GetStyle();
	//style.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.6f, 0.8f, 1.0f); // 通常時
	//style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.7f, 0.9f, 1.0f); // ホバー時
	//style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2f, 0.5f, 0.7f, 1.0f); // 押下時

	//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	//style.FramePadding.y = 10.0f; // スライダーの縦幅を増やす

	////オブジェクトの位置とスケール

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

	//ImGui::PushItemWidth(400); // スライダーの幅を200ピクセルに設定
	//ImGui::Text("Object");
	//ImGui::DragFloat3("Position", &m_pos.x, 0.1f);
	//ImGui::DragFloat3("Scale", &m_scale.x, 0.1f);
	//ImGui::DragFloat3("Rot", &m_rot.x, 0.1f);
	//ImGui::PopItemWidth();     // 元の幅に戻す（複数使うときは忘れずに）

	//ImGui::PopStyleColor();
	//ImGui::PopStyleColor();


	CObjectX::Draw();
}

//当たり判定（ｘ,y,z座標）
//D3DXVECTOR3 CPlayer::Collision(D3DXVECTOR3 pos)
//{
//	//for (int i = 0; i < NUMBLOCK; i++)
//	//{
//	//	//オブジェクト取得
//	//	CObject* pObj = CObject::GetObject(i, 3);
//
//	//	if (pObj != nullptr)
//	//	{
//	//		CGameobject* p3DBlock = (CGameobject*)pObj;
//	//		
//	//		p3DBlock->GetPos();
//	//		//種類の取得
//	//		CObject::TYPE type = pObj->GetType();
//	//		if (type == CObject::TYPE::BLOCK)
//	//		{
//	//			//ブロックの上面と当たった時
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
//	//			//ブロックの下面に当たった時
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
//	//			//ブロックの右面と当たった時
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
//	//			//ブロックの左面と当たった時
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
//	//			//ブロックの奥面と当たった時
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
//	//			//ブロックの手前と当たった時
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


//ロード処理
HRESULT CPlayer::Load()
{
	return S_OK;
}

//アンロード
void Unload()
{

}

//生成
CPlayer* CPlayer::Create()
{
	CPlayer* pObject3D = new CPlayer;

	pObject3D->Init();

	pObject3D->SetScale(D3DXVECTOR3(2.0f, 2.0f, 2.0f));

	return pObject3D;
}
