//============================================================
//
//player.h
//
// Auther : SatoshiKuramae
//============================================================
#ifndef _CPlayer_H_
#define _CPlayer_H_
#include "main.h"
#include "objectX.h"
#define PARTS_FILE	"data\\motion.txt"
#define JUMP_INC (50.0f)
#define JUMP_DEC (2.2f)		//重力

//3Dオブジェクトクラス
class CPlayer : public CObjectX
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 nor;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	}VERTEX_X;

	const char* ModelFile[NUM_MODEL] =
	{
		"data\\model\\body.x",
		/*"data\\MODEL\\hip.x",
		"data\\MODEL\\head.x",
		"data\\MODEL\\L_arm1.x",
		"data\\MODEL\\L_arm2.x",
		"data\\MODEL\\L_hand.x",
		"data\\MODEL\\R_arm1.x",
		"data\\MODEL\\R_arm2.x",
		"data\\MODEL\\R_hand.x",
		"data\\MODEL\\R_leg1.x",
		"data\\MODEL\\R_leg2.x",
		"data\\MODEL\\R_foot.x",
		"data\\MODEL\\L_leg1.x",
		"data\\MODEL\\L_leg2.x",
		"data\\MODEL\\L_foot.x",*/
	};

	CPlayer();
	~CPlayer();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Playermove();
	static HRESULT Load();
	static void Unload();
	//D3DXVECTOR3 Collision(D3DXVECTOR3 pos);
	static CPlayer* Create();
	D3DXVECTOR3& GetPos() { return m_pos; }	//位置情報などを取得
private:
	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャ
	D3DXMATRIX m_mtxWorld;
	static D3DXVECTOR3 m_PosOld;
};
#endif





