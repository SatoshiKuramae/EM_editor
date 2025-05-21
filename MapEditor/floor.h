//============================================================
//
//object2D.h
//
//============================================================
#ifndef _FLOOR_H_
#define _FLOOR_H_
#include "main.h"
#include "object3D.h"


//2Dオブジェクトクラス
class CFloor : public CObject3D
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 nor;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	}VERTEX_3D;


	CFloor();
	~CFloor() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	static CFloor* Create();
	static HRESULT Load();
	static void Unload();


protected:
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャ
};
#endif


