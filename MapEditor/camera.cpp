//===============================================================================
//
//camera.cpp
//
//Author Kuramaesatoshi
//===============================================================================
#include "camera.h"
#include "manager.h"
#include"input.h"

#include "imgui.h"
#include <algorithm>
#include <vector>

//コンストラクタ
CCamera::CCamera()
{

}

//デストラクタ
CCamera::~CCamera()
{

}

//初期設定
HRESULT CCamera::Init()
{
	m_TargetPosV.y = CAMERAPOS_V_Y;
	m_TargetPosV.z = CAMERAPOS_V_Z;
	m_flattery = false;
	CCamera::m_posV = D3DXVECTOR3(0.0f, 0.0f, CAMERA_POSV_Y);
	CCamera::m_posR = D3DXVECTOR3(0.0f, 0.0f, CAMERA_POSR_Y);
	CCamera::m_vecU = D3DXVECTOR3(0.0f, CAMERA_VEC_Y, 0.0f);
	CCamera::m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	return S_OK;
}

//カメラの設定
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 3000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}


//更新処理
void CCamera::Update()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();


	//posR=注視点posV=視点vecU=視点のベクトル

#ifdef DEBUG
	// 注視点と視点の差
	D3DXVECTOR3 offset = m_posV - m_posR;

	// 半径（XZ平面のみで算出）
	float radius = sqrtf(offset.x * offset.x + offset.z * offset.z);

	// 角度調整（入力による回転）
	if (pKeyboard->GetKeyboardPress(DIK_C)) {
		m_rot.y += 0.05f;
	}
	if (pKeyboard->GetKeyboardPress(DIK_Z)) {
		m_rot.y -= 0.05f;
	}

	if (pKeyboard->GetKeyboardPress(DIK_Y) == true)
	{
		m_posR.y += 2.0f;
		m_posV.y += 2.0f;
	}
	if (pKeyboard->GetKeyboardPress(DIK_N) == true)
	{
		m_posR.y -= 2.0f;
		m_posV.y -= 2.0f;
	}

	// 回転後のカメラ位置を更新（XZ方向の円軌道）
	m_posV.x = sinf(D3DX_PI - m_rot.y) * radius + m_posR.x;
	m_posV.z = cosf(D3DX_PI - m_rot.y) * radius + m_posR.z;

	//回転の補正
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y = -D3DX_PI;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y = D3DX_PI;
	}

	//注視点までの距離を変える
	if (pKeyboard->GetKeyboardPress(DIK_Q) == true)
	{
		m_posV.z += 1.0f;

	}
	if (pKeyboard->GetKeyboardPress(DIK_E) == true)
	{
		m_posV.z -= 1.0f;
	}

	// 矢印キー入力で移動（カメラの向き基準で前後左右）
	float moveSpeed = CAMERASPEED;



	// カメラの注視ベクトル
	D3DXVECTOR3 look = m_posR - m_posV;
	look.y = 0; // Y成分は無視して水平移動だけにする
	D3DXVec3Normalize(&look, &look);

	// 右方向ベクトル（lookベクトルとY軸の外積）
	D3DXVECTOR3 up = { 0, 1, 0 };
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &up, &look);
	D3DXVec3Normalize(&right, &right);

	// キー入力で移動
	if (pKeyboard->GetKeyboardPress(DIK_UP)) {
		m_posV += look * moveSpeed;
		m_posR += look * moveSpeed;
	}
	if (pKeyboard->GetKeyboardPress(DIK_DOWN)) {
		m_posV -= look * moveSpeed;
		m_posR -= look * moveSpeed;
	}
	if (pKeyboard->GetKeyboardPress(DIK_RIGHT)) {
		m_posV += right * moveSpeed;
		m_posR += right * moveSpeed;
	}
	if (pKeyboard->GetKeyboardPress(DIK_LEFT)) {
		m_posV -= right * moveSpeed;
		m_posR -= right * moveSpeed;
	}

	ImGui::Begin(u8"カメラ位置\n");
	ImGui::Text(u8"m_posR（注視点）{%0.3f,%0.3f,%0.3f}", m_posR.x, m_posR.y, m_posR.z);
	ImGui::Text(u8"m_posV（視点）{%0.3f,%0.3f,%0.3f}", m_posV.x, m_posV.y, m_posV.z);

	ImGui::DragFloat3(u8"m_posR:{X,Y,Z}", (float*)&m_posR, 0.1f);
	ImGui::DragFloat3(u8"m_posV:{X,Y,Z}", (float*)&m_posV, 0.1f);

	if (ImGui::Button(u8"カメラの位置リセット"))
	{
		Init();
	}

	ImGui::End();

	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
#endif // DEBUG

	
}

