//============================================================
//
//objectX.h
//
//============================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_
#include "main.h"
#include "object.h"
#define NUM_TEXTURE (50)

//3Dオブジェクトクラス
class CObjectX : public CObject
{
public:
	CObjectX();
	~CObjectX() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Load();
	//void Setpos(D3DXVECTOR3 pos);
	//void SetMove(D3DXVECTOR3 move);//移動量の設定
	//void SetRot(D3DXVECTOR3 rot);
	//void SetScale(D3DXVECTOR3 scale);
	void BindMesh(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, D3DXMATERIAL* pMat, LPDIRECT3DTEXTURE9 pTexture);
	//D3DXVECTOR3& GetScale() { return m_scale; }
	//D3DXVECTOR3& GetRot() { return m_rot; }
	//D3DXVECTOR3& GetPos() { return m_pos; }	//位置情報などを取得

	D3DXVECTOR3 GetPos() const override { return m_pos; }
	void SetPos(const D3DXVECTOR3& pos) override { m_pos = pos; }

	D3DXVECTOR3 GetRot() const override { return m_rot; }
	void SetRot(const D3DXVECTOR3& rot) override { m_rot = rot; }

	D3DXVECTOR3 GetScale() const override { return m_scale; }
	void SetScale(const D3DXVECTOR3& scale) override { m_scale = scale; }

protected:

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_rot;		//角度
	D3DXVECTOR3 m_scale;	//拡大率
	D3DXVECTOR3 m_move;		//移動量
	D3DXMATRIX m_mtxWorld;

	LPD3DXMESH m_pMesh;
	LPD3DXBUFFER m_pBuffMat;
	DWORD m_dwNumMat;
	D3DXMATERIAL* m_pMaterial;
	LPDIRECT3DTEXTURE9 m_pTexture[NUM_TEXTURE];
};
#endif


