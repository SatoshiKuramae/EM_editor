#include "camera.h"
#include "manager.h"
#include"input.h"
#include "Player.h"
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
	CCamera::m_posV = D3DXVECTOR3(0.0f, 200.0f, -600.0f);
	CCamera::m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CCamera::m_vecU = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
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

	//オブジェクトの位置とスケール

	/*ImGui::DragFloat3("CameraPosV", &m_posV.x, 0.1f);
	ImGui::DragFloat3("CameraPosR", &m_posR.x, 0.1f);
	ImGui::DragFloat3("CameraRot", &m_rot.x, 0.1f);
	ImGui::DragFloat3("Target", &m_TargetPosV.x, 0.1f);*/

	// マウス座標を取得する
	POINT currentPos;
	static POINT prevPos = { 0, 0 };
	GetCursorPos(&currentPos);

	// スクリーン座標をクライアント座標に変換する
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &currentPos);

	int deltaX = currentPos.x - prevPos.x; // 横の移動量
	int deltaY = currentPos.y - prevPos.y; // 縦の移動量

	prevPos = currentPos; // 次回のために現在位置を保存

	//if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	//{
	//	// 左クリックが押されている
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
	
	/*if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (CCamera::m_flattery == false)
		{
			CCamera::m_flattery = true;
		}
		else
		{
			CCamera::m_flattery = false;
		}
	}*/



	//posR=注視点posV=視点vecU=視点のベクトル

	//視点の移動
	/*if (pKeyboard->GetKeyboardPress(DIK_C) == true)
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
	}*/
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
	/*if (pKeyboard->GetKeyboardPress(DIK_Q) == true)
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
	}*/

	// 矢印キー入力で移動（カメラの向き基準で前後左右）
	float moveSpeed = CAMERASPEED;

	// カメラの前方向（XZ平面で回転を反映）
	D3DXVECTOR3 forward(sinf(m_rot.y), 0, cosf(m_rot.y));
	D3DXVec3Normalize(&forward, &forward);

	// カメラの右方向（forwardと上方向のクロス）
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &m_vecU, &forward);
	D3DXVec3Normalize(&right, &right);

	if (pKeyboard->GetKeyboardPress(DIK_UP)) {
		m_posV += forward * moveSpeed;
		m_posR += forward * moveSpeed;
	}
	if (pKeyboard->GetKeyboardPress(DIK_DOWN)) {
		m_posV -= forward * moveSpeed;
		m_posR -= forward * moveSpeed;
	}
	if (pKeyboard->GetKeyboardPress(DIK_RIGHT)) {
		m_posV += right * moveSpeed;
		m_posR += right * moveSpeed;
	}
	if (pKeyboard->GetKeyboardPress(DIK_LEFT)) {
		m_posV -= right * moveSpeed;
		m_posR -= right * moveSpeed;
	}

	ImGui::Begin(u8"テスト");
	ImGui::Text(u8"カメラ情報\nposR{%0.3f,%0.3f,%0.3f}", m_posR.x, m_posR.y, m_posR.z);
	ImGui::Text(u8"posV{%0.3f,%0.3f,%0.3f}", m_posV.x, m_posV.y, m_posV.z);

	ImGui::DragFloat3(u8"m_posR:{X,Y,Z}", (float*)&m_posR, 0.1f);
	ImGui::DragFloat3(u8"m_posV:{X,Y,Z}", (float*)&m_posV, 0.1f);
	ImGui::End();

	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}




void CCamera::UpdateFromMouse(const MouseInput& mouse)
{
	// 右クリックドラッグで回転
	if (mouse.rightButtonHeld)
	{
		float dx = mouse.deltaX * m_rot.x;
		float dy = mouse.deltaY * m_rot.y;

		m_rot.x += dx;
		m_rot.y += dy;

		// クランプ(代替)
		if (m_rot.y > 89.0f) m_rot.y = 89.0f;
		if (m_rot.y < -89.0f) m_rot.y = -89.0f;
		UpdateViewMatrix();
	}

	//// ホイールでズーム
	//if (mouse.wheelDelta != 0)
	//{
	//	m_Distance -= mouse.wheelDelta * zoomSpeed;
	//	m_Distance = std::clamp(m_Distance, minZoom, maxZoom);

	//	UpdateViewMatrix();
	//}

	//// 中クリック or Shift+左クリックで平行移動
	//if (mouse.middleButtonHeld || (mouse.shiftHeld && mouse.leftButtonHeld))
	//{
	//	float panX = -mouse.deltaX * panSpeed;
	//	float panY = mouse.deltaY * panSpeed;

	//	// カメラの方向に応じてパン方向を計算
	//	D3DXVECTOR3 right, up;
	//	CalculateCameraBasis(right, up); // viewMatrixからright/upベクトル抽出
	//	m_Target += right * panX + up * panY;

	//	UpdateViewMatrix();
	//}
}

void CCamera::UpdateViewMatrix()
{
	// 球面座標系でカメラ位置を更新
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
