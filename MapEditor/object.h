//============================================================
//
//object.h
//
//============================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "main.h"


class CObject
{
public:
	enum class TYPE
	{
		NONE=0,
		ENEMY,
		PLAYER,
		BULLET,
		BLOCK,
		FLOOR,
		GAMEOBJECT,
	};

	CObject(int nPriority = 3);		//�`��D��ݒ�
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	static CObject* GetObject(int nIdx,int nPri);
	
	static void ReleaseAll();	//�S�I�u�W�F�N�g�̉��
	static void UpdateAll();	//�S�I�u�W�F�N�g�̍X�V
	static void DrawAll();		//�S�I�u�W�F�N�g�̕`��
	
	//�p�����g�����߂ɂ����̏������p�����[�^���쏈�����s��
	virtual D3DXVECTOR3 GetPos() const { return D3DXVECTOR3(0, 0, 0); }
	virtual void SetPos(const D3DXVECTOR3&) {}

	virtual D3DXVECTOR3 GetRot() const { return D3DXVECTOR3(0, 0, 0); }
	virtual void SetRot(const D3DXVECTOR3&) {}

	virtual D3DXVECTOR3 GetScale() const { return D3DXVECTOR3(1, 1, 1); }
	virtual void SetScale(const D3DXVECTOR3&) {}

	virtual D3DXVECTOR3 GetMove() const { return D3DXVECTOR3(1, 1, 1); }
	virtual void SetMove(const D3DXVECTOR3&) {}

	// �I�u�W�F�N�g�^�C�v�擾
	TYPE GetType() { return m_type; }
	void SetType(TYPE type) { m_type = type; }

protected:
	void Release();				//�������g�̉��
private:
	static CObject* m_apObject[MAX_PRIORITY][NUMOBJECT];	//Priority�ő吔�ƃI�u�W�F�N�g�ő吔
	static int m_nNumAll;					//�I�u�W�F�N�g����
	int m_nID;								//�������g��ID
	int m_nPriority;						//�`��D��x
	TYPE m_type;							//�I�u�W�F�N�g�^�C�v

	//�I�u�W�F�N�g�Ǘ�
	static CObject* m_pTop;	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_pCur;	//���݁i�Ō���j�̃|�C���^
	CObject* m_Prev;		//�O�̃I�u�W�F�N�g�̃|�C���^
	CObject* m_pNext;		//���̃I�u�W�F�N�g�̃|�C���^
	bool m_bDeath;			//���S�t���O
};
#endif