//===============================================================================
//
//camera.cpp
// 
//===============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"
#define CAMERASPEED (1.0f)

struct MouseInput {
	float deltaX, deltaY;
	float wheelDelta;
	bool rightButtonHeld;
	bool middleButtonHeld;
	bool leftButtonHeld;
	bool shiftHeld;
};

//カメラ
class CCamera
{
public:
	CCamera();
	~CCamera();
	HRESULT Init();
	void Uninit();
	void Update();
	void SetCamera();	//カメラの設定
	void SetQuake(int quakeflame, float quakesize);
	void UpdateFromMouse(const MouseInput& mouse);
	void UpdateViewMatrix();

	D3DXVECTOR3& GetRot() { return m_rot; }
private:
	D3DXVECTOR3 m_posV;				//視点
	D3DXVECTOR3 m_posR;				//注視点
	D3DXVECTOR3 m_vecU;				//上方向
	D3DXMATRIX m_mtxProjection;		//プロジェクション行列
	D3DXMATRIX m_mtxView;			//ビュー行列
	D3DXVECTOR3 m_TargetPosV;		//目的の注視点
	D3DXVECTOR3 m_rot;	//向き
	bool m_flattery;		//カメラの追従
	int m_QuakeFlame;
	float m_QuakeSize;
	D3DXVECTOR3 m_AveragePlayer_pos;

	static DIMOUSESTATE m_CurrentMouseState;		//!< マウスの現在の入力情報
	static DIMOUSESTATE m_PrevMouseState;			//!< マウスの一フレーム前の入力情報
};


#endif
