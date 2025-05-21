#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"
#define NUM_LIGHT	(3)
//ライト
class CLight
{
public:
	CLight();
	~CLight();
	HRESULT Init();
	void Uninit();
	void Update();
private:
	D3DLIGHT9 m_Light[NUM_LIGHT];
	D3DXVECTOR3	vecDir;		//設定用方向ベクトル
};
#endif
