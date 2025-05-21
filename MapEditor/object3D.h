//============================================================
//
//object2D.h
//
//============================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_
#include "main.h"
#include "object.h"
#define NUM_POLYGON (15)

//3D�I�u�W�F�N�g�N���X
class CObject3D : public CObject
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 nor;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	}VERTEX_3D;


	CObject3D();
	~CObject3D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void BindTexture(LPDIRECT3DTEXTURE9 pTex, D3DXVECTOR2 fTexpos);		//�e�N�X�`�����蓖��
	static CObject3D* Create();

	D3DXVECTOR3 GetPos() const override { return m_pos; }
	void SetPos(const D3DXVECTOR3& pos) override { m_pos = pos; }

	D3DXVECTOR3 GetRot() const override { return m_rot; }
	void SetRot(const D3DXVECTOR3& rot) override { m_rot = rot; }

	D3DXVECTOR3 GetScale() const override { return m_scale; }
	void SetScale(const D3DXVECTOR3& scale) override { m_scale = scale; }

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@
	LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`��

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_rot;		//�p�x
	D3DXVECTOR3 m_scale;	//�g�嗦
	D3DXMATRIX m_mtxWorld;
	float m_NowTexpos_x;
	float m_NowTexpos_y;
};
#endif

