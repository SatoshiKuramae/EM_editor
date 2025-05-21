//============================================================
//
//object2D.h
//
//============================================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_
#include "main.h"
#include "object.h"
#define TEX_X (0.1f)

//2Dオブジェクトクラス
class CObject2D : public CObject
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;	//頂点情報
		float rhw;			//座標変換用係数
		D3DCOLOR col;		//頂点カラー
		D3DXVECTOR2 tex;
	}VERTEX_2D;

	CObject2D(int nPriority = 0);
	~CObject2D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	static CObject2D* Create();
	void BindTexture(LPDIRECT3DTEXTURE9 pTex,D3DXVECTOR2 fTexpos);		//テクスチャ割り当て
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	//移動量の設定
	D3DXVECTOR3& GetMove() { return  m_move; }			//移動量の取得
	void SetAnim(D3DXVECTOR2 AnimCnt);
	void SetParam(D3DXVECTOR3 fpos, float wide, float height);

	D3DXVECTOR3 GetPos() const override { return m_pos; }
	void SetPos(const D3DXVECTOR3& pos) override { m_pos = pos; }

	D3DXVECTOR3 GetRot() const override { return m_rot; }
	void SetRot(const D3DXVECTOR3& rot) override { m_rot = rot; }

	D3DXVECTOR3 GetScale() const override { return m_scale; }
	void SetScale(const D3DXVECTOR3& scale) override { m_scale = scale; }

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファ
	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャ

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_rot;		//角度
	D3DXVECTOR3 m_scale;	//拡大率
	float m_fAnglePolygon;			//対角線の角度
	float m_fLengthPolygon;			//対角線の長さ
	D3DXVECTOR2 m_texcnt;

	float m_Texpos_x;
	float m_Texpos_y;
	float m_NowTexpos_x;
	float m_NowTexpos_y;
};
#endif
