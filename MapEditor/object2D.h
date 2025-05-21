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

//2D�I�u�W�F�N�g�N���X
class CObject2D : public CObject
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;	//���_���
		float rhw;			//���W�ϊ��p�W��
		D3DCOLOR col;		//���_�J���[
		D3DXVECTOR2 tex;
	}VERTEX_2D;

	CObject2D(int nPriority = 0);
	~CObject2D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	static CObject2D* Create();
	void BindTexture(LPDIRECT3DTEXTURE9 pTex,D3DXVECTOR2 fTexpos);		//�e�N�X�`�����蓖��
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	//�ړ��ʂ̐ݒ�
	D3DXVECTOR3& GetMove() { return  m_move; }			//�ړ��ʂ̎擾
	void SetAnim(D3DXVECTOR2 AnimCnt);
	void SetParam(D3DXVECTOR3 fpos, float wide, float height);

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
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_rot;		//�p�x
	D3DXVECTOR3 m_scale;	//�g�嗦
	float m_fAnglePolygon;			//�Ίp���̊p�x
	float m_fLengthPolygon;			//�Ίp���̒���
	D3DXVECTOR2 m_texcnt;

	float m_Texpos_x;
	float m_Texpos_y;
	float m_NowTexpos_x;
	float m_NowTexpos_y;
};
#endif
